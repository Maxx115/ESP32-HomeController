/*
*
*
*
*
*
*/
#include "HC_ALEXA_MAN.hpp"

#include <WiFi.h>
#include "fauxmoESP.h"

#include "HC_WIFI_INTERFACE.hpp"
#include "HC_DEVICES.hpp"

fauxmoESP fauxmo;

QueueHandle_t xQueueControlRequest;
#define DEFAULT_QUEUE_SIZE 20
struct queAlexa
{
    String deviceName;
    bool state;
};
queAlexa stateChange = {"", false};

void deviceControlCallback(unsigned char deviceid, const char * device_name, bool state, unsigned char value)
{
    //Serial.printf("Device %s to state %s\n", device_name, state ? "ON" : "OFF");
    queAlexa stateChange_temp = {device_name, state};
    xQueueSend(xQueueControlRequest, (void *) &stateChange_temp, 0);
}

void fauxmoTask(void *pvParameter)
{
    Serial.println("Starting Alexa connection...");

    tasmota_device nextDevice = getNextDevice();
    while(nextDevice.id > 0)
    {
        Serial.printf("Adding %s\n", nextDevice.DeviceName.c_str());
        fauxmo.addDevice(nextDevice.DeviceName.c_str());
        nextDevice = getNextDevice(nextDevice.id);
    }


    fauxmo.onSetState(deviceControlCallback);
    fauxmo.createServer(false);
    fauxmo.setPort(80);
    fauxmo.enable(true);

    xQueueControlRequest = xQueueCreate(DEFAULT_QUEUE_SIZE, sizeof(queAlexa));

    for(;;)
    {
        fauxmo.handle();
        
        if(xQueueReceive(xQueueControlRequest, &stateChange, 0) == pdTRUE)
        {
            if(stateChange.deviceName == COMPUTER.DeviceName)
            {   
                if(stateChange.state) wakeMyPC();
            }
            else
            {
                sendDeviceRequest(stateChange.deviceName, (stateChange.state ? TASMOTA_ON : TASMOTA_OFF));
            }

            /*
            if(stateChange.deviceName == BCPU.DeviceName)
            {
                sendDeviceRequest(stateChange.deviceName, (stateChange.state ? TASMOTA_ON : TASMOTA_OFF));
            }
            else if(stateChange.deviceName == LIVINGROOMLAMP.DeviceName)
            {
                sendDeviceRequest(LIVINGROOMLAMP, (stateChange.state ? TASMOTA_ON : TASMOTA_OFF));
            }
            else if(stateChange.deviceName == PRINTER.DeviceName)
            {
                sendDeviceRequest(PRINTER, (stateChange.state ? TASMOTA_ON : TASMOTA_OFF));
            }
            else if(stateChange.deviceName == COMPUTER.DeviceName)
            {   
                if(stateChange.state) wakeMyPC();
            }
            */

        }

    }
}
