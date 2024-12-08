#include "HC_WIFI_INTERFACE.hpp"
#include "HC_DEVICES.hpp"

#include <WakeOnLan.h>

#include "WiFi.h"
#include <ArduinoHttpClient.h>

WiFiUDP UDP;
WakeOnLan WOL(UDP);

ScanResult ssidResult;

ScanResult scanSSID()
{
  Serial.println("Scan start");
  int numNetworks = WiFi.scanNetworks();
  Serial.println("Scan done");

  ssidResult.numNetworks = numNetworks;

  if (numNetworks == 0) 
  {
      Serial.println("No networks found");
      ssidResult.ssids = nullptr;
  } else 
  {
      Serial.print(numNetworks);
      Serial.println(" networks found");
      Serial.println("Nr | SSID | RSSI | CH | Encryption");

      // Create a string array to hold the SSIDs
      ssidResult.ssids = new String[numNetworks];
      ssidResult.rssis = new int[numNetworks];

      for (int i = 0; i < ssidResult.numNetworks; ++i) 
      {
          ssidResult.ssids[i] = WiFi.SSID(i);
          ssidResult.rssis[i] = WiFi.RSSI(i);

          Serial.printf("%2d | %-32.32s | %4ld | %2ld | ", i + 1, WiFi.SSID(i).c_str(), WiFi.RSSI(i), WiFi.channel(i));
          switch (WiFi.encryptionType(i)) 
          {
              case WIFI_AUTH_OPEN: Serial.print("open"); break;
              case WIFI_AUTH_WEP: Serial.print("WEP"); break;
              case WIFI_AUTH_WPA_PSK: Serial.print("WPA"); break;
              case WIFI_AUTH_WPA2_PSK: Serial.print("WPA2"); break;
              case WIFI_AUTH_WPA_WPA2_PSK: Serial.print("WPA+WPA2"); break;
              case WIFI_AUTH_WPA2_ENTERPRISE: Serial.print("WPA2-EAP"); break;
              case WIFI_AUTH_WPA3_PSK: Serial.print("WPA3"); break;
              case WIFI_AUTH_WPA2_WPA3_PSK: Serial.print("WPA2+WPA3"); break;
              case WIFI_AUTH_WAPI_PSK: Serial.print("WAPI"); break;
              default: Serial.print("unknown");
          }
          Serial.println();
      }
      Serial.println();
      WiFi.scanDelete();
      return ssidResult;
    }
}

wl_status_t wifiInit(uint8_t ip_device[], uint8_t ip_gateway[], uint8_t ip_subnet[], uint8_t ip_dns[], String hostname, 
                     String ssid, String password)
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
    WiFi.enableSTA(true);
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

  WiFi.printDiag(Serial);
  Serial.print("Home Controller IP: ");
  Serial.println(WiFi.localIP());

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
