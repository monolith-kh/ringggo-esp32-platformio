#include "stm32.h"

uint8_t spiCommand(SPIClass *spi, byte data) {
    uint8_t ret = 0;
    //use it as you would the regular arduino SPI API
    spi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    ret = spi->transfer(data);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();

    return ret;
}


uint8_t* spiStm32Command(SPIClass *spi, uint8_t cmd) {
    uint8_t ret[7] = {0, };
    //use it as you would the regular arduino SPI API
    spi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));
    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    spi->transfer(cmd);
    spi->transfer(0);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    vTaskDelay(50);

    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    spi->transferBytes(ret, ret, 7);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    spi->endTransaction();
    vTaskDelay(50);

    log_d("Receive: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", ret[0], ret[1], ret[2], ret[3], ret[4], ret[5], ret[6]);

    return ret; 
}


void Stm32Task(void* parameter)
{
    uint8_t ret = 0;

    time_t t;
    WiFiUDP rtlsClient;

    //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
    SPIClass* vspi = new SPIClass(VSPI);

    //clock miso mosi ss
    vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
    pinMode(VSPI_SS, OUTPUT);


    for (;;)
    {
        ret = spiCommand(vspi, 0b01010101); // junk data to illustrate usage
        log_d("vspi-1: 0x%x", ret);
        vTaskDelay(100);

        ret = spiCommand(vspi, 0b10101010); // junk data to illustrate usage
        log_d("vspi-2: 0x%x", ret);
        vTaskDelay(100);

        spiStm32Command(vspi, 0x41);

        rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
        Protocol_position_t protocol = { PK_POSITION_NOTI, CAR_ID, 22, 13, 0, };
        t = time(NULL);
        protocol.timestamp = (int64_t)t;
        protocol.positionX = (int16_t)POSITION_X_MAX;
        protocol.positionY = (int16_t)POSITION_Y_MAX;
        protocol.headAngle = (int16_t)HEADANGLE_MAX;
        rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
        rtlsClient.endPacket();
        log_d("done send data to rtls server");
        vTaskDelay(50);
    }

    vTaskDelete(NULL);
}
