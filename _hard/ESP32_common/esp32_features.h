/*****************************************************************************
 * @file    esp32_features.h
 * @brief   Feature Configuration Per ESP32 Variant
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Detailed feature configuration and peripheral counts for each
 *          ESP32 chip variant. Include this after the variant-specific
 *          config header.
 *****************************************************************************/

#ifndef ESP32_FEATURES_H
#define ESP32_FEATURES_H

#ifdef __cplusplus
extern "C" {
#endif

#include "esp32_common.h"

/*============================================================================
 * GPIO Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_GPIO_COUNT        40
    #define ESP32_GPIO_INPUT_ONLY   0x0000003F00000000ULL  /* GPIOs 34-39 */
    #define GPIO_IS_INPUT_ONLY(pin) ((pin) >= 34 && (pin) <= 39)
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_GPIO_COUNT        43
    #define ESP32_GPIO_INPUT_ONLY   0x0000000600000000ULL  /* GPIOs 46 */
    #define GPIO_IS_INPUT_ONLY(pin) ((pin) == 46)
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_GPIO_COUNT        45
    #define ESP32_GPIO_INPUT_ONLY   0x0000000000000000ULL  /* None */
    #define GPIO_IS_INPUT_ONLY(pin) (false)
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_GPIO_COUNT        22
    #define ESP32_GPIO_INPUT_ONLY   0x0000000000000000ULL  /* None */
    #define GPIO_IS_INPUT_ONLY(pin) (false)
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_GPIO_COUNT        31
    #define ESP32_GPIO_INPUT_ONLY   0x0000000000000000ULL  /* None */
    #define GPIO_IS_INPUT_ONLY(pin) (false)
#endif

#define GPIO_IS_VALID(pin)      ((pin) >= 0 && (pin) < ESP32_GPIO_COUNT)

/*============================================================================
 * UART Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_UART_COUNT        3   /* UART0, UART1, UART2 */
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_UART_COUNT        2   /* UART0, UART1 */
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_UART_COUNT        3   /* UART0, UART1, UART2 */
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_UART_COUNT        2   /* UART0, UART1 */
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_UART_COUNT        2   /* UART0, UART1 (LP_UART separate) */
#endif

#define ESP32_UART_FIFO_SIZE        128

/*============================================================================
 * SPI Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_SPI_COUNT         3   /* SPI1 (flash), SPI2 (HSPI), SPI3 (VSPI) */
    #define ESP32_SPI_USABLE        2   /* Only SPI2, SPI3 for general use */
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_SPI_COUNT         4   /* SPI0-1 (flash), SPI2, SPI3 */
    #define ESP32_SPI_USABLE        2
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_SPI_COUNT         3   /* SPI0-1 (flash), SPI2, SPI3 */
    #define ESP32_SPI_USABLE        2
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_SPI_COUNT         2   /* SPI0-1 (flash), SPI2 */
    #define ESP32_SPI_USABLE        1
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_SPI_COUNT         2   /* SPI0-1 (flash), SPI2 */
    #define ESP32_SPI_USABLE        1
#endif

/*============================================================================
 * I2C Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_I2C_COUNT         2   /* I2C0, I2C1 */
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_I2C_COUNT         2   /* I2C0, I2C1 */
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_I2C_COUNT         2   /* I2C0, I2C1 */
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_I2C_COUNT         1   /* I2C0 only */
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_I2C_COUNT         2   /* I2C0, LP_I2C */
#endif

#define ESP32_I2C_MAX_FREQ          1000000  /* 1MHz Fast Mode Plus */

/*============================================================================
 * ADC Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_ADC_UNITS         2
    #define ESP32_ADC1_CHANNELS     8   /* GPIO 32-39 */
    #define ESP32_ADC2_CHANNELS     10  /* GPIO 0,2,4,12-15,25-27 */
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_ADC_UNITS         2
    #define ESP32_ADC1_CHANNELS     10  /* GPIO 1-10 */
    #define ESP32_ADC2_CHANNELS     10  /* GPIO 11-20 */
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_ADC_UNITS         2
    #define ESP32_ADC1_CHANNELS     10  /* GPIO 1-10 */
    #define ESP32_ADC2_CHANNELS     10  /* GPIO 11-20 */
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_ADC_UNITS         2
    #define ESP32_ADC1_CHANNELS     5   /* GPIO 0-4 */
    #define ESP32_ADC2_CHANNELS     1   /* GPIO 5 */
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_ADC_UNITS         1
    #define ESP32_ADC1_CHANNELS     7   /* GPIO 0-6 */
    #define ESP32_ADC2_CHANNELS     0   /* Not available */
#endif

#define ESP32_ADC_RESOLUTION        12
#define ESP32_ADC_MAX_VALUE         4095

/*============================================================================
 * PWM (LEDC) Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_LEDC_CHANNELS     16
    #define ESP32_LEDC_HS_CHANNELS  8   /* High-speed (80MHz source) */
    #define ESP32_LEDC_LS_CHANNELS  8   /* Low-speed (8MHz source) */
#elif defined(EMIC_CHIP_ESP32S2) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_LEDC_CHANNELS     8
    #define ESP32_LEDC_HS_CHANNELS  0   /* No high-speed on S2/S3 */
    #define ESP32_LEDC_LS_CHANNELS  8
#elif defined(EMIC_CHIP_ESP32C3) || defined(EMIC_CHIP_ESP32C6)
    #define ESP32_LEDC_CHANNELS     6
    #define ESP32_LEDC_HS_CHANNELS  0   /* No high-speed on C3/C6 */
    #define ESP32_LEDC_LS_CHANNELS  6
#endif

#define ESP32_LEDC_MAX_DUTY         ((1 << 13) - 1)
#define ESP32_LEDC_DEFAULT_FREQ     5000

/*============================================================================
 * Timer Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_TIMER_GROUPS      2
    #define ESP32_TIMERS_PER_GROUP  2
    #define ESP32_TIMER_COUNT       4
#elif defined(EMIC_CHIP_ESP32S2) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_TIMER_GROUPS      2
    #define ESP32_TIMERS_PER_GROUP  2
    #define ESP32_TIMER_COUNT       4
#elif defined(EMIC_CHIP_ESP32C3) || defined(EMIC_CHIP_ESP32C6)
    #define ESP32_TIMER_GROUPS      2
    #define ESP32_TIMERS_PER_GROUP  1
    #define ESP32_TIMER_COUNT       2
#endif

/*============================================================================
 * Touch Sensor Configuration (if available)
 *============================================================================*/

#if ESP32_HAS_TOUCH
    #if defined(EMIC_CHIP_ESP32)
        #define ESP32_TOUCH_CHANNELS    10  /* T0-T9 */
    #elif defined(EMIC_CHIP_ESP32S2) || defined(EMIC_CHIP_ESP32S3)
        #define ESP32_TOUCH_CHANNELS    14  /* T1-T14 */
    #endif
#else
    #define ESP32_TOUCH_CHANNELS        0
#endif

/*============================================================================
 * Clock Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_CPU_FREQ_MAX_MHZ  240
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_CPU_FREQ_MAX_MHZ  240
#elif defined(EMIC_CHIP_ESP32C3) || defined(EMIC_CHIP_ESP32C6)
    #define ESP32_CPU_FREQ_MAX_MHZ  160
#endif

#define ESP32_APB_FREQ_MHZ          80
#define ESP32_XTAL_FREQ_MHZ         40

/*============================================================================
 * Memory Configuration Per Variant
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32)
    #define ESP32_SRAM_SIZE             (520 * 1024)
#elif defined(EMIC_CHIP_ESP32S2)
    #define ESP32_SRAM_SIZE             (320 * 1024)
#elif defined(EMIC_CHIP_ESP32S3)
    #define ESP32_SRAM_SIZE             (512 * 1024)
#elif defined(EMIC_CHIP_ESP32C3)
    #define ESP32_SRAM_SIZE             (400 * 1024)
#elif defined(EMIC_CHIP_ESP32C6)
    #define ESP32_SRAM_SIZE             (512 * 1024)
#endif

#define ESP32_FLASH_SIZE_DEFAULT    (4 * 1024 * 1024)

/*============================================================================
 * WiFi Configuration (common to all)
 *============================================================================*/

#define ESP32_WIFI_SSID_MAX_LEN     32
#define ESP32_WIFI_PASS_MAX_LEN     64
#define ESP32_WIFI_MAX_CONNECTIONS  4

/*============================================================================
 * Bluetooth Configuration
 *============================================================================*/

#define ESP32_BT_DEVICE_NAME_LEN    32
#define ESP32_BLE_MTU_DEFAULT       23

#if defined(EMIC_CHIP_ESP32S3) || defined(EMIC_CHIP_ESP32C3) || \
    defined(EMIC_CHIP_ESP32C6)
    #define ESP32_BLE_VERSION       5   /* BLE 5.0 */
#elif defined(EMIC_CHIP_ESP32)
    #define ESP32_BLE_VERSION       4   /* BLE 4.2 */
#else
    #define ESP32_BLE_VERSION       0   /* No BLE */
#endif

#ifdef __cplusplus
}
#endif

#endif /* ESP32_FEATURES_H */
