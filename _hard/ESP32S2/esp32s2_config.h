/*****************************************************************************
 * @file    esp32s2_config.h
 * @brief   ESP32-S2 Hardware Configuration for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration file for ESP32-S2 microcontroller.
 *          ESP32-S2 is a single-core Xtensa LX7 with WiFi only (NO Bluetooth),
 *          USB OTG support, and DAC capability.
 *****************************************************************************/

#ifndef ESP32S2_CONFIG_H
#define ESP32S2_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include common definitions */
#include "esp32_common.h"
#include "esp32_features.h"

/*============================================================================
 * MCU Identification
 *============================================================================*/
#define MCU_FAMILY              "ESP32-S2"
#define MCU_ARCHITECTURE        "Xtensa LX7"
#define MCU_CORES               1               /* Single-core */
#define MCU_FRAMEWORK           "ESP-IDF"

/*============================================================================
 * Feature Flags
 *============================================================================*/
#define ESP32S2_HAS_BT_CLASSIC  0       /* NO Bluetooth Classic */
#define ESP32S2_HAS_BLE         0       /* NO BLE - WiFi only chip */
#define ESP32S2_HAS_WIFI        1       /* WiFi 4 (802.11 b/g/n) */
#define ESP32S2_HAS_USB_OTG     1       /* USB OTG supported */
#define ESP32S2_HAS_USB_SERIAL  1       /* USB Serial/JTAG */
#define ESP32S2_HAS_TOUCH       1       /* Touch sensors available */
#define ESP32S2_HAS_DAC         1       /* DAC available (2 channels) */
#define ESP32S2_HAS_PSRAM       1       /* PSRAM support */
#define ESP32S2_HAS_CAMERA      1       /* Camera DVP interface */
#define ESP32S2_HAS_LCD         1       /* LCD parallel interface */

/*============================================================================
 * Clock Configuration
 *============================================================================*/
#define ESP32S2_CPU_FREQ_MHZ    240     /* Maximum CPU frequency */
#define ESP32S2_APB_FREQ_MHZ    80      /* APB bus frequency */
#define ESP32S2_XTAL_FREQ_MHZ   40      /* External crystal frequency */

/*============================================================================
 * GPIO Configuration
 *============================================================================*/
#define ESP32S2_GPIO_COUNT      43      /* GPIO0-GPIO42 (43 usable) */

/* GPIO Pin Notes:
 * GPIO0:       Strapping pin (boot mode)
 * GPIO18:      Strapping pin (USB/JTAG)
 * GPIO19-20:   USB D-/D+ (avoid if using USB OTG)
 * GPIO22-25:   Not available - reserved for flash
 * GPIO26:      SPICS1 (avoid on modules with PSRAM)
 * GPIO27-32:   SPI flash (avoid on WROOM modules)
 * GPIO33-37:   Not available - reserved for flash (Octal)
 * GPIO43:      UART0 TX
 * GPIO44:      UART0 RX
 * GPIO45:      Strapping pin (VDD_SPI voltage)
 * GPIO46:      Strapping pin (boot mode), input only
 */

/* GPIO46 is input only on ESP32-S2 */
#define ESP32S2_GPIO_INPUT_ONLY (1ULL << 46)

/*============================================================================
 * UART Configuration
 *============================================================================*/
#define ESP32S2_UART_COUNT      2       /* UART0, UART1 only */
#define ESP32S2_UART_FIFO_SIZE  128     /* Hardware FIFO size */

/* Default UART pins */
#define UART0_TX_DEFAULT        43
#define UART0_RX_DEFAULT        44
#define UART1_TX_DEFAULT        17
#define UART1_RX_DEFAULT        18

/*============================================================================
 * SPI Configuration
 *============================================================================*/
#define ESP32S2_SPI_COUNT       2       /* SPI2 (FSPI), SPI3 for user */

/* SPI2 (FSPI) default pins */
#define SPI2_MOSI_DEFAULT       35
#define SPI2_MISO_DEFAULT       37
#define SPI2_CLK_DEFAULT        36
#define SPI2_CS_DEFAULT         34

/* SPI3 default pins */
#define SPI3_MOSI_DEFAULT       11
#define SPI3_MISO_DEFAULT       13
#define SPI3_CLK_DEFAULT        12
#define SPI3_CS_DEFAULT         10

/*============================================================================
 * I2C Configuration
 *============================================================================*/
#define ESP32S2_I2C_COUNT       2       /* I2C0, I2C1 */
#define ESP32S2_I2C_MAX_FREQ    1000000 /* 1MHz Fast Mode Plus */

/* I2C0 default pins */
#define I2C0_SDA_DEFAULT        8
#define I2C0_SCL_DEFAULT        9

/* I2C1 default pins */
#define I2C1_SDA_DEFAULT        40
#define I2C1_SCL_DEFAULT        41

/*============================================================================
 * Timer Configuration
 *============================================================================*/
#define ESP32S2_TIMER_GROUPS    2
#define ESP32S2_TIMERS_PER_GROUP 2
#define ESP32S2_TIMER_COUNT     4

/*============================================================================
 * ADC Configuration
 *============================================================================*/
#define ESP32S2_ADC_UNITS       2
#define ESP32S2_ADC1_CHANNELS   10      /* GPIO 1-10 */
#define ESP32S2_ADC2_CHANNELS   10      /* GPIO 11-20 */
#define ESP32S2_ADC_TOTAL       20      /* 20 ADC channels total */
#define ESP32S2_ADC_RESOLUTION  13      /* 13-bit resolution */
#define ESP32S2_ADC_MAX_VALUE   8191    /* 2^13 - 1 */

/*============================================================================
 * DAC Configuration (ESP32-S2 has DAC)
 *============================================================================*/
#define ESP32S2_DAC_CHANNELS    2       /* DAC1 on GPIO17, DAC2 on GPIO18 */
#define ESP32S2_DAC_RESOLUTION  8       /* 8-bit resolution */
#define ESP32S2_DAC_MAX_VALUE   255
#define ESP32S2_DAC1_GPIO       17
#define ESP32S2_DAC2_GPIO       18

/*============================================================================
 * PWM (LEDC) Configuration
 *============================================================================*/
#define ESP32S2_LEDC_CHANNELS   8       /* 8 LEDC channels */
#define ESP32S2_LEDC_HS_CHANNELS 0      /* No high-speed channels */
#define ESP32S2_LEDC_LS_CHANNELS 8      /* All low-speed */
#define ESP32S2_LEDC_MAX_DUTY   ((1 << 14) - 1)  /* 14-bit max */
#define ESP32S2_LEDC_DEFAULT_FREQ 5000

/*============================================================================
 * Touch Sensor Configuration
 *============================================================================*/
#define ESP32S2_TOUCH_CHANNELS  14      /* T1-T14 */
/* Touch pins: GPIO 1-14 */

/*============================================================================
 * USB Configuration
 *============================================================================*/
#define ESP32S2_USB_OTG_ENABLED 1
/* USB pins: GPIO19 (D-), GPIO20 (D+) */

/*============================================================================
 * RMT Configuration
 *============================================================================*/
#define ESP32S2_RMT_CHANNELS    4       /* 4 RMT channels */

/*============================================================================
 * WiFi Configuration (NO Bluetooth)
 *============================================================================*/
#define ESP32S2_WIFI_SSID_MAX_LEN   32
#define ESP32S2_WIFI_PASS_MAX_LEN   64
#define ESP32S2_WIFI_MAX_CONNECTIONS 4

/*============================================================================
 * Bluetooth Stubs (ESP32-S2 has NO Bluetooth)
 * These macros ensure compile-time errors if Bluetooth is used
 *============================================================================*/
#define ESP32S2_BT_NOT_SUPPORTED    1
#define ESP32S2_BLE_NOT_SUPPORTED   1

/* Stub macros for compatibility */
#define BT_INIT_STUB()          ((void)0)
#define BT_DEINIT_STUB()        ((void)0)
#define BLE_INIT_STUB()         ((void)0)
#define BLE_DEINIT_STUB()       ((void)0)

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define ESP32S2_SRAM_SIZE       (320 * 1024)    /* 320KB SRAM */
#define ESP32S2_FLASH_SIZE_DEFAULT (4 * 1024 * 1024) /* 4MB typical */
#define ESP32S2_PSRAM_SIZE_DEFAULT (2 * 1024 * 1024) /* 2MB PSRAM */
#define ESP32S2_ROM_SIZE        (128 * 1024)

/*============================================================================
 * Security Features
 *============================================================================*/
#define ESP32S2_HAS_SECURE_BOOT     1
#define ESP32S2_HAS_FLASH_ENCRYPT   1
#define ESP32S2_HAS_HMAC            1
#define ESP32S2_HAS_DS              1   /* Digital Signature */

/*============================================================================
 * FreeRTOS Configuration (Single-core)
 *============================================================================*/
#define ESP32S2_FREERTOS_TICK_RATE_HZ 1000
#define ESP32S2_TASK_STACK_MIN        2048
#define ESP32S2_FREERTOS_UNICORE      1

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define ESP32S2_BOOT_PIN        0
#define ESP32S2_STRAPPING_PINS  ((1 << 0) | (1 << 45) | (1 << 46))

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_CONFIG_H */
