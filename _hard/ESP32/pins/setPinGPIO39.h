/*****************************************************************************
 * @file    setPinGPIO39.h
 * @brief   GPIO39 Pin Configuration for ESP32
 * @details GPIO39 - INPUT ONLY, ADC1_CH3, SENSOR_VN
 *          Cannot be used as output, no internal pull-up/down
 *          Often used for analog sensor input
 *****************************************************************************/
#ifndef SETPIN_GPIO39_H
#define SETPIN_GPIO39_H

#define GPIO_NUM_.{name}.    GPIO_NUM_39
#define PIN_.{name}._NUM     39
#define PIN_.{name}._ADC     ADC1_CHANNEL_3
#define PIN_.{name}._INPUT_ONLY 1

#endif /* SETPIN_GPIO39_H */
