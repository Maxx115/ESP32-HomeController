/*
*
*
*
*
*
*/

#ifndef HC_WIFI_INTERFACE
#define HC_WIFI_INTERFACE

#include "self_arduino.hpp"

void wifiInit();
void wakeMyPC();
void sendClientRequest(String ipAdress, String clientGet);
void loopback_request(String request);

#endif /* HC_WIFI_INTERFACE */