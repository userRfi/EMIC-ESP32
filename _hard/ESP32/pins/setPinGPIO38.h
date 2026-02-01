/*****************************************************************************
 * @file    setPinGPIO38.h
 * @brief   GPIO38 Pin Configuration for ESP32
 * @details GPIO38 - INPUT ONLY, ADC1_CH2
 *          Cannot be used as output, no internal pull-up/down
 *          Not exposed on most modules
 *****************************************************************************/
#ifndef SETPIN_GPIO38_H
#define SETPIN_GPIO38_H

#define GPIO_NUM_.{name}.    GPIO_NUM_38
#define PIN_.{name}._NUM     38
#define PIN_.{name}._ADC     ADC1_CHANNEL_2
#define PIN_.{name}._INPUT_ONLY 1

#endif /* SETPIN_GPIO38_H */
