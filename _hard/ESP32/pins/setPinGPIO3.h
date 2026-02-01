/*****************************************************************************
 * @file    setPinGPIO3.h
 * @brief   GPIO3 Pin Configuration for ESP32
 * @details GPIO3 - UART0 RX (default), used for serial programming
 *          Avoid using if USB-serial is needed
 *****************************************************************************/
#ifndef SETPIN_GPIO3_H
#define SETPIN_GPIO3_H

#define GPIO_NUM_.{name}.    GPIO_NUM_3
#define PIN_.{name}._NUM     3
#define PIN_.{name}._UART    UART_NUM_0
#define PIN_.{name}._UART_RX 1

#endif /* SETPIN_GPIO3_H */
