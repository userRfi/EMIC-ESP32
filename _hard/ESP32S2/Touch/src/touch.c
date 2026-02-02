/*****************************************************************************
 * @file    touch.c
 * @brief   ESP32-S2 Touch Sensor Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Touch sensor implementation for ESP32-S2.
 *          14 touch channels (T1-T14) mapped to GPIO 1-14.
 *****************************************************************************/

#include "touch.h"
#include "driver/touch_pad.h"
#include "esp_log.h"

static const char* TAG = "TOUCH_S2";

/* Touch channel thresholds */
static uint32_t touch_thresholds[TOUCH_CHANNEL_COUNT + 1] = {0};

/* Initialized channels mask */
static uint16_t initialized_channels = 0;

/* System initialized flag */
static bool touch_initialized = false;

/*============================================================================
 * Touch Sensor Implementation
 *============================================================================*/

void Touch_init(void)
{
    if (touch_initialized) {
        ESP_LOGW(TAG, "Touch already initialized");
        return;
    }

    /* Initialize touch pad peripheral */
    esp_err_t ret = touch_pad_init();
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Touch pad init failed: %d", ret);
        return;
    }

    /* Set reference voltage for charging/discharging */
    touch_pad_set_voltage(TOUCH_HVOLT_2V7, TOUCH_LVOLT_0V5, TOUCH_HVOLT_ATTEN_1V);

    /* Initialize thresholds to default */
    for (int i = 0; i <= TOUCH_CHANNEL_COUNT; i++) {
        touch_thresholds[i] = TOUCH_DEFAULT_THRESHOLD;
    }

    initialized_channels = 0;
    touch_initialized = true;

    ESP_LOGI(TAG, "Touch sensor initialized (14 channels available)");
}

void Touch_deinit(void)
{
    if (!touch_initialized) {
        return;
    }

    touch_pad_deinit();
    initialized_channels = 0;
    touch_initialized = false;

    ESP_LOGI(TAG, "Touch sensor deinitialized");
}

int Touch_configChannel(uint8_t channel)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        ESP_LOGE(TAG, "Invalid touch channel: %d (valid: 1-14)", channel);
        return -1;
    }

    if (!touch_initialized) {
        Touch_init();
    }

    /* Configure the touch pad channel */
    esp_err_t ret = touch_pad_config((touch_pad_t)channel);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure touch channel %d: %d", channel, ret);
        return -1;
    }

    initialized_channels |= (1 << channel);

    ESP_LOGI(TAG, "Touch channel %d configured (GPIO%d)", channel, channel);
    return 0;
}

uint32_t Touch_read(uint8_t channel)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        ESP_LOGE(TAG, "Invalid touch channel: %d", channel);
        return 0;
    }

    if (!(initialized_channels & (1 << channel))) {
        ESP_LOGW(TAG, "Touch channel %d not configured, configuring now", channel);
        if (Touch_configChannel(channel) != 0) {
            return 0;
        }
    }

    uint32_t touch_value = 0;
    esp_err_t ret = touch_pad_read_raw_data((touch_pad_t)channel, &touch_value);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read touch channel %d: %d", channel, ret);
        return 0;
    }

    return touch_value;
}

bool Touch_isTouched(uint8_t channel)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        return false;
    }

    uint32_t value = Touch_read(channel);
    uint32_t threshold = touch_thresholds[channel];

    /* Lower value indicates touch (capacitance increases) */
    return (value > 0) && (value < threshold);
}

void Touch_setThreshold(uint8_t channel, uint32_t threshold)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        ESP_LOGE(TAG, "Invalid touch channel: %d", channel);
        return;
    }

    touch_thresholds[channel] = threshold;
    ESP_LOGD(TAG, "Touch channel %d threshold set to %lu", channel, threshold);
}

uint32_t Touch_getThreshold(uint8_t channel)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        return 0;
    }
    return touch_thresholds[channel];
}

int Touch_enableWakeup(uint8_t channel, uint32_t threshold)
{
    if (channel < TOUCH_CHANNEL_MIN || channel > TOUCH_CHANNEL_MAX) {
        ESP_LOGE(TAG, "Invalid touch channel for wakeup: %d", channel);
        return -1;
    }

    /* Configure touch pad for wakeup */
    if (!(initialized_channels & (1 << channel))) {
        if (Touch_configChannel(channel) != 0) {
            return -1;
        }
    }

    Touch_setThreshold(channel, threshold);

    /* Enable touch sensor wakeup */
    esp_err_t ret = touch_pad_set_thresh((touch_pad_t)channel, threshold);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to set wakeup threshold: %d", ret);
        return -1;
    }

    ESP_LOGI(TAG, "Touch wakeup enabled on channel %d", channel);
    return 0;
}

int Touch_channelToGPIO(uint8_t channel)
{
    /* On ESP32-S2, touch channels 1-14 map directly to GPIO 1-14 */
    if (channel >= TOUCH_CHANNEL_MIN && channel <= TOUCH_CHANNEL_MAX) {
        return (int)channel;
    }
    return -1;
}

int Touch_GPIOToChannel(uint8_t gpio)
{
    /* On ESP32-S2, GPIO 1-14 map directly to touch channels 1-14 */
    if (gpio >= 1 && gpio <= 14) {
        return (int)gpio;
    }
    return -1;
}
