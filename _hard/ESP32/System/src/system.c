/*****************************************************************************
 * @file    system.c
 * @brief   ESP32 System Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details Implementation of system initialization, timing, and control
 *          functions using ESP-IDF APIs.
 *****************************************************************************/

#include "system.h"

/* ESP-IDF includes */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_timer.h"
#include "esp_sleep.h"
#include "esp_log.h"
#include "esp_mac.h"
#include "esp_chip_info.h"
#include "esp_cpu.h"
#include "nvs_flash.h"
#include "esp_event.h"
#include "rom/ets_sys.h"

static const char *TAG = "EMIC_System";

/*============================================================================
 * System Initialization
 *============================================================================*/

void initSystem(void)
{
    esp_err_t ret;

    /* Initialize NVS (required for WiFi and other components) */
    ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        /* NVS partition was corrupted, erase and retry */
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    /* Create default event loop */
    ret = esp_event_loop_create_default();
    if (ret != ESP_OK && ret != ESP_ERR_INVALID_STATE) {
        /* ESP_ERR_INVALID_STATE means loop already created, which is OK */
        ESP_ERROR_CHECK(ret);
    }

    ESP_LOGI(TAG, "EMIC System initialized");
    ESP_LOGI(TAG, "CPU Freq: %lu MHz, Free Heap: %lu bytes",
             HAL_GetCpuFreqMHz(), HAL_GetFreeHeap());
}

/*============================================================================
 * Timing Functions
 *============================================================================*/

void HAL_Delay_ms(uint32_t ms)
{
    if (ms == 0) {
        /* Yield to scheduler */
        taskYIELD();
    } else {
        vTaskDelay(pdMS_TO_TICKS(ms));
    }
}

void HAL_Delay_us(uint32_t us)
{
    ets_delay_us(us);
}

uint32_t HAL_GetTick(void)
{
    return (uint32_t)(esp_timer_get_time() / 1000);
}

uint64_t HAL_GetTick_us(void)
{
    return esp_timer_get_time();
}

/*============================================================================
 * System Control
 *============================================================================*/

void HAL_SystemReset(void)
{
    ESP_LOGI(TAG, "System reset requested");
    esp_restart();
    /* Does not return */
}

void HAL_DeepSleep(uint64_t sleep_time_us)
{
    ESP_LOGI(TAG, "Entering deep sleep for %llu us", sleep_time_us);
    esp_deep_sleep(sleep_time_us);
    /* Does not return - wake up causes reset */
}

void HAL_LightSleep(uint64_t sleep_time_us)
{
    ESP_LOGI(TAG, "Entering light sleep for %llu us", sleep_time_us);
    esp_sleep_enable_timer_wakeup(sleep_time_us);
    esp_light_sleep_start();
    /* Returns after wakeup */
    ESP_LOGI(TAG, "Woke up from light sleep");
}

/*============================================================================
 * Memory Functions
 *============================================================================*/

uint32_t HAL_GetFreeHeap(void)
{
    return (uint32_t)esp_get_free_heap_size();
}

uint32_t HAL_GetMinFreeHeap(void)
{
    return (uint32_t)esp_get_minimum_free_heap_size();
}

/*============================================================================
 * System Information
 *============================================================================*/

uint32_t HAL_GetCpuFreqMHz(void)
{
    return (uint32_t)(esp_clk_cpu_freq() / 1000000);
}

uint8_t HAL_GetChipRevision(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);
    return chip_info.revision;
}

void HAL_GetMacAddress(uint8_t* mac)
{
    if (mac != NULL) {
        esp_read_mac(mac, ESP_MAC_WIFI_STA);
    }
}

/*============================================================================
 * Reset Reason
 *============================================================================*/

HAL_ResetReason_t HAL_GetResetReason(void)
{
    esp_reset_reason_t reason = esp_reset_reason();

    switch (reason) {
        case ESP_RST_POWERON:
            return RESET_REASON_POWERON;
        case ESP_RST_EXT:
            return RESET_REASON_EXTERNAL;
        case ESP_RST_SW:
            return RESET_REASON_SOFTWARE;
        case ESP_RST_PANIC:
            return RESET_REASON_PANIC;
        case ESP_RST_INT_WDT:
        case ESP_RST_TASK_WDT:
        case ESP_RST_WDT:
            return RESET_REASON_WATCHDOG;
        case ESP_RST_DEEPSLEEP:
            return RESET_REASON_DEEPSLEEP;
        case ESP_RST_BROWNOUT:
            return RESET_REASON_BROWNOUT;
        default:
            return RESET_REASON_UNKNOWN;
    }
}
