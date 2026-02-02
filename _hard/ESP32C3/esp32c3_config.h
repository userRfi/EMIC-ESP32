/*****************************************************************************
 * @file    esp32c3_config.h
 * @brief   ESP32-C3 Hardware Configuration for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration file for ESP32-C3 microcontroller.
 *          ESP32-C3 is a single-core RISC-V MCU with WiFi and BLE 5.0.
 *****************************************************************************/

#ifndef ESP32C3_CONFIG_H
#define ESP32C3_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include common definitions */
#include "esp32_common.h"
#include "esp32_features.h"

/*============================================================================
 * MCU Identification
 *============================================================================*/
#define MCU_FAMILY              "ESP32-C3"
#define MCU_ARCHITECTURE        "RISC-V"
#define MCU_CORES               1
#define MCU_FRAMEWORK           "ESP-IDF"

/*============================================================================
 * Feature Flags
 *============================================================================*/
#define ESP32C3_HAS_BT_CLASSIC  0       /* No Bluetooth Classic */
#define ESP32C3_HAS_BLE         1       /* BLE 5.0 supported */
#define ESP32C3_HAS_WIFI        1       /* WiFi 4 (802.11 b/g/n) */
#define ESP32C3_HAS_USB_OTG     0       /* No USB OTG */
#define ESP32C3_HAS_USB_SERIAL  1       /* USB Serial/JTAG */
#define ESP32C3_HAS_TOUCH       0       /* No touch sensors */
#define ESP32C3_HAS_DAC         0       /* No DAC */

/*============================================================================
 * Clock Configuration
 *============================================================================*/
#define ESP32C3_CPU_FREQ_MHZ    160     /* Maximum CPU frequency */
#define ESP32C3_APB_FREQ_MHZ    80      /* APB bus frequency */
#define ESP32C3_XTAL_FREQ_MHZ   40      /* External crystal frequency */

/*============================================================================
 * GPIO Configuration
 *============================================================================*/
#define ESP32C3_GPIO_COUNT      22      /* GPIO0-GPIO21 */

/* GPIO Pin Mapping for ESP32-C3:
 * GPIO0-5:   General purpose, ADC1 channels
 * GPIO6-11:  Reserved for SPI flash (do not use)
 * GPIO12-17: General purpose (safe to use)
 * GPIO18-19: USB D-/D+ (avoid if using USB)
 * GPIO20-21: General purpose / I2C default
 *
 * Note: GPIO6-11 are typically connected to SPI flash and should NOT be used
 */
#define ESP32C3_GPIO_FLASH_PINS 0x0FC0  /* GPIOs 6-11 mask */
#define ESP32C3_GPIO_USB_PINS   0xC0000 /* GPIOs 18-19 mask */

/* All GPIOs are input/output capable on ESP32-C3 */
#define ESP32C3_GPIO_INPUT_ONLY 0x00000000

/*============================================================================
 * UART Configuration
 *============================================================================*/
#define ESP32C3_UART_COUNT      2       /* UART0, UART1 */
#define ESP32C3_UART_FIFO_SIZE  128     /* Hardware FIFO size */

/* Default UART pins */
#define UART0_TX_DEFAULT        21
#define UART0_RX_DEFAULT        20
#define UART1_TX_DEFAULT        4
#define UART1_RX_DEFAULT        5

/*============================================================================
 * SPI Configuration
 *============================================================================*/
#define ESP32C3_SPI_COUNT       1       /* SPI2 (GPSPI) available for user */

/* SPI2 default pins */
#define SPI2_MOSI_DEFAULT       7
#define SPI2_MISO_DEFAULT       2
#define SPI2_CLK_DEFAULT        6
#define SPI2_CS_DEFAULT         10

/*============================================================================
 * I2C Configuration
 *============================================================================*/
#define ESP32C3_I2C_COUNT       1       /* I2C0 only */
#define ESP32C3_I2C_MAX_FREQ    1000000 /* 1MHz Fast Mode Plus */

/* I2C0 default pins */
#define I2C0_SDA_DEFAULT        8
#define I2C0_SCL_DEFAULT        9

/*============================================================================
 * Timer Configuration
 *============================================================================*/
#define ESP32C3_TIMER_GROUPS    2       /* Timer groups */
#define ESP32C3_TIMERS_PER_GROUP 1      /* 1 timer per group */
#define ESP32C3_TIMER_COUNT     2       /* Total hardware timers */

/*============================================================================
 * ADC Configuration
 *============================================================================*/
#define ESP32C3_ADC_UNITS       2       /* ADC1, ADC2 */
#define ESP32C3_ADC1_CHANNELS   5       /* GPIO 0-4 */
#define ESP32C3_ADC2_CHANNELS   1       /* GPIO 5 */
#define ESP32C3_ADC_RESOLUTION  12      /* 12-bit resolution */
#define ESP32C3_ADC_MAX_VALUE   4095

/* ADC1 Channel Mapping:
 * ADC1_CH0: GPIO0
 * ADC1_CH1: GPIO1
 * ADC1_CH2: GPIO2
 * ADC1_CH3: GPIO3
 * ADC1_CH4: GPIO4
 * ADC2_CH0: GPIO5
 */

/*============================================================================
 * PWM (LEDC) Configuration
 *============================================================================*/
#define ESP32C3_LEDC_CHANNELS   6       /* 6 LEDC channels */
#define ESP32C3_LEDC_HS_CHANNELS 0      /* No high-speed channels */
#define ESP32C3_LEDC_LS_CHANNELS 6      /* All low-speed */
#define ESP32C3_LEDC_MAX_DUTY   ((1 << 13) - 1)
#define ESP32C3_LEDC_DEFAULT_FREQ 5000

/*============================================================================
 * RMT Configuration
 *============================================================================*/
#define ESP32C3_RMT_CHANNELS    4       /* RMT channels (2 TX, 2 RX) */

/*============================================================================
 * WiFi Configuration
 *============================================================================*/
#define ESP32C3_WIFI_SSID_MAX_LEN   32
#define ESP32C3_WIFI_PASS_MAX_LEN   64
#define ESP32C3_WIFI_MAX_CONNECTIONS 4

/*============================================================================
 * Bluetooth Configuration (BLE 5.0 only)
 *============================================================================*/
#define ESP32C3_BLE_VERSION     5       /* BLE 5.0 */
#define ESP32C3_BT_DEVICE_NAME_LEN 32
#define ESP32C3_BLE_MTU_DEFAULT 23
#define ESP32C3_BLE_CODED_PHY   1       /* Supports Coded PHY */
#define ESP32C3_BLE_2M_PHY      1       /* Supports 2M PHY */

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define ESP32C3_SRAM_SIZE       (400 * 1024)    /* 400KB SRAM */
#define ESP32C3_FLASH_SIZE_DEFAULT (4 * 1024 * 1024) /* 4MB typical */
#define ESP32C3_ROM_SIZE        (384 * 1024)    /* 384KB ROM */

/*============================================================================
 * Security Features
 *============================================================================*/
#define ESP32C3_HAS_SECURE_BOOT     1   /* Secure Boot V2 */
#define ESP32C3_HAS_FLASH_ENCRYPT   1   /* Flash Encryption */
#define ESP32C3_HAS_HMAC           1    /* HMAC accelerator */
#define ESP32C3_HAS_DS             1    /* Digital Signature */

/*============================================================================
 * FreeRTOS Configuration
 *============================================================================*/
#define ESP32C3_FREERTOS_TICK_RATE_HZ 1000
#define ESP32C3_TASK_STACK_MIN        2048

/*============================================================================
 * Strapping Pins
 *============================================================================*/
/* GPIO2:  Controls boot mode (HIGH = SPI boot, LOW = Download boot)
 * GPIO8:  Controls boot messages (HIGH = print boot messages)
 * GPIO9:  Boot button (active LOW)
 */
#define ESP32C3_BOOT_PIN        9
#define ESP32C3_STRAPPING_PINS  ((1 << 2) | (1 << 8) | (1 << 9))

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_CONFIG_H */
