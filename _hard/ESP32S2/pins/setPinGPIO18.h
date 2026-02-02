/*****************************************************************************
 * @file    setPinGPIO18.h
 * @brief   GPIO18 Pin Configuration for ESP32-S2
 * @details General purpose I/O with ADC2 channel 7.
 *          DAC channel 2 output pin.
 *          Default UART1 RX pin.
 *          Strapping pin for USB/JTAG mode.
 *****************************************************************************/
#ifndef SETPIN_GPIO18_H
#define SETPIN_GPIO18_H

#define GPIO_NUM_ADC2_CH7   GPIO_NUM_18
#define PIN_ADC2_CH7_NUM    18
#define PIN_ADC2_CH7_ADC    ADC2_CHANNEL_7
#define PIN_DAC2            GPIO_NUM_18
#define PIN_UART1_RX_DEFAULT GPIO_NUM_18

#endif /* SETPIN_GPIO18_H */
