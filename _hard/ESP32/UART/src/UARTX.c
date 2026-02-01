/*****************************************************************************
 * @file    UART.{port}..c
 * @brief   ESP32 UART.{port}. Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details UART implementation using ESP-IDF uart_driver.
 *****************************************************************************/

#include "UART.{port}..h"

/* ESP-IDF includes */
#include "driver/uart.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "UART.{port}.";

/*============================================================================
 * Default Pin Configuration
 *============================================================================*/

#if .{port}. == 0
    #define UART_TX_PIN     1
    #define UART_RX_PIN     3
#elif .{port}. == 1
    #define UART_TX_PIN     10
    #define UART_RX_PIN     9
#elif .{port}. == 2
    #define UART_TX_PIN     17
    #define UART_RX_PIN     16
#else
    #error "Invalid UART port"
#endif

/* Current pin configuration */
static int uart_tx_pin = UART_TX_PIN;
static int uart_rx_pin = UART_RX_PIN;

/* Initialization flag */
static bool uart_initialized = false;

/*============================================================================
 * Initialization
 *============================================================================*/

void UART.{port}._init(void)
{
    if (uart_initialized) {
        return; /* Already initialized */
    }

    uart_config_t uart_config = {
        .baud_rate = UART.{port}._BAUD,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    /* Configure UART parameters */
    ESP_ERROR_CHECK(uart_param_config(UART_NUM_.{port}., &uart_config));

    /* Set UART pins */
    ESP_ERROR_CHECK(uart_set_pin(UART_NUM_.{port}.,
                                  uart_tx_pin,
                                  uart_rx_pin,
                                  UART_PIN_NO_CHANGE,
                                  UART_PIN_NO_CHANGE));

    /* Install UART driver with RX buffer */
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM_.{port}.,
                                         UART.{port}._BUFFER_SIZE * 2,
                                         UART.{port}._BUFFER_SIZE,
                                         0,
                                         NULL,
                                         0));

    uart_initialized = true;
    ESP_LOGI(TAG, "UART.{port}. initialized at %d baud", UART.{port}._BAUD);
}

void UART.{port}._deinit(void)
{
    if (!uart_initialized) {
        return;
    }

    uart_driver_delete(UART_NUM_.{port}.);
    uart_initialized = false;
    ESP_LOGI(TAG, "UART.{port}. deinitialized");
}

/*============================================================================
 * Configuration Functions
 *============================================================================*/

void UART.{port}._bd(uint32_t baudRate)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    ESP_ERROR_CHECK(uart_set_baudrate(UART_NUM_.{port}., baudRate));
    ESP_LOGI(TAG, "Baud rate changed to %lu", baudRate);
}

void UART.{port}._setPins(int txPin, int rxPin)
{
    if (txPin >= 0) {
        uart_tx_pin = txPin;
    }
    if (rxPin >= 0) {
        uart_rx_pin = rxPin;
    }

    if (uart_initialized) {
        ESP_ERROR_CHECK(uart_set_pin(UART_NUM_.{port}.,
                                      uart_tx_pin,
                                      uart_rx_pin,
                                      UART_PIN_NO_CHANGE,
                                      UART_PIN_NO_CHANGE));
    }
}

/*============================================================================
 * Write Functions
 *============================================================================*/

int UART.{port}._write(const char* data, size_t len)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    return uart_write_bytes(UART_NUM_.{port}., data, len);
}

void UART.{port}._putChar(char c)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    uart_write_bytes(UART_NUM_.{port}., &c, 1);
}

void UART.{port}._print(const char* str)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    uart_write_bytes(UART_NUM_.{port}., str, strlen(str));
}

void UART.{port}._println(const char* str)
{
    UART.{port}._print(str);
    UART.{port}._print("\r\n");
}

void UART.{port}._flush(void)
{
    if (!uart_initialized) {
        return;
    }

    uart_wait_tx_done(UART_NUM_.{port}., pdMS_TO_TICKS(UART.{port}._TIMEOUT_MS));
}

/*============================================================================
 * Read Functions
 *============================================================================*/

int UART.{port}._read(char* buffer, size_t len)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    return uart_read_bytes(UART_NUM_.{port}., buffer, len,
                           pdMS_TO_TICKS(UART.{port}._TIMEOUT_MS));
}

int UART.{port}._readTimeout(char* buffer, size_t len, uint32_t timeout_ms)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    return uart_read_bytes(UART_NUM_.{port}., buffer, len,
                           pdMS_TO_TICKS(timeout_ms));
}

int UART.{port}._getChar(void)
{
    char c;
    int len = UART.{port}._read(&c, 1);
    return (len > 0) ? (int)(unsigned char)c : -1;
}

int UART.{port}._available(void)
{
    if (!uart_initialized) {
        return 0;
    }

    size_t buffered_len = 0;
    uart_get_buffered_data_len(UART_NUM_.{port}., &buffered_len);
    return (int)buffered_len;
}

int UART.{port}._readLine(char* buffer, size_t maxLen)
{
    if (!uart_initialized) {
        UART.{port}._init();
    }

    size_t idx = 0;
    char c;

    while (idx < maxLen - 1) {
        int len = uart_read_bytes(UART_NUM_.{port}., &c, 1,
                                  pdMS_TO_TICKS(UART.{port}._TIMEOUT_MS));
        if (len <= 0) {
            break; /* Timeout */
        }

        if (c == '\n') {
            break; /* End of line */
        }

        if (c != '\r') {
            buffer[idx++] = c;
        }
    }

    buffer[idx] = '\0';
    return (int)idx;
}

void UART.{port}._clearRx(void)
{
    if (!uart_initialized) {
        return;
    }

    uart_flush_input(UART_NUM_.{port}.);
}

/*============================================================================
 * Status Functions
 *============================================================================*/

bool UART.{port}._txComplete(void)
{
    if (!uart_initialized) {
        return true;
    }

    return (uart_wait_tx_done(UART_NUM_.{port}., 0) == ESP_OK);
}

bool UART.{port}._hasData(void)
{
    return (UART.{port}._available() > 0);
}
