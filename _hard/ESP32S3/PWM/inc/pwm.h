/*****************************************************************************
 * @file    pwm.h
 * @brief   ESP32-S3 PWM (LEDC) Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details PWM interface for ESP32-S3 using LEDC peripheral.
 *          8 channels total (all low-speed, no high-speed on S3).
 *****************************************************************************/

#ifndef ESP32S3_PWM_H
#define ESP32S3_PWM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration (ESP32-S3 specific)
 *============================================================================*/

/* ESP32-S3 has 8 LEDC channels, all low-speed */
#define PWM_CHANNEL_COUNT       8
#define PWM_HS_CHANNELS         0   /* No high-speed on S3 */
#define PWM_LS_CHANNELS         8   /* All channels are low-speed */

/* Default configuration */
#define PWM_DEFAULT_FREQ        5000    /* 5 kHz */
#define PWM_DEFAULT_RESOLUTION  13      /* 13-bit (0-8191) */

/* Resolution limits */
#define PWM_MIN_RESOLUTION      1
#define PWM_MAX_RESOLUTION      14  /* S3 supports up to 14-bit */

/* Speed mode (S3 only has low speed) */
#define PWM_LOW_SPEED           0

/*============================================================================
 * Initialization
 *============================================================================*/

void PWM_init(void);
void PWM_deinit(void);

/*============================================================================
 * Channel Configuration
 *============================================================================*/

int PWM_attach(uint8_t gpio, uint32_t frequency, uint8_t resolution);
int PWM_attachDefault(uint8_t gpio);
bool PWM_attachChannel(uint8_t channel, uint8_t gpio, uint32_t frequency, uint8_t resolution);
void PWM_detach(uint8_t channel);
void PWM_detachGpio(uint8_t gpio);

/*============================================================================
 * Duty Cycle Control
 *============================================================================*/

void PWM_write(uint8_t channel, uint32_t duty);
void PWM_writeGpio(uint8_t gpio, uint32_t duty);
void PWM_writePercent(uint8_t channel, float percent);
void PWM_writeGpioPercent(uint8_t gpio, float percent);
void PWM_write8bit(uint8_t channel, uint8_t duty);
uint32_t PWM_read(uint8_t channel);

/*============================================================================
 * Frequency Control
 *============================================================================*/

bool PWM_setFrequency(uint8_t channel, uint32_t frequency);
uint32_t PWM_getFrequency(uint8_t channel);
uint32_t PWM_getMaxDuty(uint8_t channel);

/*============================================================================
 * Fade Functions
 *============================================================================*/

void PWM_fade(uint8_t channel, uint32_t targetDuty, uint32_t fadeTime_ms);
void PWM_fadePercent(uint8_t channel, float targetPercent, uint32_t fadeTime_ms);
void PWM_waitFade(uint8_t channel);
bool PWM_isFading(uint8_t channel);

/*============================================================================
 * Utility Functions
 *============================================================================*/

int PWM_getChannel(uint8_t gpio);
bool PWM_isAttached(uint8_t channel);
uint8_t PWM_getResolution(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_PWM_H */
