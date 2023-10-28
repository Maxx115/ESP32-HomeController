#include "HC_SHUTTER_CONTROL.hpp"

#include "self_arduino.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
struct shutterMsg
{
    char dir = '0';
    uint8_t val = SHUTTER_OFF;
};

TimerHandle_t xShutterOff_timer;
QueueHandle_t xQueueShutter;
shutterMsg sMsg_rx;
shutterMsg sMsg_tx;

void shutterInit()
{
      /* Init pins */
    digitalWrite(pinShutterDOWN, HIGH);
    pinMode(pinShutterDOWN, OUTPUT);
    digitalWrite(pinShutterUP, HIGH);
    pinMode(pinShutterUP, OUTPUT);

    sMsg_rx.dir = '0';
    sMsg_rx.val = SHUTTER_OFF;
    xQueueShutter = xQueueCreate(1, sizeof(shutterMsg));

    xShutterOff_timer = xTimerCreate("ShutterOff", SHUTTER_TIMER, pdFALSE, ( void *) 0, vTimerCallback_ShutterOff);
    xTimerStart(xShutterOff_timer, 0);
}

void shutterUP(uint8_t status)
{
  if(digitalRead(pinShutterDOWN))
  {
    digitalWrite(pinShutterUP, status);
  }
}
void shutterDOWN(uint8_t status)
{
  if(digitalRead(pinShutterUP))
  {
    digitalWrite(pinShutterDOWN, status);
  }
}
void shutterOFF(void)
{
  shutterUP(SHUTTER_OFF);
  shutterDOWN(SHUTTER_OFF);
}

void shutterControl(char dir, uint8_t val)
{
    if(dir == 'U')
    {
        shutterUP(val);
    }
    else if(dir == 'D')
    {
        shutterDOWN(val);
    }
}



void shutterCMD(String direction, String state)
{
    sMsg_tx.dir = '-';
    sMsg_tx.val = SHUTTER_OFF;

    if(direction == "shutterUP")
    {
        sMsg_tx.dir = 'U';
        if(state == "ON")
        {
        sMsg_tx.val = SHUTTER_ON;
        }
        else if(state == "OFF")
        {
        sMsg_tx.val = SHUTTER_OFF;
        }
    }
    else if(direction == "shutterDOWN")
    {
        sMsg_tx.dir = 'D';
        if(state == "ON")
        {
        sMsg_tx.val = SHUTTER_ON;
        }
        else if(state == "OFF")
        {
        sMsg_tx.val = SHUTTER_OFF;
        }
    }

    xQueueSend(xQueueShutter, (void *) &sMsg_tx, 0);
}

void shutterTask(void *pvParameter)
{
  for(;;)
  {
    xQueueReceive(xQueueShutter, &sMsg_rx, portMAX_DELAY);
    shutterControl(sMsg_rx.dir, sMsg_rx.val);
    xTimerStart(xShutterOff_timer, 0);
  }
}

void vTimerCallback_ShutterOff(TimerHandle_t xShutterOff)
{
  shutterOFF();
}