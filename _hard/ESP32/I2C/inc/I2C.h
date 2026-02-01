/*****************************************************************************
 * @file    I2C.{port}..h
 * @brief   ESP32 I2C.{port}. Master Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details I2C Master interface for I2C.{port}.
 *          Default speed: .{speed}. Hz
 *****************************************************************************/

#ifndef ESP32_I2C.{port}._H
#define ESP32_I2C.{port}._H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define I2C.{port}._PORT           I2C_NUM_.{port}.
#define I2C.{port}._SPEED          .{speed}.

/* I2C speed presets */
#define I2C_SPEED_STANDARD      100000      /* 100 kHz */
#define I2C_SPEED_FAST          400000      /* 400 kHz */
#define I2C_SPEED_FAST_PLUS     1000000     /* 1 MHz */

/* Default pin configuration */
#if .{port}. == 0
    #define I2C.{port}._SDA_DEFAULT    21
    #define I2C.{port}._SCL_DEFAULT    22
#elif .{port}. == 1
    #define I2C.{port}._SDA_DEFAULT    25
    #define I2C.{port}._SCL_DEFAULT    26
#endif

/* Timeout in milliseconds */
#define I2C.{port}._TIMEOUT_MS     1000

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize I2C.{port}. with default speed
 */
void I2C.{port}._init(void);

/**
 * @brief Initialize I2C.{port}. with specified speed
 *
 * @param clockSpeed Clock speed in Hz
 */
void I2C.{port}._initSpeed(uint32_t clockSpeed);

/**
 * @brief Initialize I2C.{port}. with custom configuration
 *
 * @param sdaPin SDA GPIO number (-1 for default)
 * @param sclPin SCL GPIO number (-1 for default)
 * @param clockSpeed Clock speed in Hz
 */
void I2C.{port}._initEx(int sdaPin, int sclPin, uint32_t clockSpeed);

/**
 * @brief Deinitialize I2C.{port}.
 */
void I2C.{port}._deinit(void);

/*============================================================================
 * Basic Transfer Functions
 *============================================================================*/

/**
 * @brief Write data to I2C device
 *
 * @param addr 7-bit device address
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return true if successful
 */
bool I2C.{port}._write(uint8_t addr, const uint8_t* data, size_t len);

/**
 * @brief Read data from I2C device
 *
 * @param addr 7-bit device address
 * @param buffer Pointer to receive buffer
 * @param len Number of bytes to read
 * @return true if successful
 */
bool I2C.{port}._read(uint8_t addr, uint8_t* buffer, size_t len);

/**
 * @brief Write then read (combined transaction)
 *
 * @param addr 7-bit device address
 * @param txData Pointer to data to write
 * @param txLen Number of bytes to write
 * @param rxBuffer Pointer to receive buffer
 * @param rxLen Number of bytes to read
 * @return true if successful
 */
bool I2C.{port}._writeRead(uint8_t addr, const uint8_t* txData, size_t txLen,
                            uint8_t* rxBuffer, size_t rxLen);

/*============================================================================
 * Register Access Functions
 *============================================================================*/

/**
 * @brief Write a single byte to a register
 *
 * @param addr 7-bit device address
 * @param reg Register address
 * @param val Value to write
 * @return true if successful
 */
bool I2C.{port}._writeRegister(uint8_t addr, uint8_t reg, uint8_t val);

/**
 * @brief Read a single byte from a register
 *
 * @param addr 7-bit device address
 * @param reg Register address
 * @param val Pointer to store value
 * @return true if successful
 */
bool I2C.{port}._readRegister(uint8_t addr, uint8_t reg, uint8_t* val);

/**
 * @brief Write multiple bytes starting at a register
 *
 * @param addr 7-bit device address
 * @param reg Starting register address
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return true if successful
 */
bool I2C.{port}._writeRegisters(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len);

/**
 * @brief Read multiple bytes starting at a register
 *
 * @param addr 7-bit device address
 * @param reg Starting register address
 * @param buffer Pointer to receive buffer
 * @param len Number of bytes to read
 * @return true if successful
 */
bool I2C.{port}._readRegisters(uint8_t addr, uint8_t reg, uint8_t* buffer, size_t len);

/**
 * @brief Write a 16-bit value to a register (MSB first)
 *
 * @param addr 7-bit device address
 * @param reg Register address
 * @param val 16-bit value to write
 * @return true if successful
 */
bool I2C.{port}._writeRegister16(uint8_t addr, uint8_t reg, uint16_t val);

/**
 * @brief Read a 16-bit value from a register (MSB first)
 *
 * @param addr 7-bit device address
 * @param reg Register address
 * @param val Pointer to store 16-bit value
 * @return true if successful
 */
bool I2C.{port}._readRegister16(uint8_t addr, uint8_t reg, uint16_t* val);

/*============================================================================
 * Utility Functions
 *============================================================================*/

/**
 * @brief Scan I2C bus for devices
 *
 * @param foundAddrs Array to store found addresses (7-bit)
 * @param maxDevices Maximum number of devices to find
 * @return Number of devices found
 */
int I2C.{port}._scan(uint8_t* foundAddrs, int maxDevices);

/**
 * @brief Check if a device is present at address
 *
 * @param addr 7-bit device address
 * @return true if device responds
 */
bool I2C.{port}._devicePresent(uint8_t addr);

/**
 * @brief Set I2C clock speed
 *
 * @param clockSpeed New clock speed in Hz
 */
void I2C.{port}._setSpeed(uint32_t clockSpeed);

/**
 * @brief Reset I2C bus (useful for recovery from stuck state)
 */
void I2C.{port}._reset(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_I2C.{port}._H */
