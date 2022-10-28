#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <WiFiClient.h>
#include "config.h"

extern char hostName[32];
extern uint8_t carNumber;

void WifiInit();
void WifiTask(void* parameter);
void WiFiEvent(WiFiEvent_t event);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

#endif
