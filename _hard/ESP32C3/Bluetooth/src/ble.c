/*****************************************************************************
 * @file    ble.c
 * @brief   ESP32-C3 BLE Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details BLE 5.0 implementation for ESP32-C3 using NimBLE stack.
 *****************************************************************************/

#include "bt.h"

/* ESP-IDF and NimBLE includes */
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_nimble_hci.h"
#include "nimble/nimble_port.h"
#include "nimble/nimble_port_freertos.h"
#include "host/ble_hs.h"
#include "host/util/util.h"
#include "services/gap/ble_svc_gap.h"
#include "services/gatt/ble_svc_gatt.h"
#include <string.h>

static const char *TAG = "BLE";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool ble_initialized = false;
static bool ble_advertising = false;
static uint16_t ble_conn_handle = BLE_HS_CONN_HANDLE_NONE;
static char ble_device_name[BLE_DEVICE_NAME_MAX + 1] = "ESP32-C3";

/* Callbacks */
static ble_connect_cb_t connect_callback = NULL;
static ble_disconnect_cb_t disconnect_callback = NULL;

/* Advertising parameters */
static uint8_t own_addr_type;

/*============================================================================
 * Internal Functions
 *============================================================================*/

static void ble_on_reset(int reason)
{
    ESP_LOGE(TAG, "BLE reset, reason=%d", reason);
}

static void ble_on_sync(void)
{
    int rc;

    rc = ble_hs_util_ensure_addr(0);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to ensure address");
        return;
    }

    rc = ble_hs_id_infer_auto(0, &own_addr_type);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to infer address type");
        return;
    }

    ESP_LOGI(TAG, "BLE synchronized");
}

static int ble_gap_event(struct ble_gap_event *event, void *arg)
{
    switch (event->type) {
        case BLE_GAP_EVENT_CONNECT:
            if (event->connect.status == 0) {
                ble_conn_handle = event->connect.conn_handle;
                ble_advertising = false;
                ESP_LOGI(TAG, "Connected, handle=%d", ble_conn_handle);

                if (connect_callback) {
                    connect_callback(ble_conn_handle);
                }
            } else {
                ESP_LOGE(TAG, "Connection failed, status=%d", event->connect.status);
                BLE_startAdvertising();
            }
            break;

        case BLE_GAP_EVENT_DISCONNECT:
            ESP_LOGI(TAG, "Disconnected, reason=%d", event->disconnect.reason);

            if (disconnect_callback) {
                disconnect_callback(ble_conn_handle, event->disconnect.reason);
            }

            ble_conn_handle = BLE_HS_CONN_HANDLE_NONE;
            BLE_startAdvertising();
            break;

        case BLE_GAP_EVENT_ADV_COMPLETE:
            ESP_LOGI(TAG, "Advertising complete");
            ble_advertising = false;
            break;

        case BLE_GAP_EVENT_MTU:
            ESP_LOGI(TAG, "MTU updated to %d", event->mtu.value);
            break;

        default:
            break;
    }

    return 0;
}

static void ble_host_task(void *param)
{
    ESP_LOGI(TAG, "BLE Host Task started");
    nimble_port_run();
    nimble_port_freertos_deinit();
}

/*============================================================================
 * Initialization
 *============================================================================*/

void BLE_init(void)
{
    if (ble_initialized) {
        return;
    }

    esp_err_t ret;

    /* Initialize NimBLE */
    ret = nimble_port_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to init nimble: %s", esp_err_to_name(ret));
        return;
    }

    /* Configure GAP service */
    ble_svc_gap_device_name_set(ble_device_name);
    ble_svc_gap_init();
    ble_svc_gatt_init();

    /* Set callbacks */
    ble_hs_cfg.reset_cb = ble_on_reset;
    ble_hs_cfg.sync_cb = ble_on_sync;
    ble_hs_cfg.gatts_register_cb = NULL;
    ble_hs_cfg.store_status_cb = NULL;

    /* Start host task */
    nimble_port_freertos_init(ble_host_task);

    ble_initialized = true;
    ESP_LOGI(TAG, "BLE initialized (ESP32-C3, BLE 5.0)");
}

void BLE_deinit(void)
{
    if (!ble_initialized) {
        return;
    }

    BLE_stopAdvertising();

    if (ble_conn_handle != BLE_HS_CONN_HANDLE_NONE) {
        BLE_disconnect();
    }

    nimble_port_stop();
    nimble_port_deinit();

    ble_initialized = false;
    ESP_LOGI(TAG, "BLE deinitialized");
}

void BLE_setDeviceName(const char* name)
{
    if (name == NULL) {
        return;
    }

    strncpy(ble_device_name, name, BLE_DEVICE_NAME_MAX);
    ble_device_name[BLE_DEVICE_NAME_MAX] = '\0';

    if (ble_initialized) {
        ble_svc_gap_device_name_set(ble_device_name);
    }
}

/*============================================================================
 * Advertising
 *============================================================================*/

bool BLE_startAdvertising(void)
{
    if (!ble_initialized) {
        BLE_init();
    }

    if (ble_advertising) {
        return true;
    }

    struct ble_gap_adv_params adv_params;
    struct ble_hs_adv_fields fields;
    int rc;

    memset(&fields, 0, sizeof(fields));
    fields.flags = BLE_HS_ADV_F_DISC_GEN | BLE_HS_ADV_F_BREDR_UNSUP;
    fields.tx_pwr_lvl_is_present = 1;
    fields.tx_pwr_lvl = BLE_HS_ADV_TX_PWR_LVL_AUTO;
    fields.name = (uint8_t *)ble_device_name;
    fields.name_len = strlen(ble_device_name);
    fields.name_is_complete = 1;

    rc = ble_gap_adv_set_fields(&fields);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to set adv fields: %d", rc);
        return false;
    }

    memset(&adv_params, 0, sizeof(adv_params));
    adv_params.conn_mode = BLE_GAP_CONN_MODE_UND;
    adv_params.disc_mode = BLE_GAP_DISC_MODE_GEN;

    rc = ble_gap_adv_start(own_addr_type, NULL, BLE_HS_FOREVER,
                           &adv_params, ble_gap_event, NULL);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to start advertising: %d", rc);
        return false;
    }

    ble_advertising = true;
    ESP_LOGI(TAG, "Advertising started");
    return true;
}

bool BLE_stopAdvertising(void)
{
    if (!ble_advertising) {
        return true;
    }

    int rc = ble_gap_adv_stop();
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to stop advertising: %d", rc);
        return false;
    }

    ble_advertising = false;
    ESP_LOGI(TAG, "Advertising stopped");
    return true;
}

bool BLE_isAdvertising(void)
{
    return ble_advertising;
}

/*============================================================================
 * Connection
 *============================================================================*/

bool BLE_isConnected(void)
{
    return (ble_conn_handle != BLE_HS_CONN_HANDLE_NONE);
}

bool BLE_disconnect(void)
{
    if (ble_conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return true;
    }

    int rc = ble_gap_terminate(ble_conn_handle, BLE_ERR_REM_USER_CONN_TERM);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to disconnect: %d", rc);
        return false;
    }

    return true;
}

uint16_t BLE_getConnHandle(void)
{
    return ble_conn_handle;
}

/*============================================================================
 * PHY Configuration
 *============================================================================*/

bool BLE_setPhy(uint8_t txPhy, uint8_t rxPhy)
{
    if (ble_conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return false;
    }

    int rc = ble_gap_set_prefered_le_phy(ble_conn_handle, txPhy, rxPhy, 0);
    if (rc != 0) {
        ESP_LOGE(TAG, "Failed to set PHY: %d", rc);
        return false;
    }

    ESP_LOGI(TAG, "PHY set to TX=%d, RX=%d", txPhy, rxPhy);
    return true;
}

void BLE_setTxPower(int8_t power)
{
    esp_ble_tx_power_set(ESP_BLE_PWR_TYPE_DEFAULT, power);
    ESP_LOGI(TAG, "TX power set to %d dBm", power);
}

/*============================================================================
 * Data Transfer
 *============================================================================*/

uint16_t BLE_getMTU(void)
{
    if (ble_conn_handle == BLE_HS_CONN_HANDLE_NONE) {
        return BLE_MTU_DEFAULT;
    }

    return ble_att_mtu(ble_conn_handle);
}

/*============================================================================
 * Callbacks
 *============================================================================*/

void BLE_setConnectCallback(ble_connect_cb_t callback)
{
    connect_callback = callback;
}

void BLE_setDisconnectCallback(ble_disconnect_cb_t callback)
{
    disconnect_callback = callback;
}
