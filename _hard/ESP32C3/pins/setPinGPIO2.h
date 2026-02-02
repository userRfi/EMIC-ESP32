/*****************************************************************************
 * @file    setPinGPIO2.h
 * @brief   GPIO2 Pin Configuration for ESP32-C3
 * @details GPIO2 - ADC1_CH2, Strapping pin (boot mode)
 *          HIGH = SPI boot, LOW = Download boot
 *****************************************************************************/
#ifndef SETPIN_GPIO2_H
#define SETPIN_GPIO2_H

#define GPIO_NUM_.{name}.    GPIO_NUM_2
#define PIN_.{name}._NUM     2
#define PIN_.{name}._ADC     ADC1_CHANNEL_2
#define PIN_.{name}._STRAP   1

#endif /* SETPIN_GPIO2_H */
