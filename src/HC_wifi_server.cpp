/*
*
*
*
*
*

--> used by OverTheAir (OTA) update
  server.on("/update", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
      request->send(200, "text/plain", "OK");
    });
  
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "config.hpp"

#include "HC_wifi_defines.hpp"
#include "HC_wifi_init.hpp"
#include "HC_wifi_server.hpp"

#include "self_arduino.hpp"

#include "WiFi.h"
#include <ArduinoHttpClient.h>

#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"
#include <AsyncElegantOTA.h>
#include <WebSerial.h>

#include "SPIFFS.h"

#include <WakeOnLan.h>
#include <ESP32Ping.h>

#include "freertos/queue.h"

#include "HC_shutter_control.hpp"

#include "HC_time_keeper.hpp"

/* ---------------- HELP FUNCTION SECTION ---------------- */
void initQueue(QueueHandle_t xQueue)
{
  xQueueShutter_tx = xQueue;
}

String siteProcess(const String& var)
{
  String statement = String();
  if(var == "TIME")
  {
    statement = "last refresh: ";
    statement += getTimeString();
  }
  else if(var == "VERSION")
  {
    statement = "server version: ";
    statement += "0.0.1";
  }

  return statement;
}

void wakeMyPC() 
{
  WOL.sendMagicPacket(MAC_DESKTOP_CONFIG);
}

IPAddress getIPAddress()
{
  return WiFi.localIP();
}

/* ---------------- SERVER FUNCTION SECTION ---------------- */
void initServer(void)
{
  /* Init pins */
  digitalWrite(pinShutterDOWN, HIGH);
  pinMode(pinShutterDOWN, OUTPUT);
  digitalWrite(pinShutterUP, HIGH);
  pinMode(pinShutterUP, OUTPUT);

  /* Init Spiff: files from flash */
  initSpiffs();

  /* 
  --------------- Webpage Visuals --------------- 
  */

  /* 
  * Init HTML-File to be send to browser
  */
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
      request->send(SPIFFS, "/index.html", String(), false, siteProcess);
    });
  
  /*
  * Init CSS-File to be send to browser
  */
  server.on("/style.css", HTTP_GET, [](AsyncWebServerRequest *request)
    {
      request->send(SPIFFS, "/style.css", "text/css");
    });

  /* 
  --------------- Server Actions --------------- 
  */

  /*
  * Init WOL function on Server request
  */
  server.on("/sendWOL", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
      wakeMyPC();
      request->send(200, "text/plain", "OK");
    });

  /*
  * Init plug function on Server request
  */
  server.on("/plug", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
      String inputMessage1;

      if(request->hasParam("item"))
      {
        inputMessage1 = request->getParam("item")->value();
        
  	    if(inputMessage1 == "printer")
        {
          WiFiClient wifi;
          char serverAddress[] = "192.168.0.34";
          HttpClient client = HttpClient(wifi, serverAddress);
          client.get("/?m=1&o=1");
          client.beginRequest();
          client.endRequest();
          client.stop();
        }
        else if(inputMessage1 == "bcpu")
        {
          WiFiClient wifi;
          char serverAddress[] = "192.168.0.23";
          HttpClient client = HttpClient(wifi, serverAddress);
          client.get("/?m=1&o=1");
          client.beginRequest();
          client.endRequest();
          client.stop();
        }
        else if(inputMessage1 == "globe")
        {
          WiFiClient wifi;
          char serverAddress[] = "192.168.0.23";
          HttpClient client = HttpClient(wifi, serverAddress);
          client.get("/?m=1&o=2");
          client.beginRequest();
          client.endRequest();
          client.stop();
        }
        else if(inputMessage1 == "night_lamp")
        {
          WiFiClient wifi;
          char serverAddress[] = "192.168.0.35";
          HttpClient client = HttpClient(wifi, serverAddress);
          client.get("/?m=1&o=1");
          client.beginRequest();
          client.endRequest();
          client.stop();
        }
        else if(inputMessage1 == "desk_lamp")
        {
          WiFiClient wifi;
          char serverAddress[] = "192.168.0.33";
          HttpClient client = HttpClient(wifi, serverAddress);
          client.get("/?m=1&o=1");
          client.beginRequest();
          client.endRequest();
          client.stop();
        }
      }

      request->send(200, "text/plain", "OK");
    });

  /*
  * Init shutter function on serer request
  */
  server.on("/shutter", HTTP_GET, [] (AsyncWebServerRequest *request) 
    {
      String inputMessage1;
      String inputMessage2;

      if(request->hasParam("dir") && request->hasParam("status")) 
      {
        inputMessage1 = request->getParam("dir")->value();
        inputMessage2 = request->getParam("status")->value();

        sMsg_tx.dir = '-';
        sMsg_tx.val = SHUTTER_OFF;

        if(inputMessage1 == "shutterUP")
        {
          sMsg_tx.dir = 'U';
          if(inputMessage2 == "ON")
          {
            sMsg_tx.val = SHUTTER_ON;
          }
          else if(inputMessage2 == "OFF")
          {
            sMsg_tx.val = SHUTTER_OFF;
          }
        }
        else if(inputMessage1 == "shutterDOWN")
        {
          sMsg_tx.dir = 'D';
          if(inputMessage2 == "ON")
          {
            sMsg_tx.val = SHUTTER_ON;
          }
          else if(inputMessage2 == "OFF")
          {
            sMsg_tx.val = SHUTTER_OFF;
          }
        }

        xQueueSend(xQueueShutter_tx, (void *) &sMsg_tx, 0);
      }

      request->send(200, "text/plain", "OK");
    });

  /* 
  --------------- Start services ---------------
  */
  WebSerial.begin(&server);
  AsyncElegantOTA.begin(&server);
  server.begin();
}

void loopback_request(String request)
{
    WiFiClient wifi;
    char serverAddress[] = "192.168.0.200";
    HttpClient client = HttpClient(wifi, serverAddress);
    client.get(request);
    client.beginRequest();
    client.endRequest();
    client.stop();
}
