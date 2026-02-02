/*****************************************************************************
 * @file    setPinGPIO9.h
 * @brief   GPIO9 Pin Configuration for ESP32-C3
 * @details GPIO9 - Strapping pin (boot button), I2C SCL default
 *          Boot button: Active LOW enters download mode
 *****************************************************************************/
#ifndef SETPIN_GPIO9_H
#define SETPIN_GPIO9_H

#define GPIO_NUM_.{name}.    GPIO_NUM_9
#define PIN_.{name}._NUM     9
#define PIN_.{name}._STRAP   1
#define PIN_.{name}._BOOT    1

#endif /* SETPIN_GPIO9_H */
