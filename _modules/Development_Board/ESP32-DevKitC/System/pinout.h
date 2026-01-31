/**
 * @file pinout.h
 * @brief ESP32-DevKitC Pinout Configuration
 * @details Definicion de pines para la placa ESP32-DevKitC
 *
 * @author EMIC Development Team
 * @date 2026-01-31
 * @version 1.0.0
 */

#ifndef _ESP32_DEVKITC_PINOUT_H
#define _ESP32_DEVKITC_PINOUT_H

// =============================================================================
// LED Y BOTONES ONBOARD
// =============================================================================

#define PIN_LED_BUILTIN         2       // LED azul onboard
#define PIN_BUTTON_BOOT         0       // Boton BOOT

// =============================================================================
// UART0 - USB/Serial (Consola)
// =============================================================================

#define PIN_UART0_TX            1
#define PIN_UART0_RX            3

// =============================================================================
// UART1 - Disponible para usuario
// =============================================================================

#define PIN_UART1_TX            10      // Default (puede remapearse)
#define PIN_UART1_RX            9       // Default (puede remapearse)

// =============================================================================
// UART2 - Disponible para usuario
// =============================================================================

#define PIN_UART2_TX            17
#define PIN_UART2_RX            16

// =============================================================================
// I2C - Bus por defecto
// =============================================================================

#define PIN_I2C_SDA             21
#define PIN_I2C_SCL             22

// =============================================================================
// SPI - VSPI (Bus por defecto para usuario)
// =============================================================================

#define PIN_SPI_MOSI            23
#define PIN_SPI_MISO            19
#define PIN_SPI_CLK             18
#define PIN_SPI_CS              5

// =============================================================================
// SPI - HSPI (Bus secundario)
// =============================================================================

#define PIN_HSPI_MOSI           13
#define PIN_HSPI_MISO           12
#define PIN_HSPI_CLK            14
#define PIN_HSPI_CS             15

// =============================================================================
// ADC1 - Canales (Disponibles siempre)
// =============================================================================

#define PIN_ADC1_CH0            36      // VP / SVP (solo entrada)
#define PIN_ADC1_CH3            39      // VN / SVN (solo entrada)
#define PIN_ADC1_CH4            32
#define PIN_ADC1_CH5            33
#define PIN_ADC1_CH6            34      // Solo entrada
#define PIN_ADC1_CH7            35      // Solo entrada

// =============================================================================
// ADC2 - Canales (NO disponibles cuando WiFi activo)
// =============================================================================

#define PIN_ADC2_CH0            4
#define PIN_ADC2_CH2            2       // LED_BUILTIN
#define PIN_ADC2_CH3            15
#define PIN_ADC2_CH4            13
#define PIN_ADC2_CH5            12
#define PIN_ADC2_CH6            14
#define PIN_ADC2_CH7            27
#define PIN_ADC2_CH8            25
#define PIN_ADC2_CH9            26

// =============================================================================
// DAC - Salidas analogicas
// =============================================================================

#define PIN_DAC1                25
#define PIN_DAC2                26

// =============================================================================
// TOUCH - Pines capacitivos
// =============================================================================

#define PIN_TOUCH0              4
#define PIN_TOUCH2              2
#define PIN_TOUCH3              15
#define PIN_TOUCH4              13
#define PIN_TOUCH5              12
#define PIN_TOUCH6              14
#define PIN_TOUCH7              27
#define PIN_TOUCH8              33
#define PIN_TOUCH9              32

// =============================================================================
// GPIO DE USO GENERAL
// =============================================================================

#define PIN_GPIO4               4
#define PIN_GPIO5               5
#define PIN_GPIO12              12
#define PIN_GPIO13              13
#define PIN_GPIO14              14
#define PIN_GPIO15              15
#define PIN_GPIO16              16
#define PIN_GPIO17              17
#define PIN_GPIO18              18
#define PIN_GPIO19              19
#define PIN_GPIO21              21
#define PIN_GPIO22              22
#define PIN_GPIO23              23
#define PIN_GPIO25              25
#define PIN_GPIO26              26
#define PIN_GPIO27              27
#define PIN_GPIO32              32
#define PIN_GPIO33              33

// =============================================================================
// PINES SOLO ENTRADA (Sin driver de salida)
// =============================================================================

#define PIN_INPUT_ONLY_34       34
#define PIN_INPUT_ONLY_35       35
#define PIN_INPUT_ONLY_36       36      // VP
#define PIN_INPUT_ONLY_39       39      // VN

// =============================================================================
// PINES NO DISPONIBLES (Flash SPI interno)
// =============================================================================
// GPIO 6, 7, 8, 9, 10, 11 - Conectados al flash interno, NO USAR

#endif // _ESP32_DEVKITC_PINOUT_H
