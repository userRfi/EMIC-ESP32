/*****************************************************************************
 * @file    touch.h
 * @brief   ESP32-S2 Touch Sensor Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Touch sensor header for ESP32-S2.
 *          14 touch channels (T1-T14) mapped to GPIO 1-14.
 *****************************************************************************/

#ifndef ESP32S2_TOUCH_H
#define ESP32S2_TOUCH_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Touch Sensor Configuration
 *============================================================================*/
#define TOUCH_CHANNEL_COUNT     14      /* T1-T14 */
#define TOUCH_CHANNEL_MIN       1
#define TOUCH_CHANNEL_MAX       14

/* Touch channel to GPIO mapping for ESP32-S2 */
#define TOUCH_CH1_GPIO          1
#define TOUCH_CH2_GPIO          2
#define TOUCH_CH3_GPIO          3
#define TOUCH_CH4_GPIO          4
#define TOUCH_CH5_GPIO          5
#define TOUCH_CH6_GPIO          6
#define TOUCH_CH7_GPIO          7
#define TOUCH_CH8_GPIO          8
#define TOUCH_CH9_GPIO          9
#define TOUCH_CH10_GPIO         10
#define TOUCH_CH11_GPIO         11
#define TOUCH_CH12_GPIO         12
#define TOUCH_CH13_GPIO         13
#define TOUCH_CH14_GPIO         14

/* Default threshold (can be calibrated) */
#define TOUCH_DEFAULT_THRESHOLD 40000

/*============================================================================
 * Touch Sensor Functions
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
 * @brief Configure a touch channel for sensing
 * @param channel Touch channel number (1-14)
 * @return 0 on success, -1 on error
 */
int Touch_configChannel(uint8_t channel);

/**
 * @brief Read raw touch sensor value
 * @param channel Touch channel number (1-14)
 * @return Raw touch value (lower = touched), 0 on error
 */
uint32_t Touch_read(uint8_t channel);

/**
 * @brief Check if a touch channel is currently being touched
 * @param channel Touch channel number (1-14)
 * @return true if touched, false otherwise
 */
bool Touch_isTouched(uint8_t channel);

/**
 * @brief Set detection threshold for a touch channel
 * @param channel Touch channel number (1-14)
 * @param threshold Touch detection threshold
 */
void Touch_setThreshold(uint8_t channel, uint32_t threshold);

/**
 * @brief Get current threshold for a touch channel
 * @param channel Touch channel number (1-14)
 * @return Current threshold value
 */
uint32_t Touch_getThreshold(uint8_t channel);

/**
 * @brief Enable touch sensor wakeup from deep sleep
 * @param channel Touch channel to use for wakeup
 * @param threshold Wakeup threshold
 * @return 0 on success, -1 on error
 */
int Touch_enableWakeup(uint8_t channel, uint32_t threshold);

/**
 * @brief Convert touch channel number to GPIO number
 * @param channel Touch channel number (1-14)
 * @return GPIO number, or -1 if invalid channel
 */
int Touch_channelToGPIO(uint8_t channel);

/**
 * @brief Convert GPIO number to touch channel
 * @param gpio GPIO number (1-14)
 * @return Touch channel number, or -1 if not a touch GPIO
 */
int Touch_GPIOToChannel(uint8_t gpio);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_TOUCH_H */
