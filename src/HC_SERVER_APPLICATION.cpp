#include "HC_SERVER_APPLICATION.hpp"

#include "HC_DEVICES.hpp"
#include "HC_SHUTTER_CONTROL.hpp"
#include "HC_WIFI_INTERFACE.hpp"

#include "HC_time_keeper.hpp"

#include "SPIFFS.h"
#include "ESPAsyncWebServer.h"
#include <AsyncElegantOTA.h>

AsyncWebServer server(80);

void initSpiffs(void)
{
  if(!SPIFFS.begin(true))
  {
    //Serial.println("An Error has occurred while mounting SPIFFS");
  }
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
    statement += "0.0.3";
  }

  return statement;
}

void serverInit()
{
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
                sendClientRequest(TASMOTA_PRINTER, TASMOTA_POWER_1 + TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "bcpu")
            {
                sendClientRequest(TASMOTA_BCPU, TASMOTA_POWER_1 + TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "globe")
            {
                sendClientRequest(TASMOTA_GLOBE, TASMOTA_POWER_2 + TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "night_lamp")
            {
                sendClientRequest(TASMOTA_NIGHTLAMP, TASMOTA_POWER_1 + TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "desk_lamp")
            {
                sendClientRequest(TASMOTA_DESKLAMP, TASMOTA_POWER_1 + TASMOTA_TOGGLE);
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

            shutterCMD(inputMessage1, inputMessage2);
        }
        
        request->send(200, "text/plain", "OK");
    });

    /* 
    --------------- Start services ---------------
    */
    AsyncElegantOTA.begin(&server);
    server.begin();
}