#ifndef STM32_H
#define STM32_H

#include <SPI.h>
#include <Arduino.h>
#include <WiFi.h>
#include "packet.h"
#include "gameserver.h"
#include "config.h"


#define FETCH_UWB_DELAY     100
#define SPI_SIGNAL_DELAY    50
#define SPI_MAX_DATASIZE    10

#define LOG_LEVEL_TRACE     1


// Commands
#define SPI_CMD_EVENT           0x11
#define SPI_CMD_STATUS          0x21
#define SPI_CMD_REMOTE_SET      0x31
#define SPI_CMD_LOCATION_GET    0x41
#define SPI_CMD_CARNUM_GET      0x51
#define SPI_CMD_RESET_GET       0x61

#define EVENT_REMOTE_ON         0x00
#define EVENT_REMOTE_OFF        0x01
#define EVENT_MANUAL_ON         0x02
#define EVENT_MANUAL_OFF        0x03
#define EVENT_NPC_ON            0x04
#define EVENT_NPC_OFF           0x05
#define EVENT_RESUME            0x06
#define EVENT_SUSPEND           0x07

#define STATUS_READY            0x00
#define STATUS_MANUAL           0x01
#define STATUS_REMOTE           0x02
#define STATUS_PAUSE            0x03
#define STATUS_NPC              0x04

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
    uint8_t event;
    uint8_t event_data;
} spi_response_data_event;

#pragma pack(1)
typedef struct {
    uint8_t carnumber;
} spi_response_data_carnumber;

#pragma pack(1)
typedef struct {
    uint8_t reset;
} spi_response_data_reset;

#pragma pack(1)
typedef struct {
    uint16_t pos_x;
    uint16_t pos_y;
    uint16_t head_angle;
    uint16_t acc_x;
    uint16_t acc_y;
} spi_response_data_location;


extern SPIClass* vspi;
extern uint8_t carNumber;

uint8_t spiCommand(SPIClass *spi, byte data);
void spiStm32Command(SPIClass *spi, uint8_t cmd, uint8_t* data, uint8_t data_size, spi_response* response);
uint8_t GetCarNumber();
void SetEventMode(uint8_t mode);
void Stm32Init();
void Stm32Reset();
void Stm32Task(void* parameter);

#endif
