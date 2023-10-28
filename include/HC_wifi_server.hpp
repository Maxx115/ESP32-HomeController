/*
*
*
*
*
*
*/

#ifndef HC_WIFI_SERVER
#define HC_WIFI_SERVER

/* ---------------- INCLUDES ---------------- */
#include "self_arduino.hpp"
#include "freertos/queue.h"

/* ---------------- DEFINES / CONSTANTs ---------------- */
#define HTTP_PORT 80

/* ---------------- FUNCTION PROTOTYPES ---------------- */
//void initWifi(void);
//void initServer(void);
//void initQueue(QueueHandle_t xQueue);
void loopback_request(String request);

void wakeMyPC();

IPAddress getIPAddress();

#endif /* HC_WIFI_SERVER */
