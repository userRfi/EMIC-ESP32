/*****************************************************************************
 * @file    SPI.{host}..c
 * @brief   ESP32 SPI.{host}. Master Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    01/31/2026
 *
 * @details SPI Master implementation using ESP-IDF spi_master driver.
 *****************************************************************************/

#include "SPI.{host}..h"

/* ESP-IDF includes */
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "SPI.{host}.";

/*============================================================================
 * Internal State
 *============================================================================*/

#define MAX_DEVICES 4

static spi_device_handle_t device_handles[MAX_DEVICES] = {NULL};
static int current_device = -1;
static bool spi_initialized = false;
static int active_cs_pin = -1;

/* Current configuration */
static int mosi_pin = SPI.{host}._MOSI_DEFAULT;
static int miso_pin = SPI.{host}._MISO_DEFAULT;
static int clk_pin = SPI.{host}._CLK_DEFAULT;
static uint32_t clock_speed = SPI.{host}._CLOCK_SPEED;

/*============================================================================
 * Initialization
 *============================================================================*/

void SPI.{host}._init(void)
{
    SPI.{host}._initEx(-1, -1, -1, SPI.{host}._CLOCK_SPEED);
}

void SPI.{host}._initEx(int mosiPin, int misoPin, int clkPin, uint32_t clockSpeed)
{
    if (spi_initialized) {
        return;
    }

    /* Update pin configuration if specified */
    if (mosiPin >= 0) mosi_pin = mosiPin;
    if (misoPin >= 0) miso_pin = misoPin;
    if (clkPin >= 0) clk_pin = clkPin;
    clock_speed = clockSpeed;

    /* Configure SPI bus */
    spi_bus_config_t bus_config = {
        .mosi_io_num = mosi_pin,
        .miso_io_num = miso_pin,
        .sclk_io_num = clk_pin,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 4096,
        .flags = SPICOMMON_BUSFLAG_MASTER,
    };

    esp_err_t ret = spi_bus_initialize(SPI.{host}._HOST, &bus_config, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize SPI bus: %s", esp_err_to_name(ret));
        return;
    }

    spi_initialized = true;
    ESP_LOGI(TAG, "SPI.{host}. Master initialized at %lu Hz", clock_speed);
}

void SPI.{host}._deinit(void)
{
    if (!spi_initialized) {
        return;
    }

    /* Remove all devices */
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (device_handles[i] != NULL) {
            spi_bus_remove_device(device_handles[i]);
            device_handles[i] = NULL;
        }
    }

    spi_bus_free(SPI.{host}._HOST);
    spi_initialized = false;
    current_device = -1;
    ESP_LOGI(TAG, "SPI.{host}. deinitialized");
}

/*============================================================================
 * Device Management
 *============================================================================*/

int SPI.{host}._addDevice(int csPin, uint8_t mode)
{
    if (!spi_initialized) {
        SPI.{host}._init();
    }

    /* Find free slot */
    int slot = -1;
    for (int i = 0; i < MAX_DEVICES; i++) {
        if (device_handles[i] == NULL) {
            slot = i;
            break;
        }
    }

    if (slot < 0) {
        ESP_LOGE(TAG, "No free device slots");
        return -1;
    }

    /* Configure device */
    spi_device_interface_config_t dev_config = {
        .clock_speed_hz = clock_speed,
        .mode = mode & 0x03,
        .spics_io_num = csPin,
        .queue_size = 7,
        .flags = 0,
        .pre_cb = NULL,
        .post_cb = NULL,
    };

    esp_err_t ret = spi_bus_add_device(SPI.{host}._HOST, &dev_config, &device_handles[slot]);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device: %s", esp_err_to_name(ret));
        return -1;
    }

    ESP_LOGI(TAG, "Added device %d on CS pin %d", slot, csPin);

    /* Select first device by default */
    if (current_device < 0) {
        current_device = slot;
        active_cs_pin = csPin;
    }

    return slot;
}

void SPI.{host}._removeDevice(int deviceHandle)
{
    if (deviceHandle < 0 || deviceHandle >= MAX_DEVICES) {
        return;
    }

    if (device_handles[deviceHandle] != NULL) {
        spi_bus_remove_device(device_handles[deviceHandle]);
        device_handles[deviceHandle] = NULL;

        if (current_device == deviceHandle) {
            current_device = -1;
            active_cs_pin = -1;
        }
    }
}

void SPI.{host}._selectDevice(int deviceHandle)
{
    if (deviceHandle >= 0 && deviceHandle < MAX_DEVICES &&
        device_handles[deviceHandle] != NULL) {
        current_device = deviceHandle;
    }
}

/*============================================================================
 * Transfer Functions
 *============================================================================*/

void SPI.{host}._transfer(const uint8_t* txData, uint8_t* rxData, size_t len)
{
    if (!spi_initialized || current_device < 0 || len == 0) {
        return;
    }

    spi_transaction_t trans = {
        .length = len * 8,          /* Length in bits */
        .tx_buffer = txData,
        .rx_buffer = rxData,
    };

    esp_err_t ret = spi_device_transmit(device_handles[current_device], &trans);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Transfer failed: %s", esp_err_to_name(ret));
    }
}

uint8_t SPI.{host}._transferByte(uint8_t txByte)
{
    uint8_t rxByte = 0;
    SPI.{host}._transfer(&txByte, &rxByte, 1);
    return rxByte;
}

void SPI.{host}._write(const uint8_t* data, size_t len)
{
    SPI.{host}._transfer(data, NULL, len);
}

void SPI.{host}._writeByte(uint8_t data)
{
    SPI.{host}._transfer(&data, NULL, 1);
}

void SPI.{host}._writeWord(uint16_t data)
{
    uint8_t buf[2] = {(data >> 8) & 0xFF, data & 0xFF};
    SPI.{host}._write(buf, 2);
}

void SPI.{host}._read(uint8_t* data, size_t len)
{
    SPI.{host}._transfer(NULL, data, len);
}

uint8_t SPI.{host}._readByte(void)
{
    uint8_t data = 0;
    SPI.{host}._transfer(NULL, &data, 1);
    return data;
}

/*============================================================================
 * Chip Select Control
 *============================================================================*/

void SPI.{host}._csLow(void)
{
    if (active_cs_pin >= 0) {
        gpio_set_level(active_cs_pin, 0);
    }
}

void SPI.{host}._csHigh(void)
{
    if (active_cs_pin >= 0) {
        gpio_set_level(active_cs_pin, 1);
    }
}

/*============================================================================
 * Configuration Functions
 *============================================================================*/

void SPI.{host}._setSpeed(uint32_t clockSpeed)
{
    clock_speed = clockSpeed;
    /* Note: Speed change requires re-adding the device */
    ESP_LOGW(TAG, "Speed change will apply to new devices only");
}

void SPI.{host}._setMode(uint8_t mode)
{
    /* Note: Mode change requires re-adding the device */
    ESP_LOGW(TAG, "Mode change will apply to new devices only");
}

void SPI.{host}._setBitOrder(bool msbFirst)
{
    /* ESP32 SPI always uses MSB first */
    if (!msbFirst) {
        ESP_LOGW(TAG, "ESP32 SPI only supports MSB first");
    }
}
