/*****************************************************************************
 * @file    SPI.{host}..h
 * @brief   ESP32-C6 SPI.{host}. Master Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details SPI Master interface for ESP32-C6 SPI2
 *          ESP32-C6 only has SPI2 available for general use.
 *          Clock speed: .{speed}. Hz
 *****************************************************************************/

#ifndef ESP32C6_SPI.{host}._MASTER_H
#define ESP32C6_SPI.{host}._MASTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define SPI.{host}._HOST           SPI.{host}._HOST
#define SPI.{host}._CLOCK_SPEED    .{speed}.

/* ESP32-C6 SPI2 default pins */
#define SPI.{host}._MOSI_DEFAULT    7
#define SPI.{host}._MISO_DEFAULT    2
#define SPI.{host}._CLK_DEFAULT     6
#define SPI.{host}._CS_DEFAULT      10

#define SPI_MODE0   0
#define SPI_MODE1   1
#define SPI_MODE2   2
#define SPI_MODE3   3

void SPI.{host}._init(void);
void SPI.{host}._initEx(int mosiPin, int misoPin, int clkPin, uint32_t clockSpeed);
void SPI.{host}._deinit(void);

int SPI.{host}._addDevice(int csPin, uint8_t mode);
void SPI.{host}._removeDevice(int deviceHandle);
void SPI.{host}._selectDevice(int deviceHandle);

void SPI.{host}._transfer(const uint8_t* txData, uint8_t* rxData, size_t len);
uint8_t SPI.{host}._transferByte(uint8_t txByte);
void SPI.{host}._write(const uint8_t* data, size_t len);
void SPI.{host}._writeByte(uint8_t data);
void SPI.{host}._writeWord(uint16_t data);
void SPI.{host}._read(uint8_t* data, size_t len);
uint8_t SPI.{host}._readByte(void);

void SPI.{host}._csLow(void);
void SPI.{host}._csHigh(void);

void SPI.{host}._setSpeed(uint32_t clockSpeed);
void SPI.{host}._setMode(uint8_t mode);
void SPI.{host}._setBitOrder(bool msbFirst);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C6_SPI.{host}._MASTER_H */
