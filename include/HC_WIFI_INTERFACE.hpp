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
#include "WiFi.h"

#define GET_BODY true

wl_status_t wifiInit(uint8_t ip_device[], uint8_t ip_gateway[], uint8_t ip_subnet[], uint8_t ip_dns[], String hostname, String ssid, String password);
bool wakeMyPC(void);
String sendDeviceRequest(tasmota_device device, String clientGet, boolean getBody = false);
String sendDeviceStatusRequest(tasmota_device device);
void loopback_request(String request);

#endif /* HC_WIFI_INTERFACE */