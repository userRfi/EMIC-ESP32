/*****************************************************************************
 * @file    timer.c
 * @brief   ESP32-S3 Timer Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details Timer implementation for ESP32-S3 using esp_timer and hardware timers.
 *          ESP32-S3 has 4 hardware timers (2 groups x 2 timers).
 *****************************************************************************/

#include "timer.h"

/* ESP-IDF includes */
#include "esp_timer.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "Timer_S3";

/*============================================================================
 * Software Timer State
 *============================================================================*/

typedef struct {
    esp_timer_handle_t handle;
    timer_callback_t callback;
    void* arg;
    bool active;
    bool periodic;
    uint64_t period_us;
} timer_slot_t;

static timer_slot_t timers[TIMER_MAX_TIMERS] = {0};
static bool timer_initialized = false;

/*============================================================================
 * Hardware Timer State - ESP32-S3 has 2 groups x 2 timers
 *============================================================================*/

typedef struct {
    gptimer_handle_t handle;
    hw_timer_callback_t callback;
    void* arg;
    bool initialized;
} hw_timer_slot_t;

static hw_timer_slot_t hw_timers[2][2] = {0}; /* [group][timer] */

/*============================================================================
 * Internal Functions
 *============================================================================*/

static void timer_callback_wrapper(void* arg)
{
    timer_slot_t* slot = (timer_slot_t*)arg;
    if (slot->callback != NULL) {
        slot->callback(slot->arg);
    }
}

static int find_free_slot(void)
{
    for (int i = 0; i < TIMER_MAX_TIMERS; i++) {
        if (timers[i].handle == NULL) {
            return i;
        }
    }
    return -1;
}

/*============================================================================
 * Software Timer Implementation
 *============================================================================*/

void Timer_init(void)
{
    if (timer_initialized) {
        return;
    }

    memset(timers, 0, sizeof(timers));
    timer_initialized = true;
    ESP_LOGI(TAG, "Timer subsystem initialized");
}

timer_handle_t Timer_create(const char* name, uint64_t period_us,
                            timer_callback_t callback, void* arg, bool periodic)
{
    if (!timer_initialized) {
        Timer_init();
    }

    int slot = find_free_slot();
    if (slot < 0) {
        ESP_LOGE(TAG, "No free timer slots");
        return TIMER_INVALID_HANDLE;
    }

    esp_timer_create_args_t timer_args = {
        .callback = timer_callback_wrapper,
        .arg = &timers[slot],
        .dispatch_method = ESP_TIMER_TASK,
        .name = name ? name : "emic_timer",
    };

    esp_err_t ret = esp_timer_create(&timer_args, &timers[slot].handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create timer: %s", esp_err_to_name(ret));
        return TIMER_INVALID_HANDLE;
    }

    timers[slot].callback = callback;
    timers[slot].arg = arg;
    timers[slot].periodic = periodic;
    timers[slot].period_us = period_us;
    timers[slot].active = false;

    ESP_LOGI(TAG, "Created timer %d (%s), period=%llu us, periodic=%d",
             slot, name ? name : "unnamed", period_us, periodic);

    return slot;
}

timer_handle_t Timer_createOnce(const char* name, uint64_t period_us,
                                 timer_callback_t callback, void* arg)
{
    return Timer_create(name, period_us, callback, arg, false);
}

timer_handle_t Timer_createPeriodic(const char* name, uint64_t period_us,
                                     timer_callback_t callback, void* arg)
{
    return Timer_create(name, period_us, callback, arg, true);
}

bool Timer_start(timer_handle_t handle)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return false;
    }

    timer_slot_t* slot = &timers[handle];
    esp_err_t ret;

    if (slot->periodic) {
        ret = esp_timer_start_periodic(slot->handle, slot->period_us);
    } else {
        ret = esp_timer_start_once(slot->handle, slot->period_us);
    }

    if (ret == ESP_OK) {
        slot->active = true;
        return true;
    }

    ESP_LOGE(TAG, "Failed to start timer %d: %s", handle, esp_err_to_name(ret));
    return false;
}

bool Timer_stop(timer_handle_t handle)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return false;
    }

    esp_err_t ret = esp_timer_stop(timers[handle].handle);
    if (ret == ESP_OK || ret == ESP_ERR_INVALID_STATE) {
        timers[handle].active = false;
        return true;
    }

    return false;
}

bool Timer_restart(timer_handle_t handle, uint64_t period_us)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return false;
    }

    Timer_stop(handle);
    timers[handle].period_us = period_us;
    return Timer_start(handle);
}

void Timer_delete(timer_handle_t handle)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return;
    }

    Timer_stop(handle);
    esp_timer_delete(timers[handle].handle);
    memset(&timers[handle], 0, sizeof(timer_slot_t));
    ESP_LOGI(TAG, "Deleted timer %d", handle);
}

bool Timer_isActive(timer_handle_t handle)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return false;
    }

    return esp_timer_is_active(timers[handle].handle);
}

uint64_t Timer_getRemaining(timer_handle_t handle)
{
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || timers[handle].handle == NULL) {
        return 0;
    }

    uint64_t expiry;
    esp_err_t ret = esp_timer_get_expiry_time(timers[handle].handle, &expiry);
    if (ret != ESP_OK) {
        return 0;
    }

    uint64_t now = esp_timer_get_time();
    return (expiry > now) ? (expiry - now) : 0;
}

/*============================================================================
 * Hardware Timer Implementation
 *============================================================================*/

static bool IRAM_ATTR hw_timer_alarm_cb(gptimer_handle_t timer,
                                         const gptimer_alarm_event_data_t *edata,
                                         void *user_ctx)
{
    hw_timer_slot_t* slot = (hw_timer_slot_t*)user_ctx;
    if (slot->callback != NULL) {
        return slot->callback(slot->arg);
    }
    return false;
}

bool HWTimer_init(int group, int timer, uint16_t divider, bool countUp)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return false;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (slot->initialized) {
        return true; /* Already initialized */
    }

    gptimer_config_t timer_config = {
        .clk_src = GPTIMER_CLK_SRC_DEFAULT,
        .direction = countUp ? GPTIMER_COUNT_UP : GPTIMER_COUNT_DOWN,
        .resolution_hz = 1000000 / divider, /* 1MHz / divider */
    };

    esp_err_t ret = gptimer_new_timer(&timer_config, &slot->handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to create HW timer [%d][%d]: %s",
                 group, timer, esp_err_to_name(ret));
        return false;
    }

    slot->initialized = true;
    ESP_LOGI(TAG, "HW Timer [%d][%d] initialized", group, timer);
    return true;
}

bool HWTimer_setAlarm(int group, int timer, uint64_t alarm_value, bool autoReload)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return false;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (!slot->initialized) {
        return false;
    }

    gptimer_alarm_config_t alarm_config = {
        .alarm_count = alarm_value,
        .reload_count = 0,
        .flags.auto_reload_on_alarm = autoReload,
    };

    esp_err_t ret = gptimer_set_alarm_action(slot->handle, &alarm_config);
    return (ret == ESP_OK);
}

bool HWTimer_setCallback(int group, int timer, hw_timer_callback_t callback, void* arg)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return false;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (!slot->initialized) {
        return false;
    }

    slot->callback = callback;
    slot->arg = arg;

    gptimer_event_callbacks_t cbs = {
        .on_alarm = hw_timer_alarm_cb,
    };

    esp_err_t ret = gptimer_register_event_callbacks(slot->handle, &cbs, slot);
    if (ret != ESP_OK) {
        return false;
    }

    ret = gptimer_enable(slot->handle);
    return (ret == ESP_OK);
}

void HWTimer_start(int group, int timer)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (slot->initialized) {
        gptimer_start(slot->handle);
    }
}

void HWTimer_stop(int group, int timer)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (slot->initialized) {
        gptimer_stop(slot->handle);
    }
}

uint64_t HWTimer_getCount(int group, int timer)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return 0;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (!slot->initialized) {
        return 0;
    }

    uint64_t count;
    gptimer_get_raw_count(slot->handle, &count);
    return count;
}

void HWTimer_setCount(int group, int timer, uint64_t value)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (slot->initialized) {
        gptimer_set_raw_count(slot->handle, value);
    }
}

void HWTimer_deinit(int group, int timer)
{
    if (group < 0 || group > 1 || timer < 0 || timer > 1) {
        return;
    }

    hw_timer_slot_t* slot = &hw_timers[group][timer];
    if (slot->initialized) {
        gptimer_disable(slot->handle);
        gptimer_del_timer(slot->handle);
        memset(slot, 0, sizeof(hw_timer_slot_t));
        ESP_LOGI(TAG, "HW Timer [%d][%d] deinitialized", group, timer);
    }
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

void Timer_delayMs(uint32_t ms)
{
    vTaskDelay(pdMS_TO_TICKS(ms));
}

uint64_t Timer_getTimestamp(void)
{
    return esp_timer_get_time();
}
