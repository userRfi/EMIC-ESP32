/*****************************************************************************
 * @file    adc.c
 * @brief   ESP32-S3 ADC Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details ADC implementation for ESP32-S3 using ESP-IDF adc_oneshot driver.
 *          ADC1: 10 channels (GPIO 1-10)
 *          ADC2: 10 channels (GPIO 11-20)
 *****************************************************************************/

#include "adc.h"

/* ESP-IDF includes */
#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "ADC_S3";

/*============================================================================
 * Internal State
 *============================================================================*/

static adc_oneshot_unit_handle_t adc1_handle = NULL;
static adc_oneshot_unit_handle_t adc2_handle = NULL;
static adc_cali_handle_t adc1_cali_handle = NULL;
static adc_cali_handle_t adc2_cali_handle = NULL;
static bool adc1_initialized = false;
static bool adc2_initialized = false;

/* Channel configuration tracking for ESP32-S3 (10 channels each) */
static uint8_t adc1_atten[10] = {ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB,
                                  ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB,
                                  ADC_ATTEN_11dB, ADC_ATTEN_11dB};
static uint8_t adc2_atten[10] = {ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB,
                                  ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB, ADC_ATTEN_11dB,
                                  ADC_ATTEN_11dB, ADC_ATTEN_11dB};
static bool adc1_chan_configured[10] = {false};
static bool adc2_chan_configured[10] = {false};

/*============================================================================
 * ESP32-S3 GPIO to Channel Mapping
 * ADC1: GPIO 1-10 -> Channel 0-9
 * ADC2: GPIO 11-20 -> Channel 0-9
 *============================================================================*/

static const int8_t adc1_ch_to_gpio[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
static const int8_t adc2_ch_to_gpio[] = {11, 12, 13, 14, 15, 16, 17, 18, 19, 20};

/*============================================================================
 * Internal Functions
 *============================================================================*/

static adc_atten_t convert_atten(uint8_t atten)
{
    switch (atten) {
        case ADC_ATTEN_0dB:   return ADC_ATTEN_DB_0;
        case ADC_ATTEN_2_5dB: return ADC_ATTEN_DB_2_5;
        case ADC_ATTEN_6dB:   return ADC_ATTEN_DB_6;
        case ADC_ATTEN_11dB:
        default:              return ADC_ATTEN_DB_12;
    }
}

static bool init_calibration(uint8_t unit, uint8_t atten, adc_cali_handle_t* out_handle)
{
    adc_cali_handle_t handle = NULL;

#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
    adc_cali_curve_fitting_config_t cali_config = {
        .unit_id = (unit == 1) ? ADC_UNIT_1 : ADC_UNIT_2,
        .atten = convert_atten(atten),
        .bitwidth = ADC_BITWIDTH_12,
    };
    esp_err_t ret = adc_cali_create_scheme_curve_fitting(&cali_config, &handle);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
    adc_cali_line_fitting_config_t cali_config = {
        .unit_id = (unit == 1) ? ADC_UNIT_1 : ADC_UNIT_2,
        .atten = convert_atten(atten),
        .bitwidth = ADC_BITWIDTH_12,
    };
    esp_err_t ret = adc_cali_create_scheme_line_fitting(&cali_config, &handle);
#else
    esp_err_t ret = ESP_ERR_NOT_SUPPORTED;
#endif

    if (ret == ESP_OK) {
        *out_handle = handle;
        return true;
    }

    ESP_LOGW(TAG, "ADC%d calibration not available", unit);
    return false;
}

/*============================================================================
 * Initialization
 *============================================================================*/

void ADC_init(void)
{
    ADC_initUnit(ADC_UNIT_1);
}

bool ADC_initUnit(uint8_t unit)
{
    if (unit == ADC_UNIT_1) {
        if (adc1_initialized) return true;

        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = ADC_UNIT_1,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };

        esp_err_t ret = adc_oneshot_new_unit(&init_config, &adc1_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize ADC1: %s", esp_err_to_name(ret));
            return false;
        }

        init_calibration(1, ADC_ATTEN_11dB, &adc1_cali_handle);
        adc1_initialized = true;
        ESP_LOGI(TAG, "ADC1 initialized (10 channels: GPIO 1-10)");
        return true;
    }
    else if (unit == ADC_UNIT_2) {
        if (adc2_initialized) return true;

        adc_oneshot_unit_init_cfg_t init_config = {
            .unit_id = ADC_UNIT_2,
            .ulp_mode = ADC_ULP_MODE_DISABLE,
        };

        esp_err_t ret = adc_oneshot_new_unit(&init_config, &adc2_handle);
        if (ret != ESP_OK) {
            ESP_LOGE(TAG, "Failed to initialize ADC2: %s", esp_err_to_name(ret));
            return false;
        }

        init_calibration(2, ADC_ATTEN_11dB, &adc2_cali_handle);
        adc2_initialized = true;
        ESP_LOGI(TAG, "ADC2 initialized (10 channels: GPIO 11-20) - conflicts with WiFi");
        return true;
    }

    return false;
}

void ADC_deinit(void)
{
    if (adc1_initialized) {
        if (adc1_cali_handle) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
            adc_cali_delete_scheme_curve_fitting(adc1_cali_handle);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
            adc_cali_delete_scheme_line_fitting(adc1_cali_handle);
#endif
            adc1_cali_handle = NULL;
        }
        adc_oneshot_del_unit(adc1_handle);
        adc1_handle = NULL;
        adc1_initialized = false;
        memset(adc1_chan_configured, 0, sizeof(adc1_chan_configured));
    }

    if (adc2_initialized) {
        if (adc2_cali_handle) {
#if ADC_CALI_SCHEME_CURVE_FITTING_SUPPORTED
            adc_cali_delete_scheme_curve_fitting(adc2_cali_handle);
#elif ADC_CALI_SCHEME_LINE_FITTING_SUPPORTED
            adc_cali_delete_scheme_line_fitting(adc2_cali_handle);
#endif
            adc2_cali_handle = NULL;
        }
        adc_oneshot_del_unit(adc2_handle);
        adc2_handle = NULL;
        adc2_initialized = false;
        memset(adc2_chan_configured, 0, sizeof(adc2_chan_configured));
    }

    ESP_LOGI(TAG, "ADC deinitialized");
}

/*============================================================================
 * Channel Configuration
 *============================================================================*/

bool ADC_configChannel(uint8_t unit, uint8_t channel, uint8_t atten)
{
    if (unit == ADC_UNIT_1) {
        if (!adc1_initialized) ADC_initUnit(ADC_UNIT_1);
        if (channel >= 10) return false;

        adc_oneshot_chan_cfg_t config = {
            .atten = convert_atten(atten),
            .bitwidth = ADC_BITWIDTH_12,
        };

        esp_err_t ret = adc_oneshot_config_channel(adc1_handle, channel, &config);
        if (ret == ESP_OK) {
            adc1_atten[channel] = atten;
            adc1_chan_configured[channel] = true;
            return true;
        }
    }
    else if (unit == ADC_UNIT_2) {
        if (!adc2_initialized) ADC_initUnit(ADC_UNIT_2);
        if (channel >= 10) return false;

        adc_oneshot_chan_cfg_t config = {
            .atten = convert_atten(atten),
            .bitwidth = ADC_BITWIDTH_12,
        };

        esp_err_t ret = adc_oneshot_config_channel(adc2_handle, channel, &config);
        if (ret == ESP_OK) {
            adc2_atten[channel] = atten;
            adc2_chan_configured[channel] = true;
            return true;
        }
    }

    return false;
}

void ADC_setAttenuation(uint8_t unit, uint8_t channel, uint8_t atten)
{
    ADC_configChannel(unit, channel, atten);
}

bool ADC_configGpio(uint8_t gpio, uint8_t atten)
{
    uint8_t unit, channel;
    if (!ADC_gpioToChannel(gpio, &unit, &channel)) return false;
    return ADC_configChannel(unit, channel, atten);
}

/*============================================================================
 * Single-Shot Reading
 *============================================================================*/

int ADC_read(uint8_t unit, uint8_t channel)
{
    int raw = 0;

    if (unit == ADC_UNIT_1) {
        if (!adc1_initialized) ADC_initUnit(ADC_UNIT_1);
        if (channel >= 10) return -1;

        if (!adc1_chan_configured[channel]) {
            ADC_configChannel(ADC_UNIT_1, channel, adc1_atten[channel]);
        }

        esp_err_t ret = adc_oneshot_read(adc1_handle, channel, &raw);
        return (ret == ESP_OK) ? raw : -1;
    }
    else if (unit == ADC_UNIT_2) {
        if (!adc2_initialized) ADC_initUnit(ADC_UNIT_2);
        if (channel >= 10) return -1;

        if (!adc2_chan_configured[channel]) {
            ADC_configChannel(ADC_UNIT_2, channel, adc2_atten[channel]);
        }

        esp_err_t ret = adc_oneshot_read(adc2_handle, channel, &raw);
        return (ret == ESP_OK) ? raw : -1;
    }

    return -1;
}

int ADC_readGpio(uint8_t gpio)
{
    uint8_t unit, channel;
    if (!ADC_gpioToChannel(gpio, &unit, &channel)) return -1;
    return ADC_read(unit, channel);
}

int ADC_readVoltage(uint8_t unit, uint8_t channel)
{
    int raw = ADC_read(unit, channel);
    if (raw < 0) return -1;
    return ADC_rawToVoltage(unit, channel, raw);
}

int ADC_readGpioVoltage(uint8_t gpio)
{
    uint8_t unit, channel;
    if (!ADC_gpioToChannel(gpio, &unit, &channel)) return -1;
    return ADC_readVoltage(unit, channel);
}

/*============================================================================
 * Averaging Functions
 *============================================================================*/

int ADC_readAverage(uint8_t unit, uint8_t channel, uint8_t samples)
{
    if (samples == 0) samples = 1;

    int32_t sum = 0;
    for (uint8_t i = 0; i < samples; i++) {
        int val = ADC_read(unit, channel);
        if (val < 0) return -1;
        sum += val;
    }

    return (int)(sum / samples);
}

int ADC_readVoltageAverage(uint8_t unit, uint8_t channel, uint8_t samples)
{
    int raw = ADC_readAverage(unit, channel, samples);
    if (raw < 0) return -1;
    return ADC_rawToVoltage(unit, channel, raw);
}

/*============================================================================
 * Calibration
 *============================================================================*/

bool ADC_isCalibrated(uint8_t unit)
{
    if (unit == ADC_UNIT_1) return (adc1_cali_handle != NULL);
    if (unit == ADC_UNIT_2) return (adc2_cali_handle != NULL);
    return false;
}

int ADC_rawToVoltage(uint8_t unit, uint8_t channel, int raw_value)
{
    int voltage = 0;
    adc_cali_handle_t cali_handle = (unit == ADC_UNIT_1) ? adc1_cali_handle : adc2_cali_handle;

    if (cali_handle != NULL) {
        esp_err_t ret = adc_cali_raw_to_voltage(cali_handle, raw_value, &voltage);
        if (ret == ESP_OK) return voltage;
    }

    /* Fallback: approximate conversion for 11dB attenuation */
    return (raw_value * 2600) / ADC_MAX_VALUE;
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

bool ADC_gpioToChannel(uint8_t gpio, uint8_t* unit, uint8_t* channel)
{
    /* ESP32-S3 ADC1: GPIO 1-10 -> Channel 0-9 */
    if (gpio >= 1 && gpio <= 10) {
        *unit = ADC_UNIT_1;
        *channel = gpio - 1;
        return true;
    }

    /* ESP32-S3 ADC2: GPIO 11-20 -> Channel 0-9 */
    if (gpio >= 11 && gpio <= 20) {
        *unit = ADC_UNIT_2;
        *channel = gpio - 11;
        return true;
    }

    return false;
}

int ADC_channelToGpio(uint8_t unit, uint8_t channel)
{
    if (unit == ADC_UNIT_1 && channel < 10) {
        return adc1_ch_to_gpio[channel];
    } else if (unit == ADC_UNIT_2 && channel < 10) {
        return adc2_ch_to_gpio[channel];
    }
    return -1;
}

bool ADC_isAdc2Available(void)
{
    return true;
}
