#ifndef GAMESERVER_H
#define GAMESERVER_H

#define GAME_HOST   "manunited.ringggo.981park.net"
// #define GAME_HOST   "192.168.41.254"
#define GAME_PORT   9998

#define STOP      0
#define START     1
#define SUSPEND   2
#define RESUME    3

#include <WiFiClient.h>
#include "packet.h"
#include "led.h"
#include "mp3.h"
#include "bumper.h"
#include "battery.h"

extern WiFiClient gameClient;

void GameServerInit();
void GameServerTask(void* parameter);
void SendBattery(uint8_t percentage);
void SendNfc(char wrbId[], uint8_t uid[]);
void SendBumper(int bumper);

#endif
