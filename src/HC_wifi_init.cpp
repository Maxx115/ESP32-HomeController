/* --------------- INCLUDE SECTION ---------------- */
#include "config.hpp"

#include "HC_wifi_defines.hpp"
#include "HC_wifi_init.hpp"

#include "WiFi.h"
#include <ArduinoHttpClient.h>

#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <AsyncElegantOTA.h>
#include <WebSerial.h>

#include "SPIFFS.h"

#include <WakeOnLan.h>
#include <ESP32Ping.h>



void initSpiffs(void)
{
  if(!SPIFFS.begin(true))
  {
    //Serial.println("An Error has occurred while mounting SPIFFS");
  }
}

void initWifi(void)
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

void onEvent(AsyncWebSocket *server, AsyncWebSocketClient *client, AwsEventType type,
             void *arg, uint8_t *data, size_t len) {
  switch (type) {
    case WS_EVT_CONNECT:
      //Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
      break;
    case WS_EVT_DISCONNECT:
      //Serial.printf("WebSocket client #%u disconnected\n", client->id());
      break;
    case WS_EVT_DATA:
      //handleWebSocketMessage(arg, data, len);
      break;
    case WS_EVT_PONG:
    case WS_EVT_ERROR:
      break;
  }
}

void initWebSocket(void)
{
  ws.onEvent(onEvent);
  server.addHandler(&ws);

  //Serial.println("Websocket-Setup done.");
}