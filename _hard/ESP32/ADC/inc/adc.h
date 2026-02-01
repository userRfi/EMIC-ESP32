/*****************************************************************************
 * @file    adc.h
 * @brief   ESP32 ADC Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details ADC interface for ESP32 with oneshot and continuous modes.
 *          12-bit resolution (0-4095), configurable attenuation.
 *****************************************************************************/

#ifndef ESP32_ADC_H
#define ESP32_ADC_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/* ADC resolution */
#define ADC_RESOLUTION_BITS     12
#define ADC_MAX_VALUE           4095

/* ADC units */
#define ADC_UNIT_1              1
#define ADC_UNIT_2              2

/* ADC attenuation (voltage range) */
#define ADC_ATTEN_0dB           0   /* 0-800mV (100-950mV typical) */
#define ADC_ATTEN_2_5dB         1   /* 0-1100mV */
#define ADC_ATTEN_6dB           2   /* 0-1350mV */
#define ADC_ATTEN_11dB          3   /* 0-2600mV (default, full range) */

/*============================================================================
 * ADC1 Channel Mapping (GPIO)
 *============================================================================*/
#define ADC1_CH0_GPIO   36  /* SENSOR_VP */
#define ADC1_CH1_GPIO   37
#define ADC1_CH2_GPIO   38
#define ADC1_CH3_GPIO   39  /* SENSOR_VN */
#define ADC1_CH4_GPIO   32
#define ADC1_CH5_GPIO   33
#define ADC1_CH6_GPIO   34
#define ADC1_CH7_GPIO   35

/*============================================================================
 * ADC2 Channel Mapping (GPIO) - NOT usable with WiFi active!
 *============================================================================*/
#define ADC2_CH0_GPIO   4
#define ADC2_CH1_GPIO   0
#define ADC2_CH2_GPIO   2
#define ADC2_CH3_GPIO   15
#define ADC2_CH4_GPIO   13
#define ADC2_CH5_GPIO   12
#define ADC2_CH6_GPIO   14
#define ADC2_CH7_GPIO   27
#define ADC2_CH8_GPIO   25
#define ADC2_CH9_GPIO   26

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize ADC subsystem
 *
 * Initializes both ADC1 and ADC2 units with default configuration.
 */
void ADC_init(void);

/**
 * @brief Initialize specific ADC unit
 *
 * @param unit ADC unit (ADC_UNIT_1 or ADC_UNIT_2)
 * @return true if successful
 */
bool ADC_initUnit(uint8_t unit);

/**
 * @brief Deinitialize ADC
 */
void ADC_deinit(void);

/*============================================================================
 * Channel Configuration
 *============================================================================*/

/**
 * @brief Configure an ADC channel
 *
 * @param unit ADC unit (1 or 2)
 * @param channel Channel number (0-7 for ADC1, 0-9 for ADC2)
 * @param atten Attenuation (ADC_ATTEN_0dB to ADC_ATTEN_11dB)
 * @return true if successful
 */
bool ADC_configChannel(uint8_t unit, uint8_t channel, uint8_t atten);

/**
 * @brief Set channel attenuation
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @param atten Attenuation value
 */
void ADC_setAttenuation(uint8_t unit, uint8_t channel, uint8_t atten);

/**
 * @brief Configure ADC channel by GPIO number
 *
 * @param gpio GPIO pin number
 * @param atten Attenuation
 * @return true if successful
 */
bool ADC_configGpio(uint8_t gpio, uint8_t atten);

/*============================================================================
 * Single-Shot Reading
 *============================================================================*/

/**
 * @brief Read raw ADC value
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @return Raw value (0-4095) or -1 on error
 */
int ADC_read(uint8_t unit, uint8_t channel);

/**
 * @brief Read ADC value by GPIO number
 *
 * @param gpio GPIO pin number
 * @return Raw value (0-4095) or -1 on error
 */
int ADC_readGpio(uint8_t gpio);

/**
 * @brief Read and convert to millivolts
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @return Voltage in mV or -1 on error
 */
int ADC_readVoltage(uint8_t unit, uint8_t channel);

/**
 * @brief Read GPIO and convert to millivolts
 *
 * @param gpio GPIO pin number
 * @return Voltage in mV or -1 on error
 */
int ADC_readGpioVoltage(uint8_t gpio);

/*============================================================================
 * Averaging Functions
 *============================================================================*/

/**
 * @brief Read ADC with averaging
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @param samples Number of samples to average
 * @return Averaged raw value or -1 on error
 */
int ADC_readAverage(uint8_t unit, uint8_t channel, uint8_t samples);

/**
 * @brief Read ADC voltage with averaging
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @param samples Number of samples to average
 * @return Averaged voltage in mV or -1 on error
 */
int ADC_readVoltageAverage(uint8_t unit, uint8_t channel, uint8_t samples);

/*============================================================================
 * Calibration
 *============================================================================*/

/**
 * @brief Check if ADC calibration is available
 *
 * @param unit ADC unit
 * @return true if calibration data is available
 */
bool ADC_isCalibrated(uint8_t unit);

/**
 * @brief Convert raw ADC value to calibrated millivolts
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @param raw_value Raw ADC reading
 * @return Calibrated voltage in mV
 */
int ADC_rawToVoltage(uint8_t unit, uint8_t channel, int raw_value);

/*============================================================================
 * Utility Functions
 *============================================================================*/

/**
 * @brief Get ADC unit and channel for a GPIO
 *
 * @param gpio GPIO number
 * @param unit Pointer to store ADC unit
 * @param channel Pointer to store channel
 * @return true if GPIO has ADC capability
 */
bool ADC_gpioToChannel(uint8_t gpio, uint8_t* unit, uint8_t* channel);

/**
 * @brief Get GPIO for an ADC channel
 *
 * @param unit ADC unit
 * @param channel Channel number
 * @return GPIO number or -1 if invalid
 */
int ADC_channelToGpio(uint8_t unit, uint8_t channel);

/**
 * @brief Check if ADC2 is available (not in use by WiFi)
 *
 * @return true if ADC2 can be used
 */
bool ADC_isAdc2Available(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_ADC_H */
