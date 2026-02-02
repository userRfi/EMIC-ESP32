/*****************************************************************************
 * @file    setPinGPIO45.h
 * @brief   GPIO45 Pin Configuration for ESP32-S3
 * @details Strapping pin - controls VDD_SPI voltage (internal LDO or external).
 *          Should not have external pull-up/down during boot.
 *          Can be used as general purpose I/O after boot.
 *****************************************************************************/
#ifndef SETPIN_GPIO45_H
#define SETPIN_GPIO45_H

#define GPIO_NUM_STRAP45    GPIO_NUM_45
#define PIN_STRAP45_NUM     45

#endif /* SETPIN_GPIO45_H */
