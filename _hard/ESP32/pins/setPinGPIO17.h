/*****************************************************************************
 * @file    setPinGPIO17.h
 * @brief   GPIO17 Pin Configuration for ESP32
 * @details GPIO17 - General purpose, UART2 TX default
 *          Not available on ESP32-WROVER (used for PSRAM)
 *****************************************************************************/
#ifndef SETPIN_GPIO17_H
#define SETPIN_GPIO17_H

#define GPIO_NUM_.{name}.    GPIO_NUM_17
#define PIN_.{name}._NUM     17
#define PIN_.{name}._UART    UART_NUM_2
#define PIN_.{name}._UART_TX 1

#endif /* SETPIN_GPIO17_H */
