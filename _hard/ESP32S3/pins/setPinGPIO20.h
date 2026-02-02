/*****************************************************************************
 * @file    setPinGPIO20.h
 * @brief   GPIO20 Pin Configuration for ESP32-S3
 * @details USB D+ pin. Used for USB OTG data plus signal.
 *          Also has ADC2 channel 9 capability.
 *          Note: Reserved for USB on most designs.
 *****************************************************************************/
#ifndef SETPIN_GPIO20_H
#define SETPIN_GPIO20_H

#define GPIO_NUM_USB_DP     GPIO_NUM_20
#define PIN_USB_DP_NUM      20
#define PIN_USB_DP_ADC      ADC2_CHANNEL_9

#endif /* SETPIN_GPIO20_H */
