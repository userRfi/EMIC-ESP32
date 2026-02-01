/*****************************************************************************
 * @file    setPinGPIO1.h
 * @brief   GPIO1 Pin Configuration for ESP32
 * @details GPIO1 - UART0 TX (default), used for serial programming
 *          Avoid using if USB-serial is needed
 *****************************************************************************/
#ifndef SETPIN_GPIO1_H
#define SETPIN_GPIO1_H

#define GPIO_NUM_.{name}.    GPIO_NUM_1
#define PIN_.{name}._NUM     1
#define PIN_.{name}._UART    UART_NUM_0
#define PIN_.{name}._UART_TX 1

#endif /* SETPIN_GPIO1_H */
