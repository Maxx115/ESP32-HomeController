/*
*
*
*
*
*
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

//#include "HC_wifi_init.hpp"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

//#include "HC_wifi_server.hpp"

#include "HC_SERVER_APPLICATION.hpp"
#include "HC_WIFI_INTERFACE.hpp"
#include "HC_SHUTTER_CONTROL.hpp"
#include "HC_MOTION_CONTROL.hpp"
#include "HC_DEVICES.hpp"

#include "HC_tft_screen.hpp"
//#include "HC_shutter_control.hpp"
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

/* ---------------- TIMER FUNCTION DECLARATION SECTION ---------------- */
void vTimerCallback_AutoScreenOff(TimerHandle_t xAutoOffScreen_timer);

/* ---------------- RTOS TIMER DECLARATION SECTION ---------------- */
TimerHandle_t xAutoOffScreen_timer;

/* ---------------- RTOS QUEUE DECLARATION SECTION ---------------- */
//QueueHandle_t xQueueShutter;
//shutterMsg sMsg_rx;

/* ---------------- RTOS SETUP SECTION ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif
TaskHandle_t loopTaskHandle = NULL;
TaskHandle_t touchTaskHandle = NULL;
TaskHandle_t deviceTaskHandle = NULL;
TaskHandle_t shutterTaskHandle = NULL;
TaskHandle_t motionTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
bool loopTaskWDTEnabled;

extern "C" void app_main()
{
    loopTaskWDTEnabled = false;
    initArduino();

    /* Timer Setup */
    xAutoOffScreen_timer = xTimerCreate("AutoScreenOff", BACKLIGHT_TIMER, pdFALSE, ( void *) 0, vTimerCallback_AutoScreenOff);
    xTimerStart(xAutoOffScreen_timer, 0);

    /* main setup */
    shutterInit();
    wifiInit();
    serverInit();
    //initTFT(&tft, &touch, TFT_ROTATION, TFT_TEXTSIZE, ILI9341_BLACK, FreeSansBold9pt7b);

    /* Task Setup & Startup */
    //xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    //xTaskCreateUniversal(touchTask, "touchTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &touchTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    xTaskCreateUniversal(deviceTask, "deviceTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &deviceTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
    xTaskCreateUniversal(shutterTask, "shutterTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &shutterTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
    xTaskCreateUniversal(motionTask, "motionTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &motionTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
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
  //tft_printStartScreen(&tft, getIPHC());

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

/* ---------------- TIMER FUNCTION SECTION ---------------- */

void vTimerCallback_AutoScreenOff(TimerHandle_t xAutoOffScreen_timer)
{
  digitalWrite(TFT_LED, TFT_LED_OFF);
}
