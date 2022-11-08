#include "network.h"

void WifiInit()
{
    log_i("Init Wifi");

    WiFi.disconnect(true);
    WiFi.mode(WIFI_STA);
    WiFi.onEvent(WiFiEvent);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFiEventId_t eventID = WiFi.onEvent([](WiFiEvent_t event, WiFiEventInfo_t info) {
        log_w("WiFi lost connection. Reason: %d", info.wifi_sta_disconnected.reason);
        }, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);


# ifdef DEV
# else
    IPAddress localIp(192, 168, 40, 200 + carNumber);
    IPAddress gateway(192, 168, 40, 1);
    IPAddress subnet(255, 255, 255, 0);
    IPAddress primaryDNS(1, 1, 1, 1);
    IPAddress secondaryDNS(8, 8, 8, 8);
    if(!WiFi.config(localIp, gateway, subnet, primaryDNS, secondaryDNS)) {
        log_e("STA Failed to configure");
    }
# endif
    WiFi.setHostname(hostName);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    log_i("WiFi connected");
    log_i("IP address: ");
    Serial.println(IPAddress(WiFi.localIP()));
}

void WifiTask(void* parameter)
{
   
    for (;;)
    {
        if(WiFi.status() == WL_CONNECTED)
        {
            log_i("Wifi online / RSSI: %d", WiFi.RSSI());
            vTaskDelay(10000 / portTICK_PERIOD_MS);
            continue;
        }
    }
    vTaskDelete(NULL);
}

void WiFiEvent(WiFiEvent_t event)
{
    log_i("[WiFi-event] event: %d\n", event);

    switch (event) {
    case SYSTEM_EVENT_WIFI_READY:
        log_i("WiFi interface ready");
        break;
    case SYSTEM_EVENT_SCAN_DONE:
        log_i("Completed scan for access points");
        break;
    case SYSTEM_EVENT_STA_START:
        log_i("WiFi client started");
        break;
    case SYSTEM_EVENT_STA_STOP:
        log_i("WiFi clients stopped");
        break;
    case SYSTEM_EVENT_STA_CONNECTED:
        log_i("Connected to access point");
        break;
    case SYSTEM_EVENT_STA_DISCONNECTED:
        log_i("Disconnected from WiFi access point");
        break;
    case SYSTEM_EVENT_STA_AUTHMODE_CHANGE:
        log_i("Authentication mode of access point has changed");
        break;
    case SYSTEM_EVENT_STA_GOT_IP:
        log_i("Obtained IP address: ");
        Serial.println(IPAddress(WiFi.localIP()));
        break;
    case SYSTEM_EVENT_STA_LOST_IP:
        log_i("Lost IP address and IP address is reset to 0");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_SUCCESS:
        log_i("WiFi Protected Setup (WPS): succeeded in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_FAILED:
        log_i("WiFi Protected Setup (WPS): failed in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_TIMEOUT:
        log_i("WiFi Protected Setup (WPS): timeout in enrollee mode");
        break;
    case SYSTEM_EVENT_STA_WPS_ER_PIN:
        log_i("WiFi Protected Setup (WPS): pin code in enrollee mode");
        break;
    case SYSTEM_EVENT_AP_START:
        log_i("WiFi access point started");
        break;
    case SYSTEM_EVENT_AP_STOP:
        log_i("WiFi access point  stopped");
        break;
    case SYSTEM_EVENT_AP_STACONNECTED:
        log_i("Client connected");
        break;
    case SYSTEM_EVENT_AP_STADISCONNECTED:
        log_i("Client disconnected");
        break;
    case SYSTEM_EVENT_AP_STAIPASSIGNED:
        log_i("Assigned IP address to client");
        break;
    case SYSTEM_EVENT_AP_PROBEREQRECVED:
        log_i("Received probe request");
        break;
    case SYSTEM_EVENT_GOT_IP6:
        log_i("IPv6 is preferred");
        break;
    case SYSTEM_EVENT_ETH_START:
        log_i("Ethernet started");
        break;
    case SYSTEM_EVENT_ETH_STOP:
        log_i("Ethernet stopped");
        break;
    case SYSTEM_EVENT_ETH_CONNECTED:
        log_i("Ethernet connected");
        break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
        log_i("Ethernet disconnected");
        break;
    case SYSTEM_EVENT_ETH_GOT_IP:
        log_i("Obtained IP address");
        break;
    default: break;
    }
}

void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info)
{
    log_i("WiFi connected");
    log_i("IP address: ");
    Serial.println(IPAddress(info.got_ip.ip_info.ip.addr));
}
