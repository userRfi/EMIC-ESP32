/*****************************************************************************
 * @file    setPinGPIO33.h
 * @brief   GPIO33 Pin Configuration for ESP32
 * @details GPIO33 - ADC1_CH5, Touch8, XTAL32K_N
 *          Can be used for 32kHz crystal input
 *****************************************************************************/
#ifndef SETPIN_GPIO33_H
#define SETPIN_GPIO33_H

#define GPIO_NUM_.{name}.    GPIO_NUM_33
#define PIN_.{name}._NUM     33
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM8
#define PIN_.{name}._ADC     ADC1_CHANNEL_5

#endif /* SETPIN_GPIO33_H */
