/*****************************************************************************
 * @file    timer.h
 * @brief   ESP32-C3 Timer Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details Timer interface for ESP32-C3 supporting both esp_timer (software)
 *          and hardware timers. ESP32-C3 has 2 hardware timers (2 groups x 1).
 *****************************************************************************/

#ifndef ESP32C3_TIMER_H
#define ESP32C3_TIMER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

/* Maximum number of software timers */
#define TIMER_MAX_TIMERS        16

/* Hardware timer groups and timers - ESP32-C3 only has 1 timer per group */
#define TIMER_GROUP_0           0
#define TIMER_GROUP_1           1
#define TIMER_TIMER_0           0
/* Note: ESP32-C3 does not have TIMER_TIMER_1 */

/* ESP32-C3 specific: Only 2 hardware timers total */
#define ESP32C3_HW_TIMER_COUNT  2

/*============================================================================
 * Types
 *============================================================================*/

/**
 * @brief Timer callback function type
 *
 * @param arg User argument passed to timer_create
 */
typedef void (*timer_callback_t)(void* arg);

/**
 * @brief Timer handle type
 */
typedef int timer_handle_t;

#define TIMER_INVALID_HANDLE    (-1)

/*============================================================================
 * Software Timer Functions (esp_timer)
 *============================================================================*/

/**
 * @brief Initialize timer subsystem
 */
void Timer_init(void);

/**
 * @brief Create a new software timer
 *
 * @param name Timer name for debugging (can be NULL)
 * @param period_us Timer period in microseconds
 * @param callback Function to call when timer expires
 * @param arg User argument passed to callback
 * @param periodic true for repeating timer, false for one-shot
 * @return Timer handle or TIMER_INVALID_HANDLE on error
 */
timer_handle_t Timer_create(const char* name, uint64_t period_us,
                            timer_callback_t callback, void* arg, bool periodic);

/**
 * @brief Create a one-shot timer
 *
 * @param name Timer name
 * @param period_us Timer period in microseconds
 * @param callback Callback function
 * @param arg User argument
 * @return Timer handle
 */
timer_handle_t Timer_createOnce(const char* name, uint64_t period_us,
                                 timer_callback_t callback, void* arg);

/**
 * @brief Create a periodic timer
 *
 * @param name Timer name
 * @param period_us Timer period in microseconds
 * @param callback Callback function
 * @param arg User argument
 * @return Timer handle
 */
timer_handle_t Timer_createPeriodic(const char* name, uint64_t period_us,
                                     timer_callback_t callback, void* arg);

/**
 * @brief Start a timer
 *
 * @param handle Timer handle
 * @return true if successful
 */
bool Timer_start(timer_handle_t handle);

/**
 * @brief Stop a timer
 *
 * @param handle Timer handle
 * @return true if successful
 */
bool Timer_stop(timer_handle_t handle);

/**
 * @brief Restart a timer with a new period
 *
 * @param handle Timer handle
 * @param period_us New period in microseconds
 * @return true if successful
 */
bool Timer_restart(timer_handle_t handle, uint64_t period_us);

/**
 * @brief Delete a timer
 *
 * @param handle Timer handle
 */
void Timer_delete(timer_handle_t handle);

/**
 * @brief Check if timer is active
 *
 * @param handle Timer handle
 * @return true if timer is running
 */
bool Timer_isActive(timer_handle_t handle);

/**
 * @brief Get time until timer expires
 *
 * @param handle Timer handle
 * @return Time in microseconds, or 0 if not active
 */
uint64_t Timer_getRemaining(timer_handle_t handle);

/*============================================================================
 * Hardware Timer Functions
 *============================================================================*/

/**
 * @brief Hardware timer callback type
 *
 * @return true to yield from ISR, false otherwise
 */
typedef bool (*hw_timer_callback_t)(void* arg);

/**
 * @brief Initialize a hardware timer
 *
 * @param group Timer group (0 or 1)
 * @param timer Timer index (0 only for ESP32-C3)
 * @param divider Clock divider (1-65535)
 * @param countUp true for count up, false for count down
 * @return true if successful
 */
bool HWTimer_init(int group, int timer, uint16_t divider, bool countUp);

/**
 * @brief Set hardware timer alarm
 *
 * @param group Timer group
 * @param timer Timer index
 * @param alarm_value Alarm value in timer ticks
 * @param autoReload true to auto-reload on alarm
 * @return true if successful
 */
bool HWTimer_setAlarm(int group, int timer, uint64_t alarm_value, bool autoReload);

/**
 * @brief Set hardware timer callback
 *
 * @param group Timer group
 * @param timer Timer index
 * @param callback Callback function
 * @param arg User argument
 * @return true if successful
 */
bool HWTimer_setCallback(int group, int timer, hw_timer_callback_t callback, void* arg);

/**
 * @brief Start hardware timer
 *
 * @param group Timer group
 * @param timer Timer index
 */
void HWTimer_start(int group, int timer);

/**
 * @brief Stop hardware timer
 *
 * @param group Timer group
 * @param timer Timer index
 */
void HWTimer_stop(int group, int timer);

/**
 * @brief Get hardware timer counter value
 *
 * @param group Timer group
 * @param timer Timer index
 * @return Current counter value
 */
uint64_t HWTimer_getCount(int group, int timer);

/**
 * @brief Set hardware timer counter value
 *
 * @param group Timer group
 * @param timer Timer index
 * @param value New counter value
 */
void HWTimer_setCount(int group, int timer, uint64_t value);

/**
 * @brief Deinitialize hardware timer
 *
 * @param group Timer group
 * @param timer Timer index
 */
void HWTimer_deinit(int group, int timer);

/*============================================================================
 * Utility Functions
 *============================================================================*/

/**
 * @brief Delay using timer (non-blocking, use in tasks only)
 *
 * @param ms Milliseconds to delay
 */
void Timer_delayMs(uint32_t ms);

/**
 * @brief Get high-resolution timestamp
 *
 * @return Timestamp in microseconds since boot
 */
uint64_t Timer_getTimestamp(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_TIMER_H */
