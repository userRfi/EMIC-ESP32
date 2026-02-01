/*****************************************************************************
 * @file    setPinGPIO15.h
 * @brief   GPIO15 Pin Configuration for ESP32
 * @details GPIO15 - Bootstrap pin (MTDO), HSPI CS, ADC2_CH3, Touch3
 *          Outputs PWM at boot, silences boot messages if LOW
 *****************************************************************************/
#ifndef SETPIN_GPIO15_H
#define SETPIN_GPIO15_H

#define GPIO_NUM_.{name}.    GPIO_NUM_15
#define PIN_.{name}._NUM     15
#define PIN_.{name}._TOUCH   TOUCH_PAD_NUM3
#define PIN_.{name}._ADC     ADC2_CHANNEL_3
#define PIN_.{name}._SPI     SPI2_HOST
#define PIN_.{name}._SPI_CS  1

#endif /* SETPIN_GPIO15_H */
