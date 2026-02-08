/*****************************************************************************
 * @file    adc.h
 * @brief   ESP32-S3 ADC Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details ADC interface for ESP32-S3.
 *          ADC1: 10 channels (GPIO 1-10)
 *          ADC2: 10 channels (GPIO 11-20) - NOT usable with WiFi
 *          12-bit resolution (0-4095), configurable attenuation.
 *****************************************************************************/

#ifndef ESP32S3_ADC_H
#define ESP32S3_ADC_H

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
 * ESP32-S3 ADC1 Channel Mapping (GPIO 1-10)
 *============================================================================*/
#define ADC1_CH0_GPIO   1
#define ADC1_CH1_GPIO   2
#define ADC1_CH2_GPIO   3
#define ADC1_CH3_GPIO   4
#define ADC1_CH4_GPIO   5
#define ADC1_CH5_GPIO   6
#define ADC1_CH6_GPIO   7
#define ADC1_CH7_GPIO   8
#define ADC1_CH8_GPIO   9
#define ADC1_CH9_GPIO   10

/* ESP32-S3 has 10 ADC1 channels */
#define ESP32S3_ADC1_CHANNELS   10

/*============================================================================
 * ESP32-S3 ADC2 Channel Mapping (GPIO 11-20) - NOT usable with WiFi!
 *============================================================================*/
#define ADC2_CH0_GPIO   11
#define ADC2_CH1_GPIO   12
#define ADC2_CH2_GPIO   13
#define ADC2_CH3_GPIO   14
#define ADC2_CH4_GPIO   15
#define ADC2_CH5_GPIO   16
#define ADC2_CH6_GPIO   17
#define ADC2_CH7_GPIO   18
#define ADC2_CH8_GPIO   19
#define ADC2_CH9_GPIO   20

/* ESP32-S3 has 10 ADC2 channels */
#define ESP32S3_ADC2_CHANNELS   10

/*============================================================================
 * Functions
 *============================================================================*/

void ADC_init(void);
bool ADC_initUnit(uint8_t unit);
void ADC_deinit(void);

bool ADC_configChannel(uint8_t unit, uint8_t channel, uint8_t atten);
void ADC_setAttenuation(uint8_t unit, uint8_t channel, uint8_t atten);
bool ADC_configGpio(uint8_t gpio, uint8_t atten);

int ADC_read(uint8_t unit, uint8_t channel);
int ADC_readGpio(uint8_t gpio);
int ADC_readVoltage(uint8_t unit, uint8_t channel);
int ADC_readGpioVoltage(uint8_t gpio);

int ADC_readAverage(uint8_t unit, uint8_t channel, uint8_t samples);
int ADC_readVoltageAverage(uint8_t unit, uint8_t channel, uint8_t samples);

bool ADC_isCalibrated(uint8_t unit);
int ADC_rawToVoltage(uint8_t unit, uint8_t channel, int raw_value);

bool ADC_gpioToChannel(uint8_t gpio, uint8_t* unit, uint8_t* channel);
int ADC_channelToGpio(uint8_t unit, uint8_t channel);
bool ADC_isAdc2Available(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_ADC_H */
