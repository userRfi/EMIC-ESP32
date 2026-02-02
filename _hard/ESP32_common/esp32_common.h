/*****************************************************************************
 * @file    esp32_common.h
 * @brief   Common Definitions for All ESP32 Variants
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details This header provides common definitions, feature flags, and
 *          macros that are shared across all ESP32 chip variants
 *          (ESP32, ESP32-S2, ESP32-S3, ESP32-C3, ESP32-C6).
 *****************************************************************************/

#ifndef ESP32_COMMON_H
#define ESP32_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "sdkconfig.h"

/*============================================================================
 * Chip Detection (from ESP-IDF)
 *============================================================================*/

#if CONFIG_IDF_TARGET_ESP32
    #define EMIC_CHIP_ESP32         1
    #define EMIC_CHIP_FAMILY        "ESP32"
    #define EMIC_CHIP_ARCH          "Xtensa LX6"
    #define EMIC_CHIP_CORES         2
#elif CONFIG_IDF_TARGET_ESP32S2
    #define EMIC_CHIP_ESP32S2       1
    #define EMIC_CHIP_FAMILY        "ESP32-S2"
    #define EMIC_CHIP_ARCH          "Xtensa LX7"
    #define EMIC_CHIP_CORES         1
#elif CONFIG_IDF_TARGET_ESP32S3
    #define EMIC_CHIP_ESP32S3       1
    #define EMIC_CHIP_FAMILY        "ESP32-S3"
    #define EMIC_CHIP_ARCH          "Xtensa LX7"
    #define EMIC_CHIP_CORES         2
#elif CONFIG_IDF_TARGET_ESP32C3
    #define EMIC_CHIP_ESP32C3       1
    #define EMIC_CHIP_FAMILY        "ESP32-C3"
    #define EMIC_CHIP_ARCH          "RISC-V"
    #define EMIC_CHIP_CORES         1
#elif CONFIG_IDF_TARGET_ESP32C6
    #define EMIC_CHIP_ESP32C6       1
    #define EMIC_CHIP_FAMILY        "ESP32-C6"
    #define EMIC_CHIP_ARCH          "RISC-V"
    #define EMIC_CHIP_CORES         1
#else
    #error "Unsupported ESP32 chip variant"
#endif

/*============================================================================
 * Feature Detection Macros
 *============================================================================*/

/* Bluetooth Classic - only on original ESP32 */
#if defined(EMIC_CHIP_ESP32)
    #define ESP32_HAS_BT_CLASSIC    1
#else
    #define ESP32_HAS_BT_CLASSIC    0
#endif

/* BLE - available on all except ESP32-S2 */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S3) || \
    defined(EMIC_CHIP_ESP32C3) || defined(EMIC_CHIP_ESP32C6)
    #define ESP32_HAS_BLE           1
#else
    #define ESP32_HAS_BLE           0
#endif

/* WiFi - available on all variants */
#define ESP32_HAS_WIFI              1

/* WiFi 6 (802.11ax) - only on C6 */
#if defined(EMIC_CHIP_ESP32C6)
    #define ESP32_HAS_WIFI6         1
#else
    #define ESP32_HAS_WIFI6         0
#endif

/* USB OTG - available on S2 and S3 */
#if defined(EMIC_CHIP_ESP32S2) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_USB_OTG       1
#else
    #define ESP32_HAS_USB_OTG       0
#endif

/* USB Serial/JTAG - available on S3, C3, C6 */
#if defined(EMIC_CHIP_ESP32S3) || defined(EMIC_CHIP_ESP32C3) || \
    defined(EMIC_CHIP_ESP32C6)
    #define ESP32_HAS_USB_SERIAL    1
#else
    #define ESP32_HAS_USB_SERIAL    0
#endif

/* Touch Sensing - available on ESP32, S2, S3 */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S2) || \
    defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_TOUCH         1
#else
    #define ESP32_HAS_TOUCH         0
#endif

/* DAC - only on ESP32 and S2 */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S2)
    #define ESP32_HAS_DAC           1
#else
    #define ESP32_HAS_DAC           0
#endif

/* Hall Sensor - only on original ESP32 */
#if defined(EMIC_CHIP_ESP32)
    #define ESP32_HAS_HALL          1
#else
    #define ESP32_HAS_HALL          0
#endif

/* PSRAM Support */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S2) || \
    defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_PSRAM         1
#else
    #define ESP32_HAS_PSRAM         0
#endif

/* Ethernet MAC - only on original ESP32 */
#if defined(EMIC_CHIP_ESP32)
    #define ESP32_HAS_ETHERNET      1
#else
    #define ESP32_HAS_ETHERNET      0
#endif

/* SD/MMC Host - available on ESP32 and S3 */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_SDMMC         1
#else
    #define ESP32_HAS_SDMMC         0
#endif

/* Camera Interface (DVP) - ESP32 and S3 */
#if defined(EMIC_CHIP_ESP32) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_CAMERA        1
#else
    #define ESP32_HAS_CAMERA        0
#endif

/* LCD Interface - S2 and S3 */
#if defined(EMIC_CHIP_ESP32S2) || defined(EMIC_CHIP_ESP32S3)
    #define ESP32_HAS_LCD           1
#else
    #define ESP32_HAS_LCD           0
#endif

/*============================================================================
 * Architecture Detection
 *============================================================================*/

#if defined(EMIC_CHIP_ESP32C3) || defined(EMIC_CHIP_ESP32C6)
    #define EMIC_ARCH_RISCV         1
    #define EMIC_ARCH_XTENSA        0
#else
    #define EMIC_ARCH_RISCV         0
    #define EMIC_ARCH_XTENSA        1
#endif

/*============================================================================
 * ESP-IDF Version Check
 *============================================================================*/

#define ESP32_MIN_IDF_VERSION_MAJOR     5
#define ESP32_MIN_IDF_VERSION_MINOR     0

/*============================================================================
 * Common Framework Settings
 *============================================================================*/

#define MCU_FRAMEWORK           "ESP-IDF"
#define ESP32_FREERTOS_TICK_RATE_HZ 1000
#define ESP32_TASK_STACK_MIN        2048

/*============================================================================
 * Feature Stubs for Unavailable Peripherals
 *============================================================================*/

/* Bluetooth Classic stubs for chips without it */
#if !ESP32_HAS_BT_CLASSIC
    #define BT_Classic_init()               ((void)0)
    #define BT_Classic_isConnected()        (false)
    #define BT_Classic_send(data, len)      ((void)0)
    #define BT_Classic_available()          (0)
#endif

/* BLE stubs for chips without it (S2) */
#if !ESP32_HAS_BLE
    #define BLE_init()                      ((void)0)
    #define BLE_startAdvertising()          (false)
    #define BLE_stopAdvertising()           (false)
    #define BLE_isConnected()               (false)
#endif

/* Touch stubs for chips without it (C3, C6) */
#if !ESP32_HAS_TOUCH
    #define Touch_init(pin)                 ((void)0)
    #define Touch_read(pin)                 (0)
    #define Touch_setThreshold(pin, th)     ((void)0)
#endif

/* DAC stubs for chips without it */
#if !ESP32_HAS_DAC
    #define DAC_write(channel, value)       ((void)0)
    #define DAC_enable(channel)             ((void)0)
    #define DAC_disable(channel)            ((void)0)
#endif

/* USB OTG stubs for chips without it */
#if !ESP32_HAS_USB_OTG
    #define USB_init()                      ((void)0)
    #define USB_isConnected()               (false)
    #define USB_send(data, len)             (0)
    #define USB_available()                 (0)
#endif

/*============================================================================
 * Utility Macros
 *============================================================================*/

/* Compile-time feature check */
#define ESP32_REQUIRE_FEATURE(feature) \
    _Static_assert(ESP32_HAS_##feature, "Feature " #feature " not available on this chip")

/* Runtime feature check */
#define ESP32_CHECK_FEATURE(feature) \
    do { \
        if (!ESP32_HAS_##feature) { \
            ESP_LOGE("EMIC", "Feature " #feature " not available on %s", EMIC_CHIP_FAMILY); \
            return ESP_ERR_NOT_SUPPORTED; \
        } \
    } while(0)

#ifdef __cplusplus
}
#endif

#endif /* ESP32_COMMON_H */
