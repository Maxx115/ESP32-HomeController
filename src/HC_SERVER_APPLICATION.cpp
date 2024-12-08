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
    statement += "0.0.5";
  }
  else if(var == "SSID_LIST")
  {
    statement = "";
    statement += "<thead>";
    statement += "<tr>";

    statement += "<th>ID</th>";
    statement += "<th>SSID</th>";
    statement += "<th>RSSI</th>";

    statement += "</tr>";
    statement += "</thead>";

    statement += "<tbody>";

    for(int i = 0; i < ssidResult.numNetworks; i++)
    {
        statement += "<tr>";

        statement += "<td>";
        statement += i;
        statement += "</td>";

        statement += "<td>";
        statement += ssidResult.ssids[i];
        statement += "</td>";

        statement += "<td>";
        statement += ssidResult.rssis[i];
        statement += "</td>";

        statement += "</tr>";

        if(i >= 5)
        {
            break;
        }
    }

    statement += "</tbody>";
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
    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
        request->send(SPIFFS, "/setupPage.html", String(), false, siteProcess);
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

    server.on("/wifi", HTTP_GET, [](AsyncWebServerRequest *request)
    { 
        request->send(200, "text/plain", "OK");
        request->send(SPIFFS, "/index.html", String(), false, siteProcess);
        String inputMessage1 = "";
        String inputMessage2 = "";
        String inputMessage3 = "";

        if(request->hasParam("item")) 
        {
            inputMessage1 = request->getParam("item")->value();
            
            if(inputMessage1 == "wifi_connect")
            {
                if(request->hasParam("item"))
                {
                    inputMessage2 = request->getParam("ssid")->value();
                }
                if(request->hasParam("password"))
                {
                    inputMessage3 = request->getParam("password")->value();
                }
            }
        }

        Serial.println("WiFi Request Receivced");
        Serial.print("SSID: ");
        Serial.println(inputMessage2);
        Serial.print("Password: ");
        Serial.println(inputMessage3);

    });

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
        request->send(200, "text/plain", "OK");
        String inputMessage1;

        if(request->hasParam("item"))
        {
            inputMessage1 = request->getParam("item")->value();
            
            if(inputMessage1 == "printer")
            {
                sendDeviceRequest(PRINTER, TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "bcpu")
            {
                sendDeviceRequest(BCPU, TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "globe")
            {
                sendDeviceRequest(GLOBE, TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "night_lamp")
            {
                sendDeviceRequest(NIGHTLAMP, TASMOTA_TOGGLE);
            }
            else if(inputMessage1 == "desk_lamp")
            {
                if(DESKLAMP.status == "OFF") 
                {
                    DESKLAMP.status = "ON";
                    DESKLAMP.motion = true;
                }
                else if(DESKLAMP.status == "ON")
                {
                    DESKLAMP.status = "OFF";
                    DESKLAMP.motion = false;
                }

                sendDeviceRequest(DESKLAMP, TASMOTA_TOGGLE);
            }
        }
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