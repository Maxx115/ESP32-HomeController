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

#include "WIFI_CONFIG.hpp"

#define GET_BODY true

wl_status_t wifiInit(uint8_t ip_device[] = HC_IP_DEVICE, uint8_t ip_gateway[] = HC_IP_GATEWAY, uint8_t ip_subnet[] = HC_IP_SUBNET, uint8_t ip_dns[] = HC_IP_DNS, String hostname = HOSTNAME, 
                     String ssid = SSID_CONFIG, String password = PASSWORD_CONFIG);
bool wakeMyPC(void);
String sendDeviceRequest(tasmota_device device, String clientGet, boolean getBody = false);
String sendDeviceStatusRequest(tasmota_device device);
void loopback_request(String request);

#endif /* HC_WIFI_INTERFACE */