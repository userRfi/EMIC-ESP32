/*****************************************************************************
 * @file    ble.c
 * @brief   ESP32 BLE Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details BLE GATT Server implementation using ESP-IDF NimBLE stack.
 *****************************************************************************/

#include "bt.h"

/* ESP-IDF includes */
#include "esp_bt.h"
#include "esp_bt_main.h"
#include "esp_gap_ble_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "BLE";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool ble_initialized = false;
static bool ble_connected = false;
static bool ble_advertising = false;
static uint16_t ble_conn_id = 0xFFFF;
static uint8_t connected_addr[6] = {0};

/* Callbacks */
static BLE_ConnectCallback_t connect_callback = NULL;
static BLE_WriteCallback_t write_callback = NULL;
static BLE_ReadCallback_t read_callback = NULL;

/* GATT interface */
static esp_gatt_if_t gatt_if = ESP_GATT_IF_NONE;

/* Advertising configuration */
static char device_name[BT_DEVICE_NAME_MAX + 1] = "ESP32_EMIC";
static uint16_t adv_int_min = 0x20;  /* 20ms in 0.625ms units */
static uint16_t adv_int_max = 0x40;  /* 40ms in 0.625ms units */

/*============================================================================
 * Advertising Data
 *============================================================================*/

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data = NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = 0,
    .p_service_uuid = NULL,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min = 0x20,
    .adv_int_max = 0x40,
    .adv_type = ADV_TYPE_IND,
    .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
    .channel_map = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

/*============================================================================
 * GAP Event Handler
 *============================================================================*/

static void gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            ESP_LOGI(TAG, "Advertising data set");
            if (!ble_advertising) {
                esp_ble_gap_start_advertising(&adv_params);
            }
            break;

        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(TAG, "Advertising started");
                ble_advertising = true;
            } else {
                ESP_LOGE(TAG, "Advertising start failed");
            }
            break;

        case ESP_GAP_BLE_ADV_STOP_COMPLETE_EVT:
            ESP_LOGI(TAG, "Advertising stopped");
            ble_advertising = false;
            break;

        case ESP_GAP_BLE_UPDATE_CONN_PARAMS_EVT:
            ESP_LOGI(TAG, "Connection params updated");
            break;

        default:
            break;
    }
}

/*============================================================================
 * GATT Server Event Handler
 *============================================================================*/

static void gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                                 esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
        case ESP_GATTS_REG_EVT:
            if (param->reg.status == ESP_GATT_OK) {
                gatt_if = gatts_if;
                ESP_LOGI(TAG, "GATT registered, if=%d", gatts_if);

                /* Set device name */
                esp_ble_gap_set_device_name(device_name);
                esp_ble_gap_config_adv_data(&adv_data);
            }
            break;

        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(TAG, "Client connected, conn_id=%d", param->connect.conn_id);
            ble_conn_id = param->connect.conn_id;
            ble_connected = true;
            memcpy(connected_addr, param->connect.remote_bda, 6);

            /* Stop advertising when connected */
            esp_ble_gap_stop_advertising();

            if (connect_callback) {
                connect_callback(true);
            }
            break;

        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(TAG, "Client disconnected");
            ble_connected = false;
            ble_conn_id = 0xFFFF;
            memset(connected_addr, 0, 6);

            if (connect_callback) {
                connect_callback(false);
            }

            /* Restart advertising */
            esp_ble_gap_start_advertising(&adv_params);
            break;

        case ESP_GATTS_WRITE_EVT:
            if (!param->write.is_prep) {
                if (write_callback) {
                    write_callback(param->write.handle,
                                    param->write.value,
                                    param->write.len);
                }
            }
            break;

        case ESP_GATTS_READ_EVT:
            if (read_callback) {
                read_callback(param->read.handle);
            }
            break;

        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(TAG, "MTU updated to %d", param->mtu.mtu);
            break;

        default:
            break;
    }
}

/*============================================================================
 * Common Bluetooth Functions
 *============================================================================*/

bool BT_init(BT_Mode_t mode)
{
    if (ble_initialized) {
        return true;
    }

    /* Release classic BT memory if only using BLE */
    if (mode == BT_MODE_BLE) {
        ESP_ERROR_CHECK(esp_bt_controller_mem_release(ESP_BT_MODE_CLASSIC_BT));
    }

    /* Initialize BT controller */
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();

    esp_err_t ret = esp_bt_controller_init(&bt_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "BT controller init failed: %s", esp_err_to_name(ret));
        return false;
    }

    esp_bt_mode_t esp_mode;
    switch (mode) {
        case BT_MODE_CLASSIC: esp_mode = ESP_BT_MODE_CLASSIC_BT; break;
        case BT_MODE_BLE:     esp_mode = ESP_BT_MODE_BLE; break;
        case BT_MODE_DUAL:    esp_mode = ESP_BT_MODE_BTDM; break;
        default:              esp_mode = ESP_BT_MODE_BLE; break;
    }

    ret = esp_bt_controller_enable(esp_mode);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "BT controller enable failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Initialize Bluedroid */
    ret = esp_bluedroid_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid init failed: %s", esp_err_to_name(ret));
        return false;
    }

    ret = esp_bluedroid_enable();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Bluedroid enable failed: %s", esp_err_to_name(ret));
        return false;
    }

    ble_initialized = true;
    ESP_LOGI(TAG, "Bluetooth initialized");
    return true;
}

void BT_deinit(void)
{
    if (!ble_initialized) {
        return;
    }

    esp_bluedroid_disable();
    esp_bluedroid_deinit();
    esp_bt_controller_disable();
    esp_bt_controller_deinit();

    ble_initialized = false;
    ble_connected = false;
    ble_advertising = false;
    ESP_LOGI(TAG, "Bluetooth deinitialized");
}

bool BT_setDeviceName(const char* name)
{
    if (name == NULL) return false;

    strncpy(device_name, name, BT_DEVICE_NAME_MAX);
    device_name[BT_DEVICE_NAME_MAX] = '\0';

    if (ble_initialized) {
        return (esp_ble_gap_set_device_name(device_name) == ESP_OK);
    }
    return true;
}

void BT_getMacAddress(uint8_t* mac)
{
    if (mac != NULL) {
        const uint8_t* addr = esp_bt_dev_get_address();
        if (addr != NULL) {
            memcpy(mac, addr, 6);
        }
    }
}

bool BT_isEnabled(void)
{
    return ble_initialized;
}

/*============================================================================
 * BLE Functions
 *============================================================================*/

bool BLE_init(void)
{
    if (!BT_init(BT_MODE_BLE)) {
        return false;
    }

    /* Register GAP callback */
    esp_err_t ret = esp_ble_gap_register_callback(gap_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GAP callback registration failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Register GATT callback */
    ret = esp_ble_gatts_register_callback(gatts_event_handler);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GATTS callback registration failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Register GATT application */
    ret = esp_ble_gatts_app_register(0);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "GATTS app registration failed: %s", esp_err_to_name(ret));
        return false;
    }

    /* Set MTU */
    esp_ble_gatt_set_local_mtu(BLE_MTU_MAX);

    ESP_LOGI(TAG, "BLE initialized");
    return true;
}

void BLE_deinit(void)
{
    BLE_stopAdvertising();
    BLE_disconnect();
    BT_deinit();
}

bool BLE_startAdvertising(void)
{
    if (!ble_initialized) {
        if (!BLE_init()) {
            return false;
        }
    }

    if (ble_advertising) {
        return true; /* Already advertising */
    }

    esp_err_t ret = esp_ble_gap_start_advertising(&adv_params);
    return (ret == ESP_OK);
}

bool BLE_stopAdvertising(void)
{
    if (!ble_advertising) {
        return true;
    }

    esp_err_t ret = esp_ble_gap_stop_advertising();
    return (ret == ESP_OK);
}

bool BLE_setAdvertisingData(const char* deviceName, bool includeUUID)
{
    if (deviceName != NULL) {
        BT_setDeviceName(deviceName);
    }

    adv_data.include_name = true;

    return (esp_ble_gap_config_adv_data(&adv_data) == ESP_OK);
}

void BLE_setAdvertisingInterval(uint16_t minMs, uint16_t maxMs)
{
    /* Convert ms to 0.625ms units */
    adv_params.adv_int_min = (minMs * 1000) / 625;
    adv_params.adv_int_max = (maxMs * 1000) / 625;
}

bool BLE_isConnected(void)
{
    return ble_connected;
}

void BLE_disconnect(void)
{
    if (ble_connected && ble_conn_id != 0xFFFF) {
        esp_ble_gap_disconnect(connected_addr);
    }
}

bool BLE_getConnectedAddress(uint8_t* addr)
{
    if (!ble_connected || addr == NULL) {
        return false;
    }
    memcpy(addr, connected_addr, 6);
    return true;
}

/*============================================================================
 * GATT Server Functions
 *============================================================================*/

int BLE_createService(uint16_t uuid, bool isPrimary)
{
    /* Implementation would create GATT service */
    /* Simplified for initial implementation */
    ESP_LOGI(TAG, "Create service UUID=0x%04X", uuid);
    return 0; /* Return service handle */
}

int BLE_addCharacteristic(int serviceHandle, uint16_t uuid, uint8_t properties)
{
    /* Implementation would add characteristic to service */
    ESP_LOGI(TAG, "Add characteristic UUID=0x%04X, props=0x%02X", uuid, properties);
    return 0; /* Return characteristic handle */
}

bool BLE_startService(int serviceHandle)
{
    ESP_LOGI(TAG, "Start service handle=%d", serviceHandle);
    return true;
}

bool BLE_setCharacteristicValue(int charHandle, const uint8_t* value, size_t len)
{
    if (gatt_if == ESP_GATT_IF_NONE || value == NULL) {
        return false;
    }

    esp_err_t ret = esp_ble_gatts_set_attr_value(charHandle, len, value);
    return (ret == ESP_OK);
}

int BLE_getCharacteristicValue(int charHandle, uint8_t* buffer, size_t maxLen)
{
    uint16_t length = 0;
    const uint8_t* value = NULL;

    esp_err_t ret = esp_ble_gatts_get_attr_value(charHandle, &length, &value);
    if (ret != ESP_OK || value == NULL) {
        return -1;
    }

    size_t copyLen = (length < maxLen) ? length : maxLen;
    memcpy(buffer, value, copyLen);
    return copyLen;
}

bool BLE_notify(int charHandle, const uint8_t* value, size_t len)
{
    if (!ble_connected || gatt_if == ESP_GATT_IF_NONE) {
        return false;
    }

    esp_err_t ret = esp_ble_gatts_send_indicate(gatt_if, ble_conn_id, charHandle,
                                                  len, (uint8_t*)value, false);
    return (ret == ESP_OK);
}

bool BLE_indicate(int charHandle, const uint8_t* value, size_t len)
{
    if (!ble_connected || gatt_if == ESP_GATT_IF_NONE) {
        return false;
    }

    esp_err_t ret = esp_ble_gatts_send_indicate(gatt_if, ble_conn_id, charHandle,
                                                  len, (uint8_t*)value, true);
    return (ret == ESP_OK);
}

/*============================================================================
 * Scanning Functions
 *============================================================================*/

bool BLE_startScan(uint32_t durationSec)
{
    esp_ble_scan_params_t scan_params = {
        .scan_type = BLE_SCAN_TYPE_ACTIVE,
        .own_addr_type = BLE_ADDR_TYPE_PUBLIC,
        .scan_filter_policy = BLE_SCAN_FILTER_ALLOW_ALL,
        .scan_interval = 0x50,
        .scan_window = 0x30,
        .scan_duplicate = BLE_SCAN_DUPLICATE_DISABLE,
    };

    esp_err_t ret = esp_ble_gap_set_scan_params(&scan_params);
    if (ret != ESP_OK) {
        return false;
    }

    ret = esp_ble_gap_start_scanning(durationSec);
    return (ret == ESP_OK);
}

void BLE_stopScan(void)
{
    esp_ble_gap_stop_scanning();
}

bool BLE_connectToDevice(const uint8_t* addr)
{
    if (addr == NULL) return false;

    esp_bd_addr_t bda;
    memcpy(bda, addr, 6);

    esp_err_t ret = esp_ble_gattc_open(gatt_if, bda, BLE_ADDR_TYPE_PUBLIC, true);
    return (ret == ESP_OK);
}

/*============================================================================
 * Callback Registration
 *============================================================================*/

void BLE_setConnectCallback(BLE_ConnectCallback_t callback)
{
    connect_callback = callback;
}

void BLE_setWriteCallback(BLE_WriteCallback_t callback)
{
    write_callback = callback;
}

void BLE_setReadCallback(BLE_ReadCallback_t callback)
{
    read_callback = callback;
}
