/*****************************************************************************
 * @file    setPinGPIO13.h
 * @brief   GPIO13 Pin Configuration for ESP32-S3
 * @details General purpose I/O with ADC2 channel 2 and Touch sensor T13.
 *          Can be used for analog input or capacitive touch sensing.
 *          Note: ADC2 cannot be used when WiFi is active.
 *****************************************************************************/
#ifndef SETPIN_GPIO13_H
#define SETPIN_GPIO13_H

#define GPIO_NUM_ADC2_CH2   GPIO_NUM_13
#define PIN_ADC2_CH2_NUM    13
#define PIN_ADC2_CH2_TOUCH  13
#define PIN_ADC2_CH2_ADC    ADC2_CHANNEL_2

#endif /* SETPIN_GPIO13_H */
