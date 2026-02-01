/*****************************************************************************
 * @file    wifi.h
 * @brief   ESP32 WiFi Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details WiFi interface supporting Station and Access Point modes.
 *****************************************************************************/

#ifndef ESP32_WIFI_H
#define ESP32_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define WIFI_SSID_MAX_LEN       32
#define WIFI_PASS_MAX_LEN       64
#define WIFI_IP_ADDR_LEN        16
#define WIFI_MAC_ADDR_LEN       18  /* "XX:XX:XX:XX:XX:XX" + null */

/* Connection timeout in seconds */
#define WIFI_CONNECT_TIMEOUT    30

/* Maximum retry attempts */
#define WIFI_MAX_RETRY          10

/* AP mode configuration */
#define WIFI_AP_MAX_CONNECTIONS 4
#define WIFI_AP_CHANNEL         1

/*============================================================================
 * WiFi Mode
 *============================================================================*/

typedef enum {
    WIFI_MODE_OFF = 0,
    WIFI_MODE_STA,          /* Station mode only */
    WIFI_MODE_AP,           /* Access Point mode only */
    WIFI_MODE_APSTA         /* Both AP and Station */
} WiFi_Mode_t;

/*============================================================================
 * WiFi Status
 *============================================================================*/

typedef enum {
    WIFI_STATUS_IDLE = 0,
    WIFI_STATUS_CONNECTING,
    WIFI_STATUS_CONNECTED,
    WIFI_STATUS_DISCONNECTED,
    WIFI_STATUS_CONNECT_FAILED,
    WIFI_STATUS_CONNECTION_LOST,
    WIFI_STATUS_GOT_IP
} WiFi_Status_t;

/*============================================================================
 * WiFi Event Callback
 *============================================================================*/

typedef void (*WiFi_EventCallback_t)(WiFi_Status_t status);

/*============================================================================
 * WiFi Network Info
 *============================================================================*/

typedef struct {
    char ssid[WIFI_SSID_MAX_LEN + 1];
    int8_t rssi;
    uint8_t channel;
    uint8_t authmode;   /* 0=open, 1=WEP, 2=WPA, 3=WPA2, etc. */
} WiFi_NetworkInfo_t;

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize WiFi subsystem
 *
 * Initializes the WiFi driver in Station mode by default.
 * Must be called before any other WiFi functions.
 */
void WiFi_init(void);

/**
 * @brief Initialize WiFi with specific mode
 *
 * @param mode WiFi mode (STA, AP, or APSTA)
 */
void WiFi_initMode(WiFi_Mode_t mode);

/**
 * @brief Deinitialize WiFi subsystem
 */
void WiFi_deinit(void);

/**
 * @brief Set WiFi mode
 *
 * @param mode New WiFi mode
 * @return true if mode changed successfully
 */
bool WiFi_setMode(WiFi_Mode_t mode);

/**
 * @brief Get current WiFi mode
 *
 * @return Current WiFi mode
 */
WiFi_Mode_t WiFi_getMode(void);

/*============================================================================
 * Station Mode Functions
 *============================================================================*/

/**
 * @brief Connect to a WiFi network
 *
 * @param ssid Network SSID (max 32 chars)
 * @param password Network password (8-64 chars, NULL for open)
 * @return true if connection initiated
 */
bool WiFi_connect(const char* ssid, const char* password);

/**
 * @brief Connect with timeout
 *
 * @param ssid Network SSID
 * @param password Network password
 * @param timeout_sec Connection timeout in seconds
 * @return true if connected within timeout
 */
bool WiFi_connectTimeout(const char* ssid, const char* password, uint32_t timeout_sec);

/**
 * @brief Disconnect from current network
 *
 * @return true if disconnected
 */
bool WiFi_disconnect(void);

/**
 * @brief Check if connected to a network
 *
 * @return true if connected with IP address
 */
bool WiFi_isConnected(void);

/**
 * @brief Get connection status
 *
 * @return Current WiFi status
 */
WiFi_Status_t WiFi_getStatus(void);

/**
 * @brief Get current IP address
 *
 * @param buffer Buffer to store IP string (min 16 bytes)
 */
void WiFi_getIP(char* buffer);

/**
 * @brief Get gateway IP address
 *
 * @param buffer Buffer to store IP string (min 16 bytes)
 */
void WiFi_getGateway(char* buffer);

/**
 * @brief Get subnet mask
 *
 * @param buffer Buffer to store IP string (min 16 bytes)
 */
void WiFi_getSubnet(char* buffer);

/**
 * @brief Get MAC address as string
 *
 * @param buffer Buffer to store MAC string (min 18 bytes)
 */
void WiFi_getMAC(char* buffer);

/**
 * @brief Get connected network SSID
 *
 * @param buffer Buffer to store SSID (min 33 bytes)
 */
void WiFi_getSSID(char* buffer);

/**
 * @brief Get signal strength (RSSI)
 *
 * @return RSSI in dBm, or 0 if not connected
 */
int8_t WiFi_getRSSI(void);

/*============================================================================
 * Access Point Mode Functions
 *============================================================================*/

/**
 * @brief Start Access Point
 *
 * @param ssid AP SSID (1-32 chars)
 * @param password AP password (8-64 chars, NULL for open)
 * @return true if AP started
 */
bool WiFi_startAP(const char* ssid, const char* password);

/**
 * @brief Start AP with custom configuration
 *
 * @param ssid AP SSID
 * @param password AP password
 * @param channel WiFi channel (1-13)
 * @param maxConnections Max clients (1-4)
 * @return true if AP started
 */
bool WiFi_startAPEx(const char* ssid, const char* password,
                     uint8_t channel, uint8_t maxConnections);

/**
 * @brief Stop Access Point
 *
 * @return true if AP stopped
 */
bool WiFi_stopAP(void);

/**
 * @brief Get number of connected stations
 *
 * @return Number of connected clients
 */
int WiFi_getAPClientCount(void);

/**
 * @brief Get AP IP address
 *
 * @param buffer Buffer to store IP string (min 16 bytes)
 */
void WiFi_getAPIP(char* buffer);

/*============================================================================
 * Scanning Functions
 *============================================================================*/

/**
 * @brief Scan for available networks
 *
 * @param networks Array to store found networks
 * @param maxNetworks Maximum networks to find
 * @return Number of networks found
 */
int WiFi_scan(WiFi_NetworkInfo_t* networks, int maxNetworks);

/**
 * @brief Start async network scan
 *
 * @return true if scan started
 */
bool WiFi_scanAsync(void);

/**
 * @brief Check if scan is complete
 *
 * @return true if scan complete
 */
bool WiFi_scanComplete(void);

/**
 * @brief Get async scan results
 *
 * @param networks Array to store found networks
 * @param maxNetworks Maximum networks to return
 * @return Number of networks found
 */
int WiFi_getScanResults(WiFi_NetworkInfo_t* networks, int maxNetworks);

/*============================================================================
 * Event Handling
 *============================================================================*/

/**
 * @brief Set event callback
 *
 * @param callback Function to call on WiFi events
 */
void WiFi_setEventCallback(WiFi_EventCallback_t callback);

/*============================================================================
 * Power Management
 *============================================================================*/

/**
 * @brief Set WiFi power save mode
 *
 * @param enable true to enable power save
 */
void WiFi_setPowerSave(bool enable);

/**
 * @brief Set WiFi transmit power
 *
 * @param power_dbm Transmit power in dBm (8-20)
 */
void WiFi_setTxPower(int8_t power_dbm);

/*============================================================================
 * Static IP Configuration
 *============================================================================*/

/**
 * @brief Configure static IP
 *
 * @param ip IP address string (e.g., "192.168.1.100")
 * @param gateway Gateway address
 * @param subnet Subnet mask
 * @return true if configured
 */
bool WiFi_setStaticIP(const char* ip, const char* gateway, const char* subnet);

/**
 * @brief Enable DHCP (default)
 *
 * @return true if enabled
 */
bool WiFi_enableDHCP(void);

/*============================================================================
 * Hostname
 *============================================================================*/

/**
 * @brief Set hostname
 *
 * @param hostname New hostname
 * @return true if set
 */
bool WiFi_setHostname(const char* hostname);

/**
 * @brief Get hostname
 *
 * @param buffer Buffer to store hostname
 * @param bufLen Buffer length
 */
void WiFi_getHostname(char* buffer, size_t bufLen);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_WIFI_H */
