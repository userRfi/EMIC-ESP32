/*****************************************************************************
 * @file    setPinGPIO17.h
 * @brief   GPIO17 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC2 channel 6.
 *          DAC channel 1 output pin.
 *          Default UART1 TX pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO17_H
#define SETPIN_GPIO17_H

#define GPIO_NUM_ADC2_CH6   GPIO_NUM_17
#define PIN_ADC2_CH6_NUM    17
#define PIN_ADC2_CH6_ADC    ADC2_CHANNEL_6
#define PIN_DAC1            GPIO_NUM_17
#define PIN_UART1_TX_DEFAULT GPIO_NUM_17

#endif /* SETPIN_GPIO17_H */
