/*****************************************************************************
 * @file    esp32s3_config.h
 * @brief   ESP32-S3 Hardware Configuration for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration file for ESP32-S3 microcontroller.
 *          ESP32-S3 is a dual-core Xtensa LX7 with WiFi, BLE 5.0,
 *          USB OTG, and enhanced AI/ML capabilities.
 *****************************************************************************/

#ifndef ESP32S3_CONFIG_H
#define ESP32S3_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include common definitions */
#include "esp32_common.h"
#include "esp32_features.h"

/*============================================================================
 * MCU Identification
 *============================================================================*/
#define MCU_FAMILY              "ESP32-S3"
#define MCU_ARCHITECTURE        "Xtensa LX7"
#define MCU_CORES               2
#define MCU_FRAMEWORK           "ESP-IDF"

/*============================================================================
 * Feature Flags
 *============================================================================*/
#define ESP32S3_HAS_BT_CLASSIC  0       /* No Bluetooth Classic */
#define ESP32S3_HAS_BLE         1       /* BLE 5.0 supported */
#define ESP32S3_HAS_WIFI        1       /* WiFi 4 (802.11 b/g/n) */
#define ESP32S3_HAS_USB_OTG     1       /* USB OTG supported */
#define ESP32S3_HAS_USB_SERIAL  1       /* USB Serial/JTAG */
#define ESP32S3_HAS_TOUCH       1       /* Touch sensors available */
#define ESP32S3_HAS_DAC         0       /* No DAC on S3 */
#define ESP32S3_HAS_PSRAM       1       /* PSRAM support */
#define ESP32S3_HAS_CAMERA      1       /* Camera DVP interface */
#define ESP32S3_HAS_LCD         1       /* LCD parallel interface */

/*============================================================================
 * Clock Configuration
 *============================================================================*/
#define ESP32S3_CPU_FREQ_MHZ    240     /* Maximum CPU frequency */
#define ESP32S3_APB_FREQ_MHZ    80      /* APB bus frequency */
#define ESP32S3_XTAL_FREQ_MHZ   40      /* External crystal frequency */

/*============================================================================
 * GPIO Configuration
 *============================================================================*/
#define ESP32S3_GPIO_COUNT      45      /* GPIO0-GPIO48 (45 usable) */

/* GPIO Pin Notes:
 * GPIO0:       Strapping pin (boot mode)
 * GPIO3:       Strapping pin (JTAG)
 * GPIO19-20:   USB D-/D+ (avoid if using USB)
 * GPIO26-32:   SPI flash/PSRAM (avoid on WROOM modules)
 * GPIO33-37:   SPI flash (Octal flash, avoid)
 * GPIO38:      PSRAM (avoid on WROVER modules)
 * GPIO39-42:   JTAG (if not disabled)
 * GPIO43-44:   UART0 TX/RX
 * GPIO45:      Strapping pin (VDD_SPI voltage)
 * GPIO46:      Strapping pin (boot mode)
 */

/* All GPIOs on S3 are input/output capable */
#define ESP32S3_GPIO_INPUT_ONLY 0x0000000000000000ULL

/*============================================================================
 * UART Configuration
 *============================================================================*/
#define ESP32S3_UART_COUNT      3       /* UART0, UART1, UART2 */
#define ESP32S3_UART_FIFO_SIZE  128     /* Hardware FIFO size */

/* Default UART pins */
#define UART0_TX_DEFAULT        43
#define UART0_RX_DEFAULT        44
#define UART1_TX_DEFAULT        17
#define UART1_RX_DEFAULT        18
#define UART2_TX_DEFAULT        19      /* Note: shares with USB */
#define UART2_RX_DEFAULT        20      /* Note: shares with USB */

/*============================================================================
 * SPI Configuration
 *============================================================================*/
#define ESP32S3_SPI_COUNT       2       /* SPI2 (FSPI), SPI3 for user */

/* SPI2 (FSPI) default pins */
#define SPI2_MOSI_DEFAULT       11
#define SPI2_MISO_DEFAULT       13
#define SPI2_CLK_DEFAULT        12
#define SPI2_CS_DEFAULT         10

/* SPI3 default pins */
#define SPI3_MOSI_DEFAULT       35
#define SPI3_MISO_DEFAULT       37
#define SPI3_CLK_DEFAULT        36
#define SPI3_CS_DEFAULT         34

/*============================================================================
 * I2C Configuration
 *============================================================================*/
#define ESP32S3_I2C_COUNT       2       /* I2C0, I2C1 */
#define ESP32S3_I2C_MAX_FREQ    1000000 /* 1MHz Fast Mode Plus */

/* I2C0 default pins */
#define I2C0_SDA_DEFAULT        1
#define I2C0_SCL_DEFAULT        2

/* I2C1 default pins */
#define I2C1_SDA_DEFAULT        41
#define I2C1_SCL_DEFAULT        42

/*============================================================================
 * Timer Configuration
 *============================================================================*/
#define ESP32S3_TIMER_GROUPS    2
#define ESP32S3_TIMERS_PER_GROUP 2
#define ESP32S3_TIMER_COUNT     4

/*============================================================================
 * ADC Configuration
 *============================================================================*/
#define ESP32S3_ADC_UNITS       2
#define ESP32S3_ADC1_CHANNELS   10      /* GPIO 1-10 */
#define ESP32S3_ADC2_CHANNELS   10      /* GPIO 11-20 */
#define ESP32S3_ADC_RESOLUTION  12
#define ESP32S3_ADC_MAX_VALUE   4095

/*============================================================================
 * PWM (LEDC) Configuration
 *============================================================================*/
#define ESP32S3_LEDC_CHANNELS   8       /* 8 LEDC channels */
#define ESP32S3_LEDC_HS_CHANNELS 0      /* No high-speed channels */
#define ESP32S3_LEDC_LS_CHANNELS 8      /* All low-speed */
#define ESP32S3_LEDC_MAX_DUTY   ((1 << 14) - 1)  /* 14-bit max */
#define ESP32S3_LEDC_DEFAULT_FREQ 5000

/*============================================================================
 * Touch Sensor Configuration
 *============================================================================*/
#define ESP32S3_TOUCH_CHANNELS  14      /* T1-T14 */
/* Touch pins: GPIO 1-14 */

/*============================================================================
 * USB Configuration
 *============================================================================*/
#define ESP32S3_USB_OTG_ENABLED 1
/* USB pins: GPIO19 (D-), GPIO20 (D+) */

/*============================================================================
 * RMT Configuration
 *============================================================================*/
#define ESP32S3_RMT_CHANNELS    8       /* 4 TX, 4 RX channels */

/*============================================================================
 * WiFi Configuration
 *============================================================================*/
#define ESP32S3_WIFI_SSID_MAX_LEN   32
#define ESP32S3_WIFI_PASS_MAX_LEN   64
#define ESP32S3_WIFI_MAX_CONNECTIONS 4

/*============================================================================
 * Bluetooth Configuration (BLE 5.0 only)
 *============================================================================*/
#define ESP32S3_BLE_VERSION     5
#define ESP32S3_BT_DEVICE_NAME_LEN 32
#define ESP32S3_BLE_MTU_DEFAULT 23
#define ESP32S3_BLE_CODED_PHY   1
#define ESP32S3_BLE_2M_PHY      1

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define ESP32S3_SRAM_SIZE       (512 * 1024)    /* 512KB SRAM */
#define ESP32S3_FLASH_SIZE_DEFAULT (8 * 1024 * 1024) /* 8MB typical */
#define ESP32S3_PSRAM_SIZE_DEFAULT (8 * 1024 * 1024) /* 8MB PSRAM */
#define ESP32S3_ROM_SIZE        (384 * 1024)

/*============================================================================
 * AI Accelerator (Vector Extensions)
 *============================================================================*/
#define ESP32S3_HAS_VECTOR_EXT  1       /* Hardware vector extensions */

/*============================================================================
 * Security Features
 *============================================================================*/
#define ESP32S3_HAS_SECURE_BOOT     1
#define ESP32S3_HAS_FLASH_ENCRYPT   1
#define ESP32S3_HAS_HMAC            1
#define ESP32S3_HAS_DS              1

/*============================================================================
 * FreeRTOS Configuration
 *============================================================================*/
#define ESP32S3_FREERTOS_TICK_RATE_HZ 1000
#define ESP32S3_TASK_STACK_MIN        2048

/*============================================================================
 * Strapping Pins
 *============================================================================*/
#define ESP32S3_BOOT_PIN        0
#define ESP32S3_STRAPPING_PINS  ((1 << 0) | (1 << 3) | (1 << 45) | (1 << 46))

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_CONFIG_H */
