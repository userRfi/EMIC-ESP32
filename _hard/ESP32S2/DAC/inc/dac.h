/*****************************************************************************
 * @file    dac.h
 * @brief   ESP32-S2 DAC Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Digital-to-Analog Converter header for ESP32-S2.
 *          2 DAC channels: DAC1 on GPIO17, DAC2 on GPIO18
 *****************************************************************************/

#ifndef ESP32S2_DAC_H
#define ESP32S2_DAC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * DAC Configuration
 *============================================================================*/
#define DAC_CHANNEL_COUNT       2
#define DAC_RESOLUTION_BITS     8
#define DAC_MAX_VALUE           255

/* DAC channel to GPIO mapping */
#define DAC_CHANNEL_1           1
#define DAC_CHANNEL_2           2
#define DAC_CH1_GPIO            17
#define DAC_CH2_GPIO            18

/* Reference voltage (approximately) */
#define DAC_VREF_MV             3300    /* 3.3V reference */

/*============================================================================
 * DAC Functions
 *============================================================================*/

/**
 * @brief Initialize DAC subsystem
 */
void DAC_init(void);

/**
 * @brief Deinitialize DAC subsystem
 */
void DAC_deinit(void);

/**
 * @brief Enable a DAC channel
 * @param channel DAC channel (1 or 2)
 * @return 0 on success, -1 on error
 */
int DAC_enable(uint8_t channel);

/**
 * @brief Disable a DAC channel
 * @param channel DAC channel (1 or 2)
 * @return 0 on success, -1 on error
 */
int DAC_disable(uint8_t channel);

/**
 * @brief Write raw value to DAC channel
 * @param channel DAC channel (1 or 2)
 * @param value Output value (0-255)
 * @return 0 on success, -1 on error
 */
int DAC_write(uint8_t channel, uint8_t value);

/**
 * @brief Write voltage to DAC channel
 * @param channel DAC channel (1 or 2)
 * @param voltage Output voltage (0.0 - 3.3V)
 * @return 0 on success, -1 on error
 */
int DAC_writeVoltage(uint8_t channel, float voltage);

/**
 * @brief Write millivolt value to DAC channel
 * @param channel DAC channel (1 or 2)
 * @param millivolts Output voltage in millivolts (0-3300)
 * @return 0 on success, -1 on error
 */
int DAC_writeMillivolts(uint8_t channel, uint16_t millivolts);

/**
 * @brief Get GPIO number for DAC channel
 * @param channel DAC channel (1 or 2)
 * @return GPIO number, or -1 if invalid channel
 */
int DAC_getGPIO(uint8_t channel);

/**
 * @brief Check if DAC channel is enabled
 * @param channel DAC channel (1 or 2)
 * @return true if enabled
 */
bool DAC_isEnabled(uint8_t channel);

/**
 * @brief Output cosine wave on DAC channel
 * @param channel DAC channel (1 or 2)
 * @param frequency Frequency in Hz
 * @param scale Amplitude scale (0-3, 3=full scale)
 * @param offset DC offset (0-255)
 * @return 0 on success, -1 on error
 */
int DAC_cosineWave(uint8_t channel, uint32_t frequency, uint8_t scale, uint8_t offset);

/**
 * @brief Stop cosine wave output
 * @param channel DAC channel (1 or 2)
 * @return 0 on success, -1 on error
 */
int DAC_stopWave(uint8_t channel);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_DAC_H */
