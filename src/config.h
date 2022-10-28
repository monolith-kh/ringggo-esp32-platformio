#ifndef CONFIG_H
#define CONFIG_H

#define VERSION "0.1.30"

#ifdef DEV
    #define WIFI_SSID       "RINGGGO-dev"
    #define WIFI_PASSWORD   "mono1010"
    #define GAME_HOST       "manunited.ringggo.981park.net"
    #define GAME_PORT       9998
    #define RTLS_HOST       "oldtrafford.ringggo.981park.net"
    #define RTLS_PORT       9999
    #define OTA_PORT        9812
    #define OTA_PASSWORD    "ringggo"
#else
    #define WIFI_SSID       "RINGGGO"
    #define WIFI_PASSWORD   "mono1010"
    #define GAME_HOST       "jejupark-server.ringggo.981park.com"
    #define GAME_PORT       9998
    #define RTLS_HOST       "jejupark-rtls.ringggo.981park.com"
    #define RTLS_PORT       9999
    #define OTA_PORT        9812
    #define OTA_PASSWORD    "ringggo"
#endif

#endif