/*****************************************************************************
 * @file    setPinGPIO2.h
 * @brief   GPIO2 Pin Configuration for ESP32
 * @details GPIO2 - Bootstrap pin, often connected to onboard LED
 *          Can be used for: Touch sensor, ADC2_CH2, HSPI WP
 *****************************************************************************/
#ifndef SETPIN_GPIO2_H
#define SETPIN_GPIO2_H

#define GPIO_NUM_.{name}.    GPIO_NUM_2
#define PIN_.{name}._NUM     2
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM2
#define PIN_.{name}._ADC     ADC2_CHANNEL_2

#endif /* SETPIN_GPIO2_H */
