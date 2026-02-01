/*****************************************************************************
 * @file    esp32_config.h
 * @brief   ESP32 Hardware Configuration for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details Configuration file for ESP32 microcontroller family.
 *          Supports ESP32, ESP32-S2, ESP32-S3, ESP32-C3 variants.
 *****************************************************************************/

#ifndef ESP32_CONFIG_H
#define ESP32_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 * ESP-IDF Version Requirements
 *============================================================================*/
#define ESP32_MIN_IDF_VERSION_MAJOR     5
#define ESP32_MIN_IDF_VERSION_MINOR     0

/*============================================================================
 * MCU Identification
 *============================================================================*/
#define MCU_FAMILY              "ESP32"
#define MCU_ARCHITECTURE        "Xtensa LX6"
#define MCU_FRAMEWORK           "ESP-IDF"

/*============================================================================
 * Clock Configuration
 *============================================================================*/
#define ESP32_CPU_FREQ_MHZ      240         /* CPU frequency in MHz */
#define ESP32_APB_FREQ_MHZ      80          /* APB bus frequency in MHz */
#define ESP32_XTAL_FREQ_MHZ     40          /* External crystal frequency */

/*============================================================================
 * GPIO Configuration
 *============================================================================*/
#define ESP32_GPIO_COUNT        40          /* Total GPIO pins (0-39) */
#define ESP32_GPIO_INPUT_ONLY   0x0000000F00000000ULL  /* GPIOs 34-39 are input only */

/* GPIO pin masks */
#define GPIO_IS_INPUT_ONLY(pin) ((pin) >= 34 && (pin) <= 39)
#define GPIO_IS_VALID(pin)      ((pin) >= 0 && (pin) < ESP32_GPIO_COUNT)

/*============================================================================
 * UART Configuration
 *============================================================================*/
#define ESP32_UART_COUNT        3           /* UART0, UART1, UART2 */
#define ESP32_UART_FIFO_SIZE    128         /* Hardware FIFO size */

/* Default UART pins */
#define UART0_TX_DEFAULT        1
#define UART0_RX_DEFAULT        3
#define UART1_TX_DEFAULT        10
#define UART1_RX_DEFAULT        9
#define UART2_TX_DEFAULT        17
#define UART2_RX_DEFAULT        16

/*============================================================================
 * SPI Configuration
 *============================================================================*/
#define ESP32_SPI_COUNT         3           /* SPI1 (internal), SPI2 (HSPI), SPI3 (VSPI) */

/* HSPI (SPI2) default pins */
#define HSPI_MOSI_DEFAULT       13
#define HSPI_MISO_DEFAULT       12
#define HSPI_CLK_DEFAULT        14
#define HSPI_CS_DEFAULT         15

/* VSPI (SPI3) default pins */
#define VSPI_MOSI_DEFAULT       23
#define VSPI_MISO_DEFAULT       19
#define VSPI_CLK_DEFAULT        18
#define VSPI_CS_DEFAULT         5

/*============================================================================
 * I2C Configuration
 *============================================================================*/
#define ESP32_I2C_COUNT         2           /* I2C0, I2C1 */
#define ESP32_I2C_MAX_FREQ      1000000     /* 1MHz Fast Mode Plus */

/* I2C0 default pins */
#define I2C0_SDA_DEFAULT        21
#define I2C0_SCL_DEFAULT        22

/* I2C1 default pins */
#define I2C1_SDA_DEFAULT        25
#define I2C1_SCL_DEFAULT        26

/*============================================================================
 * Timer Configuration
 *============================================================================*/
#define ESP32_TIMER_GROUPS      2           /* Timer groups */
#define ESP32_TIMERS_PER_GROUP  2           /* Timers per group */
#define ESP32_TIMER_COUNT       4           /* Total hardware timers */

/*============================================================================
 * ADC Configuration
 *============================================================================*/
#define ESP32_ADC_UNITS         2           /* ADC1, ADC2 */
#define ESP32_ADC1_CHANNELS     8           /* ADC1: GPIO 32-39 */
#define ESP32_ADC2_CHANNELS     10          /* ADC2: GPIO 0,2,4,12-15,25-27 */
#define ESP32_ADC_RESOLUTION    12          /* 12-bit resolution (0-4095) */
#define ESP32_ADC_MAX_VALUE     4095        /* Maximum ADC value */

/* ADC attenuation settings */
#define ADC_ATTEN_0dB           0           /* 0-800mV range */
#define ADC_ATTEN_2_5dB         1           /* 0-1100mV range */
#define ADC_ATTEN_6dB           2           /* 0-1350mV range */
#define ADC_ATTEN_11dB          3           /* 0-2600mV range (default) */

/*============================================================================
 * PWM (LEDC) Configuration
 *============================================================================*/
#define ESP32_LEDC_CHANNELS     16          /* Total LEDC channels */
#define ESP32_LEDC_HS_CHANNELS  8           /* High-speed channels */
#define ESP32_LEDC_LS_CHANNELS  8           /* Low-speed channels */
#define ESP32_LEDC_MAX_DUTY     ((1 << 13) - 1)  /* 13-bit resolution max duty */
#define ESP32_LEDC_DEFAULT_FREQ 5000        /* Default PWM frequency in Hz */

/*============================================================================
 * WiFi Configuration
 *============================================================================*/
#define ESP32_WIFI_SSID_MAX_LEN     32      /* Maximum SSID length */
#define ESP32_WIFI_PASS_MAX_LEN     64      /* Maximum password length */
#define ESP32_WIFI_MAX_CONNECTIONS  4       /* Max STA connections in AP mode */

/*============================================================================
 * Bluetooth Configuration
 *============================================================================*/
#define ESP32_BT_DEVICE_NAME_LEN    32      /* Maximum BT device name length */
#define ESP32_BLE_MTU_DEFAULT       23      /* Default BLE MTU size */

/*============================================================================
 * Memory Configuration
 *============================================================================*/
#define ESP32_SRAM_SIZE             (520 * 1024)    /* 520KB SRAM */
#define ESP32_FLASH_SIZE_DEFAULT    (4 * 1024 * 1024) /* 4MB default flash */

/*============================================================================
 * FreeRTOS Configuration
 *============================================================================*/
#define ESP32_FREERTOS_TICK_RATE_HZ 1000    /* 1ms tick rate */
#define ESP32_TASK_STACK_MIN        2048    /* Minimum task stack size */

#ifdef __cplusplus
}
#endif

#endif /* ESP32_CONFIG_H */
