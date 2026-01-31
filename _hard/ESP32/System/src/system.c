/**
 * @file system.c
 * @brief EMIC SDK - ESP32 System Implementation
 * @details Implementacion de funciones del sistema para ESP32
 *
 * @author EMIC Development Team
 * @date 2026-01-31
 * @version 1.0.0
 */

#include "../inc/system.h"
#include "nvs_flash.h"
#include "esp_chip_info.h"

// =============================================================================
// VARIABLES PRIVADAS
// =============================================================================

static bool s_system_initialized = false;
static char s_chip_info_str[64];

// =============================================================================
// IMPLEMENTACION DE FUNCIONES
// =============================================================================

/**
 * @brief Inicializa el sistema EMIC para ESP32
 */
void System_Init(void)
{
    if (s_system_initialized) {
        return;
    }

    // Inicializar NVS (Non-Volatile Storage)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        // NVS partition was truncated and needs to be erased
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Log de inicio
    EMIC_LogInfo("=================================");
    EMIC_LogInfo("EMIC SDK for ESP32 - Initialized");
    EMIC_LogInfo("=================================");
    EMIC_LogInfo("CPU Freq: %d MHz", EMIC_FCPU / 1000000);
    EMIC_LogInfo("Free Heap: %lu bytes", (unsigned long)System_GetFreeHeap());

    s_system_initialized = true;
}

/**
 * @brief Obtiene la memoria heap libre
 */
uint32_t System_GetFreeHeap(void)
{
    return esp_get_free_heap_size();
}

/**
 * @brief Obtiene informacion del chip
 */
const char* System_GetChipInfo(void)
{
    esp_chip_info_t chip_info;
    esp_chip_info(&chip_info);

    snprintf(s_chip_info_str, sizeof(s_chip_info_str),
             "ESP32 - %d cores - Rev %d",
             chip_info.cores,
             chip_info.revision);

    return s_chip_info_str;
}
