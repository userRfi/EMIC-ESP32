/**
 * @file gpio.c
 * @brief EMIC SDK - ESP32 GPIO Implementation
 * @details Implementacion de funciones GPIO para ESP32
 *
 * @author EMIC Development Team
 * @date 2026-01-31
 * @version 1.0.0
 */

#include "../inc/gpio.h"

// =============================================================================
// CONSTANTES PRIVADAS
// =============================================================================

// Pines que no deben usarse (conectados a flash SPI interno)
static const uint8_t s_flash_pins[] = {6, 7, 8, 9, 10, 11};
static const uint8_t s_flash_pins_count = sizeof(s_flash_pins) / sizeof(s_flash_pins[0]);

// Pines solo entrada (GPIO34-39)
static const uint8_t s_input_only_pins[] = {34, 35, 36, 37, 38, 39};
static const uint8_t s_input_only_count = sizeof(s_input_only_pins) / sizeof(s_input_only_pins[0]);

// =============================================================================
// VARIABLES PRIVADAS
// =============================================================================

static bool s_gpio_initialized = false;

// =============================================================================
// FUNCIONES PRIVADAS
// =============================================================================

/**
 * @brief Verifica si un pin esta en la lista de flash
 */
static bool is_flash_pin(uint8_t pin)
{
    for (uint8_t i = 0; i < s_flash_pins_count; i++) {
        if (s_flash_pins[i] == pin) {
            return true;
        }
    }
    return false;
}

/**
 * @brief Verifica si un pin es solo entrada
 */
static bool is_input_only(uint8_t pin)
{
    for (uint8_t i = 0; i < s_input_only_count; i++) {
        if (s_input_only_pins[i] == pin) {
            return true;
        }
    }
    return false;
}

// =============================================================================
// IMPLEMENTACION DE FUNCIONES PUBLICAS
// =============================================================================

/**
 * @brief Inicializa el sistema GPIO
 */
void GPIO_Init(void)
{
    if (s_gpio_initialized) {
        return;
    }

    EMIC_LogInfo("GPIO: Initialized");
    EMIC_LogInfo("GPIO: Available pins 0-39 (except 6-11 flash)");
    EMIC_LogInfo("GPIO: Input-only pins: 34-39");

    s_gpio_initialized = true;
}

/**
 * @brief Configura un pin GPIO con el modo especificado
 */
emic_err_t GPIO_ConfigPin(uint8_t pin, emic_gpio_mode_t mode)
{
    // Validar pin
    if (!GPIO_IsValidPin(pin)) {
        EMIC_LogError("GPIO: Pin %d is not valid (flash pin)", pin);
        return EMIC_ERROR_INVALID_ARG;
    }

    // Validar modo salida en pines solo entrada
    if (!GPIO_IsOutputCapable(pin) &&
        (mode == EMIC_GPIO_MODE_OUTPUT || mode == EMIC_GPIO_MODE_OUTPUT_OD)) {
        EMIC_LogError("GPIO: Pin %d is input-only, cannot set as output", pin);
        return EMIC_ERROR_INVALID_ARG;
    }

    // Configurar pin
    gpio_config_t io_conf = {
        .pin_bit_mask = (1ULL << pin),
        .mode = GPIO_MODE_INPUT,  // Default
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE
    };

    switch (mode) {
        case EMIC_GPIO_MODE_OUTPUT:
            io_conf.mode = GPIO_MODE_OUTPUT;
            break;
        case EMIC_GPIO_MODE_INPUT:
            io_conf.mode = GPIO_MODE_INPUT;
            break;
        case EMIC_GPIO_MODE_INPUT_PULLUP:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_up_en = GPIO_PULLUP_ENABLE;
            break;
        case EMIC_GPIO_MODE_INPUT_PULLDOWN:
            io_conf.mode = GPIO_MODE_INPUT;
            io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE;
            break;
        case EMIC_GPIO_MODE_OUTPUT_OD:
            io_conf.mode = GPIO_MODE_OUTPUT_OD;
            break;
        case EMIC_GPIO_MODE_INOUT:
            io_conf.mode = GPIO_MODE_INPUT_OUTPUT;
            break;
        default:
            return EMIC_ERROR_INVALID_ARG;
    }

    esp_err_t err = gpio_config(&io_conf);
    if (err != ESP_OK) {
        EMIC_LogError("GPIO: Failed to configure pin %d", pin);
        return EMIC_ERROR;
    }

    EMIC_LogDebug("GPIO: Pin %d configured, mode=%d", pin, mode);
    return EMIC_OK;
}

/**
 * @brief Verifica si un pin es valido para uso general
 */
bool GPIO_IsValidPin(uint8_t pin)
{
    // Rango valido
    if (pin >= EMIC_GPIO_NUM_MAX) {
        return false;
    }

    // No usar pines de flash
    if (is_flash_pin(pin)) {
        return false;
    }

    return true;
}

/**
 * @brief Verifica si un pin puede ser salida
 */
bool GPIO_IsOutputCapable(uint8_t pin)
{
    if (!GPIO_IsValidPin(pin)) {
        return false;
    }

    // Pines 34-39 son solo entrada
    if (is_input_only(pin)) {
        return false;
    }

    return true;
}
