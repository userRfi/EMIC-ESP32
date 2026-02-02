/*****************************************************************************
 * @file    bt_stub.h
 * @brief   ESP32-S2 Bluetooth Stub Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Bluetooth stub header for ESP32-S2.
 *          ESP32-S2 does NOT support Bluetooth (neither Classic nor BLE).
 *          All functions return error codes or false.
 *****************************************************************************/

#ifndef ESP32S2_BT_STUB_H
#define ESP32S2_BT_STUB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*============================================================================
 * Bluetooth Availability Check
 *============================================================================*/

/**
 * @brief Check if Bluetooth is available on this chip
 * @return false - ESP32-S2 has no Bluetooth hardware
 */
bool BT_isAvailable(void);

/**
 * @brief Check if BLE is available on this chip
 * @return false - ESP32-S2 has no Bluetooth hardware
 */
bool BLE_isAvailable(void);

/*============================================================================
 * BLE Stub Functions (all return error codes)
 *============================================================================*/

/**
 * @brief Initialize BLE subsystem (STUB)
 * @return -1 indicating failure (no Bluetooth on ESP32-S2)
 */
int BLE_init(void);

/**
 * @brief Deinitialize BLE subsystem (STUB)
 * @return -1 indicating failure
 */
int BLE_deinit(void);

/**
 * @brief Set BLE device name (STUB)
 * @param name Device name (ignored)
 * @return -1 indicating failure
 */
int BLE_setDeviceName(const char* name);

/**
 * @brief Start BLE advertising (STUB)
 * @return -1 indicating failure
 */
int BLE_startAdvertising(void);

/**
 * @brief Stop BLE advertising (STUB)
 * @return -1 indicating failure
 */
int BLE_stopAdvertising(void);

/**
 * @brief Check if BLE is connected (STUB)
 * @return false - always disconnected
 */
bool BLE_isConnected(void);

/**
 * @brief Get BLE connection handle (STUB)
 * @return 0 - invalid handle
 */
uint16_t BLE_getConnectionHandle(void);

/**
 * @brief Send BLE notification (STUB)
 * @param handle Characteristic handle (ignored)
 * @param data Data to send (ignored)
 * @param len Data length (ignored)
 * @return -1 indicating failure
 */
int BLE_notify(uint16_t handle, const uint8_t* data, size_t len);

/**
 * @brief Send BLE indication (STUB)
 * @param handle Characteristic handle (ignored)
 * @param data Data to send (ignored)
 * @param len Data length (ignored)
 * @return -1 indicating failure
 */
int BLE_indicate(uint16_t handle, const uint8_t* data, size_t len);

/*============================================================================
 * Bluetooth Classic Stub Functions (ESP32-S2 has no BT Classic)
 *============================================================================*/

/**
 * @brief Initialize Bluetooth Classic (STUB)
 * @return -1 indicating failure
 */
int BT_init(void);

/**
 * @brief Deinitialize Bluetooth Classic (STUB)
 * @return -1 indicating failure
 */
int BT_deinit(void);

/*============================================================================
 * Error Message Helper
 *============================================================================*/

/**
 * @brief Get error message explaining why Bluetooth is unavailable
 * @return Constant string explaining ESP32-S2 limitation
 */
const char* BT_getUnavailableReason(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_BT_STUB_H */
