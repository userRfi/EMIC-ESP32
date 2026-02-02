/*****************************************************************************
 * @file    ESP32-WROVER.h
 * @brief   ESP32-WROVER Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-WROVER module.
 *          - Chip: ESP32 (dual-core Xtensa LX6)
 *          - Flash: 4MB/16MB
 *          - PSRAM: 8MB
 *          - Antenna: PCB or IPEX
 *****************************************************************************/

#ifndef MODULE_ESP32_WROVER_H
#define MODULE_ESP32_WROVER_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-WROVER"
#define MODULE_CHIP             "ESP32"
#define MODULE_CHIP_ID          ESP32

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define MODULE_FLASH_SIZE       (4 * 1024 * 1024)   /* 4MB flash (16MB variant available) */
#define MODULE_PSRAM_SIZE       (8 * 1024 * 1024)   /* 8MB PSRAM */
#define MODULE_HAS_PSRAM        1

/*============================================================================
 * Available Features
 *============================================================================*/
#define MODULE_HAS_WIFI         1
#define MODULE_HAS_BT_CLASSIC   1
#define MODULE_HAS_BLE          1
#define MODULE_HAS_ETHERNET     0

/*============================================================================
 * GPIO Availability
 *============================================================================*/

/* GPIOs 6-11: Connected to SPI flash - DO NOT USE */
/* GPIO 16-17: Connected to PSRAM - DO NOT USE on WROVER */
#define MODULE_FLASH_GPIOS      (0x0FC0)
#define MODULE_PSRAM_GPIOS      (0x00030000)

/* Available GPIOs (excluding flash and PSRAM) */
#define MODULE_AVAILABLE_GPIOS  0x00FFFC0F003FFFFF  /* GPIO 0-5, 12-15, 18-39 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 (programming/debug) */
#define MODULE_UART0_TX         1
#define MODULE_UART0_RX         3

/* I2C default */
#define MODULE_I2C0_SDA         21
#define MODULE_I2C0_SCL         22

/* SPI default (VSPI) - avoiding PSRAM pins */
#define MODULE_SPI_MOSI         23
#define MODULE_SPI_MISO         19
#define MODULE_SPI_CLK          18
#define MODULE_SPI_CS           5

/*============================================================================
 * PSRAM Configuration
 *============================================================================*/
#define PSRAM_CLK_PIN           17  /* Reserved for PSRAM */
#define PSRAM_CS_PIN            16  /* Reserved for PSRAM */

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         0
#define MODULE_STRAPPING_PINS   ((1<<0) | (1<<2) | (1<<5) | (1<<12) | (1<<15))

#endif /* MODULE_ESP32_WROVER_H */
