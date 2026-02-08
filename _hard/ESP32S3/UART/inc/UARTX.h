/*****************************************************************************
 * @file    UART.{port}..h
 * @brief   ESP32-S3 UART.{port}. Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details UART.{port}. interface for ESP32-S3.
 *          Default baud: .{baud}., Buffer size: .{BufferSize}. bytes
 *****************************************************************************/

#ifndef ESP32S3_UART.{port}._H
#define ESP32S3_UART.{port}._H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/*============================================================================
 * Configuration
 *============================================================================*/

#define UART.{port}._PORT           .{port}.
#define UART.{port}._BAUD           .{baud}.
#define UART.{port}._BUFFER_SIZE    .{BufferSize}.
#define UART.{port}._TIMEOUT_MS     1000

/*============================================================================
 * Initialization
 *============================================================================*/

void UART.{port}._init(void);
void UART.{port}._deinit(void);

/*============================================================================
 * Configuration Functions
 *============================================================================*/

void UART.{port}._bd(uint32_t baudRate);
void UART.{port}._setPins(int txPin, int rxPin);

/*============================================================================
 * Write Functions
 *============================================================================*/

int UART.{port}._write(const char* data, size_t len);
void UART.{port}._putChar(char c);
void UART.{port}._print(const char* str);
void UART.{port}._println(const char* str);
void UART.{port}._flush(void);

/*============================================================================
 * Read Functions
 *============================================================================*/

int UART.{port}._read(char* buffer, size_t len);
int UART.{port}._readTimeout(char* buffer, size_t len, uint32_t timeout_ms);
int UART.{port}._getChar(void);
int UART.{port}._available(void);
int UART.{port}._readLine(char* buffer, size_t maxLen);
void UART.{port}._clearRx(void);

/*============================================================================
 * Status Functions
 *============================================================================*/

bool UART.{port}._txComplete(void);
bool UART.{port}._hasData(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32S3_UART.{port}._H */
