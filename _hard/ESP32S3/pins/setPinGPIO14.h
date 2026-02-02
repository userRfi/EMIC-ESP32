/*****************************************************************************
 * @file    setPinGPIO14.h
 * @brief   GPIO14 Pin Configuration for ESP32-S3
 * @details General purpose I/O with ADC2 channel 3 and Touch sensor T14.
 *          Can be used for analog input or capacitive touch sensing.
 *          Note: ADC2 cannot be used when WiFi is active.
 *****************************************************************************/
#ifndef SETPIN_GPIO14_H
#define SETPIN_GPIO14_H

#define GPIO_NUM_ADC2_CH3   GPIO_NUM_14
#define PIN_ADC2_CH3_NUM    14
#define PIN_ADC2_CH3_TOUCH  14
#define PIN_ADC2_CH3_ADC    ADC2_CHANNEL_3

#endif /* SETPIN_GPIO14_H */
