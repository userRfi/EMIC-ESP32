/*****************************************************************************
 * @file    setPinGPIO12.h
 * @brief   GPIO12 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC2 channel 1 and Touch sensor T12.
 *          Can be used for analog input or capacitive touch sensing.
 *          Default SPI3 CLK pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO12_H
#define SETPIN_GPIO12_H

#define GPIO_NUM_ADC2_CH1   GPIO_NUM_12
#define PIN_ADC2_CH1_NUM    12
#define PIN_ADC2_CH1_TOUCH  12
#define PIN_ADC2_CH1_ADC    ADC2_CHANNEL_1
#define PIN_SPI3_CLK_DEFAULT GPIO_NUM_12

#endif /* SETPIN_GPIO12_H */
