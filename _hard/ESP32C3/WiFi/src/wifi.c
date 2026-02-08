/*****************************************************************************
 * @file    wifi.c
 * @brief   ESP32-C3 WiFi Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details WiFi implementation for ESP32-C3 using ESP-IDF esp_wifi component.
 *          WiFi 4 (802.11n) support.
 *****************************************************************************/

#include "wifi.h"

/* ESP-IDF includes */
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/ip4_addr.h"
#include <string.h>

static const char *TAG = "WiFi_C3";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool wifi_initialized = false;
static WiFi_Mode_t current_mode = WIFI_MODE_OFF;
static WiFi_Status_t current_status = WIFI_STATUS_IDLE;
static WiFi_EventCallback_t event_callback = NULL;

static esp_netif_t *sta_netif = NULL;
static esp_netif_t *ap_netif = NULL;

/* Event group for connection status */
static EventGroupHandle_t wifi_event_group = NULL;
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

/* Retry counter */
static int retry_count = 0;

/* IP address storage */
static esp_netif_ip_info_t sta_ip_info;
static esp_netif_ip_info_t ap_ip_info;

/*============================================================================
 * Event Handler
 *============================================================================*/

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                int32_t event_id, void* event_data)
{
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START:
                ESP_LOGI(TAG, "Station started");
                break;

            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "Connected to AP");
                current_status = WIFI_STATUS_CONNECTED;
                retry_count = 0;
                if (event_callback) event_callback(WIFI_STATUS_CONNECTED);
                break;

            case WIFI_EVENT_STA_DISCONNECTED:
                if (retry_count < WIFI_MAX_RETRY) {
                    esp_wifi_connect();
                    retry_count++;
                    ESP_LOGI(TAG, "Retry connection (%d/%d)", retry_count, WIFI_MAX_RETRY);
                    current_status = WIFI_STATUS_CONNECTING;
                } else {
                    xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT);
                    current_status = WIFI_STATUS_CONNECT_FAILED;
                    if (event_callback) event_callback(WIFI_STATUS_CONNECT_FAILED);
                }
                ESP_LOGI(TAG, "Disconnected from AP");
                break;

            case WIFI_EVENT_AP_START:
                ESP_LOGI(TAG, "AP started");
                break;

            case WIFI_EVENT_AP_STACONNECTED: {
                wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*)event_data;
                ESP_LOGI(TAG, "Station connected to AP, AID=%d", event->aid);
                break;
            }

            case WIFI_EVENT_AP_STADISCONNECTED: {
                wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*)event_data;
                ESP_LOGI(TAG, "Station disconnected from AP, AID=%d", event->aid);
                break;
            }

            default:
                break;
        }
    } else if (event_base == IP_EVENT) {
        switch (event_id) {
            case IP_EVENT_STA_GOT_IP: {
                ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
                memcpy(&sta_ip_info, &event->ip_info, sizeof(esp_netif_ip_info_t));
                ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
                xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
                current_status = WIFI_STATUS_GOT_IP;
                if (event_callback) event_callback(WIFI_STATUS_GOT_IP);
                break;
            }

            case IP_EVENT_STA_LOST_IP:
                ESP_LOGI(TAG, "Lost IP");
                current_status = WIFI_STATUS_CONNECTION_LOST;
                if (event_callback) event_callback(WIFI_STATUS_CONNECTION_LOST);
                break;

            default:
                break;
        }
    }
}

/*============================================================================
 * Initialization
 *============================================================================*/

void WiFi_init(void)
{
    WiFi_initMode(WIFI_MODE_STA);
}

void WiFi_initMode(WiFi_Mode_t mode)
{
    if (wifi_initialized) {
        return;
    }

    /* Create event group */
    wifi_event_group = xEventGroupCreate();

    /* Initialize TCP/IP stack */
    ESP_ERROR_CHECK(esp_netif_init());

    /* Create network interfaces */
    if (mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA) {
        sta_netif = esp_netif_create_default_wifi_sta();
    }
    if (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA) {
        ap_netif = esp_netif_create_default_wifi_ap();
    }

    /* Initialize WiFi with default config */
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    /* Register event handlers */
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                         ESP_EVENT_ANY_ID,
                                                         &wifi_event_handler,
                                                         NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT,
                                                         IP_EVENT_STA_GOT_IP,
                                                         &wifi_event_handler,
                                                         NULL, NULL));

    /* Set WiFi mode */
    wifi_mode_t esp_mode;
    switch (mode) {
        case WIFI_MODE_STA:   esp_mode = WIFI_MODE_STA; break;
        case WIFI_MODE_AP:    esp_mode = WIFI_MODE_AP; break;
        case WIFI_MODE_APSTA: esp_mode = WIFI_MODE_APSTA; break;
        default:              esp_mode = WIFI_MODE_STA; break;
    }
    ESP_ERROR_CHECK(esp_wifi_set_mode(esp_mode));
    ESP_ERROR_CHECK(esp_wifi_start());

    current_mode = mode;
    wifi_initialized = true;
    ESP_LOGI(TAG, "WiFi initialized in mode %d", mode);
}

void WiFi_deinit(void)
{
    if (!wifi_initialized) {
        return;
    }

    esp_wifi_stop();
    esp_wifi_deinit();

    if (sta_netif) {
        esp_netif_destroy(sta_netif);
        sta_netif = NULL;
    }
    if (ap_netif) {
        esp_netif_destroy(ap_netif);
        ap_netif = NULL;
    }

    if (wifi_event_group) {
        vEventGroupDelete(wifi_event_group);
        wifi_event_group = NULL;
    }

    wifi_initialized = false;
    current_mode = WIFI_MODE_OFF;
    current_status = WIFI_STATUS_IDLE;
    ESP_LOGI(TAG, "WiFi deinitialized");
}

bool WiFi_setMode(WiFi_Mode_t mode)
{
    if (!wifi_initialized) {
        WiFi_initMode(mode);
        return true;
    }

    wifi_mode_t esp_mode;
    switch (mode) {
        case WIFI_MODE_STA:   esp_mode = WIFI_MODE_STA; break;
        case WIFI_MODE_AP:    esp_mode = WIFI_MODE_AP; break;
        case WIFI_MODE_APSTA: esp_mode = WIFI_MODE_APSTA; break;
        default: return false;
    }

    esp_err_t ret = esp_wifi_set_mode(esp_mode);
    if (ret == ESP_OK) {
        current_mode = mode;
        return true;
    }
    return false;
}

WiFi_Mode_t WiFi_getMode(void)
{
    return current_mode;
}

/*============================================================================
 * Station Mode Functions
 *============================================================================*/

bool WiFi_connect(const char* ssid, const char* password)
{
    return WiFi_connectTimeout(ssid, password, WIFI_CONNECT_TIMEOUT);
}

bool WiFi_connectTimeout(const char* ssid, const char* password, uint32_t timeout_sec)
{
    if (!wifi_initialized) {
        WiFi_init();
    }

    wifi_config_t wifi_config = {0};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    if (password != NULL) {
        strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    }
    wifi_config.sta.threshold.authmode = (password != NULL) ? WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN;

    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));

    retry_count = 0;
    current_status = WIFI_STATUS_CONNECTING;
    xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT);

    esp_err_t ret = esp_wifi_connect();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start connection: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "Connecting to %s...", ssid);

    /* Wait for connection */
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group,
                                            WIFI_CONNECTED_BIT | WIFI_FAIL_BIT,
                                            pdFALSE, pdFALSE,
                                            pdMS_TO_TICKS(timeout_sec * 1000));

    if (bits & WIFI_CONNECTED_BIT) {
        ESP_LOGI(TAG, "Connected to %s", ssid);
        return true;
    } else if (bits & WIFI_FAIL_BIT) {
        ESP_LOGI(TAG, "Failed to connect to %s", ssid);
        return false;
    } else {
        ESP_LOGI(TAG, "Connection timeout");
        current_status = WIFI_STATUS_CONNECT_FAILED;
        return false;
    }
}

bool WiFi_disconnect(void)
{
    if (!wifi_initialized) {
        return false;
    }

    esp_err_t ret = esp_wifi_disconnect();
    if (ret == ESP_OK) {
        current_status = WIFI_STATUS_DISCONNECTED;
        return true;
    }
    return false;
}

bool WiFi_isConnected(void)
{
    return (current_status == WIFI_STATUS_GOT_IP);
}

WiFi_Status_t WiFi_getStatus(void)
{
    return current_status;
}

void WiFi_getIP(char* buffer)
{
    if (buffer == NULL) return;

    if (WiFi_isConnected()) {
        sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.ip));
    } else {
        strcpy(buffer, "0.0.0.0");
    }
}

void WiFi_getGateway(char* buffer)
{
    if (buffer == NULL) return;

    if (WiFi_isConnected()) {
        sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.gw));
    } else {
        strcpy(buffer, "0.0.0.0");
    }
}

void WiFi_getSubnet(char* buffer)
{
    if (buffer == NULL) return;

    if (WiFi_isConnected()) {
        sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.netmask));
    } else {
        strcpy(buffer, "0.0.0.0");
    }
}

void WiFi_getMAC(char* buffer)
{
    if (buffer == NULL) return;

    uint8_t mac[6];
    esp_wifi_get_mac(WIFI_IF_STA, mac);
    sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X",
            mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]);
}

void WiFi_getSSID(char* buffer)
{
    if (buffer == NULL) return;

    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        strncpy(buffer, (char*)ap_info.ssid, WIFI_SSID_MAX_LEN);
        buffer[WIFI_SSID_MAX_LEN] = '\0';
    } else {
        buffer[0] = '\0';
    }
}

int8_t WiFi_getRSSI(void)
{
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) {
        return ap_info.rssi;
    }
    return 0;
}

/*============================================================================
 * Access Point Mode Functions
 *============================================================================*/

bool WiFi_startAP(const char* ssid, const char* password)
{
    return WiFi_startAPEx(ssid, password, WIFI_AP_CHANNEL, WIFI_AP_MAX_CONNECTIONS);
}

bool WiFi_startAPEx(const char* ssid, const char* password,
                     uint8_t channel, uint8_t maxConnections)
{
    if (!wifi_initialized) {
        WiFi_initMode(WIFI_MODE_AP);
    }

    wifi_config_t wifi_config = {
        .ap = {
            .channel = channel,
            .max_connection = maxConnections,
            .authmode = (password != NULL && strlen(password) >= 8) ?
                        WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN,
        },
    };

    strncpy((char*)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid) - 1);
    wifi_config.ap.ssid_len = strlen(ssid);

    if (password != NULL && strlen(password) >= 8) {
        strncpy((char*)wifi_config.ap.password, password, sizeof(wifi_config.ap.password) - 1);
    }

    esp_err_t ret = esp_wifi_set_config(WIFI_IF_AP, &wifi_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure AP: %s", esp_err_to_name(ret));
        return false;
    }

    ESP_LOGI(TAG, "AP started: %s", ssid);
    return true;
}

bool WiFi_stopAP(void)
{
    /* Switch to STA mode to stop AP */
    return WiFi_setMode(WIFI_MODE_STA);
}

int WiFi_getAPClientCount(void)
{
    wifi_sta_list_t sta_list;
    if (esp_wifi_ap_get_sta_list(&sta_list) == ESP_OK) {
        return sta_list.num;
    }
    return 0;
}

void WiFi_getAPIP(char* buffer)
{
    if (buffer == NULL) return;

    esp_netif_ip_info_t ip_info;
    if (ap_netif && esp_netif_get_ip_info(ap_netif, &ip_info) == ESP_OK) {
        sprintf(buffer, IPSTR, IP2STR(&ip_info.ip));
    } else {
        strcpy(buffer, "192.168.4.1"); /* Default AP IP */
    }
}

/*============================================================================
 * Scanning Functions
 *============================================================================*/

int WiFi_scan(WiFi_NetworkInfo_t* networks, int maxNetworks)
{
    if (!wifi_initialized) {
        WiFi_init();
    }

    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false,
    };

    esp_err_t ret = esp_wifi_scan_start(&scan_config, true);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Scan failed: %s", esp_err_to_name(ret));
        return 0;
    }

    return WiFi_getScanResults(networks, maxNetworks);
}

bool WiFi_scanAsync(void)
{
    if (!wifi_initialized) {
        WiFi_init();
    }

    wifi_scan_config_t scan_config = {
        .ssid = NULL,
        .bssid = NULL,
        .channel = 0,
        .show_hidden = false,
    };

    return (esp_wifi_scan_start(&scan_config, false) == ESP_OK);
}

bool WiFi_scanComplete(void)
{
    uint16_t ap_count = 0;
    return (esp_wifi_scan_get_ap_num(&ap_count) == ESP_OK && ap_count > 0);
}

int WiFi_getScanResults(WiFi_NetworkInfo_t* networks, int maxNetworks)
{
    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);

    if (ap_count == 0 || networks == NULL) {
        return 0;
    }

    if (ap_count > maxNetworks) {
        ap_count = maxNetworks;
    }

    wifi_ap_record_t* ap_records = malloc(sizeof(wifi_ap_record_t) * ap_count);
    if (ap_records == NULL) {
        return 0;
    }

    uint16_t returned = ap_count;
    esp_wifi_scan_get_ap_records(&returned, ap_records);

    for (int i = 0; i < returned; i++) {
        strncpy(networks[i].ssid, (char*)ap_records[i].ssid, WIFI_SSID_MAX_LEN);
        networks[i].ssid[WIFI_SSID_MAX_LEN] = '\0';
        networks[i].rssi = ap_records[i].rssi;
        networks[i].channel = ap_records[i].primary;
        networks[i].authmode = ap_records[i].authmode;
    }

    free(ap_records);
    return returned;
}

/*============================================================================
 * Event Handling
 *============================================================================*/

void WiFi_setEventCallback(WiFi_EventCallback_t callback)
{
    event_callback = callback;
}

/*============================================================================
 * Power Management
 *============================================================================*/

void WiFi_setPowerSave(bool enable)
{
    if (enable) {
        esp_wifi_set_ps(WIFI_PS_MIN_MODEM);
    } else {
        esp_wifi_set_ps(WIFI_PS_NONE);
    }
}

void WiFi_setTxPower(int8_t power_dbm)
{
    /* Convert dBm to 0.25dBm units */
    int8_t power = power_dbm * 4;
    esp_wifi_set_max_tx_power(power);
}

/*============================================================================
 * Static IP Configuration
 *============================================================================*/

bool WiFi_setStaticIP(const char* ip, const char* gateway, const char* subnet)
{
    if (sta_netif == NULL) return false;

    esp_netif_dhcpc_stop(sta_netif);

    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr = esp_ip4addr_aton(ip);
    ip_info.gw.addr = esp_ip4addr_aton(gateway);
    ip_info.netmask.addr = esp_ip4addr_aton(subnet);

    return (esp_netif_set_ip_info(sta_netif, &ip_info) == ESP_OK);
}

bool WiFi_enableDHCP(void)
{
    if (sta_netif == NULL) return false;
    return (esp_netif_dhcpc_start(sta_netif) == ESP_OK);
}

/*============================================================================
 * Hostname
 *============================================================================*/

bool WiFi_setHostname(const char* hostname)
{
    if (sta_netif == NULL) return false;
    return (esp_netif_set_hostname(sta_netif, hostname) == ESP_OK);
}

void WiFi_getHostname(char* buffer, size_t bufLen)
{
    if (buffer == NULL || sta_netif == NULL) return;

    const char* hostname = NULL;
    if (esp_netif_get_hostname(sta_netif, &hostname) == ESP_OK && hostname != NULL) {
        strncpy(buffer, hostname, bufLen - 1);
        buffer[bufLen - 1] = '\0';
    } else {
        buffer[0] = '\0';
    }
}
