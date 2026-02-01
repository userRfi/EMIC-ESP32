/*****************************************************************************
 * @file    setPinGPIO37.h
 * @brief   GPIO37 Pin Configuration for ESP32
 * @details GPIO37 - INPUT ONLY, ADC1_CH1
 *          Cannot be used as output, no internal pull-up/down
 *          Not exposed on most modules
 *****************************************************************************/
#ifndef SETPIN_GPIO37_H
#define SETPIN_GPIO37_H

#define GPIO_NUM_.{name}.    GPIO_NUM_37
#define PIN_.{name}._NUM     37
#define PIN_.{name}._ADC     ADC1_CHANNEL_1
#define PIN_.{name}._INPUT_ONLY 1

#endif /* SETPIN_GPIO37_H */
