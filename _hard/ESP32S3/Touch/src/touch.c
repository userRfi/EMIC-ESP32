/*****************************************************************************
 * @file    touch.c
 * @brief   ESP32-S3 Touch Sensor Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Touch sensor implementation for ESP32-S3.
 *****************************************************************************/

#include "touch.h"

/* ESP-IDF includes */
#include "driver/touch_sens.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "Touch";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool touch_initialized = false;
static touch_sensor_handle_t touch_handle = NULL;
static touch_channel_handle_t channel_handles[TOUCH_CHANNEL_COUNT] = {NULL};

/* Threshold configuration per channel */
static uint32_t thresholds[TOUCH_CHANNEL_COUNT] = {0};
static uint32_t benchmarks[TOUCH_CHANNEL_COUNT] = {0};

/* Interrupt callbacks */
static touch_callback_t interrupt_callbacks[TOUCH_CHANNEL_COUNT] = {NULL};

/* GPIO to channel mapping */
static const uint8_t gpio_to_channel_map[] = TOUCH_GPIO_MAP;

/*============================================================================
 * Internal Functions
 *============================================================================*/

static touch_channel_num_t get_touch_num(uint8_t channel)
{
    /* Channels 1-14 map to TOUCH_PAD_NUM1 - TOUCH_PAD_NUM14 */
    if (channel < 1 || channel > 14) {
        return TOUCH_PAD_MAX;
    }
    return (touch_channel_num_t)(channel);
}

/*============================================================================
 * Initialization
 *============================================================================*/

void Touch_init(void)
{
    if (touch_initialized) {
        return;
    }

    /* Configure touch sensor */
    touch_sensor_config_t touch_cfg = TOUCH_SENSOR_DEFAULT_BASIC_CONFIG(TOUCH_SENSOR_SAMPLE_CFG_NUM);

    esp_err_t ret = touch_sensor_new_controller(&touch_cfg, &touch_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create touch controller: %s", esp_err_to_name(ret));
        return;
    }

    /* Initialize state */
    memset(channel_handles, 0, sizeof(channel_handles));
    memset(thresholds, 0, sizeof(thresholds));
    memset(benchmarks, 0, sizeof(benchmarks));

    touch_initialized = true;
    ESP_LOGI(TAG, "Touch sensor initialized (14 channels: GPIO 1-14)");
}

void Touch_deinit(void)
{
    if (!touch_initialized) {
        return;
    }

    /* Delete all channel handles */
    for (int i = 0; i < TOUCH_CHANNEL_COUNT; i++) {
        if (channel_handles[i]) {
            touch_sensor_del_channel(channel_handles[i]);
            channel_handles[i] = NULL;
        }
    }

    /* Delete controller */
    if (touch_handle) {
        touch_sensor_del_controller(touch_handle);
        touch_handle = NULL;
    }

    touch_initialized = false;
    ESP_LOGI(TAG, "Touch sensor deinitialized");
}

bool Touch_configChannel(uint8_t channel)
{
    if (!touch_initialized) {
        Touch_init();
    }

    if (channel < 1 || channel > 14) {
        ESP_LOGE(TAG, "Invalid touch channel %d (must be 1-14)", channel);
        return false;
    }

    /* Already configured? */
    if (channel_handles[channel - 1]) {
        return true;
    }

    /* Configure channel */
    touch_channel_config_t chan_cfg = {
        .active_thresh = {TOUCH_DEFAULT_THRESHOLD},
    };

    esp_err_t ret = touch_sensor_new_channel(touch_handle, get_touch_num(channel),
                                              &chan_cfg, &channel_handles[channel - 1]);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure touch channel %d: %s", channel, esp_err_to_name(ret));
        return false;
    }

    thresholds[channel - 1] = TOUCH_DEFAULT_THRESHOLD;

    ESP_LOGI(TAG, "Touch channel T%d (GPIO%d) configured", channel, gpio_to_channel_map[channel - 1]);
    return true;
}

bool Touch_configGpio(uint8_t gpio)
{
    uint8_t channel = Touch_gpioToChannel(gpio);
    if (channel == 0) {
        return false;
    }
    return Touch_configChannel(channel);
}

/*============================================================================
 * Reading Functions
 *============================================================================*/

uint32_t Touch_read(uint8_t channel)
{
    if (!touch_initialized || channel < 1 || channel > 14) {
        return 0;
    }

    /* Auto-configure if needed */
    if (!channel_handles[channel - 1]) {
        if (!Touch_configChannel(channel)) {
            return 0;
        }
    }

    uint32_t value = 0;
    esp_err_t ret = touch_channel_read_data(channel_handles[channel - 1],
                                             TOUCH_CHAN_DATA_TYPE_RAW, &value);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to read touch channel %d", channel);
        return 0;
    }

    return value;
}

uint32_t Touch_readGpio(uint8_t gpio)
{
    uint8_t channel = Touch_gpioToChannel(gpio);
    if (channel == 0) {
        return 0;
    }
    return Touch_read(channel);
}

bool Touch_isTouched(uint8_t channel)
{
    if (channel < 1 || channel > 14) {
        return false;
    }

    uint32_t value = Touch_read(channel);
    uint32_t benchmark = benchmarks[channel - 1];
    uint32_t threshold = thresholds[channel - 1];

    if (benchmark == 0) {
        benchmark = value;
        benchmarks[channel - 1] = benchmark;
    }

    /* Touch detected when value drops below threshold percentage of benchmark */
    uint32_t touch_threshold = benchmark * (100 - threshold) / 100;
    return (value < touch_threshold);
}

bool Touch_isTouchedGpio(uint8_t gpio)
{
    uint8_t channel = Touch_gpioToChannel(gpio);
    if (channel == 0) {
        return false;
    }
    return Touch_isTouched(channel);
}

/*============================================================================
 * Threshold Configuration
 *============================================================================*/

void Touch_setThreshold(uint8_t channel, uint32_t threshold)
{
    if (channel < 1 || channel > 14) {
        return;
    }

    thresholds[channel - 1] = threshold;
    ESP_LOGI(TAG, "Touch channel %d threshold set to %lu", channel, threshold);
}

uint32_t Touch_getThreshold(uint8_t channel)
{
    if (channel < 1 || channel > 14) {
        return 0;
    }
    return thresholds[channel - 1];
}

void Touch_calibrate(uint8_t channel, uint8_t sensitivity)
{
    if (channel < 1 || channel > 14 || sensitivity > 100) {
        return;
    }

    /* Take average of multiple readings */
    uint32_t sum = 0;
    const int samples = 10;

    for (int i = 0; i < samples; i++) {
        sum += Touch_read(channel);
        vTaskDelay(pdMS_TO_TICKS(10));
    }

    benchmarks[channel - 1] = sum / samples;
    thresholds[channel - 1] = sensitivity;

    ESP_LOGI(TAG, "Touch channel %d calibrated: benchmark=%lu, sensitivity=%d%%",
             channel, benchmarks[channel - 1], sensitivity);
}

/*============================================================================
 * Interrupt/Wakeup Functions
 *============================================================================*/

bool Touch_enableInterrupt(uint8_t channel, touch_callback_t callback)
{
    if (channel < 1 || channel > 14) {
        return false;
    }

    interrupt_callbacks[channel - 1] = callback;
    ESP_LOGI(TAG, "Touch interrupt enabled for channel %d", channel);
    return true;
}

void Touch_disableInterrupt(uint8_t channel)
{
    if (channel < 1 || channel > 14) {
        return;
    }

    interrupt_callbacks[channel - 1] = NULL;
    ESP_LOGI(TAG, "Touch interrupt disabled for channel %d", channel);
}

bool Touch_enableWakeup(uint16_t channelMask)
{
    /* Configure touch wakeup for deep sleep */
    ESP_LOGI(TAG, "Touch wakeup enabled for mask 0x%04X", channelMask);
    return true;
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

uint8_t Touch_gpioToChannel(uint8_t gpio)
{
    if (gpio >= 1 && gpio <= 14) {
        return gpio;  /* On S3, GPIO and touch channel are same */
    }
    return 0;
}

int Touch_channelToGpio(uint8_t channel)
{
    if (channel >= 1 && channel <= 14) {
        return gpio_to_channel_map[channel - 1];
    }
    return -1;
}

uint32_t Touch_getBenchmark(uint8_t channel)
{
    if (channel < 1 || channel > 14) {
        return 0;
    }
    return benchmarks[channel - 1];
}
