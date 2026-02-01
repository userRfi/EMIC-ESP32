/*****************************************************************************
 * @file    setPinGPIO8.h
 * @brief   GPIO8 Pin Configuration for ESP32
 * @details GPIO8 - Connected to SPI flash (SD1), DO NOT USE
 *          Reserved for internal flash communication
 *****************************************************************************/
#ifndef SETPIN_GPIO8_H
#define SETPIN_GPIO8_H

#define GPIO_NUM_.{name}.    GPIO_NUM_8
#define PIN_.{name}._NUM     8
#define PIN_.{name}._RESERVED 1
/* WARNING: GPIO6-11 are connected to SPI flash - do not use */

#endif /* SETPIN_GPIO8_H */
