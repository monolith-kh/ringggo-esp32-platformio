#ifndef NETWORK_H
#define NETWORK_H

#define WIFI_SSID      "ringo-dev"
#define WIFI_PASSWORD  "mono1010"


#include <WiFi.h>
#include <WiFiClient.h>


void WifiInit();
void WifiTask(void* parameter);
void WiFiEvent(WiFiEvent_t event);
void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info);

#endif
