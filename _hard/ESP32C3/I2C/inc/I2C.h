/*****************************************************************************
 * @file    I2C.{port}..h
 * @brief   ESP32-C3 I2C.{port}. Master Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details I2C Master interface for ESP32-C3.
 *          ESP32-C3 has only I2C0 available.
 *          Default speed: .{speed}. Hz
 *****************************************************************************/

#ifndef ESP32C3_I2C.{port}._H
#define ESP32C3_I2C.{port}._H

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
#define I2C_SPEED_STANDARD      100000
#define I2C_SPEED_FAST          400000
#define I2C_SPEED_FAST_PLUS     1000000

/* ESP32-C3 Default I2C0 pins */
#define I2C.{port}._SDA_DEFAULT    8
#define I2C.{port}._SCL_DEFAULT    9

/* Timeout in milliseconds */
#define I2C.{port}._TIMEOUT_MS     1000

/*============================================================================
 * Initialization
 *============================================================================*/

void I2C.{port}._init(void);
void I2C.{port}._initSpeed(uint32_t clockSpeed);
void I2C.{port}._initEx(int sdaPin, int sclPin, uint32_t clockSpeed);
void I2C.{port}._deinit(void);

/*============================================================================
 * Basic Transfer Functions
 *============================================================================*/

bool I2C.{port}._write(uint8_t addr, const uint8_t* data, size_t len);
bool I2C.{port}._read(uint8_t addr, uint8_t* buffer, size_t len);
bool I2C.{port}._writeRead(uint8_t addr, const uint8_t* txData, size_t txLen,
                            uint8_t* rxBuffer, size_t rxLen);

/*============================================================================
 * Register Access Functions
 *============================================================================*/

bool I2C.{port}._writeRegister(uint8_t addr, uint8_t reg, uint8_t val);
bool I2C.{port}._readRegister(uint8_t addr, uint8_t reg, uint8_t* val);
bool I2C.{port}._writeRegisters(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len);
bool I2C.{port}._readRegisters(uint8_t addr, uint8_t reg, uint8_t* buffer, size_t len);
bool I2C.{port}._writeRegister16(uint8_t addr, uint8_t reg, uint16_t val);
bool I2C.{port}._readRegister16(uint8_t addr, uint8_t reg, uint16_t* val);

/*============================================================================
 * Utility Functions
 *============================================================================*/

int I2C.{port}._scan(uint8_t* foundAddrs, int maxDevices);
bool I2C.{port}._devicePresent(uint8_t addr);
void I2C.{port}._setSpeed(uint32_t clockSpeed);
void I2C.{port}._reset(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32C3_I2C.{port}._H */
