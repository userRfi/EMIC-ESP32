/*****************************************************************************
 * @file    setPinGPIO40.h
 * @brief   GPIO40 Pin Configuration for ESP32-S2
 * @details General purpose I/O pin. JTAG MTDO if JTAG enabled.
 *          Available for user applications when JTAG disabled.
 *          Default I2C1 SDA pin.
 *****************************************************************************/
#ifndef SETPIN_GPIO40_H
#define SETPIN_GPIO40_H

#define GPIO_NUM_IO40           GPIO_NUM_40
#define PIN_IO40_NUM            40
#define PIN_MTDO                GPIO_NUM_40
#define PIN_I2C1_SDA_DEFAULT    GPIO_NUM_40

#endif /* SETPIN_GPIO40_H */
