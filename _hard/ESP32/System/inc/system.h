/*****************************************************************************
 * @file    system.h
 * @brief   ESP32 System Functions Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details Public interface for ESP32 system initialization, timing,
 *          and reset functions.
 *****************************************************************************/

#ifndef ESP32_SYSTEM_H
#define ESP32_SYSTEM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

/*============================================================================
 * System Initialization
 *============================================================================*/

/**
 * @brief Initialize the ESP32 system
 *
 * Performs the following initialization sequence:
 * - Initialize NVS (Non-Volatile Storage)
 * - Configure system clocks
 * - Initialize FreeRTOS (if not already running)
 * - Set up default event loop
 *
 * @note This function should be called once at startup before any
 *       other peripheral initialization.
 */
void initSystem(void);

/*============================================================================
 * Timing Functions
 *============================================================================*/

/**
 * @brief Delay execution for specified milliseconds
 *
 * Uses FreeRTOS vTaskDelay for cooperative multitasking.
 * During the delay, other tasks can execute.
 *
 * @param ms Number of milliseconds to delay (0 = yield to scheduler)
 */
void HAL_Delay_ms(uint32_t ms);

/**
 * @brief Delay execution for specified microseconds
 *
 * Uses esp_rom_delay_us for precise microsecond delays.
 * This is a blocking delay that does not yield to the scheduler.
 *
 * @param us Number of microseconds to delay
 * @warning For delays > 1000us, prefer HAL_Delay_ms() for better system responsiveness
 */
void HAL_Delay_us(uint32_t us);

/**
 * @brief Get system tick count in milliseconds
 *
 * Returns the number of milliseconds since system boot.
 * Uses esp_timer for high-resolution timing.
 *
 * @return Tick count in milliseconds (wraps after ~49 days)
 */
uint32_t HAL_GetTick(void);

/**
 * @brief Get system tick count in microseconds
 *
 * Returns the number of microseconds since system boot.
 * Uses esp_timer for high-resolution timing.
 *
 * @return Tick count in microseconds (64-bit value)
 */
uint64_t HAL_GetTick_us(void);

/*============================================================================
 * System Control
 *============================================================================*/

/**
 * @brief Perform a software reset of the ESP32
 *
 * This function does not return. The system will restart and
 * execute from the beginning of the application.
 */
void HAL_SystemReset(void);

/**
 * @brief Enter deep sleep mode for specified duration
 *
 * The ESP32 will enter deep sleep and wake up after the specified
 * time. Most RAM contents are lost during deep sleep.
 *
 * @param sleep_time_us Sleep duration in microseconds
 */
void HAL_DeepSleep(uint64_t sleep_time_us);

/**
 * @brief Enter light sleep mode for specified duration
 *
 * The ESP32 will enter light sleep and wake up after the specified
 * time. RAM contents are preserved during light sleep.
 *
 * @param sleep_time_us Sleep duration in microseconds
 */
void HAL_LightSleep(uint64_t sleep_time_us);

/*============================================================================
 * Memory Functions
 *============================================================================*/

/**
 * @brief Get available heap memory
 *
 * @return Free heap size in bytes
 */
uint32_t HAL_GetFreeHeap(void);

/**
 * @brief Get minimum heap memory since boot
 *
 * @return Minimum free heap size in bytes (high water mark)
 */
uint32_t HAL_GetMinFreeHeap(void);

/*============================================================================
 * System Information
 *============================================================================*/

/**
 * @brief Get CPU frequency in MHz
 *
 * @return CPU frequency (typically 80, 160, or 240 MHz)
 */
uint32_t HAL_GetCpuFreqMHz(void);

/**
 * @brief Get chip revision
 *
 * @return Chip revision number
 */
uint8_t HAL_GetChipRevision(void);

/**
 * @brief Get MAC address
 *
 * @param mac Buffer to store MAC address (6 bytes)
 */
void HAL_GetMacAddress(uint8_t* mac);

/*============================================================================
 * Reset Reason
 *============================================================================*/

typedef enum {
    RESET_REASON_UNKNOWN     = 0,
    RESET_REASON_POWERON     = 1,
    RESET_REASON_EXTERNAL    = 2,
    RESET_REASON_SOFTWARE    = 3,
    RESET_REASON_PANIC       = 4,
    RESET_REASON_WATCHDOG    = 5,
    RESET_REASON_DEEPSLEEP   = 6,
    RESET_REASON_BROWNOUT    = 7
} HAL_ResetReason_t;

/**
 * @brief Get the reason for the last reset
 *
 * @return Reset reason enumeration value
 */
HAL_ResetReason_t HAL_GetResetReason(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_SYSTEM_H */
