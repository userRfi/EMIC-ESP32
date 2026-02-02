/*****************************************************************************
 * @file    setPinGPIO8.h
 * @brief   GPIO8 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC1 channel 7 and Touch sensor T8.
 *          Can be used for analog input or capacitive touch sensing.
 *          Default I2C0 SDA pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO8_H
#define SETPIN_GPIO8_H

#define GPIO_NUM_ADC1_CH7   GPIO_NUM_8
#define PIN_ADC1_CH7_NUM    8
#define PIN_ADC1_CH7_TOUCH  8
#define PIN_ADC1_CH7_ADC    ADC1_CHANNEL_7
#define PIN_I2C0_SDA_DEFAULT GPIO_NUM_8

#endif /* SETPIN_GPIO8_H */
