/*****************************************************************************
 * @file    setPinGPIO19.h
 * @brief   GPIO19 Pin Configuration for ESP32-S2
 * @details USB D- pin. Used for USB OTG data minus signal.
 *          Also has ADC2 channel 8 capability.
 *          Note: Reserved for USB on most designs.
 *****************************************************************************/
#ifndef SETPIN_GPIO19_H
#define SETPIN_GPIO19_H

#define GPIO_NUM_USB_DN     GPIO_NUM_19
#define PIN_USB_DN_NUM      19
#define PIN_USB_DN_ADC      ADC2_CHANNEL_8

#endif /* SETPIN_GPIO19_H */
