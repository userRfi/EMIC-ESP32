/*****************************************************************************
 * @file    setPinGPIO16.h
 * @brief   GPIO16 Pin Configuration for ESP32
 * @details GPIO16 - General purpose, UART2 RX default
 *          Not available on ESP32-WROVER (used for PSRAM)
 *****************************************************************************/
#ifndef SETPIN_GPIO16_H
#define SETPIN_GPIO16_H

#define GPIO_NUM_.{name}.    GPIO_NUM_16
#define PIN_.{name}._NUM     16
#define PIN_.{name}._UART    UART_NUM_2
#define PIN_.{name}._UART_RX 1

#endif /* SETPIN_GPIO16_H */
