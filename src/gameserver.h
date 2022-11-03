#ifndef GAMESERVER_H
#define GAMESERVER_H

#define STOP      0
#define START     1
#define SUSPEND   2
#define RESUME    3

#define GAME_PLAYTIME       180000+10000
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
extern unsigned long gamePlayTime;
extern unsigned long healthcheckTime;
extern boolean inGame;

void GameServerInit();
void GameServerTask(void* parameter);
void SendBattery(uint8_t percentage);
void SendNfc(char wrbId[], uint8_t uid[]);
void SendBumper(int bumper);
void GameStart();
void GameStop();

#endif
