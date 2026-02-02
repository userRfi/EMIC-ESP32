/*****************************************************************************
 * @file    ESP32-S2-WROOM.h
 * @brief   ESP32-S2-WROOM Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-S2-WROOM module.
 *          - Chip: ESP32-S2 (single-core Xtensa LX7)
 *          - Flash: 4MB
 *          - PSRAM: None (WROVER has 2MB)
 *          - NOTE: NO Bluetooth support
 *****************************************************************************/

#ifndef MODULE_ESP32_S2_WROOM_H
#define MODULE_ESP32_S2_WROOM_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-S2-WROOM"
#define MODULE_CHIP             "ESP32-S2"
#define MODULE_CHIP_ID          ESP32S2

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define MODULE_FLASH_SIZE       (4 * 1024 * 1024)   /* 4MB flash */
#define MODULE_PSRAM_SIZE       0                    /* No PSRAM on WROOM */
#define MODULE_HAS_PSRAM        0

/*============================================================================
 * Available Features
 *============================================================================*/
#define MODULE_HAS_WIFI         1
#define MODULE_HAS_BT_CLASSIC   0   /* NO Bluetooth on S2! */
#define MODULE_HAS_BLE          0   /* NO Bluetooth on S2! */
#define MODULE_HAS_USB_OTG      1   /* USB OTG supported */
#define MODULE_HAS_TOUCH        1   /* 14 touch channels */
#define MODULE_HAS_DAC          1   /* 2 DAC channels */
#define MODULE_HAS_LCD          1   /* LCD interface */

/*============================================================================
 * GPIO Availability
 *============================================================================*/

/* GPIOs 26-32: Connected to SPI flash - DO NOT USE */
#define MODULE_FLASH_GPIOS      (0x1FC000000ULL)

/* GPIO 46: Input only */
#define MODULE_INPUT_ONLY_GPIOS (1ULL << 46)

/* USB pins */
#define MODULE_USB_GPIOS        ((1<<19) | (1<<20))

/* Available GPIOs */
#define MODULE_AVAILABLE_GPIOS  0x0001FFFF803FFFFFULL  /* GPIO 0-21, 33-46 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 */
#define MODULE_UART0_TX         43
#define MODULE_UART0_RX         44

/* USB OTG */
#define MODULE_USB_DN           19
#define MODULE_USB_DP           20

/* I2C default */
#define MODULE_I2C0_SDA         8
#define MODULE_I2C0_SCL         9

/* SPI default */
#define MODULE_SPI_MOSI         35
#define MODULE_SPI_MISO         37
#define MODULE_SPI_CLK          36
#define MODULE_SPI_CS           34

/* DAC */
#define MODULE_DAC1_PIN         17  /* DAC1 */
#define MODULE_DAC2_PIN         18  /* DAC2 */

/*============================================================================
 * Touch Channels
 *============================================================================*/
#define MODULE_TOUCH_GPIOS      0x00007FFE  /* GPIO 1-14 = T1-T14 */

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         0
#define MODULE_STRAPPING_PINS   ((1<<0) | (1<<45) | (1<<46))

#endif /* MODULE_ESP32_S2_WROOM_H */
