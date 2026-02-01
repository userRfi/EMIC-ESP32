/*****************************************************************************
 * @file    setPinGPIO36.h
 * @brief   GPIO36 Pin Configuration for ESP32
 * @details GPIO36 - INPUT ONLY, ADC1_CH0, SENSOR_VP
 *          Cannot be used as output, no internal pull-up/down
 *          Often used for analog sensor input
 *****************************************************************************/
#ifndef SETPIN_GPIO36_H
#define SETPIN_GPIO36_H

#define GPIO_NUM_.{name}.    GPIO_NUM_36
#define PIN_.{name}._NUM     36
#define PIN_.{name}._ADC     ADC1_CHANNEL_0
#define PIN_.{name}._INPUT_ONLY 1

#endif /* SETPIN_GPIO36_H */
