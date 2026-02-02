/*****************************************************************************
 * @file    pwm.c
 * @brief   ESP32-C3 PWM (LEDC) Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details PWM implementation for ESP32-C3 (6 low-speed channels).
 *****************************************************************************/

#include "pwm.h"

/* ESP-IDF includes */
#include "driver/ledc.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "PWM";

/*============================================================================
 * Internal State
 *============================================================================*/

typedef struct {
    int8_t gpio;
    uint8_t resolution;
    uint32_t frequency;
    uint32_t duty;
    bool initialized;
} pwm_channel_t;

static pwm_channel_t channels[PWM_CHANNEL_COUNT] = {0};
static bool pwm_initialized = false;
static bool timer_configured[4] = {false};

/*============================================================================
 * Internal Functions
 *============================================================================*/

static ledc_timer_t get_timer_for_channel(uint8_t channel)
{
    return (ledc_timer_t)(channel % 4);
}

static int find_free_channel(void)
{
    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        if (channels[i].gpio < 0 && !channels[i].initialized) {
            return i;
        }
    }
    return -1;
}

static bool configure_timer(uint8_t channel, uint32_t frequency, uint8_t resolution)
{
    ledc_timer_t timer = get_timer_for_channel(channel);

    ledc_timer_config_t timer_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,  /* C3 only has low-speed */
        .timer_num = timer,
        .duty_resolution = resolution,
        .freq_hz = frequency,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    esp_err_t ret = ledc_timer_config(&timer_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure timer %d: %s", timer, esp_err_to_name(ret));
        return false;
    }

    timer_configured[timer] = true;
    return true;
}

/*============================================================================
 * Initialization
 *============================================================================*/

void PWM_init(void)
{
    if (pwm_initialized) {
        return;
    }

    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        channels[i].gpio = -1;
        channels[i].resolution = PWM_DEFAULT_RESOLUTION;
        channels[i].frequency = PWM_DEFAULT_FREQ;
        channels[i].duty = 0;
        channels[i].initialized = false;
    }

    esp_err_t ret = ledc_fade_func_install(0);
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        ESP_LOGW(TAG, "Failed to install fade service");
    }

    pwm_initialized = true;
    ESP_LOGI(TAG, "PWM initialized (ESP32-C3: 6 low-speed channels)");
}

void PWM_deinit(void)
{
    if (!pwm_initialized) {
        return;
    }

    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        if (channels[i].initialized) {
            PWM_detach(i);
        }
    }

    ledc_fade_func_uninstall();
    memset(timer_configured, 0, sizeof(timer_configured));
    pwm_initialized = false;
    ESP_LOGI(TAG, "PWM deinitialized");
}

/*============================================================================
 * Channel Configuration
 *============================================================================*/

int PWM_attach(uint8_t gpio, uint32_t frequency, uint8_t resolution)
{
    if (!pwm_initialized) {
        PWM_init();
    }

    if (resolution < PWM_MIN_RESOLUTION || resolution > PWM_MAX_RESOLUTION) {
        ESP_LOGE(TAG, "Invalid resolution %d (must be 1-14 on C3)", resolution);
        return -1;
    }

    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        if (channels[i].gpio == gpio) {
            ESP_LOGW(TAG, "GPIO %d already attached to channel %d", gpio, i);
            return i;
        }
    }

    int channel = find_free_channel();
    if (channel < 0) {
        ESP_LOGE(TAG, "No free PWM channels (max 6 on C3)");
        return -1;
    }

    if (PWM_attachChannel(channel, gpio, frequency, resolution)) {
        return channel;
    }

    return -1;
}

int PWM_attachDefault(uint8_t gpio)
{
    return PWM_attach(gpio, PWM_DEFAULT_FREQ, PWM_DEFAULT_RESOLUTION);
}

bool PWM_attachChannel(uint8_t channel, uint8_t gpio, uint32_t frequency, uint8_t resolution)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return false;
    }

    if (!pwm_initialized) {
        PWM_init();
    }

    if (!configure_timer(channel, frequency, resolution)) {
        return false;
    }

    ledc_channel_config_t channel_config = {
        .speed_mode = LEDC_LOW_SPEED_MODE,
        .channel = (ledc_channel_t)channel,
        .timer_sel = get_timer_for_channel(channel),
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = gpio,
        .duty = 0,
        .hpoint = 0,
    };

    esp_err_t ret = ledc_channel_config(&channel_config);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to configure channel %d: %s", channel, esp_err_to_name(ret));
        return false;
    }

    channels[channel].gpio = gpio;
    channels[channel].resolution = resolution;
    channels[channel].frequency = frequency;
    channels[channel].duty = 0;
    channels[channel].initialized = true;

    ESP_LOGI(TAG, "Attached GPIO %d to channel %d (freq=%lu Hz, res=%d bits)",
             gpio, channel, frequency, resolution);

    return true;
}

void PWM_detach(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    ledc_stop(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, 0);

    channels[channel].gpio = -1;
    channels[channel].initialized = false;
    channels[channel].duty = 0;

    ESP_LOGI(TAG, "Detached channel %d", channel);
}

void PWM_detachGpio(uint8_t gpio)
{
    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        if (channels[i].gpio == gpio) {
            PWM_detach(i);
            return;
        }
    }
}

/*============================================================================
 * Duty Cycle Control
 *============================================================================*/

void PWM_write(uint8_t channel, uint32_t duty)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    uint32_t max_duty = (1 << channels[channel].resolution) - 1;
    if (duty > max_duty) {
        duty = max_duty;
    }

    ledc_set_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel, duty);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);

    channels[channel].duty = duty;
}

void PWM_writeGpio(uint8_t gpio, uint32_t duty)
{
    int channel = PWM_getChannel(gpio);
    if (channel >= 0) {
        PWM_write(channel, duty);
    }
}

void PWM_writePercent(uint8_t channel, float percent)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    if (percent < 0.0f) percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;

    uint32_t max_duty = (1 << channels[channel].resolution) - 1;
    uint32_t duty = (uint32_t)((percent / 100.0f) * max_duty);

    PWM_write(channel, duty);
}

void PWM_writeGpioPercent(uint8_t gpio, float percent)
{
    int channel = PWM_getChannel(gpio);
    if (channel >= 0) {
        PWM_writePercent(channel, percent);
    }
}

void PWM_write8bit(uint8_t channel, uint8_t duty)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    uint32_t max_duty = (1 << channels[channel].resolution) - 1;
    uint32_t scaled_duty = (duty * max_duty) / 255;

    PWM_write(channel, scaled_duty);
}

uint32_t PWM_read(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return 0;
    }
    return channels[channel].duty;
}

/*============================================================================
 * Frequency Control
 *============================================================================*/

bool PWM_setFrequency(uint8_t channel, uint32_t frequency)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return false;
    }

    esp_err_t ret = ledc_set_freq(LEDC_LOW_SPEED_MODE,
                                   get_timer_for_channel(channel),
                                   frequency);
    if (ret == ESP_OK) {
        channels[channel].frequency = frequency;
        return true;
    }

    return false;
}

uint32_t PWM_getFrequency(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return 0;
    }
    return channels[channel].frequency;
}

uint32_t PWM_getMaxDuty(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return 0;
    }
    return (1 << channels[channel].resolution) - 1;
}

/*============================================================================
 * Fade Functions
 *============================================================================*/

void PWM_fade(uint8_t channel, uint32_t targetDuty, uint32_t fadeTime_ms)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    ledc_set_fade_time_and_start(LEDC_LOW_SPEED_MODE,
                                  (ledc_channel_t)channel,
                                  targetDuty,
                                  fadeTime_ms,
                                  LEDC_FADE_NO_WAIT);
}

void PWM_fadePercent(uint8_t channel, float targetPercent, uint32_t fadeTime_ms)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    if (targetPercent < 0.0f) targetPercent = 0.0f;
    if (targetPercent > 100.0f) targetPercent = 100.0f;

    uint32_t max_duty = (1 << channels[channel].resolution) - 1;
    uint32_t targetDuty = (uint32_t)((targetPercent / 100.0f) * max_duty);

    PWM_fade(channel, targetDuty, fadeTime_ms);
}

void PWM_waitFade(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return;
    }

    ledc_fade_wait_done(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);
}

bool PWM_isFading(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT || !channels[channel].initialized) {
        return false;
    }

    uint32_t current = ledc_get_duty(LEDC_LOW_SPEED_MODE, (ledc_channel_t)channel);
    return (current != channels[channel].duty);
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

int PWM_getChannel(uint8_t gpio)
{
    for (int i = 0; i < PWM_CHANNEL_COUNT; i++) {
        if (channels[i].gpio == gpio) {
            return i;
        }
    }
    return -1;
}

bool PWM_isAttached(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return false;
    }
    return channels[channel].initialized;
}

uint8_t PWM_getResolution(uint8_t channel)
{
    if (channel >= PWM_CHANNEL_COUNT) {
        return 0;
    }
    return channels[channel].resolution;
}
