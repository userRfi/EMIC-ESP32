/*****************************************************************************
 * @file    SPI.{host}._Slave.c
 * @brief   ESP32-C3 SPI.{host}. Slave Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/07/2026
 *
 * @details SPI Slave implementation for ESP32-C3 using ESP-IDF spi_slave driver.
 *          ESP32-C3 only has SPI2 available for general use.
 *****************************************************************************/

#include "SPI.{host}._Slave.h"

/* ESP-IDF includes */
#include "driver/spi_slave.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include <string.h>

static const char *TAG = "SPI.{host}._Slave_C3";

/*============================================================================
 * Internal State
 *============================================================================*/

static bool slave_initialized = false;
static SPI.{host}._SlaveCallback_t transaction_callback = NULL;

/* DMA-capable buffers */
WORD_ALIGNED_ATTR static uint8_t rx_buffer[SPI.{host}._SLAVE_BUFFER_SIZE];
WORD_ALIGNED_ATTR static uint8_t tx_buffer[SPI.{host}._SLAVE_BUFFER_SIZE];

static size_t last_rx_len = 0;

/* Pin configuration */
static int mosi_pin = SPI.{host}._SLAVE_MOSI_DEFAULT;
static int miso_pin = SPI.{host}._SLAVE_MISO_DEFAULT;
static int clk_pin = SPI.{host}._SLAVE_CLK_DEFAULT;
static int cs_pin = SPI.{host}._SLAVE_CS_DEFAULT;

/*============================================================================
 * Callback Wrappers
 *============================================================================*/

static void IRAM_ATTR spi_slave_post_trans_cb(spi_slave_transaction_t *trans)
{
    last_rx_len = trans->trans_len / 8; /* Convert bits to bytes */

    if (transaction_callback != NULL) {
        transaction_callback(rx_buffer, last_rx_len);
    }
}

/*============================================================================
 * Initialization
 *============================================================================*/

void SPI.{host}._Slave_init(void)
{
    SPI.{host}._Slave_initEx(-1, -1, -1, -1);
}

void SPI.{host}._Slave_initEx(int mosiPin, int misoPin, int clkPin, int csPin)
{
    if (slave_initialized) {
        return;
    }

    /* Update pin configuration if specified */
    if (mosiPin >= 0) mosi_pin = mosiPin;
    if (misoPin >= 0) miso_pin = misoPin;
    if (clkPin >= 0) clk_pin = clkPin;
    if (csPin >= 0) cs_pin = csPin;

    /* Configure bus */
    spi_bus_config_t bus_config = {
        .mosi_io_num = mosi_pin,
        .miso_io_num = miso_pin,
        .sclk_io_num = clk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = SPI.{host}._SLAVE_BUFFER_SIZE,
    };

    /* Configure slave interface */
    spi_slave_interface_config_t slave_config = {
        .mode = 0,
        .spics_io_num = cs_pin,
        .queue_size = 3,
        .flags = 0,
        .post_setup_cb = NULL,
        .post_trans_cb = spi_slave_post_trans_cb,
    };

    /* Enable pull-ups on signals */
    gpio_set_pull_mode(mosi_pin, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(clk_pin, GPIO_PULLUP_ONLY);
    gpio_set_pull_mode(cs_pin, GPIO_PULLUP_ONLY);

    esp_err_t ret = spi_slave_initialize(SPI.{host}._SLAVE_HOST, &bus_config,
                                          &slave_config, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI slave: %s", esp_err_to_name(ret));
        return;
    }

    slave_initialized = true;
    ESP_LOGI(TAG, "SPI.{host}. Slave initialized");
}

void SPI.{host}._Slave_deinit(void)
{
    if (!slave_initialized) {
        return;
    }

    spi_slave_free(SPI.{host}._SLAVE_HOST);
    slave_initialized = false;
    transaction_callback = NULL;
    ESP_LOGI(TAG, "SPI.{host}. Slave deinitialized");
}

/*============================================================================
 * Transaction Functions
 *============================================================================*/

bool SPI.{host}._Slave_queueTx(const uint8_t* txData, size_t len)
{
    if (!slave_initialized) {
        SPI.{host}._Slave_init();
    }

    if (len > SPI.{host}._SLAVE_BUFFER_SIZE) {
        len = SPI.{host}._SLAVE_BUFFER_SIZE;
    }

    memcpy(tx_buffer, txData, len);

    spi_slave_transaction_t trans = {
        .length = len * 8,
        .tx_buffer = tx_buffer,
        .rx_buffer = rx_buffer,
    };

    esp_err_t ret = spi_slave_queue_trans(SPI.{host}._SLAVE_HOST, &trans, 0);
    return (ret == ESP_OK);
}

int SPI.{host}._Slave_receive(uint8_t* rxData, size_t maxLen, uint32_t timeout_ms)
{
    if (!slave_initialized) {
        SPI.{host}._Slave_init();
    }

    /* Clear TX buffer for receive-only */
    memset(tx_buffer, 0, SPI.{host}._SLAVE_BUFFER_SIZE);

    spi_slave_transaction_t trans = {
        .length = maxLen * 8,
        .tx_buffer = tx_buffer,
        .rx_buffer = rx_buffer,
    };

    /* Queue and wait for transaction */
    esp_err_t ret = spi_slave_queue_trans(SPI.{host}._SLAVE_HOST, &trans,
                                           pdMS_TO_TICKS(timeout_ms));
    if (ret != ESP_OK) {
        return -1;
    }

    spi_slave_transaction_t *ret_trans;
    ret = spi_slave_get_trans_result(SPI.{host}._SLAVE_HOST, &ret_trans,
                                      pdMS_TO_TICKS(timeout_ms));
    if (ret != ESP_OK) {
        return -1;
    }

    size_t rx_len = ret_trans->trans_len / 8;
    if (rx_len > maxLen) {
        rx_len = maxLen;
    }

    memcpy(rxData, rx_buffer, rx_len);
    return (int)rx_len;
}

bool SPI.{host}._Slave_hasData(void)
{
    return (last_rx_len > 0);
}

int SPI.{host}._Slave_available(void)
{
    return (int)last_rx_len;
}

/*============================================================================
 * Callback Registration
 *============================================================================*/

void SPI.{host}._Slave_setCallback(SPI.{host}._SlaveCallback_t callback)
{
    transaction_callback = callback;
}

/*============================================================================
 * Configuration
 *============================================================================*/

void SPI.{host}._Slave_setMode(uint8_t mode)
{
    /* Mode change requires reinitializing */
    ESP_LOGW(TAG, "Mode change requires reinitialization");
}
