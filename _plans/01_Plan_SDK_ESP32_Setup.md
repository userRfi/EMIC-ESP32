# Plan 01: Setup Inicial del SDK EMIC para ESP32

**Fecha:** 2026-01-31
**Rama:** TEST-ESP32-SDK
**Estado:** Pendiente

---

## 1. Objetivo

Crear la estructura base del SDK EMIC para microcontroladores ESP32 utilizando el toolchain de Espressif (ESP-IDF).

---

## 2. Toolchain y Entorno

| Componente | Valor |
|------------|-------|
| **Compilador** | xtensa-esp32-elf-gcc (ESP-IDF) |
| **Framework** | ESP-IDF v5.x |
| **MCUs Target** | ESP32, ESP32-S2, ESP32-S3, ESP32-C3 |
| **Arquitecturas** | Xtensa LX6/LX7, RISC-V |
| **IDE Recomendado** | VSCode + ESP-IDF Extension |

---

## 3. Estructura de Carpetas a Crear

```
EMIC-ESP32/
├── _hard/
│   └── ESP32/
│       ├── GPIO/
│       ├── UART/
│       ├── Timer/
│       ├── SPI/
│       ├── I2C/
│       ├── ADC/
│       ├── WiFi/
│       ├── BLE/
│       └── System/
├── _hal/
│   └── ESP32/
│       └── hal_config.h
├── _api/
│   ├── Actuators/
│   ├── Indicators/
│   ├── Inputs/
│   ├── Sensors/
│   ├── Timers/
│   ├── Storage/
│   ├── Protocols/
│   ├── Wireless/
│   └── System/
├── _drivers/
│   ├── Displays/
│   ├── Sensors/
│   ├── Communication/
│   └── Storage/
├── _modules/
│   ├── Actuators/                    # Motores, reles, valvulas
│   ├── Custom/                       # Modulos personalizados
│   ├── Development_Board/            # Placas de desarrollo
│   ├── Digital_In_Out/               # Entradas/salidas digitales
│   ├── Displays/                     # Pantallas y visualizadores
│   ├── Graphic_Displays/             # Pantallas graficas
│   ├── Indicators/                   # LEDs, buzzers
│   ├── Inputs/                       # Botones, encoders, teclados
│   ├── Power_Supply/                 # Fuentes de alimentacion
│   ├── Sensors/                      # Sensores varios
│   ├── Storage/                      # Almacenamiento (SD, EEPROM)
│   ├── Testing/                      # Modulos de prueba
│   ├── Wired_Communication/          # RS485, CAN, Ethernet
│   ├── Wired_Control/                # Control cableado
│   ├── Wireless_Communication/       # WiFi, BLE, LoRa, Zigbee
│   │   ├── WiFi_Station/
│   │   ├── WiFi_AP/
│   │   ├── BLE_Server/
│   │   ├── BLE_Client/
│   │   └── ESP_NOW/
│   ├── Audio/                        # I2S, DAC audio
│   ├── Video/                        # Camara, streaming
│   └── IoT_Platforms/                # MQTT, HTTP, WebSocket
│       ├── MQTT_Client/
│       ├── HTTP_Server/
│       ├── WebSocket/
│       └── OTA_Update/
├── _templates/
│   └── ESP-IDF/
│       ├── CMakeLists.txt
│       ├── main/
│       └── sdkconfig.defaults
├── _util/
│   ├── Math/
│   ├── String/
│   └── Conversion/
├── _main/
│   └── baremetal/
└── _plans/
    └── (este archivo)
```

---

## 4. Fases de Implementacion

### Fase 0: Estructura de Modulos (Prioridad Alta)
- [x] Crear carpeta `_modules/` con todas las categorias
- [x] Crear carpeta `_api/` con categorias base
- [x] Crear carpeta `_drivers/` con categorias base
- [x] Crear carpeta `_util/` con categorias base
- [x] Crear carpeta `_main/` con template baremetal

### Fase 1: Estructura Base _hard/ (Prioridad Alta)
- [x] Crear carpeta `_hard/ESP32/`
- [x] Crear carpeta `_hard/ESP32/System/`
- [x] Crear archivo `system.emic` con configuracion base
- [x] Crear archivo `system.h` con definiciones del MCU

### Fase 2: GPIO (Prioridad Alta)
- [ ] Crear carpeta `_hard/ESP32/GPIO/`
- [ ] Implementar `gpio.emic`
- [ ] Implementar `gpio.h` con macros HAL
- [ ] Documentar con tags DOXYGEN

### Fase 3: UART (Prioridad Alta)
- [ ] Crear carpeta `_hard/ESP32/UART/`
- [ ] Implementar `uart.emic`
- [ ] Implementar `uart.h` y `uart.c`
- [ ] Soportar UART0, UART1, UART2

### Fase 4: Timer (Prioridad Media)
- [ ] Crear carpeta `_hard/ESP32/Timer/`
- [ ] Implementar timers de hardware
- [ ] Implementar timers de software (FreeRTOS)

### Fase 5: SPI/I2C (Prioridad Media)
- [ ] Implementar SPI Master/Slave
- [ ] Implementar I2C Master/Slave

### Fase 6: ADC (Prioridad Media)
- [ ] Implementar lectura analogica
- [ ] Calibracion ADC

### Fase 7: WiFi (Prioridad Alta para ESP32)
- [ ] Crear carpeta `_hard/ESP32/WiFi/`
- [ ] Implementar modo Station
- [ ] Implementar modo AP
- [ ] Implementar modo AP+Station

### Fase 8: BLE (Prioridad Media)
- [ ] Crear carpeta `_hard/ESP32/BLE/`
- [ ] Implementar BLE Server
- [ ] Implementar BLE Client

---

## 5. Dependencias ESP-IDF

Componentes de ESP-IDF a utilizar:

```cmake
REQUIRES driver esp_wifi esp_event nvs_flash esp_timer freertos
```

| Componente ESP-IDF | Uso en EMIC |
|--------------------|-------------|
| `driver` | GPIO, UART, SPI, I2C, ADC |
| `esp_wifi` | WiFi Station/AP |
| `esp_event` | Sistema de eventos |
| `nvs_flash` | Almacenamiento no volatil |
| `esp_timer` | Timers de alta resolucion |
| `freertos` | RTOS base |

---

## 6. Convenciones de Codigo

### Macros HAL (compatibles con EMIC)

```c
// GPIO
HAL_GPIO_Init()
HAL_GPIO_PinCfgOutput(pin)
HAL_GPIO_PinCfgInput(pin)
HAL_GPIO_PinSet(pin, value)
HAL_GPIO_PinGet(pin)
HAL_GPIO_PinToggle(pin)

// UART
HAL_UART_Init(uart_num, baudrate)
HAL_UART_Write(uart_num, data, len)
HAL_UART_Read(uart_num, buffer, len)
HAL_UART_Available(uart_num)

// Timer
HAL_Timer_Init(timer_num, period_us)
HAL_Timer_Start(timer_num)
HAL_Timer_Stop(timer_num)
```

### Prefijos de archivos

| Tipo | Prefijo | Ejemplo |
|------|---------|---------|
| Header hardware | `_hard_` | `gpio.h` |
| Source hardware | `_hard_` | `uart.c` |
| Scripts EMIC | ninguno | `gpio.emic` |

---

## 7. Primer Entregable (MVP)

El MVP debe permitir:

1. Controlar un LED via GPIO
2. Enviar datos por UART
3. Usar un timer basico

Esto valida el flujo completo:
```
generate.emic -> codigo C -> ESP-IDF -> .bin -> ESP32
```

---

## 8. Proximos Pasos

1. **Inmediato:** Crear estructura de carpetas base
2. **Siguiente:** Implementar `_hard/ESP32/System/`
3. **Despues:** Implementar GPIO como prueba de concepto

---

## 9. Referencias

- ESP-IDF Programming Guide: https://docs.espressif.com/projects/esp-idf/
- EMIC Manual: `Manual_Desarrollo_EMIC_V2/`
- Estructura _hard/: `Seccion_2_Estructura_SDK/10_Carpeta_Hard.md`

---

*Plan creado para el desarrollo del SDK EMIC-ESP32*
