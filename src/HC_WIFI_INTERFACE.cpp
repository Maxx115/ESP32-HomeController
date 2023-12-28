#include "HC_WIFI_INTERFACE.hpp"
#include "WIFI_CONFIG.hpp"

#include <WakeOnLan.h>

#include "WiFi.h"
#include <ArduinoHttpClient.h>



WiFiUDP UDP;
WakeOnLan WOL(UDP);

wl_status_t wifiInit(uint8_t ip_device[], uint8_t ip_gateway[], uint8_t ip_subnet[], uint8_t ip_dns[], String hostname, String ssid, String password)
{
  IPAddress ipAddress_device(ip_device);
  IPAddress ipAddress_gateway(ip_gateway);
  IPAddress ipAddress_subnet(ip_subnet);
  IPAddress ipAddress_dns(ip_dns);

  wl_status_t wifiReturn = WL_DISCONNECTED;

  if(!WiFi.config(ipAddress_device, ipAddress_gateway, ipAddress_subnet, ipAddress_dns))
  {
    //Serial.println("STA failed to configure");
  }
  else
  {
    WiFi.setHostname(hostname.c_str());
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());
    
    for(int i = 0; (i < 50) && (WiFi.status() != WL_CONNECTED); i++)
    {
      delay(100);
    }
    wifiReturn = WiFi.status();

    if(wifiReturn == WL_CONNECTED)
    {
      WOL.calculateBroadcastAddress(WiFi.localIP(), WiFi.subnetMask());
    }
  }

  return wifiReturn;
}

bool wakeMyPC() 
{
  bool retValue = false;

  if(WiFi.status() == WL_CONNECTED)
  {
    retValue = WOL.sendMagicPacket(MAC_DESKTOP_CONFIG);
  }
  
  return retValue;
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
  
}
