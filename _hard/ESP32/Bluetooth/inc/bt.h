/*****************************************************************************
 * @file    bt.h
 * @brief   ESP32 Bluetooth Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details Combined Bluetooth Classic and BLE interface.
 *****************************************************************************/

#ifndef ESP32_BT_H
#define ESP32_BT_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define BT_DEVICE_NAME_MAX      32
#define BLE_MTU_DEFAULT         23
#define BLE_MTU_MAX             517

/*============================================================================
 * Bluetooth Mode
 *============================================================================*/

typedef enum {
    BT_MODE_DISABLED = 0,
    BT_MODE_CLASSIC,        /* Bluetooth Classic only (BR/EDR) */
    BT_MODE_BLE,            /* Bluetooth Low Energy only */
    BT_MODE_DUAL            /* Both Classic and BLE */
} BT_Mode_t;

/*============================================================================
 * BLE Service/Characteristic UUIDs
 *============================================================================*/

/* Standard 16-bit UUIDs */
#define BLE_UUID_DEVICE_INFO_SERVICE    0x180A
#define BLE_UUID_BATTERY_SERVICE        0x180F
#define BLE_UUID_HEART_RATE_SERVICE     0x180D

/*============================================================================
 * Common Bluetooth Functions
 *============================================================================*/

/**
 * @brief Initialize Bluetooth subsystem
 *
 * @param mode Bluetooth mode to enable
 * @return true if initialized successfully
 */
bool BT_init(BT_Mode_t mode);

/**
 * @brief Deinitialize Bluetooth
 */
void BT_deinit(void);

/**
 * @brief Set device name
 *
 * @param name Device name (max 32 chars)
 * @return true if set successfully
 */
bool BT_setDeviceName(const char* name);

/**
 * @brief Get device MAC address
 *
 * @param mac Buffer to store MAC (6 bytes)
 */
void BT_getMacAddress(uint8_t* mac);

/**
 * @brief Check if Bluetooth is enabled
 *
 * @return true if enabled
 */
bool BT_isEnabled(void);

/*============================================================================
 * Bluetooth Classic (SPP) Functions
 *============================================================================*/

/**
 * @brief Initialize Bluetooth Classic SPP
 *
 * @return true if initialized
 */
bool BT_Classic_init(void);

/**
 * @brief Start SPP server
 *
 * @param serviceName Service name (visible to clients)
 * @return true if started
 */
bool BT_Classic_startServer(const char* serviceName);

/**
 * @brief Check if a client is connected
 *
 * @return true if connected
 */
bool BT_Classic_isConnected(void);

/**
 * @brief Write data to connected client
 *
 * @param data Data to write
 * @param len Length of data
 * @return Number of bytes written
 */
int BT_Classic_write(const uint8_t* data, size_t len);

/**
 * @brief Read data from client
 *
 * @param buffer Buffer to store data
 * @param maxLen Maximum bytes to read
 * @return Number of bytes read
 */
int BT_Classic_read(uint8_t* buffer, size_t maxLen);

/**
 * @brief Check bytes available
 *
 * @return Number of bytes available
 */
int BT_Classic_available(void);

/**
 * @brief Disconnect current client
 */
void BT_Classic_disconnect(void);

/*============================================================================
 * BLE Functions
 *============================================================================*/

/**
 * @brief Initialize BLE subsystem
 *
 * @return true if initialized
 */
bool BLE_init(void);

/**
 * @brief Deinitialize BLE
 */
void BLE_deinit(void);

/*----------------------------------------------------------------------------
 * BLE Advertising
 *----------------------------------------------------------------------------*/

/**
 * @brief Start BLE advertising
 *
 * @return true if started
 */
bool BLE_startAdvertising(void);

/**
 * @brief Stop BLE advertising
 *
 * @return true if stopped
 */
bool BLE_stopAdvertising(void);

/**
 * @brief Set advertising data
 *
 * @param deviceName Name to advertise
 * @param includeUUID Include service UUID in advertisement
 * @return true if set
 */
bool BLE_setAdvertisingData(const char* deviceName, bool includeUUID);

/**
 * @brief Set advertising interval
 *
 * @param minMs Minimum interval in ms (20-10240)
 * @param maxMs Maximum interval in ms
 */
void BLE_setAdvertisingInterval(uint16_t minMs, uint16_t maxMs);

/*----------------------------------------------------------------------------
 * BLE Connection
 *----------------------------------------------------------------------------*/

/**
 * @brief Check if a device is connected
 *
 * @return true if connected
 */
bool BLE_isConnected(void);

/**
 * @brief Disconnect current connection
 */
void BLE_disconnect(void);

/**
 * @brief Get connected device address
 *
 * @param addr Buffer to store address (6 bytes)
 * @return true if connected
 */
bool BLE_getConnectedAddress(uint8_t* addr);

/*----------------------------------------------------------------------------
 * BLE GATT Server
 *----------------------------------------------------------------------------*/

/**
 * @brief Create a GATT service
 *
 * @param uuid 16-bit or 128-bit UUID
 * @param isPrimary true for primary service
 * @return Service handle or -1 on error
 */
int BLE_createService(uint16_t uuid, bool isPrimary);

/**
 * @brief Add characteristic to service
 *
 * @param serviceHandle Service handle
 * @param uuid Characteristic UUID
 * @param properties Property flags (read, write, notify, etc.)
 * @return Characteristic handle or -1 on error
 */
int BLE_addCharacteristic(int serviceHandle, uint16_t uuid, uint8_t properties);

/**
 * @brief Start a service
 *
 * @param serviceHandle Service handle
 * @return true if started
 */
bool BLE_startService(int serviceHandle);

/**
 * @brief Set characteristic value
 *
 * @param charHandle Characteristic handle
 * @param value Value data
 * @param len Value length
 * @return true if set
 */
bool BLE_setCharacteristicValue(int charHandle, const uint8_t* value, size_t len);

/**
 * @brief Get characteristic value
 *
 * @param charHandle Characteristic handle
 * @param buffer Buffer to store value
 * @param maxLen Maximum length
 * @return Actual length or -1 on error
 */
int BLE_getCharacteristicValue(int charHandle, uint8_t* buffer, size_t maxLen);

/**
 * @brief Send notification to client
 *
 * @param charHandle Characteristic handle
 * @param value Value data
 * @param len Value length
 * @return true if sent
 */
bool BLE_notify(int charHandle, const uint8_t* value, size_t len);

/**
 * @brief Send indication to client
 *
 * @param charHandle Characteristic handle
 * @param value Value data
 * @param len Value length
 * @return true if sent and acknowledged
 */
bool BLE_indicate(int charHandle, const uint8_t* value, size_t len);

/*----------------------------------------------------------------------------
 * BLE GATT Client (Central)
 *----------------------------------------------------------------------------*/

/**
 * @brief Start scanning for BLE devices
 *
 * @param durationSec Scan duration in seconds (0 for continuous)
 * @return true if started
 */
bool BLE_startScan(uint32_t durationSec);

/**
 * @brief Stop scanning
 */
void BLE_stopScan(void);

/**
 * @brief Connect to a device by address
 *
 * @param addr Device address (6 bytes)
 * @return true if connection initiated
 */
bool BLE_connectToDevice(const uint8_t* addr);

/*----------------------------------------------------------------------------
 * BLE Callbacks
 *----------------------------------------------------------------------------*/

typedef void (*BLE_ConnectCallback_t)(bool connected);
typedef void (*BLE_WriteCallback_t)(int charHandle, const uint8_t* data, size_t len);
typedef void (*BLE_ReadCallback_t)(int charHandle);

/**
 * @brief Set connection callback
 */
void BLE_setConnectCallback(BLE_ConnectCallback_t callback);

/**
 * @brief Set write callback for characteristics
 */
void BLE_setWriteCallback(BLE_WriteCallback_t callback);

/**
 * @brief Set read callback for characteristics
 */
void BLE_setReadCallback(BLE_ReadCallback_t callback);

/*============================================================================
 * Characteristic Properties
 *============================================================================*/

#define BLE_CHAR_PROP_BROADCAST     0x01
#define BLE_CHAR_PROP_READ          0x02
#define BLE_CHAR_PROP_WRITE_NR      0x04  /* Write without response */
#define BLE_CHAR_PROP_WRITE         0x08  /* Write with response */
#define BLE_CHAR_PROP_NOTIFY        0x10
#define BLE_CHAR_PROP_INDICATE      0x20

#ifdef __cplusplus
}
#endif

#endif /* ESP32_BT_H */
