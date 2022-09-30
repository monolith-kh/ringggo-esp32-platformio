#include <Arduino.h>
#include <Wire.h>

#include "packet.h"

#include "stm32.h"
#include "network.h"
#include "led.h"
#include "mp3.h"
#include "nfc.h"
#include "battery.h"
#include "bumper.h"
#include "gameserver.h"

// xQueueHandle xQueueLed, xQueueMp3;
xQueueHandle xQueueLed, xQueueMp3, xQueueBumper, xQueueBattery;
CRGB leds[NUM_LEDS];
DFRobotDFPlayerMini mp3Player;
int volume;
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
float g_fBatteryVolt; 
uint8_t g_dBatteryValue;
WiFiClient gameClient;


void setup(void)
{
    Serial.begin(115200);

    log_i("Hello RINGGGO");

    WifiInit();

    xTaskCreate(
        WifiTask,
        "WifiTask",
        10000,
        NULL,
        10,
        NULL
    );

    xTaskCreate(
        LedTask,
        "LedTask",
        10000,
        NULL,
        9,
        NULL
    );

    xTaskCreate(
        Mp3Task,
        "Mp3Task",
        10000,
        NULL,
        8,
        NULL
    );

    xTaskCreate(
        NfcTask,
        "NfcTask",
        10000,
        NULL,
        8,
        NULL
    );

    xTaskCreate(
        BatteryTask,
        "BatteryTask",
        10000,
        NULL,
        7,
        NULL
    );

    xTaskCreate(
        GameServerTask,         // Task 함수 이름
        "GameServerTask",       // Task 이름
        10000,                  // Task 스택 크기
        NULL,                   // Task 파라미터
        9,                      // Task 우선순위
        NULL);                  // Task handle

    // xTaskCreate(
    //     GameServerSendTask,         // Task 함수 이름
    //     "GameServerSendTask",       // Task 이름
    //     10000,                  // Task 스택 크기
    //     NULL,                   // Task 파라미터
    //     9,                      // Task 우선순위
    //     NULL);                  // Task handle

    xTaskCreate(
        BumperTask,
        "BumperTask",
        10000,
        NULL,
        2,
        NULL
    );

    xTaskCreate(
        Stm32Task,
        "stm32Task",
        10000,
        NULL,
        1,
        NULL
    );

}


void loop(void)
{

}

