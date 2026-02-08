/*****************************************************************************
 * @file    timer.c
 * @brief   ESP32-C6 Timer Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details Timer implementation for ESP32-C6. 2 hardware timers (2 groups x 1).
 *****************************************************************************/

#include "timer.h"

#include "esp_timer.h"
#include "driver/gptimer.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string.h>

static const char *TAG = "Timer_C6";

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

typedef struct {
    gptimer_handle_t handle;
    hw_timer_callback_t callback;
    void* arg;
    bool initialized;
} hw_timer_slot_t;

static hw_timer_slot_t hw_timers[2][1] = {0}; /* [group][timer] - only 1 timer per group */

static void timer_callback_wrapper(void* arg) {
    timer_slot_t* slot = (timer_slot_t*)arg;
    if (slot->callback) slot->callback(slot->arg);
}

static int find_free_slot(void) {
    for (int i = 0; i < TIMER_MAX_TIMERS; i++) if (timers[i].handle == NULL) return i;
    return -1;
}

void Timer_init(void) {
    if (timer_initialized) return;
    memset(timers, 0, sizeof(timers));
    timer_initialized = true;
    ESP_LOGI(TAG, "Timer subsystem initialized");
}

timer_handle_t Timer_create(const char* name, uint64_t period_us, timer_callback_t callback, void* arg, bool periodic) {
    if (!timer_initialized) Timer_init();
    int slot = find_free_slot();
    if (slot < 0) return TIMER_INVALID_HANDLE;

    esp_timer_create_args_t timer_args = { .callback = timer_callback_wrapper, .arg = &timers[slot], .dispatch_method = ESP_TIMER_TASK, .name = name ? name : "emic_timer" };
    if (esp_timer_create(&timer_args, &timers[slot].handle) != ESP_OK) return TIMER_INVALID_HANDLE;

    timers[slot].callback = callback;
    timers[slot].arg = arg;
    timers[slot].periodic = periodic;
    timers[slot].period_us = period_us;
    timers[slot].active = false;
    return slot;
}

timer_handle_t Timer_createOnce(const char* name, uint64_t period_us, timer_callback_t callback, void* arg) { return Timer_create(name, period_us, callback, arg, false); }
timer_handle_t Timer_createPeriodic(const char* name, uint64_t period_us, timer_callback_t callback, void* arg) { return Timer_create(name, period_us, callback, arg, true); }

bool Timer_start(timer_handle_t handle) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return false;
    timer_slot_t* slot = &timers[handle];
    esp_err_t ret = slot->periodic ? esp_timer_start_periodic(slot->handle, slot->period_us) : esp_timer_start_once(slot->handle, slot->period_us);
    if (ret == ESP_OK) { slot->active = true; return true; }
    return false;
}

bool Timer_stop(timer_handle_t handle) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return false;
    esp_err_t ret = esp_timer_stop(timers[handle].handle);
    if (ret == ESP_OK || ret == ESP_ERR_INVALID_STATE) { timers[handle].active = false; return true; }
    return false;
}

bool Timer_restart(timer_handle_t handle, uint64_t period_us) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return false;
    Timer_stop(handle);
    timers[handle].period_us = period_us;
    return Timer_start(handle);
}

void Timer_delete(timer_handle_t handle) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return;
    Timer_stop(handle);
    esp_timer_delete(timers[handle].handle);
    memset(&timers[handle], 0, sizeof(timer_slot_t));
}

bool Timer_isActive(timer_handle_t handle) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return false;
    return esp_timer_is_active(timers[handle].handle);
}

uint64_t Timer_getRemaining(timer_handle_t handle) {
    if (handle < 0 || handle >= TIMER_MAX_TIMERS || !timers[handle].handle) return 0;
    uint64_t expiry;
    if (esp_timer_get_expiry_time(timers[handle].handle, &expiry) != ESP_OK) return 0;
    uint64_t now = esp_timer_get_time();
    return (expiry > now) ? (expiry - now) : 0;
}

static bool IRAM_ATTR hw_timer_alarm_cb(gptimer_handle_t timer, const gptimer_alarm_event_data_t *edata, void *user_ctx) {
    hw_timer_slot_t* slot = (hw_timer_slot_t*)user_ctx;
    if (slot->callback) return slot->callback(slot->arg);
    return false;
}

bool HWTimer_init(int group, int timer, uint16_t divider, bool countUp) {
    if (group < 0 || group > 1 || timer != 0) { ESP_LOGE(TAG, "Invalid timer: ESP32-C6 only has timer 0 in each group"); return false; }
    hw_timer_slot_t* slot = &hw_timers[group][0];
    if (slot->initialized) return true;

    gptimer_config_t timer_config = { .clk_src = GPTIMER_CLK_SRC_DEFAULT, .direction = countUp ? GPTIMER_COUNT_UP : GPTIMER_COUNT_DOWN, .resolution_hz = 1000000 / divider };
    if (gptimer_new_timer(&timer_config, &slot->handle) != ESP_OK) return false;
    slot->initialized = true;
    return true;
}

bool HWTimer_setAlarm(int group, int timer, uint64_t alarm_value, bool autoReload) {
    if (group < 0 || group > 1 || timer != 0) return false;
    hw_timer_slot_t* slot = &hw_timers[group][0];
    if (!slot->initialized) return false;
    gptimer_alarm_config_t alarm_config = { .alarm_count = alarm_value, .reload_count = 0, .flags.auto_reload_on_alarm = autoReload };
    return gptimer_set_alarm_action(slot->handle, &alarm_config) == ESP_OK;
}

bool HWTimer_setCallback(int group, int timer, hw_timer_callback_t callback, void* arg) {
    if (group < 0 || group > 1 || timer != 0) return false;
    hw_timer_slot_t* slot = &hw_timers[group][0];
    if (!slot->initialized) return false;
    slot->callback = callback;
    slot->arg = arg;
    gptimer_event_callbacks_t cbs = { .on_alarm = hw_timer_alarm_cb };
    if (gptimer_register_event_callbacks(slot->handle, &cbs, slot) != ESP_OK) return false;
    return gptimer_enable(slot->handle) == ESP_OK;
}

void HWTimer_start(int group, int timer) { if (group < 0 || group > 1 || timer != 0) return; hw_timer_slot_t* slot = &hw_timers[group][0]; if (slot->initialized) gptimer_start(slot->handle); }
void HWTimer_stop(int group, int timer) { if (group < 0 || group > 1 || timer != 0) return; hw_timer_slot_t* slot = &hw_timers[group][0]; if (slot->initialized) gptimer_stop(slot->handle); }

uint64_t HWTimer_getCount(int group, int timer) {
    if (group < 0 || group > 1 || timer != 0) return 0;
    hw_timer_slot_t* slot = &hw_timers[group][0];
    if (!slot->initialized) return 0;
    uint64_t count;
    gptimer_get_raw_count(slot->handle, &count);
    return count;
}

void HWTimer_setCount(int group, int timer, uint64_t value) { if (group < 0 || group > 1 || timer != 0) return; hw_timer_slot_t* slot = &hw_timers[group][0]; if (slot->initialized) gptimer_set_raw_count(slot->handle, value); }

void HWTimer_deinit(int group, int timer) {
    if (group < 0 || group > 1 || timer != 0) return;
    hw_timer_slot_t* slot = &hw_timers[group][0];
    if (slot->initialized) {
        gptimer_disable(slot->handle);
        gptimer_del_timer(slot->handle);
        memset(slot, 0, sizeof(hw_timer_slot_t));
    }
}

void Timer_delayMs(uint32_t ms) { vTaskDelay(pdMS_TO_TICKS(ms)); }
uint64_t Timer_getTimestamp(void) { return esp_timer_get_time(); }
