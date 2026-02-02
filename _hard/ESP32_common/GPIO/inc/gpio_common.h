/*****************************************************************************
 * @file    gpio_common.h
 * @brief   Common GPIO HAL Macros for All ESP32 Variants
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details Hardware Abstraction Layer macros for GPIO control.
 *          Compatible with all ESP32 variants (ESP32, S2, S3, C3, C6).
 *****************************************************************************/

#ifndef ESP32_GPIO_COMMON_H
#define ESP32_GPIO_COMMON_H

#ifdef __cplusplus
extern "C" {
#endif

#include "driver/gpio.h"
#include "esp32_common.h"
#include "esp32_features.h"
#include <stdbool.h>

/*============================================================================
 * GPIO Configuration Values
 *============================================================================*/

/* Direction configuration */
#define GPIO_CFG_INPUT          GPIO_MODE_INPUT
#define GPIO_CFG_OUTPUT         GPIO_MODE_OUTPUT
#define GPIO_CFG_OUTPUT_OD      GPIO_MODE_OUTPUT_OD
#define GPIO_CFG_INPUT_OUTPUT   GPIO_MODE_INPUT_OUTPUT
#define GPIO_CFG_INPUT_OUTPUT_OD GPIO_MODE_INPUT_OUTPUT_OD
#define GPIO_CFG_DISABLED       GPIO_MODE_DISABLE

/* Pull-up/Pull-down configuration */
#define GPIO_CFG_PULLUP         GPIO_PULLUP_ENABLE
#define GPIO_CFG_PULLDOWN       GPIO_PULLDOWN_ENABLE
#define GPIO_CFG_NOPULL         GPIO_FLOATING

/* Combined configurations for convenience */
#define GPIO_CFG_INPUT_PULLUP   0x10    /* Input with pull-up */
#define GPIO_CFG_INPUT_PULLDOWN 0x11    /* Input with pull-down */
#define GPIO_CFG_ANALOG         0x20    /* Analog mode (for ADC) */

/*============================================================================
 * GPIO Configuration Macros
 *============================================================================*/

/**
 * @brief Configure GPIO pin direction and mode
 *
 * @param name Pin identifier (maps to GPIO_NUM_##name)
 * @param cfg_value Configuration value (GPIO_CFG_INPUT, GPIO_CFG_OUTPUT, etc.)
 *
 * Usage:
 *   HAL_GPIO_PinCfg(LED, GPIO_CFG_OUTPUT);
 *   HAL_GPIO_PinCfg(BUTTON, GPIO_CFG_INPUT_PULLUP);
 */
#define HAL_GPIO_PinCfg(name, cfg_value) do { \
    gpio_config_t io_conf = {0}; \
    io_conf.pin_bit_mask = (1ULL << GPIO_NUM_##name); \
    if ((cfg_value) == GPIO_CFG_INPUT_PULLUP) { \
        io_conf.mode = GPIO_MODE_INPUT; \
        io_conf.pull_up_en = GPIO_PULLUP_ENABLE; \
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; \
    } else if ((cfg_value) == GPIO_CFG_INPUT_PULLDOWN) { \
        io_conf.mode = GPIO_MODE_INPUT; \
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE; \
        io_conf.pull_down_en = GPIO_PULLDOWN_ENABLE; \
    } else if ((cfg_value) == GPIO_CFG_ANALOG) { \
        io_conf.mode = GPIO_MODE_DISABLE; \
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE; \
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; \
    } else { \
        io_conf.mode = (cfg_value); \
        io_conf.pull_up_en = GPIO_PULLUP_DISABLE; \
        io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE; \
    } \
    io_conf.intr_type = GPIO_INTR_DISABLE; \
    gpio_config(&io_conf); \
} while(0)

/**
 * @brief Configure GPIO with custom parameters
 */
#define HAL_GPIO_PinCfgEx(name, mode, pull_up, pull_down) do { \
    gpio_config_t io_conf = {0}; \
    io_conf.pin_bit_mask = (1ULL << GPIO_NUM_##name); \
    io_conf.mode = (mode); \
    io_conf.pull_up_en = (pull_up); \
    io_conf.pull_down_en = (pull_down); \
    io_conf.intr_type = GPIO_INTR_DISABLE; \
    gpio_config(&io_conf); \
} while(0)

/*============================================================================
 * GPIO Output Macros
 *============================================================================*/

/**
 * @brief Set GPIO pin output value
 */
#define HAL_GPIO_PinSet(name, value) \
    gpio_set_level(GPIO_NUM_##name, (value) ? 1 : 0)

/**
 * @brief Set GPIO pin high
 */
#define HAL_GPIO_PinSetHigh(name) \
    gpio_set_level(GPIO_NUM_##name, 1)

/**
 * @brief Set GPIO pin low
 */
#define HAL_GPIO_PinSetLow(name) \
    gpio_set_level(GPIO_NUM_##name, 0)

/**
 * @brief Toggle GPIO pin output
 */
#define HAL_GPIO_PinToggle(name) \
    gpio_set_level(GPIO_NUM_##name, !gpio_get_level(GPIO_NUM_##name))

/*============================================================================
 * GPIO Input Macros
 *============================================================================*/

/**
 * @brief Read GPIO pin input value
 */
#define HAL_GPIO_PinGet(name) \
    gpio_get_level(GPIO_NUM_##name)

/**
 * @brief Check if GPIO pin is high
 */
#define HAL_GPIO_PinIsHigh(name) \
    (gpio_get_level(GPIO_NUM_##name) != 0)

/**
 * @brief Check if GPIO pin is low
 */
#define HAL_GPIO_PinIsLow(name) \
    (gpio_get_level(GPIO_NUM_##name) == 0)

/*============================================================================
 * GPIO Direct Access (using GPIO number directly)
 *============================================================================*/

/**
 * @brief Configure GPIO by number
 */
#define HAL_GPIO_ConfigByNum(num, mode) \
    gpio_set_direction((gpio_num_t)(num), (mode))

/**
 * @brief Set GPIO output by number
 */
#define HAL_GPIO_SetByNum(num, value) \
    gpio_set_level((gpio_num_t)(num), (value) ? 1 : 0)

/**
 * @brief Get GPIO input by number
 */
#define HAL_GPIO_GetByNum(num) \
    gpio_get_level((gpio_num_t)(num))

/*============================================================================
 * GPIO Interrupt Configuration
 *============================================================================*/

/* Interrupt types */
#define GPIO_INT_DISABLE        GPIO_INTR_DISABLE
#define GPIO_INT_RISING         GPIO_INTR_POSEDGE
#define GPIO_INT_FALLING        GPIO_INTR_NEGEDGE
#define GPIO_INT_BOTH           GPIO_INTR_ANYEDGE
#define GPIO_INT_LOW            GPIO_INTR_LOW_LEVEL
#define GPIO_INT_HIGH           GPIO_INTR_HIGH_LEVEL

/**
 * @brief Configure GPIO interrupt
 */
#define HAL_GPIO_SetInterrupt(name, type) \
    gpio_set_intr_type(GPIO_NUM_##name, (type))

/**
 * @brief Enable GPIO interrupt
 */
#define HAL_GPIO_EnableInterrupt(name) \
    gpio_intr_enable(GPIO_NUM_##name)

/**
 * @brief Disable GPIO interrupt
 */
#define HAL_GPIO_DisableInterrupt(name) \
    gpio_intr_disable(GPIO_NUM_##name)

/*============================================================================
 * GPIO Utility Macros
 *============================================================================*/

/**
 * @brief Check if pin is input-only (chip-specific)
 */
#define HAL_GPIO_IsInputOnly(num) \
    GPIO_IS_INPUT_ONLY(num)

/**
 * @brief Check if pin is valid for this chip
 */
#define HAL_GPIO_IsValid(num) \
    GPIO_IS_VALID(num)

/**
 * @brief Reset GPIO to default state
 */
#define HAL_GPIO_Reset(name) \
    gpio_reset_pin(GPIO_NUM_##name)

#ifdef __cplusplus
}
#endif

#endif /* ESP32_GPIO_COMMON_H */
