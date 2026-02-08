/*****************************************************************************
 * @file    timer.h
 * @brief   ESP32-C6 Timer Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details Timer interface for ESP32-C6. 2 hardware timers (2 groups x 1).
 *****************************************************************************/

#ifndef ESP32C6_TIMER_H
#define ESP32C6_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#define TIMER_MAX_TIMERS        16
#define TIMER_GROUP_0           0
#define TIMER_GROUP_1           1
#define TIMER_TIMER_0           0
/* Note: ESP32-C6 does not have TIMER_TIMER_1 */

#define ESP32C6_HW_TIMER_COUNT  2

typedef void (*timer_callback_t)(void* arg);
typedef int timer_handle_t;
#define TIMER_INVALID_HANDLE    (-1)

void Timer_init(void);
timer_handle_t Timer_create(const char* name, uint64_t period_us, timer_callback_t callback, void* arg, bool periodic);
timer_handle_t Timer_createOnce(const char* name, uint64_t period_us, timer_callback_t callback, void* arg);
timer_handle_t Timer_createPeriodic(const char* name, uint64_t period_us, timer_callback_t callback, void* arg);
bool Timer_start(timer_handle_t handle);
bool Timer_stop(timer_handle_t handle);
bool Timer_restart(timer_handle_t handle, uint64_t period_us);
void Timer_delete(timer_handle_t handle);
bool Timer_isActive(timer_handle_t handle);
uint64_t Timer_getRemaining(timer_handle_t handle);

typedef bool (*hw_timer_callback_t)(void* arg);
bool HWTimer_init(int group, int timer, uint16_t divider, bool countUp);
bool HWTimer_setAlarm(int group, int timer, uint64_t alarm_value, bool autoReload);
bool HWTimer_setCallback(int group, int timer, hw_timer_callback_t callback, void* arg);
void HWTimer_start(int group, int timer);
void HWTimer_stop(int group, int timer);
uint64_t HWTimer_getCount(int group, int timer);
void HWTimer_setCount(int group, int timer, uint64_t value);
void HWTimer_deinit(int group, int timer);

void Timer_delayMs(uint32_t ms);
uint64_t Timer_getTimestamp(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C6_TIMER_H */
