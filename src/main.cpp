#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

#include "packet.h"

#include "stm32.h"
#include "network.h"
#include "led.h"
#include "mp3.h"
#include "nfc.h"
#include "battery.h"
#include "bumper.h"
#include "gameserver.h"
#include "config.h"


xQueueHandle xQueueLed, xQueueMp3;
CRGB leds[NUM_LEDS];
DFRobotDFPlayerMini mp3Player;
int volume;
Adafruit_PN532 nfc(PN532_SCK, PN532_MISO, PN532_MOSI, PN532_SS);
float g_fBatteryVolt; 
uint8_t g_dBatteryValue;
WiFiClient gameClient;
SPIClass* vspi;
char hostName[32] = { 0, };
uint8_t carNumber;
unsigned long healthcheckTime;
unsigned long gamePlayTime;
unsigned long nowTime;
boolean inGame;
uint32_t colorList[9];

// led
uint8_t ledIndex;
uint8_t type;
uint16_t ledTime;
uint32_t color;

void setup(void)
{
    Serial.begin(115200);

    log_i("Hello RINGGGO");

    Stm32Init();

    carNumber = 0;
    while(carNumber == 0)
    {
        carNumber = GetCarNumber();
        log_i("Car Number: %d", carNumber);
    }
    Stm32Reset();

#ifdef DEV
    sprintf(hostName, "CAR_%d(%s)DEV", carNumber, VERSION);
#else
    sprintf(hostName, "CAR_%d(%s)", carNumber, VERSION);
#endif
    log_i("hostname: %s", hostName);

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
        GameServerTask,
        "GameServerTask",
        10000,
        NULL,
        9,
        NULL);

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

    ArduinoOTA.setPort(OTA_PORT);
    ArduinoOTA.setHostname(hostName);
    ArduinoOTA.setPassword(OTA_PASSWORD);

    ArduinoOTA.onStart([]() {
        String type;
        if (ArduinoOTA.getCommand() == U_FLASH) {
            type = "sketch";
        } else {  // U_FS
            type = "filesystem";
        }

        // NOTE: if updating FS this would be the place to unmount FS using FS.end()
        Serial.println("Start updating " + type);
    });
    ArduinoOTA.onEnd([]() {
        Serial.println("\nEnd");
    });
    ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    });
    ArduinoOTA.onError([](ota_error_t error) {
        Serial.printf("Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR) {
            Serial.println("Auth Failed");
        } else if (error == OTA_BEGIN_ERROR) {
            Serial.println("Begin Failed");
        } else if (error == OTA_CONNECT_ERROR) {
            Serial.println("Connect Failed");
        } else if (error == OTA_RECEIVE_ERROR) {
            Serial.println("Receive Failed");
        } else if (error == OTA_END_ERROR) {
            Serial.println("End Failed");
        }
    });
    ArduinoOTA.begin();

}


void loop(void)
{
    ArduinoOTA.handle();

    nowTime = millis();
    if (inGame && ((nowTime - gamePlayTime) > GAME_PLAYTIME)) {
        log_i("stop game by timer(180 seconds)");
        GameStop();
    }
    log_d("timestamp: %lu ms", nowTime);
    vTaskDelay(1000);
}
