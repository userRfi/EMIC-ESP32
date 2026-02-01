# Main component CMakeLists.txt

idf_component_register(
    SRCS
        "main.c"
        "../emic_generated/*.c"
    INCLUDE_DIRS
        "."
        "../emic_generated/inc"
    REQUIRES
        driver
        esp_timer
        esp_wifi
        nvs_flash
        esp_event
        esp_netif
        bt
        freertos
)

# Add any additional compile options for the main component
target_compile_options(${COMPONENT_LIB} PRIVATE
    -Wall
    -Wextra
    -Wno-unused-parameter
)
