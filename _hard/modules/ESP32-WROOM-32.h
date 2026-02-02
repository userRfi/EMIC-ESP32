/*****************************************************************************
 * @file    ESP32-WROOM-32.h
 * @brief   ESP32-WROOM-32 Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-WROOM-32 module.
 *          - Chip: ESP32 (dual-core Xtensa LX6)
 *          - Flash: 4MB
 *          - PSRAM: None
 *          - Antenna: PCB antenna
 *****************************************************************************/

#ifndef MODULE_ESP32_WROOM_32_H
#define MODULE_ESP32_WROOM_32_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-WROOM-32"
#define MODULE_CHIP             "ESP32"
#define MODULE_CHIP_ID          ESP32

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define MODULE_FLASH_SIZE       (4 * 1024 * 1024)   /* 4MB flash */
#define MODULE_PSRAM_SIZE       0                    /* No PSRAM */
#define MODULE_HAS_PSRAM        0

/*============================================================================
 * Available Features
 *============================================================================*/
#define MODULE_HAS_WIFI         1
#define MODULE_HAS_BT_CLASSIC   1
#define MODULE_HAS_BLE          1
#define MODULE_HAS_ETHERNET     0   /* No external PHY */

/*============================================================================
 * GPIO Availability
 * Some GPIOs are used internally by the module
 *============================================================================*/

/* GPIOs 6-11: Connected to SPI flash - DO NOT USE */
#define MODULE_FLASH_GPIOS      (0x0FC0)

/* Available GPIOs (directly accessible on module pins) */
#define MODULE_AVAILABLE_GPIOS  0x00FFFE0F003FFFFF  /* GPIO 0-5, 12-39 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 (programming/debug) */
#define MODULE_UART0_TX         1
#define MODULE_UART0_RX         3

/* I2C default */
#define MODULE_I2C0_SDA         21
#define MODULE_I2C0_SCL         22

/* SPI default (VSPI) */
#define MODULE_SPI_MOSI         23
#define MODULE_SPI_MISO         19
#define MODULE_SPI_CLK          18
#define MODULE_SPI_CS           5

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         0   /* Boot button */
#define MODULE_STRAPPING_PINS   ((1<<0) | (1<<2) | (1<<5) | (1<<12) | (1<<15))

#endif /* MODULE_ESP32_WROOM_32_H */
