/*****************************************************************************
 * @file    ESP32-C6-WROOM-1.h
 * @brief   ESP32-C6-WROOM-1 Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-C6-WROOM-1 module.
 *          - Chip: ESP32-C6 (single-core RISC-V)
 *          - Flash: 4MB/8MB
 *          - Antenna: PCB antenna
 *          - WiFi 6 (802.11ax) support
 *****************************************************************************/

#ifndef MODULE_ESP32_C6_WROOM_1_H
#define MODULE_ESP32_C6_WROOM_1_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-C6-WROOM-1"
#define MODULE_CHIP             "ESP32-C6"
#define MODULE_CHIP_ID          ESP32C6

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define MODULE_FLASH_SIZE       (8 * 1024 * 1024)   /* 8MB flash */
#define MODULE_PSRAM_SIZE       0                    /* No PSRAM */
#define MODULE_HAS_PSRAM        0

/*============================================================================
 * Available Features
 *============================================================================*/
#define MODULE_HAS_WIFI         1
#define MODULE_HAS_WIFI6        1   /* WiFi 6 (802.11ax)! */
#define MODULE_HAS_BT_CLASSIC   0   /* No Classic */
#define MODULE_HAS_BLE          1   /* BLE 5.0 */
#define MODULE_HAS_THREAD       1   /* Thread/Zigbee support */
#define MODULE_HAS_USB_SERIAL   1   /* USB Serial/JTAG */
#define MODULE_HAS_TOUCH        0   /* No touch on C6 */

/*============================================================================
 * GPIO Availability
 *============================================================================*/

/* GPIOs 24-30: SPI flash - DO NOT USE */
#define MODULE_FLASH_GPIOS      (0x7F000000)

/* USB pins */
#define MODULE_USB_GPIOS        ((1<<12) | (1<<13))

/* Available GPIOs */
#define MODULE_AVAILABLE_GPIOS  0x00FFFFFF  /* GPIO 0-23 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 */
#define MODULE_UART0_TX         16
#define MODULE_UART0_RX         17

/* USB Serial/JTAG */
#define MODULE_USB_DN           12
#define MODULE_USB_DP           13

/* I2C default */
#define MODULE_I2C0_SDA         6
#define MODULE_I2C0_SCL         7

/* SPI default */
#define MODULE_SPI_MOSI         19
#define MODULE_SPI_MISO         20
#define MODULE_SPI_CLK          21
#define MODULE_SPI_CS           18

/*============================================================================
 * ADC Channels
 *============================================================================*/
#define MODULE_ADC1_GPIOS       0x0000007F  /* GPIO 0-6 = ADC1 CH0-6 */

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         9
#define MODULE_STRAPPING_PINS   ((1<<8) | (1<<9) | (1<<15))

#endif /* MODULE_ESP32_C6_WROOM_1_H */
