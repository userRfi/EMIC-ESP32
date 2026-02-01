/*****************************************************************************
 * @file    pwm.h
 * @brief   ESP32 PWM (LEDC) Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details PWM interface using ESP32 LEDC peripheral.
 *          16 channels total (8 high-speed + 8 low-speed).
 *****************************************************************************/

#ifndef ESP32_PWM_H
#define ESP32_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/* Number of PWM channels */
#define PWM_CHANNEL_COUNT       16
#define PWM_HS_CHANNELS         8   /* High-speed channels (0-7) */
#define PWM_LS_CHANNELS         8   /* Low-speed channels (8-15) */

/* Default configuration */
#define PWM_DEFAULT_FREQ        5000    /* 5 kHz */
#define PWM_DEFAULT_RESOLUTION  13      /* 13-bit (0-8191) */

/* Resolution limits */
#define PWM_MIN_RESOLUTION      1
#define PWM_MAX_RESOLUTION      16

/* Speed mode */
#define PWM_HIGH_SPEED          0
#define PWM_LOW_SPEED           1

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize PWM subsystem
 */
void PWM_init(void);

/**
 * @brief Deinitialize PWM subsystem
 */
void PWM_deinit(void);

/*============================================================================
 * Channel Configuration
 *============================================================================*/

/**
 * @brief Attach PWM to a GPIO pin
 *
 * @param gpio GPIO pin number (0-39, excluding input-only pins)
 * @param frequency PWM frequency in Hz (1 Hz to 40 MHz depending on resolution)
 * @param resolution Resolution in bits (1-16)
 * @return Channel number (0-15) or -1 on error
 */
int PWM_attach(uint8_t gpio, uint32_t frequency, uint8_t resolution);

/**
 * @brief Attach PWM with default settings (5kHz, 13-bit)
 *
 * @param gpio GPIO pin number
 * @return Channel number or -1 on error
 */
int PWM_attachDefault(uint8_t gpio);

/**
 * @brief Attach PWM to a specific channel
 *
 * @param channel Channel number (0-15)
 * @param gpio GPIO pin number
 * @param frequency PWM frequency in Hz
 * @param resolution Resolution in bits
 * @return true if successful
 */
bool PWM_attachChannel(uint8_t channel, uint8_t gpio, uint32_t frequency, uint8_t resolution);

/**
 * @brief Detach PWM from a channel
 *
 * @param channel Channel number
 */
void PWM_detach(uint8_t channel);

/**
 * @brief Detach PWM from a GPIO
 *
 * @param gpio GPIO pin number
 */
void PWM_detachGpio(uint8_t gpio);

/*============================================================================
 * Duty Cycle Control
 *============================================================================*/

/**
 * @brief Set PWM duty cycle (raw value)
 *
 * @param channel Channel number
 * @param duty Duty cycle value (0 to 2^resolution - 1)
 */
void PWM_write(uint8_t channel, uint32_t duty);

/**
 * @brief Set PWM duty cycle by GPIO
 *
 * @param gpio GPIO pin number
 * @param duty Duty cycle value
 */
void PWM_writeGpio(uint8_t gpio, uint32_t duty);

/**
 * @brief Set PWM duty cycle as percentage
 *
 * @param channel Channel number
 * @param percent Duty cycle percentage (0.0 - 100.0)
 */
void PWM_writePercent(uint8_t channel, float percent);

/**
 * @brief Set PWM duty cycle as percentage by GPIO
 *
 * @param gpio GPIO pin number
 * @param percent Duty cycle percentage
 */
void PWM_writeGpioPercent(uint8_t gpio, float percent);

/**
 * @brief Set PWM to 8-bit duty (0-255)
 *
 * Convenience function for Arduino-compatible analogWrite
 *
 * @param channel Channel number
 * @param duty 8-bit duty value (0-255)
 */
void PWM_write8bit(uint8_t channel, uint8_t duty);

/**
 * @brief Get current duty cycle
 *
 * @param channel Channel number
 * @return Current duty value
 */
uint32_t PWM_read(uint8_t channel);

/*============================================================================
 * Frequency Control
 *============================================================================*/

/**
 * @brief Set PWM frequency
 *
 * @param channel Channel number
 * @param frequency New frequency in Hz
 * @return true if successful
 */
bool PWM_setFrequency(uint8_t channel, uint32_t frequency);

/**
 * @brief Get current PWM frequency
 *
 * @param channel Channel number
 * @return Current frequency in Hz
 */
uint32_t PWM_getFrequency(uint8_t channel);

/**
 * @brief Get maximum duty value for channel's resolution
 *
 * @param channel Channel number
 * @return Maximum duty value (2^resolution - 1)
 */
uint32_t PWM_getMaxDuty(uint8_t channel);

/*============================================================================
 * Fade Functions
 *============================================================================*/

/**
 * @brief Fade PWM to a target duty over time
 *
 * @param channel Channel number
 * @param targetDuty Target duty value
 * @param fadeTime_ms Fade duration in milliseconds
 */
void PWM_fade(uint8_t channel, uint32_t targetDuty, uint32_t fadeTime_ms);

/**
 * @brief Fade PWM to percentage over time
 *
 * @param channel Channel number
 * @param targetPercent Target duty percentage
 * @param fadeTime_ms Fade duration in milliseconds
 */
void PWM_fadePercent(uint8_t channel, float targetPercent, uint32_t fadeTime_ms);

/**
 * @brief Wait for fade to complete
 *
 * @param channel Channel number
 */
void PWM_waitFade(uint8_t channel);

/**
 * @brief Check if fade is in progress
 *
 * @param channel Channel number
 * @return true if fading
 */
bool PWM_isFading(uint8_t channel);

/*============================================================================
 * Phase Control
 *============================================================================*/

/**
 * @brief Set PWM phase offset
 *
 * @param channel Channel number
 * @param phase Phase offset (0 to 2^resolution - 1)
 */
void PWM_setPhase(uint8_t channel, uint32_t phase);

/*============================================================================
 * Utility Functions
 *============================================================================*/

/**
 * @brief Get channel for a GPIO
 *
 * @param gpio GPIO pin number
 * @return Channel number or -1 if not attached
 */
int PWM_getChannel(uint8_t gpio);

/**
 * @brief Check if channel is in use
 *
 * @param channel Channel number
 * @return true if channel is attached to a GPIO
 */
bool PWM_isAttached(uint8_t channel);

/**
 * @brief Get resolution of a channel
 *
 * @param channel Channel number
 * @return Resolution in bits
 */
uint8_t PWM_getResolution(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_PWM_H */
