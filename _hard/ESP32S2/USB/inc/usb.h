/*****************************************************************************
 * @file    usb.h
 * @brief   ESP32-S2 USB OTG Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details USB OTG header for ESP32-S2 supporting CDC and HID modes.
 *          Uses TinyUSB stack.
 *****************************************************************************/

#ifndef ESP32S2_USB_H
#define ESP32S2_USB_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/*============================================================================
 * USB Configuration
 *============================================================================*/
#define USB_DN_GPIO             19      /* USB D- pin */
#define USB_DP_GPIO             20      /* USB D+ pin */

/* USB CDC Configuration */
#define USB_CDC_RX_BUFFER_SIZE  512
#define USB_CDC_TX_BUFFER_SIZE  512

/*============================================================================
 * USB Device Types
 *============================================================================*/
typedef enum {
    USB_MODE_NONE = 0,
    USB_MODE_CDC,           /* CDC ACM (Virtual COM Port) */
    USB_MODE_HID,           /* Human Interface Device */
    USB_MODE_MSC,           /* Mass Storage Class */
    USB_MODE_COMPOSITE      /* Multiple classes */
} usb_mode_t;

/*============================================================================
 * USB Functions
 *============================================================================*/

/**
 * @brief Initialize USB subsystem with default CDC mode
 */
void USB_init(void);

/**
 * @brief Initialize USB subsystem with specified mode
 * @param mode USB device mode
 * @return 0 on success, -1 on error
 */
int USB_initMode(usb_mode_t mode);

/**
 * @brief Deinitialize USB subsystem
 */
void USB_deinit(void);

/**
 * @brief Check if USB is connected to a host
 * @return true if connected
 */
bool USB_isConnected(void);

/**
 * @brief Check if USB CDC is ready for communication
 * @return true if ready
 */
bool USB_cdcReady(void);

/**
 * @brief Write data to USB CDC
 * @param data Data buffer to write
 * @param len Number of bytes to write
 * @return Number of bytes written, -1 on error
 */
int USB_write(const uint8_t* data, size_t len);

/**
 * @brief Write string to USB CDC
 * @param str Null-terminated string
 * @return Number of bytes written, -1 on error
 */
int USB_writeString(const char* str);

/**
 * @brief Read data from USB CDC
 * @param buffer Buffer to store data
 * @param len Maximum bytes to read
 * @return Number of bytes read, -1 on error
 */
int USB_read(uint8_t* buffer, size_t len);

/**
 * @brief Get number of bytes available to read
 * @return Number of bytes available
 */
int USB_available(void);

/**
 * @brief Flush USB CDC transmit buffer
 */
void USB_flush(void);

/**
 * @brief Set USB CDC line coding callback
 * @param callback Function to call when line coding changes
 */
void USB_setLineCodingCallback(void (*callback)(uint32_t baud, uint8_t stop_bits,
                                                  uint8_t parity, uint8_t data_bits));

/**
 * @brief Get current baud rate (from line coding)
 * @return Baud rate
 */
uint32_t USB_getBaudRate(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_USB_H */
