/*****************************************************************************
 * @file    setPinGPIO41.h
 * @brief   GPIO41 Pin Configuration for ESP32-S2
 * @details General purpose I/O pin. JTAG MTDI if JTAG enabled.
 *          Available for user applications when JTAG disabled.
 *          Default I2C1 SCL pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO41_H
#define SETPIN_GPIO41_H

#define GPIO_NUM_IO41           GPIO_NUM_41
#define PIN_IO41_NUM            41
#define PIN_MTDI                GPIO_NUM_41
#define PIN_I2C1_SCL_DEFAULT    GPIO_NUM_41

#endif /* SETPIN_GPIO41_H */
