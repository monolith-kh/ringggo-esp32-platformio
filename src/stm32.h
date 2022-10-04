#ifndef STM32_H
#define STM32_H

#include <SPI.h>
#include <Arduino.h>
#include "packet.h"
#include "config.h"
#include <WiFi.h>

#define POSITION_X_MAX  1500
#define POSITION_Y_MAX  2000
#define HEADANGLE_MAX  360


// Define ALTERNATE_PINS to use non-standard GPIO pins for SPI bus

#define VSPI_MOSI   25
#define VSPI_MISO   26
#define VSPI_SCLK   27
#define VSPI_SS     14

#define VSPI_CLK     1000000


uint8_t spiCommand(SPIClass *spi, byte data);
void Stm32Task(void* parameter);

#endif
