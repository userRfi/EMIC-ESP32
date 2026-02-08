/*****************************************************************************
 * @file    wifi.c
 * @brief   ESP32-C6 WiFi Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details WiFi implementation for ESP32-C6 with WiFi 6 (802.11ax) support.
 *****************************************************************************/

#include "wifi.h"

#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/event_groups.h"
#include "lwip/ip4_addr.h"
#include <string.h>

static const char *TAG = "WiFi_C6";

static bool wifi_initialized = false;
static WiFi_Mode_t current_mode = WIFI_MODE_OFF;
static WiFi_Status_t current_status = WIFI_STATUS_IDLE;
static WiFi_EventCallback_t event_callback = NULL;

static esp_netif_t *sta_netif = NULL;
static esp_netif_t *ap_netif = NULL;

static EventGroupHandle_t wifi_event_group = NULL;
#define WIFI_CONNECTED_BIT  BIT0
#define WIFI_FAIL_BIT       BIT1

static int retry_count = 0;
static esp_netif_ip_info_t sta_ip_info;

/* WiFi 6 state */
static bool wifi6_enabled = true;  /* WiFi 6 enabled by default on C6 */
static bool twt_active = false;

static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT) {
        switch (event_id) {
            case WIFI_EVENT_STA_START: ESP_LOGI(TAG, "Station started (WiFi 6)"); break;
            case WIFI_EVENT_STA_CONNECTED:
                ESP_LOGI(TAG, "Connected to AP");
                current_status = WIFI_STATUS_CONNECTED;
                retry_count = 0;
                if (event_callback) event_callback(WIFI_STATUS_CONNECTED);
                break;
            case WIFI_EVENT_STA_DISCONNECTED:
                twt_active = false;  /* TWT is disconnected on WiFi disconnect */
                if (retry_count < WIFI_MAX_RETRY) { esp_wifi_connect(); retry_count++; current_status = WIFI_STATUS_CONNECTING; }
                else { xEventGroupSetBits(wifi_event_group, WIFI_FAIL_BIT); current_status = WIFI_STATUS_CONNECT_FAILED; if (event_callback) event_callback(WIFI_STATUS_CONNECT_FAILED); }
                break;
            case WIFI_EVENT_AP_START: ESP_LOGI(TAG, "AP started"); break;
            case WIFI_EVENT_AP_STACONNECTED: { wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*)event_data; ESP_LOGI(TAG, "Station connected, AID=%d", event->aid); break; }
            case WIFI_EVENT_AP_STADISCONNECTED: { wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*)event_data; ESP_LOGI(TAG, "Station disconnected, AID=%d", event->aid); break; }
            default: break;
        }
    } else if (event_base == IP_EVENT) {
        if (event_id == IP_EVENT_STA_GOT_IP) {
            ip_event_got_ip_t* event = (ip_event_got_ip_t*)event_data;
            memcpy(&sta_ip_info, &event->ip_info, sizeof(esp_netif_ip_info_t));
            ESP_LOGI(TAG, "Got IP: " IPSTR, IP2STR(&event->ip_info.ip));
            xEventGroupSetBits(wifi_event_group, WIFI_CONNECTED_BIT);
            current_status = WIFI_STATUS_GOT_IP;
            if (event_callback) event_callback(WIFI_STATUS_GOT_IP);
        } else if (event_id == IP_EVENT_STA_LOST_IP) {
            current_status = WIFI_STATUS_CONNECTION_LOST;
            if (event_callback) event_callback(WIFI_STATUS_CONNECTION_LOST);
        }
    }
}

void WiFi_init(void) { WiFi_initMode(WIFI_MODE_STA); }

void WiFi_initMode(WiFi_Mode_t mode) {
    if (wifi_initialized) return;
    wifi_event_group = xEventGroupCreate();
    ESP_ERROR_CHECK(esp_netif_init());
    if (mode == WIFI_MODE_STA || mode == WIFI_MODE_APSTA) sta_netif = esp_netif_create_default_wifi_sta();
    if (mode == WIFI_MODE_AP || mode == WIFI_MODE_APSTA) ap_netif = esp_netif_create_default_wifi_ap();
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL, NULL));
    ESP_ERROR_CHECK(esp_event_handler_instance_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL, NULL));
    wifi_mode_t esp_mode;
    switch (mode) { case WIFI_MODE_STA: esp_mode = WIFI_MODE_STA; break; case WIFI_MODE_AP: esp_mode = WIFI_MODE_AP; break; case WIFI_MODE_APSTA: esp_mode = WIFI_MODE_APSTA; break; default: esp_mode = WIFI_MODE_STA; break; }
    ESP_ERROR_CHECK(esp_wifi_set_mode(esp_mode));
    ESP_ERROR_CHECK(esp_wifi_start());
    current_mode = mode;
    wifi_initialized = true;
    ESP_LOGI(TAG, "WiFi 6 (802.11ax) initialized in mode %d", mode);
}

void WiFi_deinit(void) {
    if (!wifi_initialized) return;
    twt_active = false;
    esp_wifi_stop();
    esp_wifi_deinit();
    if (sta_netif) { esp_netif_destroy(sta_netif); sta_netif = NULL; }
    if (ap_netif) { esp_netif_destroy(ap_netif); ap_netif = NULL; }
    if (wifi_event_group) { vEventGroupDelete(wifi_event_group); wifi_event_group = NULL; }
    wifi_initialized = false;
    current_mode = WIFI_MODE_OFF;
    current_status = WIFI_STATUS_IDLE;
}

bool WiFi_setMode(WiFi_Mode_t mode) {
    if (!wifi_initialized) { WiFi_initMode(mode); return true; }
    wifi_mode_t esp_mode;
    switch (mode) { case WIFI_MODE_STA: esp_mode = WIFI_MODE_STA; break; case WIFI_MODE_AP: esp_mode = WIFI_MODE_AP; break; case WIFI_MODE_APSTA: esp_mode = WIFI_MODE_APSTA; break; default: return false; }
    if (esp_wifi_set_mode(esp_mode) == ESP_OK) { current_mode = mode; return true; }
    return false;
}

WiFi_Mode_t WiFi_getMode(void) { return current_mode; }

bool WiFi_connect(const char* ssid, const char* password) { return WiFi_connectTimeout(ssid, password, WIFI_CONNECT_TIMEOUT); }

bool WiFi_connectTimeout(const char* ssid, const char* password, uint32_t timeout_sec) {
    if (!wifi_initialized) WiFi_init();
    wifi_config_t wifi_config = {0};
    strncpy((char*)wifi_config.sta.ssid, ssid, sizeof(wifi_config.sta.ssid) - 1);
    if (password) strncpy((char*)wifi_config.sta.password, password, sizeof(wifi_config.sta.password) - 1);
    wifi_config.sta.threshold.authmode = password ? WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN;
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_STA, &wifi_config));
    retry_count = 0;
    current_status = WIFI_STATUS_CONNECTING;
    xEventGroupClearBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT);
    if (esp_wifi_connect() != ESP_OK) return false;
    EventBits_t bits = xEventGroupWaitBits(wifi_event_group, WIFI_CONNECTED_BIT | WIFI_FAIL_BIT, pdFALSE, pdFALSE, pdMS_TO_TICKS(timeout_sec * 1000));
    if (bits & WIFI_CONNECTED_BIT) return true;
    if (bits & WIFI_FAIL_BIT) return false;
    current_status = WIFI_STATUS_CONNECT_FAILED;
    return false;
}

bool WiFi_disconnect(void) {
    if (!wifi_initialized) return false;
    twt_active = false;
    if (esp_wifi_disconnect() == ESP_OK) { current_status = WIFI_STATUS_DISCONNECTED; return true; }
    return false;
}

bool WiFi_isConnected(void) { return current_status == WIFI_STATUS_GOT_IP; }
WiFi_Status_t WiFi_getStatus(void) { return current_status; }

void WiFi_getIP(char* buffer) { if (!buffer) return; if (WiFi_isConnected()) sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.ip)); else strcpy(buffer, "0.0.0.0"); }
void WiFi_getGateway(char* buffer) { if (!buffer) return; if (WiFi_isConnected()) sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.gw)); else strcpy(buffer, "0.0.0.0"); }
void WiFi_getSubnet(char* buffer) { if (!buffer) return; if (WiFi_isConnected()) sprintf(buffer, IPSTR, IP2STR(&sta_ip_info.netmask)); else strcpy(buffer, "0.0.0.0"); }
void WiFi_getMAC(char* buffer) { if (!buffer) return; uint8_t mac[6]; esp_wifi_get_mac(WIFI_IF_STA, mac); sprintf(buffer, "%02X:%02X:%02X:%02X:%02X:%02X", mac[0], mac[1], mac[2], mac[3], mac[4], mac[5]); }
void WiFi_getSSID(char* buffer) { if (!buffer) return; wifi_ap_record_t ap_info; if (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) { strncpy(buffer, (char*)ap_info.ssid, WIFI_SSID_MAX_LEN); buffer[WIFI_SSID_MAX_LEN] = '\0'; } else buffer[0] = '\0'; }
int8_t WiFi_getRSSI(void) { wifi_ap_record_t ap_info; return (esp_wifi_sta_get_ap_info(&ap_info) == ESP_OK) ? ap_info.rssi : 0; }

bool WiFi_startAP(const char* ssid, const char* password) { return WiFi_startAPEx(ssid, password, WIFI_AP_CHANNEL, WIFI_AP_MAX_CONNECTIONS); }

bool WiFi_startAPEx(const char* ssid, const char* password, uint8_t channel, uint8_t maxConnections) {
    if (!wifi_initialized) WiFi_initMode(WIFI_MODE_AP);
    wifi_config_t wifi_config = { .ap = { .channel = channel, .max_connection = maxConnections, .authmode = (password && strlen(password) >= 8) ? WIFI_AUTH_WPA2_PSK : WIFI_AUTH_OPEN } };
    strncpy((char*)wifi_config.ap.ssid, ssid, sizeof(wifi_config.ap.ssid) - 1);
    wifi_config.ap.ssid_len = strlen(ssid);
    if (password && strlen(password) >= 8) strncpy((char*)wifi_config.ap.password, password, sizeof(wifi_config.ap.password) - 1);
    return esp_wifi_set_config(WIFI_IF_AP, &wifi_config) == ESP_OK;
}

bool WiFi_stopAP(void) { return WiFi_setMode(WIFI_MODE_STA); }
int WiFi_getAPClientCount(void) { wifi_sta_list_t sta_list; return (esp_wifi_ap_get_sta_list(&sta_list) == ESP_OK) ? sta_list.num : 0; }
void WiFi_getAPIP(char* buffer) { if (!buffer) return; esp_netif_ip_info_t ip_info; if (ap_netif && esp_netif_get_ip_info(ap_netif, &ip_info) == ESP_OK) sprintf(buffer, IPSTR, IP2STR(&ip_info.ip)); else strcpy(buffer, "192.168.4.1"); }

int WiFi_scan(WiFi_NetworkInfo_t* networks, int maxNetworks) {
    if (!wifi_initialized) WiFi_init();
    wifi_scan_config_t scan_config = {0};
    if (esp_wifi_scan_start(&scan_config, true) != ESP_OK) return 0;
    return WiFi_getScanResults(networks, maxNetworks);
}

bool WiFi_scanAsync(void) { if (!wifi_initialized) WiFi_init(); wifi_scan_config_t scan_config = {0}; return esp_wifi_scan_start(&scan_config, false) == ESP_OK; }
bool WiFi_scanComplete(void) { uint16_t ap_count = 0; return esp_wifi_scan_get_ap_num(&ap_count) == ESP_OK && ap_count > 0; }

int WiFi_getScanResults(WiFi_NetworkInfo_t* networks, int maxNetworks) {
    uint16_t ap_count = 0;
    esp_wifi_scan_get_ap_num(&ap_count);
    if (ap_count == 0 || !networks) return 0;
    if (ap_count > maxNetworks) ap_count = maxNetworks;
    wifi_ap_record_t* ap_records = malloc(sizeof(wifi_ap_record_t) * ap_count);
    if (!ap_records) return 0;
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

void WiFi_setEventCallback(WiFi_EventCallback_t callback) { event_callback = callback; }
void WiFi_setPowerSave(bool enable) { esp_wifi_set_ps(enable ? WIFI_PS_MIN_MODEM : WIFI_PS_NONE); }
void WiFi_setTxPower(int8_t power_dbm) { esp_wifi_set_max_tx_power(power_dbm * 4); }

bool WiFi_setStaticIP(const char* ip, const char* gateway, const char* subnet) {
    if (!sta_netif) return false;
    esp_netif_dhcpc_stop(sta_netif);
    esp_netif_ip_info_t ip_info;
    ip_info.ip.addr = esp_ip4addr_aton(ip);
    ip_info.gw.addr = esp_ip4addr_aton(gateway);
    ip_info.netmask.addr = esp_ip4addr_aton(subnet);
    return esp_netif_set_ip_info(sta_netif, &ip_info) == ESP_OK;
}

bool WiFi_enableDHCP(void) { return sta_netif && esp_netif_dhcpc_start(sta_netif) == ESP_OK; }
bool WiFi_setHostname(const char* hostname) { return sta_netif && esp_netif_set_hostname(sta_netif, hostname) == ESP_OK; }
void WiFi_getHostname(char* buffer, size_t bufLen) { if (!buffer || !sta_netif) return; const char* hostname = NULL; if (esp_netif_get_hostname(sta_netif, &hostname) == ESP_OK && hostname) strncpy(buffer, hostname, bufLen - 1); else buffer[0] = '\0'; }

/*============================================================================
 * WiFi 6 (802.11ax) Extensions for ESP32-C6
 *============================================================================*/

bool WiFi6_isConnected(void) {
    if (!WiFi_isConnected()) return false;
    wifi_ap_record_t ap_info;
    if (esp_wifi_sta_get_ap_info(&ap_info) != ESP_OK) return false;
    /* Check if connected AP supports WiFi 6 (802.11ax) */
    /* The phy_11ax field indicates WiFi 6 support */
#ifdef CONFIG_SOC_WIFI_HE_SUPPORT
    return ap_info.phy_11ax;
#else
    return false;
#endif
}

bool WiFi6_enable(bool enable) {
    wifi6_enabled = enable;
    ESP_LOGI(TAG, "WiFi 6 %s", enable ? "enabled" : "disabled");
    /* Note: On ESP32-C6, WiFi 6 is hardware-enabled and always active when
     * connecting to WiFi 6 APs. This function mainly tracks user preference. */
    return true;
}

bool WiFi6_enableTWT(uint32_t wake_duration_us, uint32_t wake_interval_us) {
    if (!WiFi6_isConnected()) {
        ESP_LOGW(TAG, "TWT requires WiFi 6 connection");
        return false;
    }

#ifdef CONFIG_SOC_WIFI_HE_SUPPORT
    wifi_twt_setup_config_t twt_setup_config = {
        .setup_cmd = TWT_REQUEST,
        .flow_id = 0,
        .twt_id = 0,
        .flow_type = 0,  /* Announced TWT */
        .min_wake_dura = (uint8_t)(wake_duration_us / 256),  /* In 256us units */
        .wake_invl_expn = 10,  /* Exponent for wake interval */
        .wake_invl_mant = (uint16_t)(wake_interval_us / 1024),  /* Mantissa */
        .trigger = 1,
        .timeout_time_ms = 5000,
    };

    esp_err_t ret = esp_wifi_sta_itwt_setup(&twt_setup_config);
    if (ret == ESP_OK) {
        twt_active = true;
        ESP_LOGI(TAG, "TWT enabled: wake=%lu us, interval=%lu us", wake_duration_us, wake_interval_us);
        return true;
    }
    ESP_LOGE(TAG, "TWT setup failed: %s", esp_err_to_name(ret));
#else
    ESP_LOGW(TAG, "TWT not supported in this build");
#endif
    return false;
}

bool WiFi6_disableTWT(void) {
    if (!twt_active) return true;

#ifdef CONFIG_SOC_WIFI_HE_SUPPORT
    wifi_twt_setup_config_t twt_teardown = {
        .setup_cmd = TWT_TEARDOWN,
        .flow_id = 0,
        .twt_id = 0,
    };

    esp_err_t ret = esp_wifi_sta_itwt_setup(&twt_teardown);
    if (ret == ESP_OK) {
        twt_active = false;
        ESP_LOGI(TAG, "TWT disabled");
        return true;
    }
    ESP_LOGE(TAG, "TWT teardown failed: %s", esp_err_to_name(ret));
#endif
    twt_active = false;
    return true;
}

bool WiFi6_isTWTActive(void) {
    return twt_active;
}
