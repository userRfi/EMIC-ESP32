/*****************************************************************************
 * @file    usb.c
 * @brief   ESP32-S2 USB OTG Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details USB OTG implementation for ESP32-S2 using TinyUSB stack.
 *****************************************************************************/

#include "usb.h"
#include "esp_log.h"
#include "tinyusb.h"
#include "tusb_cdc_acm.h"
#include <string.h>

static const char* TAG = "USB_S2";

/* USB state */
static bool usb_initialized = false;
static usb_mode_t current_mode = USB_MODE_NONE;
static uint32_t current_baud_rate = 115200;

/* Line coding callback */
static void (*line_coding_cb)(uint32_t, uint8_t, uint8_t, uint8_t) = NULL;

/*============================================================================
 * TinyUSB Callbacks
 *============================================================================*/

void tud_cdc_line_coding_cb(uint8_t itf, cdc_line_coding_t const* p_line_coding)
{
    (void)itf;
    current_baud_rate = p_line_coding->bit_rate;

    if (line_coding_cb != NULL) {
        line_coding_cb(p_line_coding->bit_rate,
                       p_line_coding->stop_bits,
                       p_line_coding->parity,
                       p_line_coding->data_bits);
    }
}

/*============================================================================
 * USB Implementation
 *============================================================================*/

void USB_init(void)
{
    USB_initMode(USB_MODE_CDC);
}

int USB_initMode(usb_mode_t mode)
{
    if (usb_initialized) {
        ESP_LOGW(TAG, "USB already initialized");
        return 0;
    }

    ESP_LOGI(TAG, "Initializing USB (mode: %d)", mode);

    /* TinyUSB configuration */
    const tinyusb_config_t tusb_cfg = {
        .device_descriptor = NULL,          /* Use default */
        .string_descriptor = NULL,          /* Use default */
        .external_phy = false,
        .configuration_descriptor = NULL,   /* Use default */
    };

    esp_err_t ret = tinyusb_driver_install(&tusb_cfg);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "TinyUSB driver install failed: %d", ret);
        return -1;
    }

    if (mode == USB_MODE_CDC || mode == USB_MODE_COMPOSITE) {
        /* Initialize CDC ACM */
        tinyusb_config_cdcacm_t acm_cfg = {
            .usb_dev = TINYUSB_USBDEV_0,
            .cdc_port = TINYUSB_CDC_ACM_0,
            .rx_unread_buf_sz = USB_CDC_RX_BUFFER_SIZE,
            .callback_rx = NULL,
            .callback_rx_wanted_char = NULL,
            .callback_line_state_changed = NULL,
            .callback_line_coding_changed = NULL,
        };

        ret = tusb_cdc_acm_init(&acm_cfg);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "CDC ACM init failed: %d", ret);
            return -1;
        }
    }

    current_mode = mode;
    usb_initialized = true;

    ESP_LOGI(TAG, "USB initialized successfully");
    return 0;
}

void USB_deinit(void)
{
    if (!usb_initialized) {
        return;
    }

    /* Note: TinyUSB doesn't have a clean uninstall in ESP-IDF */
    usb_initialized = false;
    current_mode = USB_MODE_NONE;

    ESP_LOGI(TAG, "USB deinitialized");
}

bool USB_isConnected(void)
{
    if (!usb_initialized) {
        return false;
    }
    return tud_connected();
}

bool USB_cdcReady(void)
{
    if (!usb_initialized || current_mode != USB_MODE_CDC) {
        return false;
    }
    return tud_cdc_connected();
}

int USB_write(const uint8_t* data, size_t len)
{
    if (!usb_initialized || data == NULL || len == 0) {
        return -1;
    }

    if (!USB_cdcReady()) {
        ESP_LOGW(TAG, "CDC not ready for write");
        return 0;
    }

    size_t written = tinyusb_cdcacm_write_queue(TINYUSB_CDC_ACM_0, data, len);
    tinyusb_cdcacm_write_flush(TINYUSB_CDC_ACM_0, 0);

    return (int)written;
}

int USB_writeString(const char* str)
{
    if (str == NULL) {
        return -1;
    }
    return USB_write((const uint8_t*)str, strlen(str));
}

int USB_read(uint8_t* buffer, size_t len)
{
    if (!usb_initialized || buffer == NULL || len == 0) {
        return -1;
    }

    size_t rx_size = 0;
    esp_err_t ret = tinyusb_cdcacm_read(TINYUSB_CDC_ACM_0, buffer, len, &rx_size);
    if (ret != ESP_OK) {
        return -1;
    }

    return (int)rx_size;
}

int USB_available(void)
{
    if (!usb_initialized) {
        return 0;
    }

    /* TinyUSB doesn't directly expose available bytes,
       so we check if there's any data */
    return tud_cdc_available();
}

void USB_flush(void)
{
    if (!usb_initialized) {
        return;
    }
    tinyusb_cdcacm_write_flush(TINYUSB_CDC_ACM_0, pdMS_TO_TICKS(100));
}

void USB_setLineCodingCallback(void (*callback)(uint32_t baud, uint8_t stop_bits,
                                                 uint8_t parity, uint8_t data_bits))
{
    line_coding_cb = callback;
}

uint32_t USB_getBaudRate(void)
{
    return current_baud_rate;
}
