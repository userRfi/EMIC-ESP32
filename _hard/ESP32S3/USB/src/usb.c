/*****************************************************************************
 * @file    usb.c
 * @brief   ESP32-S3 USB OTG Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details USB OTG implementation for ESP32-S3 using TinyUSB.
 *****************************************************************************/

#include "usb.h"

/* ESP-IDF and TinyUSB includes */
#include "esp_log.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include "tusb_console.h"
#include <string.h>

static const char *TAG = "USB";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool usb_initialized = false;
static uint8_t usb_mode = USB_MODE_CDC;

/* Callbacks */
static usb_connect_cb_t connect_callback = NULL;
static usb_disconnect_cb_t disconnect_callback = NULL;
static usb_rx_cb_t rx_callback = NULL;

/* Receive buffer */
static uint8_t rx_buffer[USB_RX_BUFFER_SIZE];
static size_t rx_buffer_len = 0;

/*============================================================================
 * TinyUSB Callbacks
 *============================================================================*/

void tud_mount_cb(void)
{
    ESP_LOGI(TAG, "USB mounted");
    if (connect_callback) {
        connect_callback();
    }
}

void tud_umount_cb(void)
{
    ESP_LOGI(TAG, "USB unmounted");
    if (disconnect_callback) {
        disconnect_callback();
    }
}

void tud_cdc_rx_cb(uint8_t itf)
{
    (void)itf;

    /* Read available data */
    size_t available = tud_cdc_n_available(itf);
    if (available > 0) {
        size_t to_read = (available < USB_RX_BUFFER_SIZE - rx_buffer_len) ?
                         available : (USB_RX_BUFFER_SIZE - rx_buffer_len);

        if (to_read > 0) {
            size_t read = tud_cdc_n_read(itf, rx_buffer + rx_buffer_len, to_read);
            rx_buffer_len += read;

            if (rx_callback) {
                rx_callback(rx_buffer + rx_buffer_len - read, read);
            }
        }
    }
}

void tud_cdc_line_state_cb(uint8_t itf, bool dtr, bool rts)
{
    (void)itf;
    (void)rts;

    if (dtr) {
        ESP_LOGI(TAG, "Terminal connected");
    } else {
        ESP_LOGI(TAG, "Terminal disconnected");
    }
}

/*============================================================================
 * Initialization
 *============================================================================*/

void USB_init(void)
{
    USB_initMode(USB_MODE_CDC);
}

void USB_initMode(uint8_t mode)
{
    if (usb_initialized) {
        return;
    }

    usb_mode = mode;

    /* TinyUSB configuration */
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,  /* Use default */
        .string_descriptor = NULL,  /* Use default */
        .external_phy = false,
        .configuration_descriptor = NULL,  /* Use default */
    };

    esp_err_t ret = tinyusb_driver_install(&tusb_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to install TinyUSB: %s", esp_err_to_name(ret));
        return;
    }

    if (mode == USB_MODE_CDC) {
        /* Initialize CDC ACM */
        tinyusb_config_cdcacm_t acm_cfg = {
            .usb_dev = TINYUSB_USBDEV_0,
            .cdc_port = TINYUSB_CDC_ACM_0,
            .rx_unread_buf_sz = USB_RX_BUFFER_SIZE,
            .callback_rx = NULL,
            .callback_rx_wanted_char = NULL,
            .callback_line_state_changed = NULL,
            .callback_line_coding_changed = NULL,
        };

        ret = tusb_cdc_acm_init(&acm_cfg);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to init CDC ACM: %s", esp_err_to_name(ret));
            return;
        }
    }

    usb_initialized = true;
    ESP_LOGI(TAG, "USB initialized (ESP32-S3 OTG, mode=%d)", mode);
}

void USB_deinit(void)
{
    if (!usb_initialized) {
        return;
    }

    tinyusb_driver_uninstall();
    usb_initialized = false;
    rx_buffer_len = 0;

    ESP_LOGI(TAG, "USB deinitialized");
}

/*============================================================================
 * Connection Status
 *============================================================================*/

bool USB_isConnected(void)
{
    if (!usb_initialized) {
        return false;
    }
    return tud_connected();
}

bool USB_isMounted(void)
{
    if (!usb_initialized) {
        return false;
    }
    return tud_mounted();
}

bool USB_isDTR(void)
{
    if (!usb_initialized || usb_mode != USB_MODE_CDC) {
        return false;
    }
    return tud_cdc_connected();
}

/*============================================================================
 * CDC Write Functions
 *============================================================================*/

int USB_write(const uint8_t* data, size_t len)
{
    if (!usb_initialized || usb_mode != USB_MODE_CDC) {
        return 0;
    }

    if (!tud_cdc_connected()) {
        return 0;
    }

    return tud_cdc_write(data, len);
}

void USB_putChar(uint8_t c)
{
    USB_write(&c, 1);
}

void USB_print(const char* str)
{
    if (str) {
        USB_write((const uint8_t*)str, strlen(str));
    }
}

void USB_println(const char* str)
{
    USB_print(str);
    USB_print("\r\n");
}

void USB_flush(void)
{
    if (!usb_initialized || usb_mode != USB_MODE_CDC) {
        return;
    }

    tud_cdc_write_flush();
}

/*============================================================================
 * CDC Read Functions
 *============================================================================*/

int USB_read(uint8_t* buffer, size_t len)
{
    if (!usb_initialized || usb_mode != USB_MODE_CDC) {
        return 0;
    }

    /* First check internal buffer */
    if (rx_buffer_len > 0) {
        size_t to_copy = (len < rx_buffer_len) ? len : rx_buffer_len;
        memcpy(buffer, rx_buffer, to_copy);

        /* Shift remaining data */
        if (to_copy < rx_buffer_len) {
            memmove(rx_buffer, rx_buffer + to_copy, rx_buffer_len - to_copy);
        }
        rx_buffer_len -= to_copy;

        return to_copy;
    }

    /* Read directly from CDC */
    return tud_cdc_read(buffer, len);
}

int USB_getChar(void)
{
    uint8_t c;
    if (USB_read(&c, 1) > 0) {
        return c;
    }
    return -1;
}

int USB_available(void)
{
    if (!usb_initialized || usb_mode != USB_MODE_CDC) {
        return 0;
    }

    return rx_buffer_len + tud_cdc_available();
}

int USB_readLine(char* buffer, size_t maxLen)
{
    size_t idx = 0;

    while (idx < maxLen - 1) {
        int c = USB_getChar();
        if (c < 0) {
            break;  /* No more data */
        }

        if (c == '\n') {
            break;  /* End of line */
        }

        if (c != '\r') {
            buffer[idx++] = (char)c;
        }
    }

    buffer[idx] = '\0';
    return idx;
}

/*============================================================================
 * USB Device Configuration
 *============================================================================*/

void USB_setDescriptor(const char* manufacturer, const char* product, const char* serial)
{
    /* Note: Must be called before USB_init() to take effect */
    (void)manufacturer;
    (void)product;
    (void)serial;
    ESP_LOGW(TAG, "Descriptor setting not implemented - use before init");
}

void USB_setVidPid(uint16_t vid, uint16_t pid)
{
    /* Note: Must be called before USB_init() to take effect */
    (void)vid;
    (void)pid;
    ESP_LOGW(TAG, "VID/PID setting not implemented - use before init");
}

/*============================================================================
 * Callbacks
 *============================================================================*/

void USB_setConnectCallback(usb_connect_cb_t callback)
{
    connect_callback = callback;
}

void USB_setDisconnectCallback(usb_disconnect_cb_t callback)
{
    disconnect_callback = callback;
}

void USB_setReceiveCallback(usb_rx_cb_t callback)
{
    rx_callback = callback;
}
