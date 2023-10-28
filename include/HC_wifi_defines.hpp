/*
*
*
*
*
*
*/

#ifndef HC_WIFI_DEFINES
#define HC_WIFI_DEFINES

#include "WiFi.h"
#include "ESPAsyncWebServer.h"
#include <WakeOnLan.h>
#include "freertos/queue.h"
#include "HC_shutter_control.hpp"

extern IPAddress local_IP;
extern IPAddress gateway;
extern IPAddress subnet;
extern IPAddress dns;
extern String hostname;

extern AsyncWebServer server;

extern QueueHandle_t xQueueShutter_tx;

extern AsyncWebSocket ws;

extern WakeOnLan WOL;

extern shutterMsg sMsg_tx;

#endif /* HC_WIFI_DEFINES */