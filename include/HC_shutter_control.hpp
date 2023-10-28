/*
*
*
*
*
*
*/

#ifndef HC_SHUTTER_API
#define HC_SHUTTER_API

#include "self_arduino.hpp"

#define SHUTTER_TIMER 21000

#define pinShutterUP    32
#define pinShutterDOWN  25
#define SHUTTER_OFF HIGH
#define SHUTTER_ON LOW

void shutterInit();
void initShutter(void);
void shutterCMD(String direction, String state);

void shutterTask(void *pvParameter);
void vTimerCallback_ShutterOff(TimerHandle_t xShutterOff);

#endif /* HC_SHUTTER_API */