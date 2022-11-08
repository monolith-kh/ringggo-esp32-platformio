#include "stm32.h"


uint8_t spiCommand(SPIClass *spi, byte data);
void spiStm32Command(SPIClass *spi, uint8_t cmd, uint8_t* data, uint8_t data_size, spi_response* response);
void Stm32Reset();


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


void spiStm32Command(SPIClass *spi, uint8_t cmd, uint8_t* data, uint8_t data_size, spi_response* response) {
    //use it as you would the regular arduino SPI API
    spi->beginTransaction(SPISettings(VSPI_CLK, MSBFIRST, SPI_MODE0));

    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    spi->transfer(cmd);
    spi->transfer(data_size);
    spi->transfer(data, data_size);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    vTaskDelay(SPI_SIGNAL_DELAY);

    memset(response->data, 0, SPI_MAX_DATASIZE);

    digitalWrite(VSPI_SS, LOW); //pull SS slow to prep other end for transfer
    spi->transferBytes(NULL, &(response->size), 1);
    if (LOG_LEVEL_TRACE) {
        log_d("Receive Data Size: %d", response->size);
    }
    spi->transferBytes(NULL, response->data, response->size);
    digitalWrite(VSPI_SS, HIGH); //pull ss high to signify end of data transfer
    vTaskDelay(SPI_SIGNAL_DELAY);
    
    spi->endTransaction();

    if (LOG_LEVEL_TRACE) {
        log_d("Receive: 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x 0x%02x", 
            response->data[0], 
            response->data[1], 
            response->data[2], 
            response->data[3], 
            response->data[4], 
            response->data[5], 
            response->data[6],
            response->data[7],
            response->data[8],
            response->data[9]);
    }
}

uint8_t GetCarNumber()
{
    uint8_t data[SPI_MAX_DATASIZE] = {0, };
    spi_response response;

    spiStm32Command(vspi, SPI_CMD_CARNUM_GET, data, 0, &response);
    spi_response_data_carnumber* car_number = (spi_response_data_carnumber*) response.data;
    log_i("CarNumber: %d", car_number->carnumber);

    return car_number->carnumber;
}

void SetEventModeCallback(spi_response* response)
{
    spi_response_data_event* event = (spi_response_data_event*) response->data;
    log_i("event data: %d, %d", event->event, event->event_data);
}

void SetEventMode(uint8_t mode)
{
    spi_request request;

    request.command = SPI_CMD_EVENT;
    request.size = 1;
    if (mode == STOP) {
        request.data[0] = EVENT_MANUAL_OFF;
    } else if (mode ==START) {
        request.data[0] = EVENT_MANUAL_ON;
    } else if (mode == SUSPEND) {
        request.data[0] = EVENT_SUSPEND;
    } else if (mode == RESUME) {
        request.data[0] = EVENT_RESUME;
    } else if (mode == REMOTE_ON) {
        request.data[0] = EVENT_REMOTE_ON;
    } else if (mode == REMOTE_OFF) {
        request.data[0] = EVENT_REMOTE_OFF;
    } else {
        log_i("wrong activation mode");
    }

    xQueueSendToFront(xQueueStm, &request, FETCH_UWB_DELAY);
}

int SetRemote(uint16_t right_rpm, uint16_t left_rpm)
{
    spi_data_remote remote_data;
    remote_data.right_rpm = right_rpm;
    remote_data.left_rpm = left_rpm;

    spi_request request;
    request.command = SPI_CMD_REMOTE_SET;
    request.size = sizeof(remote_data);
    memcpy(request.data, &remote_data, sizeof(remote_data));

    return xQueueSendToFront(xQueueStm, &request, FETCH_UWB_DELAY);
}

void Stm32Init()
{
    xQueueStm = xQueueCreate(10, sizeof(spi_request));
    //initialise two instances of the SPIClass attached to VSPI and HSPI respectively
    vspi = new SPIClass(VSPI);

    //clock miso mosi ss
    vspi->begin(VSPI_SCLK, VSPI_MISO, VSPI_MOSI, VSPI_SS);
    pinMode(VSPI_SS, OUTPUT);

    Stm32Reset();
}

void Stm32Reset()
{
    log_i("Reset Stm32");
    uint8_t data[SPI_MAX_DATASIZE] = {0, };
    spi_response response;

    spiStm32Command(vspi, SPI_CMD_RESET_GET, data, 0, &response);
    spi_response_data_reset* reset = (spi_response_data_reset*) response.data;
    log_i("reset data: %d", reset->reset);
}

void Stm32Task(void* parameter)
{
    uint8_t data[SPI_MAX_DATASIZE] = {0, };
    spi_response response;
    BaseType_t xStatus;

    time_t t;
    WiFiUDP rtlsClient;

    log_d("==========");
    log_d("Get Status");
    spiStm32Command(vspi, SPI_CMD_STATUS, data, 0, &response);
    spi_response_data_status* status = (spi_response_data_status*) response.data;
    log_d("Status: 0x%02x", status->status);
    log_d("Status Data: 0x%02x", status->status_data);

    log_d("==========");
    log_d("Get CarNumber");
    spiStm32Command(vspi, SPI_CMD_CARNUM_GET, data, 0, &response);
    spi_response_data_carnumber* car_number = (spi_response_data_carnumber*) response.data;
    log_d("CarNumber: %d", car_number->carnumber);
    log_d("==========");

    for (;;)
    {
        spiStm32Command(vspi, SPI_CMD_LOCATION_GET, data, 0, &response);
        spi_response_data_location* location = (spi_response_data_location*) response.data;

        if(location->pos_x && location->pos_y) {
            rtlsClient.beginPacket(RTLS_HOST, RTLS_PORT);
            Protocol_position_t protocol = { PK_POSITION_NOTI, CAR, 26, carNumber, 0, };
            t = millis();
            protocol.timestamp = (int64_t)t;
            protocol.positionX = location->pos_x;
            protocol.positionY = location->pos_y;
            protocol.accX= location->acc_x;
            protocol.accY= location->acc_y;
            protocol.headAngle = location->head_angle;
            rtlsClient.write((uint8_t *)&protocol, sizeof(protocol));
            rtlsClient.endPacket();
            log_d("done send data to rtls server");
        }

        spi_request request;
        xStatus = xQueueReceive(xQueueStm, &request, FETCH_UWB_DELAY);
        if (xStatus == pdPASS)
        {
            log_i("stm squeue exist");
            spiStm32Command(vspi, request.command, request.data, request.size, &response);
            if (request.callback != NULL)
            {
                request.callback(&response);
            }
        }
    }

    vTaskDelete(NULL);
}
