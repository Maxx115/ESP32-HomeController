/*
*
*
*
*
*
*/

#ifndef HC_WIFI_INIT
#define HC_WIFI_INIT

/* ---------------- INCLUDES ---------------- */
#include "self_arduino.hpp"
#include "freertos/queue.h"

/* ---------------- FUNCTION PROTOTYPES ---------------- */
void initWifi(void);
void initServer(void);
void initQueue(QueueHandle_t xQueue);
void initSpiffs(void);
void initQueue(QueueHandle_t xQueue);

#endif /* HC_WIFI_INIT */