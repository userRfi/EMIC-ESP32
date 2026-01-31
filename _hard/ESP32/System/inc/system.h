/**
 * @file system.h
 * @brief EMIC SDK - ESP32 System Definitions
 * @details Definiciones base del sistema para microcontroladores ESP32
 *          usando el framework ESP-IDF de Espressif.
 *
 * @author EMIC Development Team
 * @date 2026-01-31
 * @version 1.0.0
 *
 * @note Este archivo es parte del SDK EMIC para ESP32
 * @see https://docs.espressif.com/projects/esp-idf/
 */

#ifndef _EMIC_SYSTEM_ESP32_H
#define _EMIC_SYSTEM_ESP32_H

// =============================================================================
// INCLUDES ESP-IDF
// =============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

// ESP-IDF Core
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_log.h"
#include "esp_err.h"

// =============================================================================
// DEFINICIONES DEL MCU
// =============================================================================

#define EMIC_MCU_NAME           "ESP32"
#define EMIC_MCU_FAMILY         "ESP32"
#define EMIC_MCU_ARCH           "Xtensa_LX6"
#define EMIC_MCU_BITS           32

// =============================================================================
// FRECUENCIAS DEL SISTEMA
// =============================================================================

#define EMIC_FCPU               240000000UL     // 240 MHz CPU frequency
#define EMIC_FAPB               80000000UL      // 80 MHz APB frequency
#define EMIC_FXTAL              40000000UL      // 40 MHz Crystal

// =============================================================================
// PERIFERICOS DISPONIBLES
// =============================================================================

#define EMIC_GPIO_COUNT         34
#define EMIC_UART_COUNT         3
#define EMIC_SPI_COUNT          4
#define EMIC_I2C_COUNT          2
#define EMIC_ADC_COUNT          2
#define EMIC_TIMER_COUNT        4
#define EMIC_PWM_CHANNELS       16

// =============================================================================
// CARACTERISTICAS ESP32
// =============================================================================

#define EMIC_HAS_WIFI           1
#define EMIC_HAS_BLUETOOTH      1
#define EMIC_HAS_BLE            1
#define EMIC_HAS_ETHERNET       0
#define EMIC_HAS_USB            0
#define EMIC_HAS_CAN            1
#define EMIC_HAS_DAC            1
#define EMIC_HAS_TOUCH          1

// =============================================================================
// MEMORIA
// =============================================================================

#define EMIC_FLASH_SIZE         (4 * 1024 * 1024)   // 4 MB
#define EMIC_SRAM_SIZE          (520 * 1024)        // 520 KB
#define EMIC_PSRAM_SUPPORT      1

// =============================================================================
// MACROS DE UTILIDAD
// =============================================================================

/**
 * @brief Delay en milisegundos (compatible con FreeRTOS)
 * @param ms Tiempo en milisegundos
 */
#define EMIC_DelayMs(ms)        vTaskDelay(pdMS_TO_TICKS(ms))

/**
 * @brief Delay en microsegundos
 * @param us Tiempo en microsegundos
 */
#define EMIC_DelayUs(us)        esp_rom_delay_us(us)

/**
 * @brief Obtener tiempo en milisegundos desde el inicio
 * @return Tiempo en milisegundos
 */
#define EMIC_GetTickMs()        (xTaskGetTickCount() * portTICK_PERIOD_MS)

/**
 * @brief Reiniciar el sistema
 */
#define EMIC_SystemReset()      esp_restart()

/**
 * @brief Deshabilitar interrupciones globales
 */
#define EMIC_DisableInterrupts() portDISABLE_INTERRUPTS()

/**
 * @brief Habilitar interrupciones globales
 */
#define EMIC_EnableInterrupts()  portENABLE_INTERRUPTS()

// =============================================================================
// LOGGING (wrapper sobre ESP_LOG)
// =============================================================================

#define EMIC_LOG_TAG            "EMIC"

#define EMIC_LogError(fmt, ...)   ESP_LOGE(EMIC_LOG_TAG, fmt, ##__VA_ARGS__)
#define EMIC_LogWarn(fmt, ...)    ESP_LOGW(EMIC_LOG_TAG, fmt, ##__VA_ARGS__)
#define EMIC_LogInfo(fmt, ...)    ESP_LOGI(EMIC_LOG_TAG, fmt, ##__VA_ARGS__)
#define EMIC_LogDebug(fmt, ...)   ESP_LOGD(EMIC_LOG_TAG, fmt, ##__VA_ARGS__)
#define EMIC_LogVerbose(fmt, ...) ESP_LOGV(EMIC_LOG_TAG, fmt, ##__VA_ARGS__)

// =============================================================================
// TIPOS DE DATOS EMIC
// =============================================================================

typedef uint8_t     emic_u8_t;
typedef uint16_t    emic_u16_t;
typedef uint32_t    emic_u32_t;
typedef uint64_t    emic_u64_t;

typedef int8_t      emic_s8_t;
typedef int16_t     emic_s16_t;
typedef int32_t     emic_s32_t;
typedef int64_t     emic_s64_t;

typedef bool        emic_bool_t;

// =============================================================================
// CODIGOS DE ERROR EMIC
// =============================================================================

typedef enum {
    EMIC_OK = 0,                    /**< Sin error */
    EMIC_ERROR = -1,                /**< Error generico */
    EMIC_ERROR_INVALID_ARG = -2,    /**< Argumento invalido */
    EMIC_ERROR_NO_MEM = -3,         /**< Sin memoria */
    EMIC_ERROR_TIMEOUT = -4,        /**< Timeout */
    EMIC_ERROR_NOT_INIT = -5,       /**< No inicializado */
    EMIC_ERROR_BUSY = -6,           /**< Recurso ocupado */
} emic_err_t;

// =============================================================================
// PROTOTIPOS DE FUNCIONES
// =============================================================================

/**
 * @fn void System_Init(void)
 * @alias system.init
 * @brief Inicializa el sistema EMIC para ESP32
 * @details Configura el sistema base incluyendo NVS y logging.
 *          Debe llamarse al inicio del programa.
 * @param None
 * @return None
 */
void System_Init(void);

/**
 * @fn uint32_t System_GetFreeHeap(void)
 * @alias system.getFreeHeap
 * @brief Obtiene la memoria heap libre
 * @return Bytes de heap disponibles
 */
uint32_t System_GetFreeHeap(void);

/**
 * @fn const char* System_GetChipInfo(void)
 * @alias system.getChipInfo
 * @brief Obtiene informacion del chip
 * @return String con informacion del chip
 */
const char* System_GetChipInfo(void);

#endif // _EMIC_SYSTEM_ESP32_H
