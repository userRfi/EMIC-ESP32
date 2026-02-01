/*****************************************************************************
 * @file    setPinGPIO32.h
 * @brief   GPIO32 Pin Configuration for ESP32
 * @details GPIO32 - ADC1_CH4, Touch9, XTAL32K_P
 *          Can be used for 32kHz crystal input
 *****************************************************************************/
#ifndef SETPIN_GPIO32_H
#define SETPIN_GPIO32_H

#define GPIO_NUM_.{name}.    GPIO_NUM_32
#define PIN_.{name}._NUM     32
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM9
#define PIN_.{name}._ADC     ADC1_CHANNEL_4

#endif /* SETPIN_GPIO32_H */
