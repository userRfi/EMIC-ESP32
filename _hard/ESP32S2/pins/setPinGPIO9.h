/*****************************************************************************
 * @file    setPinGPIO9.h
 * @brief   GPIO9 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC1 channel 8 and Touch sensor T9.
 *          Can be used for analog input or capacitive touch sensing.
 *          Default I2C0 SCL pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO9_H
#define SETPIN_GPIO9_H

#define GPIO_NUM_ADC1_CH8   GPIO_NUM_9
#define PIN_ADC1_CH8_NUM    9
#define PIN_ADC1_CH8_TOUCH  9
#define PIN_ADC1_CH8_ADC    ADC1_CHANNEL_8
#define PIN_I2C0_SCL_DEFAULT GPIO_NUM_9

#endif /* SETPIN_GPIO9_H */
