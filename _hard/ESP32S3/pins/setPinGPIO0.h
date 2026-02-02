/*****************************************************************************
 * @file    setPinGPIO0.h
 * @brief   GPIO0 Pin Configuration for ESP32-S3
 * @details Strapping pin - controls boot mode. Active-low for download boot.
 *          Has RTC function for deep sleep wakeup.
 *****************************************************************************/
#ifndef SETPIN_GPIO0_H
#define SETPIN_GPIO0_H

#define GPIO_NUM_BOOT       GPIO_NUM_0
#define PIN_BOOT_NUM        0

#endif /* SETPIN_GPIO0_H */
