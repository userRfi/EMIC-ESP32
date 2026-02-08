/*****************************************************************************
 * @file    SPI.{host}._Slave.h
 * @brief   ESP32-S2 SPI.{host}. Slave Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *****************************************************************************/

#ifndef ESP32S2_SPI.{host}._SLAVE_H
#define ESP32S2_SPI.{host}._SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SPI.{host}._SLAVE_HOST     SPI.{host}._HOST

#if .{host}. == 2
    #define SPI.{host}._SLAVE_MOSI_DEFAULT    35
    #define SPI.{host}._SLAVE_MISO_DEFAULT    37
    #define SPI.{host}._SLAVE_CLK_DEFAULT     36
    #define SPI.{host}._SLAVE_CS_DEFAULT      34
#elif .{host}. == 3
    #define SPI.{host}._SLAVE_MOSI_DEFAULT    11
    #define SPI.{host}._SLAVE_MISO_DEFAULT    13
    #define SPI.{host}._SLAVE_CLK_DEFAULT     12
    #define SPI.{host}._SLAVE_CS_DEFAULT      10
#endif

#define SPI.{host}._SLAVE_BUFFER_SIZE    128

typedef void (*SPI.{host}._SlaveCallback_t)(const uint8_t* rxData, size_t len);

void SPI.{host}._Slave_init(void);
void SPI.{host}._Slave_initEx(int mosiPin, int misoPin, int clkPin, int csPin);
void SPI.{host}._Slave_deinit(void);
bool SPI.{host}._Slave_queueTx(const uint8_t* txData, size_t len);
int SPI.{host}._Slave_receive(uint8_t* rxData, size_t maxLen, uint32_t timeout_ms);
bool SPI.{host}._Slave_hasData(void);
int SPI.{host}._Slave_available(void);
void SPI.{host}._Slave_setCallback(SPI.{host}._SlaveCallback_t callback);
void SPI.{host}._Slave_setMode(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S2_SPI.{host}._SLAVE_H */
