#include "HC_MOTION_CONTROL.hpp"

#include "HC_DEVICES.hpp"
#include "HC_WIFI_INTERFACE.hpp"

#include "self_arduino.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define MOTION_POLL_DELAY_MS 250
#define MOTION_OFF_DELAY_MS 60000
#define MOTION_OFF_DELAY_SEC MOTION_OFF_DELAY_MS/1000
#define MOTION_OFF_DELAY_COUNTS (1000*MOTION_OFF_DELAY_SEC)/MOTION_POLL_DELAY_MS

void motionTask(void *pvParameters)
{
  const int PIN_TO_SENSOR = 26;
  int pinStateCurrent = LOW;
  int pinStatePrevious = LOW;
  unsigned int motionStopCount = 0;

  pinMode(PIN_TO_SENSOR, INPUT);

  for(;;)
  {
    pinStatePrevious = pinStateCurrent;
    pinStateCurrent = digitalRead(PIN_TO_SENSOR);

    while(DESKLAMP.motion == MOTION_INACTIVE){ vTaskDelay(MOTION_POLL_DELAY_MS); }

    if(pinStateCurrent == HIGH)
    {
      motionStopCount = 0;
      if(DESKLAMP.motion == MOTION_ACTIVE && DESKLAMP.status == "OFF")
      {
        sendDeviceRequest(DESKLAMP, TASMOTA_ON);
        vTaskDelay(MOTION_OFF_DELAY_MS);
      }
    }
    else if(pinStateCurrent == LOW)
    {
      if(DESKLAMP.status == "ON") motionStopCount++;
      else motionStopCount = 0;
      if(DESKLAMP.motion == MOTION_ACTIVE && DESKLAMP.status == "ON" && motionStopCount == MOTION_OFF_DELAY_COUNTS)
      {
        sendDeviceRequest(DESKLAMP, TASMOTA_OFF);
      }
    }

    vTaskDelay(MOTION_POLL_DELAY_MS);
  }
}
