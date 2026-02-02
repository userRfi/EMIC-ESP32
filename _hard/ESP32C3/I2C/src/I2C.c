/*****************************************************************************
 * @file    I2C.{port}..c
 * @brief   ESP32-C3 I2C.{port}. Master Implementation for EMIC SDK
 * @author  EMIC Team
 * @version v1.0.0
 * @date    02/01/2026
 *
 * @details I2C Master implementation for ESP32-C3.
 *****************************************************************************/

#include "I2C.{port}..h"

/* ESP-IDF includes */
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include <string.h>

static const char *TAG = "I2C.{port}.";

/*============================================================================
 * Internal State
 *============================================================================*/

static i2c_master_bus_handle_t bus_handle = NULL;
static bool i2c_initialized = false;

static int sda_pin = I2C.{port}._SDA_DEFAULT;
static int scl_pin = I2C.{port}._SCL_DEFAULT;
static uint32_t clock_speed = I2C.{port}._SPEED;

#define MAX_CACHED_DEVICES 8
typedef struct {
    uint8_t addr;
    i2c_master_dev_handle_t handle;
} device_cache_t;

static device_cache_t device_cache[MAX_CACHED_DEVICES] = {0};
static int cache_count = 0;

/*============================================================================
 * Internal Functions
 *============================================================================*/

static i2c_master_dev_handle_t get_device_handle(uint8_t addr)
{
    for (int i = 0; i < cache_count; i++) {
        if (device_cache[i].addr == addr) {
            return device_cache[i].handle;
        }
    }

    if (cache_count >= MAX_CACHED_DEVICES) {
        ESP_LOGW(TAG, "Device cache full, removing oldest");
        i2c_master_bus_rm_device(device_cache[0].handle);
        memmove(&device_cache[0], &device_cache[1],
                (MAX_CACHED_DEVICES - 1) * sizeof(device_cache_t));
        cache_count--;
    }

    i2c_device_config_t dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = addr,
        .scl_speed_hz = clock_speed,
    };

    i2c_master_dev_handle_t dev_handle;
    esp_err_t ret = i2c_master_bus_add_device(bus_handle, &dev_config, &dev_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to add device 0x%02X: %s", addr, esp_err_to_name(ret));
        return NULL;
    }

    device_cache[cache_count].addr = addr;
    device_cache[cache_count].handle = dev_handle;
    cache_count++;

    return dev_handle;
}

/*============================================================================
 * Initialization
 *============================================================================*/

void I2C.{port}._init(void)
{
    I2C.{port}._initEx(-1, -1, I2C.{port}._SPEED);
}

void I2C.{port}._initSpeed(uint32_t clockSpeed)
{
    I2C.{port}._initEx(-1, -1, clockSpeed);
}

void I2C.{port}._initEx(int sdaPin, int sclPin, uint32_t clockSpeed)
{
    if (i2c_initialized) {
        return;
    }

    if (sdaPin >= 0) sda_pin = sdaPin;
    if (sclPin >= 0) scl_pin = sclPin;
    clock_speed = clockSpeed;

    i2c_master_bus_config_t bus_config = {
        .i2c_port = I2C.{port}._PORT,
        .sda_io_num = sda_pin,
        .scl_io_num = scl_pin,
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .glitch_ignore_cnt = 7,
        .flags.enable_internal_pullup = true,
    };

    esp_err_t ret = i2c_new_master_bus(&bus_config, &bus_handle);
    if (ret != ESP_OK) {
        ESP_LOGE(TAG, "Failed to initialize I2C bus: %s", esp_err_to_name(ret));
        return;
    }

    i2c_initialized = true;
    cache_count = 0;
    ESP_LOGI(TAG, "I2C.{port}. initialized at %lu Hz (SDA=%d, SCL=%d)",
             clock_speed, sda_pin, scl_pin);
}

void I2C.{port}._deinit(void)
{
    if (!i2c_initialized) {
        return;
    }

    for (int i = 0; i < cache_count; i++) {
        i2c_master_bus_rm_device(device_cache[i].handle);
    }
    cache_count = 0;

    i2c_del_master_bus(bus_handle);
    bus_handle = NULL;
    i2c_initialized = false;
    ESP_LOGI(TAG, "I2C.{port}. deinitialized");
}

/*============================================================================
 * Basic Transfer Functions
 *============================================================================*/

bool I2C.{port}._write(uint8_t addr, const uint8_t* data, size_t len)
{
    if (!i2c_initialized) {
        I2C.{port}._init();
    }

    i2c_master_dev_handle_t dev = get_device_handle(addr);
    if (dev == NULL) {
        return false;
    }

    esp_err_t ret = i2c_master_transmit(dev, data, len, I2C.{port}._TIMEOUT_MS);
    return (ret == ESP_OK);
}

bool I2C.{port}._read(uint8_t addr, uint8_t* buffer, size_t len)
{
    if (!i2c_initialized) {
        I2C.{port}._init();
    }

    i2c_master_dev_handle_t dev = get_device_handle(addr);
    if (dev == NULL) {
        return false;
    }

    esp_err_t ret = i2c_master_receive(dev, buffer, len, I2C.{port}._TIMEOUT_MS);
    return (ret == ESP_OK);
}

bool I2C.{port}._writeRead(uint8_t addr, const uint8_t* txData, size_t txLen,
                            uint8_t* rxBuffer, size_t rxLen)
{
    if (!i2c_initialized) {
        I2C.{port}._init();
    }

    i2c_master_dev_handle_t dev = get_device_handle(addr);
    if (dev == NULL) {
        return false;
    }

    esp_err_t ret = i2c_master_transmit_receive(dev, txData, txLen,
                                                  rxBuffer, rxLen,
                                                  I2C.{port}._TIMEOUT_MS);
    return (ret == ESP_OK);
}

/*============================================================================
 * Register Access Functions
 *============================================================================*/

bool I2C.{port}._writeRegister(uint8_t addr, uint8_t reg, uint8_t val)
{
    uint8_t data[2] = {reg, val};
    return I2C.{port}._write(addr, data, 2);
}

bool I2C.{port}._readRegister(uint8_t addr, uint8_t reg, uint8_t* val)
{
    return I2C.{port}._writeRead(addr, &reg, 1, val, 1);
}

bool I2C.{port}._writeRegisters(uint8_t addr, uint8_t reg, const uint8_t* data, size_t len)
{
    if (len > 127) {
        return false;
    }

    uint8_t buffer[128];
    buffer[0] = reg;
    memcpy(&buffer[1], data, len);

    return I2C.{port}._write(addr, buffer, len + 1);
}

bool I2C.{port}._readRegisters(uint8_t addr, uint8_t reg, uint8_t* buffer, size_t len)
{
    return I2C.{port}._writeRead(addr, &reg, 1, buffer, len);
}

bool I2C.{port}._writeRegister16(uint8_t addr, uint8_t reg, uint16_t val)
{
    uint8_t data[3] = {reg, (val >> 8) & 0xFF, val & 0xFF};
    return I2C.{port}._write(addr, data, 3);
}

bool I2C.{port}._readRegister16(uint8_t addr, uint8_t reg, uint16_t* val)
{
    uint8_t data[2];
    if (!I2C.{port}._writeRead(addr, &reg, 1, data, 2)) {
        return false;
    }
    *val = ((uint16_t)data[0] << 8) | data[1];
    return true;
}

/*============================================================================
 * Utility Functions
 *============================================================================*/

int I2C.{port}._scan(uint8_t* foundAddrs, int maxDevices)
{
    if (!i2c_initialized) {
        I2C.{port}._init();
    }

    int found = 0;
    ESP_LOGI(TAG, "Scanning I2C bus...");

    for (uint8_t addr = 0x08; addr < 0x78 && found < maxDevices; addr++) {
        if (I2C.{port}._devicePresent(addr)) {
            if (foundAddrs != NULL) {
                foundAddrs[found] = addr;
            }
            ESP_LOGI(TAG, "Found device at 0x%02X", addr);
            found++;
        }
    }

    ESP_LOGI(TAG, "Scan complete. Found %d devices.", found);
    return found;
}

bool I2C.{port}._devicePresent(uint8_t addr)
{
    if (!i2c_initialized) {
        I2C.{port}._init();
    }

    esp_err_t ret = i2c_master_probe(bus_handle, addr, I2C.{port}._TIMEOUT_MS);
    return (ret == ESP_OK);
}

void I2C.{port}._setSpeed(uint32_t clockSpeed)
{
    clock_speed = clockSpeed;

    for (int i = 0; i < cache_count; i++) {
        i2c_master_bus_rm_device(device_cache[i].handle);
    }
    cache_count = 0;

    ESP_LOGI(TAG, "Speed changed to %lu Hz", clockSpeed);
}

void I2C.{port}._reset(void)
{
    if (!i2c_initialized) {
        return;
    }

    ESP_LOGI(TAG, "Resetting I2C bus...");
    i2c_master_bus_reset(bus_handle);
}
