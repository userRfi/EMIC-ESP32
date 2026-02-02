/*****************************************************************************
 * @file    bt.h
 * @brief   ESP32-C3 Bluetooth (BLE 5.0) Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details BLE 5.0 interface for ESP32-C3.
 *          NOTE: ESP32-C3 does NOT support Bluetooth Classic.
 *          Supports: BLE 5.0, Coded PHY, 2M PHY, Extended Advertising
 *****************************************************************************/

#ifndef ESP32C3_BT_H
#define ESP32C3_BT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Feature Detection
 *============================================================================*/

/* ESP32-C3 Bluetooth capabilities */
#define BT_HAS_CLASSIC          0   /* No Bluetooth Classic */
#define BT_HAS_BLE              1   /* BLE 5.0 supported */
#define BT_BLE_VERSION          5   /* BLE 5.0 */
#define BT_HAS_CODED_PHY        1   /* Coded PHY (long range) */
#define BT_HAS_2M_PHY           1   /* 2M PHY (high speed) */

/*============================================================================
 * Configuration
 *============================================================================*/

#define BLE_DEVICE_NAME_MAX     32
#define BLE_MTU_DEFAULT         23
#define BLE_MTU_MAX             517

/*============================================================================
 * Bluetooth Classic Stubs (not available on C3)
 *============================================================================*/

/* These macros provide compile-time stubs for BT Classic */
#define BT_Classic_init()               ((void)0)
#define BT_Classic_deinit()             ((void)0)
#define BT_Classic_isConnected()        (false)
#define BT_Classic_send(data, len)      (0)
#define BT_Classic_available()          (0)
#define BT_Classic_read(buf, len)       (0)
#define BT_Classic_setPin(pin)          ((void)0)

/*============================================================================
 * BLE Initialization
 *============================================================================*/

/**
 * @brief Initialize BLE subsystem
 */
void BLE_init(void);

/**
 * @brief Deinitialize BLE subsystem
 */
void BLE_deinit(void);

/**
 * @brief Set BLE device name
 *
 * @param name Device name (max 32 characters)
 */
void BLE_setDeviceName(const char* name);

/*============================================================================
 * BLE Advertising
 *============================================================================*/

/**
 * @brief Start BLE advertising
 *
 * @return true if advertising started successfully
 */
bool BLE_startAdvertising(void);

/**
 * @brief Stop BLE advertising
 *
 * @return true if advertising stopped
 */
bool BLE_stopAdvertising(void);

/**
 * @brief Check if currently advertising
 *
 * @return true if advertising
 */
bool BLE_isAdvertising(void);

/*============================================================================
 * BLE Connection
 *============================================================================*/

/**
 * @brief Check if connected to a central device
 *
 * @return true if connected
 */
bool BLE_isConnected(void);

/**
 * @brief Disconnect from current connection
 *
 * @return true if disconnected
 */
bool BLE_disconnect(void);

/**
 * @brief Get connection handle
 *
 * @return Connection handle or 0xFFFF if not connected
 */
uint16_t BLE_getConnHandle(void);

/*============================================================================
 * BLE PHY Configuration (BLE 5.0 features)
 *============================================================================*/

/**
 * @brief Set preferred PHY (BLE 5.0)
 *
 * @param txPhy TX PHY (1=1M, 2=2M, 4=Coded)
 * @param rxPhy RX PHY (1=1M, 2=2M, 4=Coded)
 * @return true if successful
 */
bool BLE_setPhy(uint8_t txPhy, uint8_t rxPhy);

/**
 * @brief Set TX power level
 *
 * @param power Power level in dBm (-12 to +12)
 */
void BLE_setTxPower(int8_t power);

/*============================================================================
 * BLE Data Transfer
 *============================================================================*/

/**
 * @brief Get current MTU size
 *
 * @return MTU size in bytes
 */
uint16_t BLE_getMTU(void);

/*============================================================================
 * Callbacks (set these before init)
 *============================================================================*/

typedef void (*ble_connect_cb_t)(uint16_t conn_handle);
typedef void (*ble_disconnect_cb_t)(uint16_t conn_handle, uint8_t reason);

/**
 * @brief Set connection callback
 */
void BLE_setConnectCallback(ble_connect_cb_t callback);

/**
 * @brief Set disconnection callback
 */
void BLE_setDisconnectCallback(ble_disconnect_cb_t callback);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_BT_H */
