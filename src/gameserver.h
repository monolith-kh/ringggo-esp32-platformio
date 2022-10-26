#ifndef GAMESERVER_H
#define GAMESERVER_H

#define STOP      0
#define START     1
#define SUSPEND   2
#define RESUME    3

#define RECONNECT_DELAY     10000

#include <WiFiClient.h>
#include "packet.h"
#include "led.h"
#include "mp3.h"
#include "bumper.h"
#include "battery.h"
#include "stm32.h"
#include "config.h"

extern WiFiClient gameClient;
extern uint8_t carNumber;
extern unsigned long currentTime;
extern unsigned long healthcheckTime;

void GameServerInit();
void GameServerTask(void* parameter);
void SendBattery(uint8_t percentage);
void SendNfc(char wrbId[], uint8_t uid[]);
void SendBumper(int bumper);

#endif
