/*****************************************************************************
 * @file    SPI.{host}._Slave.h
 * @brief   ESP32-C3 SPI.{host}. Slave Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details SPI Slave interface for ESP32-C3 SPI2
 *          ESP32-C3 only has SPI2 available for general use.
 *****************************************************************************/

#ifndef ESP32C3_SPI.{host}._SLAVE_H
#define ESP32C3_SPI.{host}._SLAVE_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define SPI.{host}._SLAVE_HOST     SPI.{host}._HOST

/* ESP32-C3 SPI2 default pins */
#define SPI.{host}._SLAVE_MOSI_DEFAULT    7
#define SPI.{host}._SLAVE_MISO_DEFAULT    2
#define SPI.{host}._SLAVE_CLK_DEFAULT     6
#define SPI.{host}._SLAVE_CS_DEFAULT      10

/* Buffer size for slave transactions */
#define SPI.{host}._SLAVE_BUFFER_SIZE    128

/*============================================================================
 * Callback Types
 *============================================================================*/

/**
 * @brief Callback for transaction complete
 *
 * @param rxData Pointer to received data
 * @param len Number of bytes received
 */
typedef void (*SPI.{host}._SlaveCallback_t)(const uint8_t* rxData, size_t len);

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize SPI.{host}. as Slave
 *
 * Configures SPI with default pins.
 */
void SPI.{host}._Slave_init(void);

/**
 * @brief Initialize SPI.{host}. Slave with custom configuration
 *
 * @param mosiPin MOSI GPIO number (-1 for default)
 * @param misoPin MISO GPIO number (-1 for default)
 * @param clkPin CLK GPIO number (-1 for default)
 * @param csPin CS GPIO number (-1 for default)
 */
void SPI.{host}._Slave_initEx(int mosiPin, int misoPin, int clkPin, int csPin);

/**
 * @brief Deinitialize SPI.{host}. Slave
 */
void SPI.{host}._Slave_deinit(void);

/*============================================================================
 * Transaction Functions
 *============================================================================*/

/**
 * @brief Queue data to transmit when master initiates transfer
 *
 * @param txData Pointer to data to transmit
 * @param len Number of bytes to queue
 * @return true if successfully queued
 */
bool SPI.{host}._Slave_queueTx(const uint8_t* txData, size_t len);

/**
 * @brief Wait for a transaction from master
 *
 * @param rxData Pointer to buffer for received data
 * @param maxLen Maximum bytes to receive
 * @param timeout_ms Timeout in milliseconds (0 = no wait)
 * @return Number of bytes received, or -1 on timeout
 */
int SPI.{host}._Slave_receive(uint8_t* rxData, size_t maxLen, uint32_t timeout_ms);

/**
 * @brief Check if a transaction is pending
 *
 * @return true if transaction data available
 */
bool SPI.{host}._Slave_hasData(void);

/**
 * @brief Get number of bytes available from last transaction
 *
 * @return Number of bytes available
 */
int SPI.{host}._Slave_available(void);

/*============================================================================
 * Callback Registration
 *============================================================================*/

/**
 * @brief Set callback for transaction complete
 *
 * @param callback Function to call when transaction completes
 */
void SPI.{host}._Slave_setCallback(SPI.{host}._SlaveCallback_t callback);

/*============================================================================
 * Configuration
 *============================================================================*/

/**
 * @brief Set SPI mode
 *
 * @param mode SPI mode (0-3)
 */
void SPI.{host}._Slave_setMode(uint8_t mode);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_SPI.{host}._SLAVE_H */
