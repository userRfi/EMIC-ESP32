/*****************************************************************************
 * @file    SPI.{host}..h
 * @brief   ESP32-S2 SPI.{host}. Master Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details SPI Master interface for ESP32-S2 SPI.{host}. (SPI2=2, SPI3=3)
 *          Clock speed: .{speed}. Hz
 *****************************************************************************/

#ifndef ESP32S2_SPI.{host}._MASTER_H
#define ESP32S2_SPI.{host}._MASTER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define SPI.{host}._HOST           SPI.{host}._HOST
#define SPI.{host}._CLOCK_SPEED    .{speed}.

/* Default pin configuration for ESP32-S2 */
#if .{host}. == 2
    /* SPI2 defaults */
    #define SPI.{host}._MOSI_DEFAULT    35
    #define SPI.{host}._MISO_DEFAULT    37
    #define SPI.{host}._CLK_DEFAULT     36
    #define SPI.{host}._CS_DEFAULT      34
#elif .{host}. == 3
    /* SPI3 defaults */
    #define SPI.{host}._MOSI_DEFAULT    11
    #define SPI.{host}._MISO_DEFAULT    13
    #define SPI.{host}._CLK_DEFAULT     12
    #define SPI.{host}._CS_DEFAULT      10
#endif

/* SPI modes */
#define SPI_MODE0   0
#define SPI_MODE1   1
#define SPI_MODE2   2
#define SPI_MODE3   3

/*============================================================================
 * Functions
 *============================================================================*/

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

#endif /* ESP32S2_SPI.{host}._MASTER_H */
