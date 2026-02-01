/*****************************************************************************
 * @file    UART.{port}..h
 * @brief   ESP32 UART.{port}. Header for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details UART.{port}. interface with configurable buffer size and baud rate.
 *          Default baud: .{baud}., Buffer size: .{BufferSize}. bytes
 *****************************************************************************/

#ifndef ESP32_UART.{port}._H
#define ESP32_UART.{port}._H

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

/* Default timeout in milliseconds */
#define UART.{port}._TIMEOUT_MS     1000

/*============================================================================
 * Initialization
 *============================================================================*/

/**
 * @brief Initialize UART.{port}.
 *
 * Configures the UART with:
 * - Baud rate: .{baud}.
 * - Data bits: 8
 * - Parity: None
 * - Stop bits: 1
 * - Flow control: None
 */
void UART.{port}._init(void);

/**
 * @brief Deinitialize UART.{port}.
 *
 * Releases the UART driver resources.
 */
void UART.{port}._deinit(void);

/*============================================================================
 * Configuration Functions
 *============================================================================*/

/**
 * @brief Set UART baud rate
 *
 * @param baudRate New baud rate (e.g., 9600, 115200, 921600)
 */
void UART.{port}._bd(uint32_t baudRate);

/**
 * @brief Configure UART pins
 *
 * @param txPin GPIO number for TX (or -1 to keep default)
 * @param rxPin GPIO number for RX (or -1 to keep default)
 */
void UART.{port}._setPins(int txPin, int rxPin);

/*============================================================================
 * Write Functions
 *============================================================================*/

/**
 * @brief Write data to UART
 *
 * @param data Pointer to data buffer
 * @param len Number of bytes to write
 * @return Number of bytes written
 */
int UART.{port}._write(const char* data, size_t len);

/**
 * @brief Write a single character
 *
 * @param c Character to write
 */
void UART.{port}._putChar(char c);

/**
 * @brief Write a null-terminated string
 *
 * @param str String to write
 */
void UART.{port}._print(const char* str);

/**
 * @brief Write a string with newline
 *
 * @param str String to write
 */
void UART.{port}._println(const char* str);

/**
 * @brief Flush TX buffer (wait for transmission complete)
 */
void UART.{port}._flush(void);

/*============================================================================
 * Read Functions
 *============================================================================*/

/**
 * @brief Read data from UART
 *
 * @param buffer Pointer to receive buffer
 * @param len Maximum bytes to read
 * @return Number of bytes read
 */
int UART.{port}._read(char* buffer, size_t len);

/**
 * @brief Read data with timeout
 *
 * @param buffer Pointer to receive buffer
 * @param len Maximum bytes to read
 * @param timeout_ms Timeout in milliseconds
 * @return Number of bytes read
 */
int UART.{port}._readTimeout(char* buffer, size_t len, uint32_t timeout_ms);

/**
 * @brief Read a single character (blocking with timeout)
 *
 * @return Character read or -1 on timeout
 */
int UART.{port}._getChar(void);

/**
 * @brief Get number of bytes available in RX buffer
 *
 * @return Number of bytes available
 */
int UART.{port}._available(void);

/**
 * @brief Read a line (until newline or buffer full)
 *
 * @param buffer Buffer to store the line
 * @param maxLen Maximum length of line
 * @return Length of line read (excluding newline)
 */
int UART.{port}._readLine(char* buffer, size_t maxLen);

/**
 * @brief Clear RX buffer
 */
void UART.{port}._clearRx(void);

/*============================================================================
 * Status Functions
 *============================================================================*/

/**
 * @brief Check if TX is complete (buffer empty)
 *
 * @return true if TX complete
 */
bool UART.{port}._txComplete(void);

/**
 * @brief Check if data is available for reading
 *
 * @return true if data available
 */
bool UART.{port}._hasData(void);

#ifdef __cplusplus
}
#endif

#endif /* ESP32_UART.{port}._H */
