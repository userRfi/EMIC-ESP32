/*****************************************************************************
 * @file    bt_classic.c
 * @brief   ESP32 Bluetooth Classic Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details Bluetooth Classic SPP implementation using ESP-IDF.
 *****************************************************************************/

#include "bt.h"

#ifdef CONFIG_BT_CLASSIC_ENABLED

/* ESP-IDF includes */
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_bt_api.h"
#include "esp_bt_device.h"
#include "esp_spp_api.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include <string.h>

static const char *TAG = "BT_Classic";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool bt_classic_initialized = false;
static bool spp_connected = false;
static uint32_t spp_handle = 0;

/* RX buffer */
#define RX_BUFFER_SIZE 1024
static uint8_t rx_buffer[RX_BUFFER_SIZE];
static volatile size_t rx_head = 0;
static volatile size_t rx_tail = 0;

/*============================================================================
 * Internal Functions
 *============================================================================*/

static size_t rx_available(void)
{
    if (rx_head >= rx_tail) {
        return rx_head - rx_tail;
    }
    return RX_BUFFER_SIZE - rx_tail + rx_head;
}

static void rx_push(const uint8_t* data, size_t len)
{
    for (size_t i = 0; i < len; i++) {
        size_t next = (rx_head + 1) % RX_BUFFER_SIZE;
        if (next != rx_tail) { /* Not full */
            rx_buffer[rx_head] = data[i];
            rx_head = next;
        }
    }
}

static int rx_pop(uint8_t* data, size_t maxLen)
{
    size_t count = 0;
    while (count < maxLen && rx_tail != rx_head) {
        data[count++] = rx_buffer[rx_tail];
        rx_tail = (rx_tail + 1) % RX_BUFFER_SIZE;
    }
    return count;
}

/*============================================================================
 * SPP Callback
 *============================================================================*/

static void spp_callback(esp_spp_cb_event_t event, esp_spp_cb_param_t *param)
{
    switch (event) {
        case ESP_SPP_INIT_EVT:
            ESP_LOGI(TAG, "SPP initialized");
            break;

        case ESP_SPP_SRV_OPEN_EVT:
            ESP_LOGI(TAG, "SPP client connected");
            spp_connected = true;
            spp_handle = param->srv_open.handle;
            break;

        case ESP_SPP_CLOSE_EVT:
            ESP_LOGI(TAG, "SPP connection closed");
            spp_connected = false;
            spp_handle = 0;
            break;

        case ESP_SPP_DATA_IND_EVT:
            /* Data received */
            rx_push(param->data_ind.data, param->data_ind.len);
            break;

        case ESP_SPP_WRITE_EVT:
            /* Write complete */
            break;

        default:
            break;
    }
}

/*============================================================================
 * GAP Callback
 *============================================================================*/

static void gap_callback(esp_bt_gap_cb_event_t event, esp_bt_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_BT_GAP_AUTH_CMPL_EVT:
            if (param->auth_cmpl.stat == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(TAG, "Authentication success");
            } else {
                ESP_LOGE(TAG, "Authentication failed: %d", param->auth_cmpl.stat);
            }
            break;

        case ESP_BT_GAP_PIN_REQ_EVT:
            /* Simple pairing - accept default PIN */
            esp_bt_pin_code_t pin = {'1', '2', '3', '4'};
            esp_bt_gap_pin_reply(param->pin_req.bda, true, 4, pin);
            break;

        default:
            break;
    }
}

/*============================================================================
 * Public Functions
 *============================================================================*/

bool BT_Classic_init(void)
{
    if (bt_classic_initialized) {
        return true;
    }

    /* Register GAP callback */
    esp_err_t ret = esp_bt_gap_register_callback(gap_callback);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GAP callback registration failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Register SPP callback and init */
    esp_spp_cfg_t spp_cfg = {
        .mode = ESP_SPP_MODE_CB,
        .enable_l2cap_ertm = true,
    };

    ret = esp_spp_enhanced_init(&spp_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPP init failed: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_spp_register_callback(spp_callback);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "SPP callback registration failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Set discoverable and connectable */
    esp_bt_gap_set_scan_mode(ESP_BT_CONNECTABLE, ESP_BT_GENERAL_DISCOVERABLE);

    bt_classic_initialized = true;
    ESP_LOGI(TAG, "Bluetooth Classic initialized");
    return true;
}

bool BT_Classic_startServer(const char* serviceName)
{
    if (!bt_classic_initialized) {
        if (!BT_Classic_init()) {
            return false;
        }
    }

    esp_spp_srv_open_t srv = {
        .sec_mask = ESP_SPP_SEC_AUTHENTICATE,
        .role = ESP_SPP_ROLE_SLAVE,
        .max_session = 1,
    };
    strncpy((char*)srv.name, serviceName, ESP_SPP_SERVER_NAME_MAX - 1);

    esp_err_t ret = esp_spp_start_srv(&srv);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start SPP server: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "SPP server started: %s", serviceName);
    return true;
}

bool BT_Classic_isConnected(void)
{
    return spp_connected;
}

int BT_Classic_write(const uint8_t* data, size_t len)
{
    if (!spp_connected || spp_handle == 0) {
        return -1;
    }

    esp_err_t ret = esp_spp_write(spp_handle, len, (uint8_t*)data);
    if (ret == ESP_OK) {
        return len;
    }
    return -1;
}

int BT_Classic_read(uint8_t* buffer, size_t maxLen)
{
    return rx_pop(buffer, maxLen);
}

int BT_Classic_available(void)
{
    return rx_available();
}

void BT_Classic_disconnect(void)
{
    if (spp_connected && spp_handle != 0) {
        esp_spp_disconnect(spp_handle);
    }
}

#else /* CONFIG_BT_CLASSIC_ENABLED not defined */

/* Stub implementations when Bluetooth Classic is not enabled */
bool BT_Classic_init(void) { return false; }
bool BT_Classic_startServer(const char* serviceName) { return false; }
bool BT_Classic_isConnected(void) { return false; }
int BT_Classic_write(const uint8_t* data, size_t len) { return -1; }
int BT_Classic_read(uint8_t* buffer, size_t maxLen) { return 0; }
int BT_Classic_available(void) { return 0; }
void BT_Classic_disconnect(void) { }

#endif /* CONFIG_BT_CLASSIC_ENABLED */
