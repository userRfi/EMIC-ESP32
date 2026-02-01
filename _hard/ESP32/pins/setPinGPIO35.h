/*****************************************************************************
 * @file    setPinGPIO35.h
 * @brief   GPIO35 Pin Configuration for ESP32
 * @details GPIO35 - INPUT ONLY, ADC1_CH7
 *          Cannot be used as output, no internal pull-up/down
 *****************************************************************************/
#ifndef SETPIN_GPIO35_H
#define SETPIN_GPIO35_H

#define GPIO_NUM_.{name}.    GPIO_NUM_35
#define PIN_.{name}._NUM     35
#define PIN_.{name}._ADC     ADC1_CHANNEL_7
#define PIN_.{name}._INPUT_ONLY 1

#endif /* SETPIN_GPIO35_H */
