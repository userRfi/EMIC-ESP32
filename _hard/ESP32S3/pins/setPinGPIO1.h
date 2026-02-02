/*****************************************************************************
 * @file    setPinGPIO1.h
 * @brief   GPIO1 Pin Configuration for ESP32-S3
 * @details General purpose I/O with ADC1 channel 0 and Touch sensor T1.
 *          Can be used for analog input or capacitive touch sensing.
 *****************************************************************************/
#ifndef SETPIN_GPIO1_H
#define SETPIN_GPIO1_H

#define GPIO_NUM_ADC1_CH0   GPIO_NUM_1
#define PIN_ADC1_CH0_NUM    1
#define PIN_ADC1_CH0_TOUCH  1
#define PIN_ADC1_CH0_ADC    ADC1_CHANNEL_0

#endif /* SETPIN_GPIO1_H */
