/*****************************************************************************
 * @file    ESP32-C3-MINI-1.h
 * @brief   ESP32-C3-MINI-1 Module Configuration
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration for ESP32-C3-MINI-1 module.
 *          - Chip: ESP32-C3 (single-core RISC-V)
 *          - Flash: 4MB
 *          - Antenna: PCB antenna
 *          - Small form factor
 *****************************************************************************/

#ifndef MODULE_ESP32_C3_MINI_1_H
#define MODULE_ESP32_C3_MINI_1_H

/*============================================================================
 * Module Identification
 *============================================================================*/
#define MODULE_NAME             "ESP32-C3-MINI-1"
#define MODULE_CHIP             "ESP32-C3"
#define MODULE_CHIP_ID          ESP32C3

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
#define MODULE_HAS_BT_CLASSIC   0   /* No Classic on C3 */
#define MODULE_HAS_BLE          1   /* BLE 5.0 */
#define MODULE_HAS_USB_SERIAL   1   /* USB Serial/JTAG */
#define MODULE_HAS_TOUCH        0   /* No touch on C3 */

/*============================================================================
 * GPIO Availability
 *============================================================================*/

/* GPIOs 11: VDD_SPI - DO NOT USE */
/* GPIOs 12-17: Available but often used for flash on some variants */
#define MODULE_FLASH_GPIOS      (1<<11)

/* USB pins */
#define MODULE_USB_GPIOS        ((1<<18) | (1<<19))

/* Available GPIOs (safe to use) */
#define MODULE_AVAILABLE_GPIOS  0x003F07FF  /* GPIO 0-10, 12-21 */

/*============================================================================
 * Default Pin Assignments
 *============================================================================*/

/* UART0 */
#define MODULE_UART0_TX         21
#define MODULE_UART0_RX         20

/* USB Serial/JTAG */
#define MODULE_USB_DN           18
#define MODULE_USB_DP           19

/* I2C default */
#define MODULE_I2C0_SDA         8
#define MODULE_I2C0_SCL         9

/* SPI default */
#define MODULE_SPI_MOSI         7
#define MODULE_SPI_MISO         2
#define MODULE_SPI_CLK          6
#define MODULE_SPI_CS           10

/*============================================================================
 * ADC Channels
 *============================================================================*/
#define MODULE_ADC1_GPIOS       0x0000001F  /* GPIO 0-4 = ADC1 CH0-4 */
#define MODULE_ADC2_GPIOS       0x00000020  /* GPIO 5 = ADC2 CH0 */

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define MODULE_BOOT_PIN         9
#define MODULE_STRAPPING_PINS   ((1<<2) | (1<<8) | (1<<9))

#endif /* MODULE_ESP32_C3_MINI_1_H */
