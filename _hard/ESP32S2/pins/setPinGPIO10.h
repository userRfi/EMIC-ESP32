/*****************************************************************************
 * @file    setPinGPIO10.h
 * @brief   GPIO10 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC1 channel 9 and Touch sensor T10.
 *          Can be used for analog input or capacitive touch sensing.
 *          Default SPI3 CS pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO10_H
#define SETPIN_GPIO10_H

#define GPIO_NUM_ADC1_CH9   GPIO_NUM_10
#define PIN_ADC1_CH9_NUM    10
#define PIN_ADC1_CH9_TOUCH  10
#define PIN_ADC1_CH9_ADC    ADC1_CHANNEL_9
#define PIN_SPI3_CS_DEFAULT GPIO_NUM_10

#endif /* SETPIN_GPIO10_H */
