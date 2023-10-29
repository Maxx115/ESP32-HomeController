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

void sendClientRequest(String ipAdress, String clientGet)
{
    WiFiClient wifi;
    HttpClient client = HttpClient(wifi, ipAdress);

    client.get(clientGet);
    client.beginRequest();
    client.endRequest();
    client.stop();
}

void loopback_request(String request)
{
    sendClientRequest("192.168.0.200", request);
}
