/*****************************************************************************
 * @file    SPI.{host}..h
 * @brief   ESP32-S3 SPI.{host}. Master Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details SPI Master interface for ESP32-S3 SPI.{host}. (SPI2=2, SPI3=3)
 *          Clock speed: .{speed}. Hz
 *****************************************************************************/

#ifndef ESP32S3_SPI.{host}._MASTER_H
#define ESP32S3_SPI.{host}._MASTER_H

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

/* Default pin configuration for ESP32-S3 */
#if .{host}. == 2
    /* SPI2 defaults */
    #define SPI.{host}._MOSI_DEFAULT    11
    #define SPI.{host}._MISO_DEFAULT    13
    #define SPI.{host}._CLK_DEFAULT     12
    #define SPI.{host}._CS_DEFAULT      10
#elif .{host}. == 3
    /* SPI3 defaults */
    #define SPI.{host}._MOSI_DEFAULT    35
    #define SPI.{host}._MISO_DEFAULT    37
    #define SPI.{host}._CLK_DEFAULT     36
    #define SPI.{host}._CS_DEFAULT      34
#endif

/* SPI modes */
#define SPI_MODE0   0   /* CPOL=0, CPHA=0 */
#define SPI_MODE1   1   /* CPOL=0, CPHA=1 */
#define SPI_MODE2   2   /* CPOL=1, CPHA=0 */
#define SPI_MODE3   3   /* CPOL=1, CPHA=1 */

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize SPI.{host}. as Master
 *
 * Configures SPI with default pins and clock speed.
 */
void SPI.{host}._init(void);

/**
 * @brief Initialize SPI.{host}. with custom configuration
 *
 * @param mosiPin MOSI GPIO number (-1 for default)
 * @param misoPin MISO GPIO number (-1 for default)
 * @param clkPin CLK GPIO number (-1 for default)
 * @param clockSpeed Clock speed in Hz
 */
void SPI.{host}._initEx(int mosiPin, int misoPin, int clkPin, uint32_t clockSpeed);

/**
 * @brief Deinitialize SPI.{host}.
 */
void SPI.{host}._deinit(void);

/*============================================================================
 * Device Management
 *============================================================================*/

/**
 * @brief Add a device to the SPI bus
 *
 * @param csPin Chip select GPIO number
 * @param mode SPI mode (0-3)
 * @return Device handle (>= 0) or -1 on error
 */
int SPI.{host}._addDevice(int csPin, uint8_t mode);

/**
 * @brief Remove a device from the SPI bus
 *
 * @param deviceHandle Handle returned by addDevice
 */
void SPI.{host}._removeDevice(int deviceHandle);

/**
 * @brief Select the active device for transfers
 *
 * @param deviceHandle Handle returned by addDevice
 */
void SPI.{host}._selectDevice(int deviceHandle);

/*============================================================================
 * Transfer Functions
 *============================================================================*/

/**
 * @brief Full-duplex SPI transfer
 *
 * @param txData Pointer to transmit data (NULL for read-only)
 * @param rxData Pointer to receive buffer (NULL for write-only)
 * @param len Number of bytes to transfer
 */
void SPI.{host}._transfer(const uint8_t* txData, uint8_t* rxData, size_t len);

/**
 * @brief Transfer a single byte (full-duplex)
 *
 * @param txByte Byte to transmit
 * @return Byte received
 */
uint8_t SPI.{host}._transferByte(uint8_t txByte);

/**
 * @brief Write data to SPI (ignore received data)
 *
 * @param data Pointer to transmit data
 * @param len Number of bytes to write
 */
void SPI.{host}._write(const uint8_t* data, size_t len);

/**
 * @brief Write a single byte
 *
 * @param data Byte to write
 */
void SPI.{host}._writeByte(uint8_t data);

/**
 * @brief Write a 16-bit value (MSB first)
 *
 * @param data 16-bit value to write
 */
void SPI.{host}._writeWord(uint16_t data);

/**
 * @brief Read data from SPI (transmit zeros)
 *
 * @param data Pointer to receive buffer
 * @param len Number of bytes to read
 */
void SPI.{host}._read(uint8_t* data, size_t len);

/**
 * @brief Read a single byte
 *
 * @return Byte read
 */
uint8_t SPI.{host}._readByte(void);

/*============================================================================
 * Chip Select Control
 *============================================================================*/

/**
 * @brief Assert chip select (pull low)
 */
void SPI.{host}._csLow(void);

/**
 * @brief Deassert chip select (pull high)
 */
void SPI.{host}._csHigh(void);

/*============================================================================
 * Configuration Functions
 *============================================================================*/

/**
 * @brief Set SPI clock speed
 *
 * @param clockSpeed Clock speed in Hz
 */
void SPI.{host}._setSpeed(uint32_t clockSpeed);

/**
 * @brief Set SPI mode
 *
 * @param mode SPI mode (0-3)
 */
void SPI.{host}._setMode(uint8_t mode);

/**
 * @brief Set bit order
 *
 * @param msbFirst true for MSB first, false for LSB first
 */
void SPI.{host}._setBitOrder(bool msbFirst);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_SPI.{host}._MASTER_H */
