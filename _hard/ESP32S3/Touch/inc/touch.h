/*****************************************************************************
 * @file    touch.h
 * @brief   ESP32-S3 Touch Sensor Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Touch sensor interface for ESP32-S3.
 *          14 touch channels (T1-T14) on GPIO 1-14.
 *****************************************************************************/

#ifndef ESP32S3_TOUCH_H
#define ESP32S3_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/* Number of touch channels on ESP32-S3 */
#define TOUCH_CHANNEL_COUNT     14

/* Touch channel to GPIO mapping (ESP32-S3) */
#define TOUCH_GPIO_MAP { \
    1,  /* T1 = GPIO1 */  \
    2,  /* T2 = GPIO2 */  \
    3,  /* T3 = GPIO3 */  \
    4,  /* T4 = GPIO4 */  \
    5,  /* T5 = GPIO5 */  \
    6,  /* T6 = GPIO6 */  \
    7,  /* T7 = GPIO7 */  \
    8,  /* T8 = GPIO8 */  \
    9,  /* T9 = GPIO9 */  \
    10, /* T10 = GPIO10 */ \
    11, /* T11 = GPIO11 */ \
    12, /* T12 = GPIO12 */ \
    13, /* T13 = GPIO13 */ \
    14  /* T14 = GPIO14 */ \
}

/* Default threshold (higher = less sensitive) */
#define TOUCH_DEFAULT_THRESHOLD     40

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize touch sensor subsystem
 */
void Touch_init(void);

/**
 * @brief Deinitialize touch sensor subsystem
 */
void Touch_deinit(void);

/**
 * @brief Configure a touch channel
 *
 * @param channel Touch channel number (1-14)
 * @return true if successful
 */
bool Touch_configChannel(uint8_t channel);

/**
 * @brief Configure touch channel by GPIO number
 *
 * @param gpio GPIO number (1-14)
 * @return true if successful
 */
bool Touch_configGpio(uint8_t gpio);

/*============================================================================
 * Reading Functions
 *============================================================================*/

/**
 * @brief Read raw touch sensor value
 *
 * @param channel Touch channel number (1-14)
 * @return Raw touch value (lower value = touched)
 */
uint32_t Touch_read(uint8_t channel);

/**
 * @brief Read touch sensor by GPIO number
 *
 * @param gpio GPIO number (1-14)
 * @return Raw touch value
 */
uint32_t Touch_readGpio(uint8_t gpio);

/**
 * @brief Check if a touch channel is being touched
 *
 * @param channel Touch channel number (1-14)
 * @return true if touched
 */
bool Touch_isTouched(uint8_t channel);

/**
 * @brief Check if a GPIO is being touched
 *
 * @param gpio GPIO number (1-14)
 * @return true if touched
 */
bool Touch_isTouchedGpio(uint8_t gpio);

/*============================================================================
 * Threshold Configuration
 *============================================================================*/

/**
 * @brief Set touch detection threshold
 *
 * Lower threshold = more sensitive
 *
 * @param channel Touch channel number
 * @param threshold Threshold value (percentage of untouched value)
 */
void Touch_setThreshold(uint8_t channel, uint32_t threshold);

/**
 * @brief Get current threshold for channel
 *
 * @param channel Touch channel number
 * @return Current threshold value
 */
uint32_t Touch_getThreshold(uint8_t channel);

/**
 * @brief Auto-calibrate threshold for a channel
 *
 * Takes multiple readings and sets threshold based on average
 *
 * @param channel Touch channel number
 * @param sensitivity Sensitivity (1-100, higher = more sensitive)
 */
void Touch_calibrate(uint8_t channel, uint8_t sensitivity);

/*============================================================================
 * Interrupt/Wakeup Functions
 *============================================================================*/

typedef void (*touch_callback_t)(uint8_t channel);

/**
 * @brief Enable touch interrupt for channel
 *
 * @param channel Touch channel number
 * @param callback Function to call when touched
 * @return true if successful
 */
bool Touch_enableInterrupt(uint8_t channel, touch_callback_t callback);

/**
 * @brief Disable touch interrupt for channel
 *
 * @param channel Touch channel number
 */
void Touch_disableInterrupt(uint8_t channel);

/**
 * @brief Enable touch wakeup from deep sleep
 *
 * @param channelMask Bitmask of channels to enable (bit 0 = T1, etc.)
 * @return true if successful
 */
bool Touch_enableWakeup(uint16_t channelMask);

/*============================================================================
 * Utility Functions
 *============================================================================*/

/**
 * @brief Convert GPIO number to touch channel
 *
 * @param gpio GPIO number
 * @return Touch channel number or 0 if not a touch GPIO
 */
uint8_t Touch_gpioToChannel(uint8_t gpio);

/**
 * @brief Convert touch channel to GPIO number
 *
 * @param channel Touch channel number
 * @return GPIO number or -1 if invalid
 */
int Touch_channelToGpio(uint8_t channel);

/**
 * @brief Get benchmark (untouched) value for a channel
 *
 * @param channel Touch channel number
 * @return Benchmark value
 */
uint32_t Touch_getBenchmark(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_TOUCH_H */
