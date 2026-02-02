/*****************************************************************************
 * @file    ESP32-S3-WROOM-1.h
 * @brief   ESP32-S3-WROOM-1 Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-S3-WROOM-1 module.
 *          - Chip: ESP32-S3 (dual-core Xtensa LX7)
 *          - Flash: 8MB/16MB
 *          - PSRAM: None or 2MB/8MB (variants)
 *          - Antenna: PCB antenna
 *****************************************************************************/

#ifndef MODULE_ESP32_S3_WROOM_1_H
#define MODULE_ESP32_S3_WROOM_1_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-S3-WROOM-1"
#define MODULE_CHIP             "ESP32-S3"
#define MODULE_CHIP_ID          ESP32S3

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define MODULE_FLASH_SIZE       (8 * 1024 * 1024)   /* 8MB flash default */
#define MODULE_PSRAM_SIZE       0                    /* No PSRAM (N variant) */
#define MODULE_HAS_PSRAM        0                    /* Set to 1 for -N8R2/N8R8 */

/*============================================================================
 * Available Features
 *============================================================================*/
#define MODULE_HAS_WIFI         1
#define MODULE_HAS_BT_CLASSIC   0   /* No Classic on S3 */
#define MODULE_HAS_BLE          1   /* BLE 5.0 */
#define MODULE_HAS_USB_OTG      1
#define MODULE_HAS_TOUCH        1   /* 14 touch channels */

/*============================================================================
 * GPIO Availability
 *============================================================================*/

/* GPIOs 26-32: Connected to SPI flash - DO NOT USE */
#define MODULE_FLASH_GPIOS      (0x1FC000000ULL)

/* USB pins (optional use) */
#define MODULE_USB_GPIOS        ((1<<19) | (1<<20))

/* Available GPIOs */
#define MODULE_AVAILABLE_GPIOS  0x0001FFFF803FFFFFULL  /* GPIO 0-21, 35-48 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 (programming/debug) */
#define MODULE_UART0_TX         43
#define MODULE_UART0_RX         44

/* USB */
#define MODULE_USB_DN           19
#define MODULE_USB_DP           20

/* I2C default */
#define MODULE_I2C0_SDA         1
#define MODULE_I2C0_SCL         2

/* SPI default */
#define MODULE_SPI_MOSI         11
#define MODULE_SPI_MISO         13
#define MODULE_SPI_CLK          12
#define MODULE_SPI_CS           10

/*============================================================================
 * Touch Channels
 *============================================================================*/
#define MODULE_TOUCH_GPIOS      0x00007FFE  /* GPIO 1-14 = T1-T14 */

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         0
#define MODULE_STRAPPING_PINS   ((1<<0) | (1<<3) | (1<<45) | (1<<46))

#endif /* MODULE_ESP32_S3_WROOM_1_H */
