#include "HC_WIFI_INTERFACE.hpp"
#include "WIFI_CONFIG.hpp"

#include <WakeOnLan.h>

#include "WiFi.h"
#include <ArduinoHttpClient.h>

IPAddress local_IP(HC_IP_DEVICE);
IPAddress gateway(HC_IP_GATEWAX);
IPAddress subnet(HC_IP_SUBNET);
IPAddress dns(HC_IP_DNS);
String hostname = HOSTNAME;

WiFiUDP UDP;
WakeOnLan WOL(UDP);

void wifiInit(void)
{
  if(!WiFi.config(local_IP, gateway, subnet, dns))
  {
    //Serial.println("STA failed to configure");
  }
  else
  {
    WiFi.setHostname(hostname.c_str());
  }

  WiFi.mode(WIFI_STA);

  WiFi.begin(SSID_CONFIG, PASSWORD_CONFIG);
  //Serial.print("Connecting to ");
  //Serial.print(ssid);

  while(WiFi.status() != WL_CONNECTED)
  {
    //Serial.print('.');
    delay(1000);
  }  
  WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());

  //Serial.println(WiFi.localIP());
  //Serial.print("RRSI: ");
  //Serial.println(WiFi.RSSI());
  //Serial.println("Wifi-Setup done.");
}

void wakeMyPC() 
{
  WOL.sendMagicPacket(MAC_DESKTOP_CONFIG);
}

String sendClientRequest(String ipAdress, String clientGet, boolean getBody)
{
    String body = "";
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, ipAdress);

    client.get(clientGet);
    if(getBody)
    {
      client.responseStatusCode();
      body = client.responseBody();
    }
    client.stop();

    return body;
}

String sendDeviceRequest(tasmota_device device, String clientGet, boolean getBody)
{
  sendQueueStatusRequest(device);
  return sendClientRequest(device.ip, device.power + clientGet, getBody);
}

String sendDeviceStatusRequest(tasmota_device device)
{
  return sendClientRequest(device.ip, device.power, true);
}

void loopback_request(String request)
{
    sendClientRequest(HC_IP_DEVICE_STR, request);
}
