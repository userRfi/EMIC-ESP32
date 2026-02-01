/*****************************************************************************
 * @file    setPinGPIO13.h
 * @brief   GPIO13 Pin Configuration for ESP32
 * @details GPIO13 - HSPI MOSI, ADC2_CH4, Touch4
 *          General purpose, safe to use
 *****************************************************************************/
#ifndef SETPIN_GPIO13_H
#define SETPIN_GPIO13_H

#define GPIO_NUM_.{name}.    GPIO_NUM_13
#define PIN_.{name}._NUM     13
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM4
#define PIN_.{name}._ADC     ADC2_CHANNEL_4
#define PIN_.{name}._SPI     SPI2_HOST
#define PIN_.{name}._SPI_MOSI 1

#endif /* SETPIN_GPIO13_H */
