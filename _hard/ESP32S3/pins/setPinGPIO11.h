/*****************************************************************************
 * @file    setPinGPIO11.h
 * @brief   GPIO11 Pin Configuration for ESP32-S3
 * @details General purpose I/O with ADC2 channel 0 and Touch sensor T11.
 *          Can be used for analog input or capacitive touch sensing.
 *          Note: ADC2 cannot be used when WiFi is active.
 *****************************************************************************/
#ifndef SETPIN_GPIO11_H
#define SETPIN_GPIO11_H

#define GPIO_NUM_ADC2_CH0   GPIO_NUM_11
#define PIN_ADC2_CH0_NUM    11
#define PIN_ADC2_CH0_TOUCH  11
#define PIN_ADC2_CH0_ADC    ADC2_CHANNEL_0

#endif /* SETPIN_GPIO11_H */
