/*****************************************************************************
 * @file    adc.h
 * @brief   ESP32-C3 ADC Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details ADC interface for ESP32-C3.
 *          ADC1: 5 channels (GPIO 0-4)
 *          ADC2: 1 channel (GPIO 5)
 *          12-bit resolution (0-4095), configurable attenuation.
 *****************************************************************************/

#ifndef ESP32C3_ADC_H
#define ESP32C3_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define ADC_RESOLUTION_BITS     12
#define ADC_MAX_VALUE           4095

/* ADC units */
#define ADC_UNIT_1              1
#define ADC_UNIT_2              2

/* ADC attenuation (voltage range) */
#define ADC_ATTEN_0dB           0   /* 0-800mV */
#define ADC_ATTEN_2_5dB         1   /* 0-1100mV */
#define ADC_ATTEN_6dB           2   /* 0-1350mV */
#define ADC_ATTEN_11dB          3   /* 0-2600mV (default) */

/*============================================================================
 * ESP32-C3 ADC1 Channel Mapping (GPIO)
 *============================================================================*/
#define ADC1_CH0_GPIO   0
#define ADC1_CH1_GPIO   1
#define ADC1_CH2_GPIO   2
#define ADC1_CH3_GPIO   3
#define ADC1_CH4_GPIO   4

/* ESP32-C3 has 5 ADC1 channels */
#define ESP32C3_ADC1_CHANNELS   5

/*============================================================================
 * ESP32-C3 ADC2 Channel Mapping - NOT usable with WiFi active!
 *============================================================================*/
#define ADC2_CH0_GPIO   5

/* ESP32-C3 has 1 ADC2 channel */
#define ESP32C3_ADC2_CHANNELS   1

/*============================================================================
 * Initialization
 *============================================================================*/

void ADC_init(void);
bool ADC_initUnit(uint8_t unit);
void ADC_deinit(void);

/*============================================================================
 * Channel Configuration
 *============================================================================*/

bool ADC_configChannel(uint8_t unit, uint8_t channel, uint8_t atten);
void ADC_setAttenuation(uint8_t unit, uint8_t channel, uint8_t atten);
bool ADC_configGpio(uint8_t gpio, uint8_t atten);

/*============================================================================
 * Single-Shot Reading
 *============================================================================*/

int ADC_read(uint8_t unit, uint8_t channel);
int ADC_readGpio(uint8_t gpio);
int ADC_readVoltage(uint8_t unit, uint8_t channel);
int ADC_readGpioVoltage(uint8_t gpio);

/*============================================================================
 * Averaging Functions
 *============================================================================*/

int ADC_readAverage(uint8_t unit, uint8_t channel, uint8_t samples);
int ADC_readVoltageAverage(uint8_t unit, uint8_t channel, uint8_t samples);

/*============================================================================
 * Calibration
 *============================================================================*/

bool ADC_isCalibrated(uint8_t unit);
int ADC_rawToVoltage(uint8_t unit, uint8_t channel, int raw_value);

/*============================================================================
 * Utility Functions
 *============================================================================*/

bool ADC_gpioToChannel(uint8_t gpio, uint8_t* unit, uint8_t* channel);
int ADC_channelToGpio(uint8_t unit, uint8_t channel);
bool ADC_isAdc2Available(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_ADC_H */
