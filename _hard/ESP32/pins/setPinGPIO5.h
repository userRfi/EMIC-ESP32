/*****************************************************************************
 * @file    setPinGPIO5.h
 * @brief   GPIO5 Pin Configuration for ESP32
 * @details GPIO5 - Bootstrap pin (strapping), VSPI CS default
 *          Has internal pull-up, outputs PWM at boot
 *****************************************************************************/
#ifndef SETPIN_GPIO5_H
#define SETPIN_GPIO5_H

#define GPIO_NUM_.{name}.    GPIO_NUM_5
#define PIN_.{name}._NUM     5
#define PIN_.{name}._SPI     SPI3_HOST
#define PIN_.{name}._SPI_CS  1

#endif /* SETPIN_GPIO5_H */
