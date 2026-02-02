/*****************************************************************************
 * @file    bt_stub.c
 * @brief   ESP32-S2 Bluetooth Stub Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Bluetooth stub implementation for ESP32-S2.
 *          ESP32-S2 does NOT support Bluetooth (neither Classic nor BLE).
 *          All functions return error codes, false, or 0 as appropriate.
 *
 * @note    If you need Bluetooth, consider using:
 *          - ESP32 (Classic + BLE 4.2)
 *          - ESP32-C3 (BLE 5.0)
 *          - ESP32-S3 (BLE 5.0)
 *          - ESP32-C6 (BLE 5.0 + 802.15.4)
 *****************************************************************************/

#include "bt_stub.h"
#include "esp_log.h"

static const char* TAG = "BT_STUB";

/*============================================================================
 * Bluetooth Availability Check
 *============================================================================*/

bool BT_isAvailable(void)
{
    ESP_LOGW(TAG, "Bluetooth is not available on ESP32-S2");
    return false;
}

bool BLE_isAvailable(void)
{
    ESP_LOGW(TAG, "BLE is not available on ESP32-S2");
    return false;
}

/*============================================================================
 * BLE Stub Functions
 *============================================================================*/

int BLE_init(void)
{
    ESP_LOGE(TAG, "BLE_init() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BLE_deinit(void)
{
    ESP_LOGE(TAG, "BLE_deinit() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BLE_setDeviceName(const char* name)
{
    (void)name;  /* Suppress unused parameter warning */
    ESP_LOGE(TAG, "BLE_setDeviceName() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BLE_startAdvertising(void)
{
    ESP_LOGE(TAG, "BLE_startAdvertising() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BLE_stopAdvertising(void)
{
    ESP_LOGE(TAG, "BLE_stopAdvertising() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

bool BLE_isConnected(void)
{
    /* Always return false - no Bluetooth means no connection */
    return false;
}

uint16_t BLE_getConnectionHandle(void)
{
    ESP_LOGW(TAG, "BLE_getConnectionHandle(): ESP32-S2 has no Bluetooth hardware");
    return 0;
}

int BLE_notify(uint16_t handle, const uint8_t* data, size_t len)
{
    (void)handle;
    (void)data;
    (void)len;
    ESP_LOGE(TAG, "BLE_notify() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BLE_indicate(uint16_t handle, const uint8_t* data, size_t len)
{
    (void)handle;
    (void)data;
    (void)len;
    ESP_LOGE(TAG, "BLE_indicate() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

/*============================================================================
 * Bluetooth Classic Stub Functions
 *============================================================================*/

int BT_init(void)
{
    ESP_LOGE(TAG, "BT_init() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

int BT_deinit(void)
{
    ESP_LOGE(TAG, "BT_deinit() failed: ESP32-S2 has no Bluetooth hardware");
    return -1;
}

/*============================================================================
 * Error Message Helper
 *============================================================================*/

const char* BT_getUnavailableReason(void)
{
    return "ESP32-S2 does not have Bluetooth hardware. "
           "For Bluetooth support, use ESP32, ESP32-C3, ESP32-S3, or ESP32-C6.";
}
