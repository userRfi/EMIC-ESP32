# CMakeLists.txt for ESP32 EMIC SDK Hardware Layer
# Integrates with ESP-IDF build system

# Component requirements for ESP-IDF
set(COMPONENT_REQUIRES
    driver
    esp_timer
    esp_wifi
    nvs_flash
    esp_event
    esp_netif
    bt
)

# Optional components (may not be present on all variants)
set(COMPONENT_PRIV_REQUIRES
    freertos
    heap
    log
    soc
    hal
)

# Source files to compile
set(COMPONENT_SRCS
    "System/src/system.c"
    "UART/src/UARTX.c"
    "SPI/Master/src/spi.c"
    "SPI/Slave/src/spi.c"
    "I2C/src/I2C.c"
    "Timer/src/timer.c"
    "ADC/src/adc.c"
    "PWM/src/pwm.c"
    "WiFi/src/wifi.c"
    "Bluetooth/src/bt_classic.c"
    "Bluetooth/src/ble.c"
)

# Include directories
set(COMPONENT_ADD_INCLUDEDIRS
    "."
    "System/inc"
    "GPIO/inc"
    "UART/inc"
    "SPI/Master/inc"
    "SPI/Slave/inc"
    "I2C/inc"
    "Timer/inc"
    "ADC/inc"
    "PWM/inc"
    "WiFi/inc"
    "Bluetooth/inc"
    "pins"
)

# Register component with ESP-IDF
idf_component_register(
    SRCS ${COMPONENT_SRCS}
    INCLUDE_DIRS ${COMPONENT_ADD_INCLUDEDIRS}
    REQUIRES ${COMPONENT_REQUIRES}
    PRIV_REQUIRES ${COMPONENT_PRIV_REQUIRES}
)

# Compiler flags for EMIC compatibility
target_compile_definitions(${COMPONENT_LIB} PUBLIC
    EMIC_ESP32=1
    ESP_PLATFORM=1
)

# Optional: Enable specific features based on sdkconfig
if(CONFIG_BT_ENABLED)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC EMIC_BT_ENABLED=1)
endif()

if(CONFIG_ESP_WIFI_ENABLED)
    target_compile_definitions(${COMPONENT_LIB} PUBLIC EMIC_WIFI_ENABLED=1)
endif()
