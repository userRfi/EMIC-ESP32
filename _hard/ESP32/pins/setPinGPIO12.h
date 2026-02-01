/*****************************************************************************
 * @file    setPinGPIO12.h
 * @brief   GPIO12 Pin Configuration for ESP32
 * @details GPIO12 - Bootstrap pin (MTDI), HSPI MISO, ADC2_CH5, Touch5
 *          Must be LOW during boot for 3.3V flash
 *****************************************************************************/
#ifndef SETPIN_GPIO12_H
#define SETPIN_GPIO12_H

#define GPIO_NUM_.{name}.    GPIO_NUM_12
#define PIN_.{name}._NUM     12
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM5
#define PIN_.{name}._ADC     ADC2_CHANNEL_5
#define PIN_.{name}._SPI     SPI2_HOST
#define PIN_.{name}._SPI_MISO 1

#endif /* SETPIN_GPIO12_H */
