#ifndef NETWORK_H
#define NETWORK_H

#include <WiFi.h>
#include <WiFiClient.h>
#include "config.h"


void WifiInit();
void WifiTask(void* parameter);
void WiFiEvent(WiFiEvent_t event);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

#endif
