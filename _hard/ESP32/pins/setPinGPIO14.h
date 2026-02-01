/*****************************************************************************
 * @file    setPinGPIO14.h
 * @brief   GPIO14 Pin Configuration for ESP32
 * @details GPIO14 - HSPI CLK, ADC2_CH6, Touch6
 *          Outputs PWM at boot
 *****************************************************************************/
#ifndef SETPIN_GPIO14_H
#define SETPIN_GPIO14_H

#define GPIO_NUM_.{name}.    GPIO_NUM_14
#define PIN_.{name}._NUM     14
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM6
#define PIN_.{name}._ADC     ADC2_CHANNEL_6
#define PIN_.{name}._SPI     SPI2_HOST
#define PIN_.{name}._SPI_CLK 1

#endif /* SETPIN_GPIO14_H */
