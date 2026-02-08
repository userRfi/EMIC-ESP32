/*****************************************************************************
 * @file    wifi.h
 * @brief   ESP32-C6 WiFi Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details WiFi interface for ESP32-C6 with WiFi 6 (802.11ax) support.
 *          Includes TWT (Target Wake Time) for improved power efficiency.
 *****************************************************************************/

#ifndef ESP32C6_WIFI_H
#define ESP32C6_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define WIFI_SSID_MAX_LEN       32
#define WIFI_PASS_MAX_LEN       64
#define WIFI_IP_ADDR_LEN        16
#define WIFI_MAC_ADDR_LEN       18
#define WIFI_CONNECT_TIMEOUT    30
#define WIFI_MAX_RETRY          10
#define WIFI_AP_MAX_CONNECTIONS 4
#define WIFI_AP_CHANNEL         1

typedef enum { WIFI_MODE_OFF = 0, WIFI_MODE_STA, WIFI_MODE_AP, WIFI_MODE_APSTA } WiFi_Mode_t;
typedef enum { WIFI_STATUS_IDLE = 0, WIFI_STATUS_CONNECTING, WIFI_STATUS_CONNECTED, WIFI_STATUS_DISCONNECTED, WIFI_STATUS_CONNECT_FAILED, WIFI_STATUS_CONNECTION_LOST, WIFI_STATUS_GOT_IP } WiFi_Status_t;
typedef void (*WiFi_EventCallback_t)(WiFi_Status_t status);
typedef struct { char ssid[WIFI_SSID_MAX_LEN + 1]; int8_t rssi; uint8_t channel; uint8_t authmode; } WiFi_NetworkInfo_t;

/*============================================================================
 * Standard WiFi Functions
 *============================================================================*/

void WiFi_init(void);
void WiFi_initMode(WiFi_Mode_t mode);
void WiFi_deinit(void);
bool WiFi_setMode(WiFi_Mode_t mode);
WiFi_Mode_t WiFi_getMode(void);

bool WiFi_connect(const char* ssid, const char* password);
bool WiFi_connectTimeout(const char* ssid, const char* password, uint32_t timeout_sec);
bool WiFi_disconnect(void);
bool WiFi_isConnected(void);
WiFi_Status_t WiFi_getStatus(void);

void WiFi_getIP(char* buffer);
void WiFi_getGateway(char* buffer);
void WiFi_getSubnet(char* buffer);
void WiFi_getMAC(char* buffer);
void WiFi_getSSID(char* buffer);
int8_t WiFi_getRSSI(void);

bool WiFi_startAP(const char* ssid, const char* password);
bool WiFi_startAPEx(const char* ssid, const char* password, uint8_t channel, uint8_t maxConnections);
bool WiFi_stopAP(void);
int WiFi_getAPClientCount(void);
void WiFi_getAPIP(char* buffer);

int WiFi_scan(WiFi_NetworkInfo_t* networks, int maxNetworks);
bool WiFi_scanAsync(void);
bool WiFi_scanComplete(void);
int WiFi_getScanResults(WiFi_NetworkInfo_t* networks, int maxNetworks);

void WiFi_setEventCallback(WiFi_EventCallback_t callback);
void WiFi_setPowerSave(bool enable);
void WiFi_setTxPower(int8_t power_dbm);

bool WiFi_setStaticIP(const char* ip, const char* gateway, const char* subnet);
bool WiFi_enableDHCP(void);
bool WiFi_setHostname(const char* hostname);
void WiFi_getHostname(char* buffer, size_t bufLen);

/*============================================================================
 * WiFi 6 (802.11ax) Extensions for ESP32-C6
 *============================================================================*/

/**
 * @brief Check if connected to a WiFi 6 (802.11ax) network
 */
bool WiFi6_isConnected(void);

/**
 * @brief Enable/disable WiFi 6 features
 * @note WiFi 6 is enabled by default on ESP32-C6
 */
bool WiFi6_enable(bool enable);

/**
 * @brief Enable Target Wake Time (TWT) for power savings
 * @param wake_duration_us Wake duration in microseconds
 * @param wake_interval_us Wake interval in microseconds
 * @return true if TWT setup successful
 */
bool WiFi6_enableTWT(uint32_t wake_duration_us, uint32_t wake_interval_us);

/**
 * @brief Disable Target Wake Time (TWT)
 */
bool WiFi6_disableTWT(void);

/**
 * @brief Check if TWT is active
 */
bool WiFi6_isTWTActive(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C6_WIFI_H */
