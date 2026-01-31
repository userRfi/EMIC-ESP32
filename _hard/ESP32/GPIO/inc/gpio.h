/**
 * @file gpio.h
 * @brief EMIC SDK - ESP32 GPIO Hardware Abstraction
 * @details Macros y funciones HAL para control de GPIO en ESP32
 *          Compatible con la capa HAL de EMIC.
 *
 * @author EMIC Development Team
 * @date 2026-01-31
 * @version 1.0.0
 *
 * @note Pines 34-39 son solo de entrada
 * @note Pines 6-11 estan conectados al flash SPI (no usar)
 */

#ifndef _EMIC_GPIO_ESP32_H
#define _EMIC_GPIO_ESP32_H

#include "../System/inc/system.h"
#include "driver/gpio.h"

// =============================================================================
// DEFINICIONES DE PINES ESP32
// =============================================================================

#define EMIC_GPIO_NUM_MAX       40      // GPIO0 - GPIO39

// Pines solo entrada (no tienen driver de salida)
#define GPIO_INPUT_ONLY_34      34
#define GPIO_INPUT_ONLY_35      35
#define GPIO_INPUT_ONLY_36      36
#define GPIO_INPUT_ONLY_37      37
#define GPIO_INPUT_ONLY_38      38
#define GPIO_INPUT_ONLY_39      39

// =============================================================================
// MODOS DE GPIO
// =============================================================================

typedef enum {
    EMIC_GPIO_MODE_OUTPUT       = GPIO_MODE_OUTPUT,
    EMIC_GPIO_MODE_INPUT        = GPIO_MODE_INPUT,
    EMIC_GPIO_MODE_INPUT_PULLUP = GPIO_MODE_INPUT,      // + config pullup
    EMIC_GPIO_MODE_INPUT_PULLDOWN = GPIO_MODE_INPUT,    // + config pulldown
    EMIC_GPIO_MODE_OUTPUT_OD    = GPIO_MODE_OUTPUT_OD,
    EMIC_GPIO_MODE_INOUT        = GPIO_MODE_INPUT_OUTPUT,
} emic_gpio_mode_t;

// =============================================================================
// NIVELES DE GPIO
// =============================================================================

#define EMIC_GPIO_LOW           0
#define EMIC_GPIO_HIGH          1

// =============================================================================
// MACROS HAL - COMPATIBLES CON EMIC
// =============================================================================

/**
 * @brief Configura un pin como salida
 * @param pin Numero de GPIO (0-33)
 */
#define HAL_GPIO_PinCfgOutput(pin) \
    do { \
        gpio_config_t io_conf = { \
            .pin_bit_mask = (1ULL << (pin)), \
            .mode = GPIO_MODE_OUTPUT, \
            .pull_up_en = GPIO_PULLUP_DISABLE, \
            .pull_down_en = GPIO_PULLDOWN_DISABLE, \
            .intr_type = GPIO_INTR_DISABLE \
        }; \
        gpio_config(&io_conf); \
    } while(0)

/**
 * @brief Configura un pin como entrada
 * @param pin Numero de GPIO (0-39)
 */
#define HAL_GPIO_PinCfgInput(pin) \
    do { \
        gpio_config_t io_conf = { \
            .pin_bit_mask = (1ULL << (pin)), \
            .mode = GPIO_MODE_INPUT, \
            .pull_up_en = GPIO_PULLUP_DISABLE, \
            .pull_down_en = GPIO_PULLDOWN_DISABLE, \
            .intr_type = GPIO_INTR_DISABLE \
        }; \
        gpio_config(&io_conf); \
    } while(0)

/**
 * @brief Configura un pin como entrada con pull-up
 * @param pin Numero de GPIO (0-39)
 */
#define HAL_GPIO_PinCfgInputPullUp(pin) \
    do { \
        gpio_config_t io_conf = { \
            .pin_bit_mask = (1ULL << (pin)), \
            .mode = GPIO_MODE_INPUT, \
            .pull_up_en = GPIO_PULLUP_ENABLE, \
            .pull_down_en = GPIO_PULLDOWN_DISABLE, \
            .intr_type = GPIO_INTR_DISABLE \
        }; \
        gpio_config(&io_conf); \
    } while(0)

/**
 * @brief Configura un pin como entrada con pull-down
 * @param pin Numero de GPIO (0-39)
 */
#define HAL_GPIO_PinCfgInputPullDown(pin) \
    do { \
        gpio_config_t io_conf = { \
            .pin_bit_mask = (1ULL << (pin)), \
            .mode = GPIO_MODE_INPUT, \
            .pull_up_en = GPIO_PULLUP_DISABLE, \
            .pull_down_en = GPIO_PULLDOWN_ENABLE, \
            .intr_type = GPIO_INTR_DISABLE \
        }; \
        gpio_config(&io_conf); \
    } while(0)

/**
 * @brief Establece el nivel de un pin
 * @param pin Numero de GPIO
 * @param value 0 = LOW, 1 = HIGH
 */
#define HAL_GPIO_PinSet(pin, value) \
    gpio_set_level((gpio_num_t)(pin), (value))

/**
 * @brief Lee el nivel de un pin
 * @param pin Numero de GPIO
 * @return 0 = LOW, 1 = HIGH
 */
#define HAL_GPIO_PinGet(pin) \
    gpio_get_level((gpio_num_t)(pin))

/**
 * @brief Invierte el nivel de un pin (toggle)
 * @param pin Numero de GPIO
 */
#define HAL_GPIO_PinToggle(pin) \
    gpio_set_level((gpio_num_t)(pin), !gpio_get_level((gpio_num_t)(pin)))

/**
 * @brief Pone un pin en HIGH
 * @param pin Numero de GPIO
 */
#define HAL_GPIO_PinHigh(pin) \
    gpio_set_level((gpio_num_t)(pin), 1)

/**
 * @brief Pone un pin en LOW
 * @param pin Numero de GPIO
 */
#define HAL_GPIO_PinLow(pin) \
    gpio_set_level((gpio_num_t)(pin), 0)

// =============================================================================
// MACROS COMPATIBLES CON SINTAXIS EMIC TRADICIONAL
// =============================================================================

// Para uso con nombres de pin definidos en gpio.emic
// Ejemplo: PIN_LED1 = 2 -> LED1_Set(1)
#define GPIO_Set(name, value)       HAL_GPIO_PinSet(PIN_##name, value)
#define GPIO_Get(name)              HAL_GPIO_PinGet(PIN_##name)
#define GPIO_Toggle(name)           HAL_GPIO_PinToggle(PIN_##name)
#define GPIO_High(name)             HAL_GPIO_PinHigh(PIN_##name)
#define GPIO_Low(name)              HAL_GPIO_PinLow(PIN_##name)
#define GPIO_CfgOutput(name)        HAL_GPIO_PinCfgOutput(PIN_##name)
#define GPIO_CfgInput(name)         HAL_GPIO_PinCfgInput(PIN_##name)
#define GPIO_CfgInputPullUp(name)   HAL_GPIO_PinCfgInputPullUp(PIN_##name)
#define GPIO_CfgInputPullDown(name) HAL_GPIO_PinCfgInputPullDown(PIN_##name)

// =============================================================================
// PROTOTIPOS DE FUNCIONES
// =============================================================================

/**
 * @fn void GPIO_Init(void)
 * @alias gpio.init
 * @brief Inicializa el sistema GPIO
 * @details Llamada automaticamente por EMIC en el grupo inits
 * @param None
 * @return None
 */
void GPIO_Init(void);

/**
 * @fn emic_err_t GPIO_ConfigPin(uint8_t pin, emic_gpio_mode_t mode)
 * @alias gpio.configPin
 * @brief Configura un pin GPIO con el modo especificado
 * @param pin Numero de GPIO (0-39)
 * @param mode Modo de operacion (ver emic_gpio_mode_t)
 * @return EMIC_OK si exito, codigo de error si falla
 */
emic_err_t GPIO_ConfigPin(uint8_t pin, emic_gpio_mode_t mode);

/**
 * @fn bool GPIO_IsValidPin(uint8_t pin)
 * @alias gpio.isValidPin
 * @brief Verifica si un pin es valido para uso general
 * @param pin Numero de GPIO
 * @return true si es valido, false si no
 */
bool GPIO_IsValidPin(uint8_t pin);

/**
 * @fn bool GPIO_IsOutputCapable(uint8_t pin)
 * @alias gpio.isOutputCapable
 * @brief Verifica si un pin puede ser salida
 * @param pin Numero de GPIO
 * @return true si puede ser salida, false si solo entrada
 */
bool GPIO_IsOutputCapable(uint8_t pin);

#endif // _EMIC_GPIO_ESP32_H
