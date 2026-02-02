/*****************************************************************************
 * @file    esp32c6_config.h
 * @brief   ESP32-C6 Hardware Configuration for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Configuration file for ESP32-C6 microcontroller.
 *          ESP32-C6 is a single-core RISC-V MCU with WiFi 6, BLE 5.0,
 *          and Thread/Zigbee (802.15.4) support.
 *****************************************************************************/

#ifndef ESP32C6_CONFIG_H
#define ESP32C6_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* Include common definitions */
#include "esp32_common.h"
#include "esp32_features.h"

/*============================================================================
 * MCU Identification
 *============================================================================*/
#define MCU_FAMILY              "ESP32-C6"
#define MCU_ARCHITECTURE        "RISC-V"
#define MCU_CORES               1
#define MCU_FRAMEWORK           "ESP-IDF"

/*============================================================================
 * Feature Flags
 *============================================================================*/
#define ESP32C6_HAS_BT_CLASSIC  0       /* No Bluetooth Classic */
#define ESP32C6_HAS_BLE         1       /* BLE 5.0 supported */
#define ESP32C6_HAS_WIFI        1       /* WiFi 6 (802.11ax) */
#define ESP32C6_HAS_WIFI6       1       /* First ESP32 with WiFi 6! */
#define ESP32C6_HAS_USB_OTG     0       /* No USB OTG */
#define ESP32C6_HAS_USB_SERIAL  1       /* USB Serial/JTAG */
#define ESP32C6_HAS_TOUCH       0       /* No touch sensors */
#define ESP32C6_HAS_DAC         0       /* No DAC */
#define ESP32C6_HAS_THREAD      1       /* Thread (802.15.4) support */
#define ESP32C6_HAS_ZIGBEE      1       /* Zigbee (802.15.4) support */
#define ESP32C6_HAS_LP_UART     1       /* Low-power UART */

/*============================================================================
 * Clock Configuration
 *============================================================================*/
#define ESP32C6_CPU_FREQ_MHZ    160     /* Maximum CPU frequency */
#define ESP32C6_APB_FREQ_MHZ    80      /* APB bus frequency */
#define ESP32C6_XTAL_FREQ_MHZ   40      /* External crystal frequency */

/*============================================================================
 * GPIO Configuration
 *============================================================================*/
#define ESP32C6_GPIO_COUNT      31      /* GPIO0-GPIO30 */

/* GPIO Pin Mapping for ESP32-C6:
 * GPIO0-6:   General purpose, ADC1 channels (CH0-CH6)
 * GPIO7-14:  General purpose
 * GPIO15:    XTAL_32K_P (external 32kHz crystal)
 * GPIO16:    XTAL_32K_N (external 32kHz crystal)
 * GPIO17-22: General purpose
 * GPIO23:    Reserved (do not use)
 * GPIO24-30: General purpose, SPI flash (GPIO24-28 on some modules)
 *
 * Note: Some GPIOs may be connected to SPI flash depending on module
 */
#define ESP32C6_GPIO_FLASH_PINS     0x1F000000  /* GPIOs 24-28 mask (module dependent) */
#define ESP32C6_GPIO_XTAL_PINS      0x00018000  /* GPIOs 15-16 mask (32kHz crystal) */
#define ESP32C6_GPIO_USB_PINS       0x00003000  /* GPIOs 12-13 (USB D-/D+) */

/* All GPIOs are input/output capable on ESP32-C6 */
#define ESP32C6_GPIO_INPUT_ONLY     0x00000000

/*============================================================================
 * UART Configuration
 *============================================================================*/
#define ESP32C6_UART_COUNT      2       /* UART0, UART1 */
#define ESP32C6_LP_UART_COUNT   1       /* LP_UART0 (low-power domain) */
#define ESP32C6_UART_FIFO_SIZE  128     /* Hardware FIFO size */

/* Default UART pins */
#define UART0_TX_DEFAULT        16
#define UART0_RX_DEFAULT        17
#define UART1_TX_DEFAULT        4
#define UART1_RX_DEFAULT        5
#define LP_UART_TX_DEFAULT      5
#define LP_UART_RX_DEFAULT      4

/*============================================================================
 * SPI Configuration
 *============================================================================*/
#define ESP32C6_SPI_COUNT       1       /* SPI2 (GPSPI) available for user */

/* SPI2 default pins */
#define SPI2_MOSI_DEFAULT       7
#define SPI2_MISO_DEFAULT       2
#define SPI2_CLK_DEFAULT        6
#define SPI2_CS_DEFAULT         10

/*============================================================================
 * I2C Configuration
 *============================================================================*/
#define ESP32C6_I2C_COUNT       1       /* I2C0 only */
#define ESP32C6_LP_I2C_COUNT    1       /* LP_I2C0 (low-power domain) */
#define ESP32C6_I2C_MAX_FREQ    1000000 /* 1MHz Fast Mode Plus */

/* I2C0 default pins */
#define I2C0_SDA_DEFAULT        6
#define I2C0_SCL_DEFAULT        7

/*============================================================================
 * Timer Configuration
 *============================================================================*/
#define ESP32C6_TIMER_GROUPS    2       /* Timer groups */
#define ESP32C6_TIMERS_PER_GROUP 1      /* 1 timer per group */
#define ESP32C6_TIMER_COUNT     2       /* Total hardware timers */

/*============================================================================
 * ADC Configuration
 *============================================================================*/
#define ESP32C6_ADC_UNITS       1       /* ADC1 only */
#define ESP32C6_ADC1_CHANNELS   7       /* GPIO 0-6 (channels 0-6) */
#define ESP32C6_ADC_RESOLUTION  12      /* 12-bit resolution */
#define ESP32C6_ADC_MAX_VALUE   4095

/* ADC1 Channel Mapping:
 * ADC1_CH0: GPIO0
 * ADC1_CH1: GPIO1
 * ADC1_CH2: GPIO2
 * ADC1_CH3: GPIO3
 * ADC1_CH4: GPIO4
 * ADC1_CH5: GPIO5
 * ADC1_CH6: GPIO6
 */

/*============================================================================
 * PWM (LEDC) Configuration
 *============================================================================*/
#define ESP32C6_LEDC_CHANNELS   6       /* 6 LEDC channels */
#define ESP32C6_LEDC_HS_CHANNELS 0      /* No high-speed channels */
#define ESP32C6_LEDC_LS_CHANNELS 6      /* All low-speed */
#define ESP32C6_LEDC_MAX_DUTY   ((1 << 14) - 1)
#define ESP32C6_LEDC_DEFAULT_FREQ 5000

/*============================================================================
 * RMT Configuration
 *============================================================================*/
#define ESP32C6_RMT_CHANNELS    4       /* RMT channels (2 TX, 2 RX) */

/*============================================================================
 * WiFi 6 Configuration (802.11ax)
 *============================================================================*/
#define ESP32C6_WIFI_VERSION    6       /* WiFi 6 (802.11ax) */
#define ESP32C6_WIFI_11AX       1       /* 802.11ax support */
#define ESP32C6_WIFI_SSID_MAX_LEN   32
#define ESP32C6_WIFI_PASS_MAX_LEN   64
#define ESP32C6_WIFI_MAX_CONNECTIONS 4

/* WiFi 6 specific features */
#define ESP32C6_WIFI6_OFDMA         1   /* OFDMA support */
#define ESP32C6_WIFI6_MU_MIMO       0   /* No MU-MIMO (STA mode) */
#define ESP32C6_WIFI6_TWT           1   /* Target Wake Time support */
#define ESP32C6_WIFI6_BSS_COLORING  1   /* BSS Coloring support */

/*============================================================================
 * Bluetooth Configuration (BLE 5.0 only)
 *============================================================================*/
#define ESP32C6_BLE_VERSION     5       /* BLE 5.0 */
#define ESP32C6_BT_DEVICE_NAME_LEN 32
#define ESP32C6_BLE_MTU_DEFAULT 23
#define ESP32C6_BLE_CODED_PHY   1       /* Supports Coded PHY */
#define ESP32C6_BLE_2M_PHY      1       /* Supports 2M PHY */

/*============================================================================
 * Thread/Zigbee Configuration (802.15.4)
 *============================================================================*/
#define ESP32C6_802154_ENABLED  1       /* IEEE 802.15.4 radio */
#define ESP32C6_THREAD_ENABLED  1       /* Thread protocol support */
#define ESP32C6_ZIGBEE_ENABLED  1       /* Zigbee protocol support */
#define ESP32C6_MATTER_READY    1       /* Matter-ready (Thread + WiFi) */

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define ESP32C6_SRAM_SIZE       (512 * 1024)    /* 512KB SRAM */
#define ESP32C6_FLASH_SIZE_DEFAULT (4 * 1024 * 1024) /* 4MB typical */
#define ESP32C6_ROM_SIZE        (320 * 1024)    /* 320KB ROM */
#define ESP32C6_LP_SRAM_SIZE    (16 * 1024)     /* 16KB Low-power SRAM */

/*============================================================================
 * Security Features
 *============================================================================*/
#define ESP32C6_HAS_SECURE_BOOT     1   /* Secure Boot V2 */
#define ESP32C6_HAS_FLASH_ENCRYPT   1   /* Flash Encryption */
#define ESP32C6_HAS_HMAC           1    /* HMAC accelerator */
#define ESP32C6_HAS_DS             1    /* Digital Signature */
#define ESP32C6_HAS_AES            1    /* AES accelerator */
#define ESP32C6_HAS_SHA            1    /* SHA accelerator */
#define ESP32C6_HAS_RSA            1    /* RSA accelerator */
#define ESP32C6_HAS_ECC            1    /* ECC accelerator */

/*============================================================================
 * FreeRTOS Configuration
 *============================================================================*/
#define ESP32C6_FREERTOS_TICK_RATE_HZ 1000
#define ESP32C6_TASK_STACK_MIN        2048

/*============================================================================
 * Strapping Pins
 *============================================================================*/
/* GPIO8:  Boot mode select (HIGH = SPI boot, LOW = Download boot)
 * GPIO9:  Boot button (active LOW)
 * GPIO15: 32kHz crystal input
 */
#define ESP32C6_BOOT_PIN        9
#define ESP32C6_STRAPPING_PINS  ((1 << 8) | (1 << 9) | (1 << 15))

/*============================================================================
 * Low-Power Features
 *============================================================================*/
#define ESP32C6_HAS_LP_CORE         1   /* Low-power RISC-V coprocessor */
#define ESP32C6_HAS_LP_GPIO         1   /* Low-power GPIO */
#define ESP32C6_HAS_LP_I2C          1   /* Low-power I2C */
#define ESP32C6_HAS_LP_UART         1   /* Low-power UART */

#ifdef __cplusplus
}
#endif

#endif /* ESP32C6_CONFIG_H */
