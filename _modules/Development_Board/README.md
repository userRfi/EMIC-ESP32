# Modulos de Placas de Desarrollo ESP32

Este directorio contiene las configuraciones para las placas de desarrollo ESP32 mas utilizadas.

## Placas Oficiales Espressif

| Placa | MCU | Arquitectura | WiFi | BLE | Flash | PSRAM | USB |
|-------|-----|--------------|------|-----|-------|-------|-----|
| **ESP32-DevKitC** | ESP32 | Xtensa LX6 | Si | Si | 4MB | No | CP2102 |
| **ESP32-DevKitC-V4** | ESP32 | Xtensa LX6 | Si | Si | 4MB | No | CP2102 |
| **ESP32-WROOM-32** | ESP32 | Xtensa LX6 | Si | Si | 4MB | No | - |
| **ESP32-WROVER-KIT** | ESP32 | Xtensa LX6 | Si | Si | 4MB | 8MB | FTDI |
| **ESP32-S2-DevKitM** | ESP32-S2 | Xtensa LX7 | Si | No | 4MB | No | Nativo |
| **ESP32-S3-DevKitC** | ESP32-S3 | Xtensa LX7 | Si | 5.0 | 8MB | 8MB | OTG |
| **ESP32-C3-DevKitM** | ESP32-C3 | RISC-V | Si | 5.0 | 4MB | No | CDC |
| **ESP32-C6-DevKitC** | ESP32-C6 | RISC-V | Si/6 | 5.0 | 8MB | No | CDC |
| **ESP32-PICO-KIT** | ESP32-PICO | Xtensa LX6 | Si | Si | 4MB | No | CP2102 |

## Placas de Terceros

| Placa | MCU | Caracteristicas Especiales |
|-------|-----|---------------------------|
| **NodeMCU-32S** | ESP32 | Compatible NodeMCU, 30 pines |
| **ESP32-CAM** | ESP32 | Camara OV2640, microSD, Flash LED |
| **TTGO-T-Display** | ESP32 | Display TFT 1.14" integrado |
| **TTGO-T-Beam** | ESP32 | GPS, LoRa, 18650 holder |
| **Lolin-D32** | ESP32 | LiPo charger, bajo consumo |
| **Lolin-D32-Pro** | ESP32 | LiPo, TF card, PSRAM |
| **M5Stack-Core** | ESP32 | Display, speaker, bateria, modular |
| **M5StickC** | ESP32-PICO | Ultra compacto, display, IMU |
| **Seeed-XIAO-ESP32** | ESP32-C3 | Ultra pequeno, USB-C |
| **Adafruit-Feather-ESP32** | ESP32 | Formato Feather, LiPo |

## Estructura de Cada Modulo

```
<nombre-placa>/
├── module.json          # Metadata y configuracion
└── System/
    ├── pinout.h         # Definicion de pines
    └── board.emic       # Configuracion EMIC
```

## Uso

Para usar una placa en tu proyecto EMIC:

```emic
EMIC:setInput(DEV:_modules/Development_Board/ESP32-DevKitC/System/board.emic)
```

## Comparativa de Arquitecturas

| Arquitectura | MCUs | Cores | Frecuencia | Caracteristicas |
|--------------|------|-------|------------|-----------------|
| **Xtensa LX6** | ESP32 | 2 | 240 MHz | Bluetooth Classic + BLE |
| **Xtensa LX7** | ESP32-S2/S3 | 1/2 | 240 MHz | USB nativo, AI accel (S3) |
| **RISC-V** | ESP32-C3/C6 | 1 | 160 MHz | Bajo costo, WiFi 6 (C6) |

## Notas Importantes

- Los pines GPIO 6-11 estan conectados al flash SPI interno (NO USAR)
- Los pines GPIO 34-39 son solo entrada (sin driver de salida)
- ADC2 no funciona cuando WiFi esta activo
- Algunas placas requieren programador externo (ESP32-CAM)
