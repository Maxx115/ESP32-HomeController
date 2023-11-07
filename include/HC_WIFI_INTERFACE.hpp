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
#include "HC_DEVICES.hpp"

#define GET_BODY true

void wifiInit();
void wakeMyPC();
String sendClientRequest(String ipAdress, String clientGet, boolean getBody = false);
String sendDeviceRequest(tasmota_device device, String clientGet, boolean getBody = false);
String sendDeviceStatusRequest(tasmota_device device);
void loopback_request(String request);

#endif /* HC_WIFI_INTERFACE */