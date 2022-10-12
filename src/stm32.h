#ifndef STM32_H
#define STM32_H

#include <SPI.h>
#include <Arduino.h>
#include "packet.h"
#include "config.h"
#include <WiFi.h>

#define POSITION_X_MAX  1500
#define POSITION_Y_MAX  2000
#define HEADANGLE_MAX   360

#define FETCH_UWB_DELAY 1000
#define SPI_SIGNAL_DELAY    50
#define SPI_MAX_DATASIZE    8

#define LOG_LEVEL_TRACE 1


// Commands
#define SPI_CMD_EVENT           0x11
#define SPI_CMD_STATUS          0x21
#define SPI_CMD_REMOTE_SET      0x31
#define SPI_CMD_LOCATION_GET    0x41
#define SPI_CMD_CARNUM_GET      0x51


// Define ALTERNATE_PINS to use non-standard GPIO pins for SPI bus

#define VSPI_MOSI   25
#define VSPI_MISO   26
#define VSPI_SCLK   27
#define VSPI_SS     14

#define VSPI_CLK    1000000


#pragma pack(1)
typedef struct {
    uint8_t size;
    uint8_t data[SPI_MAX_DATASIZE];
} spi_response;

#pragma pack(1)
typedef struct {
    uint8_t status;
    uint8_t status_data;
} spi_response_data_status;

#pragma pack(1)
typedef struct {
    uint8_t carnumber;
} spi_response_data_carnumber;

#pragma pack(1)
typedef struct {
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t head_angle;
} spi_response_data_location;


uint8_t spiCommand(SPIClass *spi, byte data);
void Stm32Task(void* parameter);

#endif
