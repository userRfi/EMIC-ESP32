/*****************************************************************************
 * @file    usb.h
 * @brief   ESP32-S3 USB OTG Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details USB OTG interface for ESP32-S3 using TinyUSB.
 *          Supports CDC (serial) and HID modes.
 *****************************************************************************/

#ifndef ESP32S3_USB_H
#define ESP32S3_USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/* USB Mode */
#define USB_MODE_CDC            0   /* USB CDC (Serial) */
#define USB_MODE_HID            1   /* USB HID */
#define USB_MODE_MSC            2   /* USB Mass Storage */

/* USB VID/PID (defaults) */
#define USB_VID_DEFAULT         0x303A  /* Espressif VID */
#define USB_PID_DEFAULT         0x1001  /* Default PID */

/* Buffer sizes */
#define USB_TX_BUFFER_SIZE      1024
#define USB_RX_BUFFER_SIZE      1024

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize USB subsystem in CDC mode
 */
void USB_init(void);

/**
 * @brief Initialize USB with specific mode
 *
 * @param mode USB mode (USB_MODE_CDC, USB_MODE_HID, USB_MODE_MSC)
 */
void USB_initMode(uint8_t mode);

/**
 * @brief Deinitialize USB
 */
void USB_deinit(void);

/*============================================================================
 * Connection Status
 *============================================================================*/

/**
 * @brief Check if USB is connected to host
 *
 * @return true if connected
 */
bool USB_isConnected(void);

/**
 * @brief Check if USB is mounted by host
 *
 * @return true if mounted
 */
bool USB_isMounted(void);

/**
 * @brief Check if DTR signal is set (terminal open)
 *
 * @return true if DTR is set
 */
bool USB_isDTR(void);

/*============================================================================
 * CDC Write Functions
 *============================================================================*/

/**
 * @brief Write data to USB CDC
 *
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return Number of bytes written
 */
int USB_write(const uint8_t* data, size_t len);

/**
 * @brief Write a single byte
 *
 * @param c Byte to write
 */
void USB_putChar(uint8_t c);

/**
 * @brief Write a null-terminated string
 *
 * @param str String to write
 */
void USB_print(const char* str);

/**
 * @brief Write a string with newline
 *
 * @param str String to write
 */
void USB_println(const char* str);

/**
 * @brief Flush TX buffer
 */
void USB_flush(void);

/*============================================================================
 * CDC Read Functions
 *============================================================================*/

/**
 * @brief Read data from USB CDC
 *
 * @param buffer Buffer to store data
 * @param len Maximum bytes to read
 * @return Number of bytes read
 */
int USB_read(uint8_t* buffer, size_t len);

/**
 * @brief Read a single byte
 *
 * @return Byte read or -1 if no data
 */
int USB_getChar(void);

/**
 * @brief Get number of bytes available to read
 *
 * @return Number of bytes available
 */
int USB_available(void);

/**
 * @brief Read a line (until newline or buffer full)
 *
 * @param buffer Buffer to store line
 * @param maxLen Maximum line length
 * @return Number of characters read
 */
int USB_readLine(char* buffer, size_t maxLen);

/*============================================================================
 * USB Device Configuration
 *============================================================================*/

/**
 * @brief Set USB device descriptor strings
 *
 * @param manufacturer Manufacturer string
 * @param product Product string
 * @param serial Serial number string
 */
void USB_setDescriptor(const char* manufacturer, const char* product, const char* serial);

/**
 * @brief Set USB VID/PID
 *
 * @param vid Vendor ID
 * @param pid Product ID
 */
void USB_setVidPid(uint16_t vid, uint16_t pid);

/*============================================================================
 * Callbacks
 *============================================================================*/

typedef void (*usb_connect_cb_t)(void);
typedef void (*usb_disconnect_cb_t)(void);
typedef void (*usb_rx_cb_t)(uint8_t* data, size_t len);

/**
 * @brief Set USB connect callback
 */
void USB_setConnectCallback(usb_connect_cb_t callback);

/**
 * @brief Set USB disconnect callback
 */
void USB_setDisconnectCallback(usb_disconnect_cb_t callback);

/**
 * @brief Set USB receive callback (called when data arrives)
 */
void USB_setReceiveCallback(usb_rx_cb_t callback);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_USB_H */
