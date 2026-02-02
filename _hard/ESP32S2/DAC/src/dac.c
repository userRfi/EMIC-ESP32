/*****************************************************************************
 * @file    dac.c
 * @brief   ESP32-S2 DAC Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Digital-to-Analog Converter implementation for ESP32-S2.
 *          2 DAC channels: DAC1 on GPIO17, DAC2 on GPIO18
 *****************************************************************************/

#include "dac.h"
#include "driver/dac.h"
#include "driver/dac_cosine.h"
#include "esp_log.h"

static const char* TAG = "DAC_S2";

/* DAC state */
static bool dac_initialized = false;
static bool dac_enabled[3] = {false, false, false};  /* Index 1 and 2 used */

/*============================================================================
 * Helper Functions
 *============================================================================*/

static dac_channel_t channel_to_dac(uint8_t channel)
{
    switch (channel) {
        case 1: return DAC_CHAN_0;
        case 2: return DAC_CHAN_1;
        default: return DAC_CHAN_0;
    }
}

/*============================================================================
 * DAC Implementation
 *============================================================================*/

void DAC_init(void)
{
    if (dac_initialized) {
        ESP_LOGW(TAG, "DAC already initialized");
        return;
    }

    dac_enabled[1] = false;
    dac_enabled[2] = false;
    dac_initialized = true;

    ESP_LOGI(TAG, "DAC initialized (2 channels: GPIO17, GPIO18)");
}

void DAC_deinit(void)
{
    if (!dac_initialized) {
        return;
    }

    /* Disable both channels */
    DAC_disable(1);
    DAC_disable(2);

    dac_initialized = false;
    ESP_LOGI(TAG, "DAC deinitialized");
}

int DAC_enable(uint8_t channel)
{
    if (channel < 1 || channel > 2) {
        ESP_LOGE(TAG, "Invalid DAC channel: %d (valid: 1-2)", channel);
        return -1;
    }

    if (!dac_initialized) {
        DAC_init();
    }

    esp_err_t ret = dac_output_enable(channel_to_dac(channel));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to enable DAC channel %d: %d", channel, ret);
        return -1;
    }

    dac_enabled[channel] = true;
    ESP_LOGI(TAG, "DAC channel %d enabled (GPIO%d)", channel,
             channel == 1 ? DAC_CH1_GPIO : DAC_CH2_GPIO);
    return 0;
}

int DAC_disable(uint8_t channel)
{
    if (channel < 1 || channel > 2) {
        ESP_LOGE(TAG, "Invalid DAC channel: %d", channel);
        return -1;
    }

    if (!dac_enabled[channel]) {
        return 0;
    }

    esp_err_t ret = dac_output_disable(channel_to_dac(channel));
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to disable DAC channel %d: %d", channel, ret);
        return -1;
    }

    dac_enabled[channel] = false;
    ESP_LOGI(TAG, "DAC channel %d disabled", channel);
    return 0;
}

int DAC_write(uint8_t channel, uint8_t value)
{
    if (channel < 1 || channel > 2) {
        ESP_LOGE(TAG, "Invalid DAC channel: %d", channel);
        return -1;
    }

    if (!dac_initialized) {
        DAC_init();
    }

    /* Auto-enable if not enabled */
    if (!dac_enabled[channel]) {
        if (DAC_enable(channel) != 0) {
            return -1;
        }
    }

    esp_err_t ret = dac_output_voltage(channel_to_dac(channel), value);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to write DAC channel %d: %d", channel, ret);
        return -1;
    }

    return 0;
}

int DAC_writeVoltage(uint8_t channel, float voltage)
{
    /* Clamp voltage to valid range */
    if (voltage < 0.0f) voltage = 0.0f;
    if (voltage > 3.3f) voltage = 3.3f;

    /* Convert voltage to 8-bit value */
    uint8_t value = (uint8_t)((voltage / 3.3f) * 255.0f);
    return DAC_write(channel, value);
}

int DAC_writeMillivolts(uint8_t channel, uint16_t millivolts)
{
    /* Clamp to valid range */
    if (millivolts > DAC_VREF_MV) {
        millivolts = DAC_VREF_MV;
    }

    /* Convert millivolts to 8-bit value */
    uint8_t value = (uint8_t)(((uint32_t)millivolts * 255) / DAC_VREF_MV);
    return DAC_write(channel, value);
}

int DAC_getGPIO(uint8_t channel)
{
    switch (channel) {
        case 1: return DAC_CH1_GPIO;
        case 2: return DAC_CH2_GPIO;
        default: return -1;
    }
}

bool DAC_isEnabled(uint8_t channel)
{
    if (channel < 1 || channel > 2) {
        return false;
    }
    return dac_enabled[channel];
}

int DAC_cosineWave(uint8_t channel, uint32_t frequency, uint8_t scale, uint8_t offset)
{
    if (channel < 1 || channel > 2) {
        ESP_LOGE(TAG, "Invalid DAC channel: %d", channel);
        return -1;
    }

    if (!dac_initialized) {
        DAC_init();
    }

    /* Clamp scale to valid range (0-3) */
    if (scale > 3) scale = 3;

    dac_cosine_config_t cos_cfg = {
        .chan_id = channel_to_dac(channel),
        .freq_hz = frequency,
        .clk_src = DAC_COSINE_CLK_SRC_DEFAULT,
        .atten = (dac_cosine_atten_t)scale,
        .phase = DAC_COSINE_PHASE_0,
        .offset = offset,
        .flags = {
            .force_set_freq = false,
        }
    };

    dac_cosine_handle_t handle;
    esp_err_t ret = dac_cosine_new_channel(&cos_cfg, &handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure cosine wave: %d", ret);
        return -1;
    }

    ret = dac_cosine_start(handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to start cosine wave: %d", ret);
        return -1;
    }

    dac_enabled[channel] = true;
    ESP_LOGI(TAG, "DAC channel %d cosine wave started at %lu Hz", channel, frequency);
    return 0;
}

int DAC_stopWave(uint8_t channel)
{
    if (channel < 1 || channel > 2) {
        ESP_LOGE(TAG, "Invalid DAC channel: %d", channel);
        return -1;
    }

    /* Disable the channel output */
    return DAC_disable(channel);
}
