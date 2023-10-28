/*
*
*
*
*
*
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

#include "HC_wifi_init.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

#include "HC_wifi_server.hpp"
#include "HC_tft_screen.hpp"
#include "HC_shutter_control.hpp"
#include "HC_time_keeper.hpp"

/* ---------------- DEFINES / CONSTANTS ---------------- */

/* ---------------- GLOBEL VARIABLE SECTION ---------------- */
/* TFT objects */
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
TS_Point touch_point;

/* ---------------- RTOS FUNCTION DECLARATION SECTION ---------------- */
void loopTask(void *pvParameters);
void touchTask(void *pvParameters);
void shutterTask(void *pvParameter);
void timeTask(void *pvParameter);

/* ---------------- TIMER FUNCTION DECLARATION SECTION ---------------- */
void vTimerCallback_AutoScreenOff(TimerHandle_t xAutoOffScreen_timer);
void vTimerCallback_ShutterOff(TimerHandle_t xShutterOff_timer);

/* ---------------- RTOS TIMER DECLARATION SECTION ---------------- */
TimerHandle_t xAutoOffScreen_timer;
TimerHandle_t xShutterOff_timer;

/* ---------------- RTOS QUEUE DECLARATION SECTION ---------------- */
QueueHandle_t xQueueShutter;
shutterMsg sMsg_rx;

/* ---------------- RTOS SETUP SECTION ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif
TaskHandle_t loopTaskHandle = NULL;
TaskHandle_t touchTaskHandle = NULL;
TaskHandle_t shutterTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
bool loopTaskWDTEnabled;

extern "C" void app_main()
{
    loopTaskWDTEnabled = false;
    initArduino();

    /* Timer Setup */
    xAutoOffScreen_timer = xTimerCreate("AutoScreenOff", BACKLIGHT_TIMER, pdFALSE, ( void *) 0, vTimerCallback_AutoScreenOff);
    xTimerStart(xAutoOffScreen_timer, 0);
    xShutterOff_timer = xTimerCreate("ShutterOff", SHUTTER_TIMER, pdFALSE, ( void *) 0, vTimerCallback_ShutterOff);
    xTimerStart(xShutterOff_timer, 0);

    /* Queue Setup */
    sMsg_rx.dir = '0';
    sMsg_rx.val = SHUTTER_OFF;
    xQueueShutter = xQueueCreate(1, sizeof(shutterMsg));

    /* main setup */
    initWifi();
    initServer();
    initQueue(xQueueShutter);
    //initTFT(&tft, &touch, TFT_ROTATION, TFT_TEXTSIZE, ILI9341_BLACK, FreeSansBold9pt7b);

    /* Task Setup & Startup */
    //xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    //xTaskCreateUniversal(touchTask, "touchTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &touchTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    xTaskCreateUniversal(shutterTask, "shutterTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &shutterTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
    xTaskCreateUniversal(timeTask, "timeTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &timeTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
}

/* ---------------- RTOS TASK SECTION ---------------- */

/* 
* 
*/
void loopTask(void *pvParameters)
{
  for(;;)
  {
    vTaskDelay(1000);  
  }
}

/*
* Polls the screen to toggle backlight LED
* + WakeOnLan MyPC
*/
void touchTask(void *pvParameters)
{
  /* Print Start Screen */
  tft_printStartScreen(&tft, getIPAddress());

  for(;;)
  {
    /* Touch Sense */
    touch_point = touch.getPoint();
    if(touch_point.z > MINPRESSURE)
    {
      xTimerStart(xAutoOffScreen_timer, 0);
      digitalWrite(TFT_LED, !digitalRead(TFT_LED));
      if(!digitalRead(TFT_LED))
      {
        wakeMyPC();
      }
      while(touch_point.z > MINPRESSURE)
      {
        touch_point = touch.getPoint();
        vTaskDelay(100);
      }
    }
  }
}

/* 
* Receives commands to drive shutter up / down 
*/
void shutterTask(void *pvParameter)
{
  for(;;)
  {
    xQueueReceive(xQueueShutter, &sMsg_rx, portMAX_DELAY);
    shutterControl(sMsg_rx.dir, sMsg_rx.val);
    xTimerStart(xShutterOff_timer, 0);
  }
}

/* 
* Keeps track of time & time-events
*/
void timeTask(void *pvParameter)
{
  initTime();
  //vTaskDelay(5000);
  updateTime();
  vTaskDelay(60000 - (getTimeSeconds() * 1000));
  setTimeSeconds(0);

  for(;;)
  {
    uptickTime();
    trackEvent();
    vTaskDelay(60000);
  }
}

/* ---------------- TIMER FUNCTION SECTION ---------------- */

void vTimerCallback_AutoScreenOff(TimerHandle_t xAutoOffScreen_timer)
{
  digitalWrite(TFT_LED, TFT_LED_OFF);
}

void vTimerCallback_ShutterOff(TimerHandle_t xShutterOff_timer)
{
  shutterOFF();
}


