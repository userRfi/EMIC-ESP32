/*****************************************************************************
 * @file    setPinGPIO0.h
 * @brief   GPIO0 Pin Configuration for ESP32
 * @details GPIO0 - Bootstrap pin, has internal pull-up
 *          Can be used for: Boot mode selection, touch sensor, ADC2_CH1
 *****************************************************************************/
#ifndef SETPIN_GPIO0_H
#define SETPIN_GPIO0_H

#define GPIO_NUM_.{name}.    GPIO_NUM_0
#define PIN_.{name}._NUM     0
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM1
#define PIN_.{name}._ADC     ADC2_CHANNEL_1

#endif /* SETPIN_GPIO0_H */
