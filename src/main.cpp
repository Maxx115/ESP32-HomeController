/*
*
*
*
*
*
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

#if UNIT_TEST==1
#include "test/test_main.hpp"
#endif

// FREE_RTOS
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_task_wdt.h"

// HOMECONTROLLER
#include "HC_SERVER_APPLICATION.hpp"
#include "HC_WIFI_INTERFACE.hpp"
#include "HC_SHUTTER_CONTROL.hpp"
#include "HC_MOTION_CONTROL.hpp"
#include "HC_DEVICES.hpp"
#include "HC_NVM_MAN.hpp"

#include "HC_tft_screen.hpp"
//#include "HC_shutter_control.hpp"
#include "HC_time_keeper.hpp"

//#include "HC_CRYPTO.hpp"
#include "HC_ALEXA_MAN.hpp"

#include "WIFI_CONFIG.hpp"

/* ---------------- DEFINES / CONSTANTS ---------------- */

/* ---------------- FUNCTIONS ---------------- */

/* ---------------- GLOBEL VARIABLE ---------------- */
/* TFT objects */
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);
XPT2046_Touchscreen touch(TOUCH_CS, TOUCH_IRQ);
TS_Point touch_point;

/* ---------------- RTOS FUNCTION DECLARATION ---------------- */
void loopTask(void *pvParameters);
void touchTask(void *pvParameters);

/* ---------------- TIMER FUNCTION DECLARATION ---------------- */
void vTimerCallback_AutoScreenOff(TimerHandle_t xAutoOffScreen_timer);

/* ---------------- RTOS TIMER DECLARATION ---------------- */
TimerHandle_t xAutoOffScreen_timer;

/* ---------------- RTOS QUEUE DECLARATION ---------------- */
//QueueHandle_t xQueueShutter;
//shutterMsg sMsg_rx;

/* ---------------- RTOS SETUP ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif
TaskHandle_t loopTaskHandle = NULL;
TaskHandle_t touchTaskHandle = NULL;
TaskHandle_t deviceTaskHandle = NULL;
TaskHandle_t shutterTaskHandle = NULL;
TaskHandle_t motionTaskHandle = NULL;
TaskHandle_t timeTaskHandle = NULL;
TaskHandle_t fauxmoTaskHandle = NULL;
bool loopTaskWDTEnabled;

extern "C" void app_main()
{
    loopTaskWDTEnabled = false;
    initArduino();
    Serial.begin(9600);
    initNVM();

    #if UNIT_TEST==1
    test_main();
    #else
    
      #if ERASE_WIFI==1
      if(nvm_erase_namespace(WIFI_NVM))
      {
        Serial.println("WiFi NVM Erased!");
      }
      else
      {
        Serial.println("WiFi NVM not Erased!");
      }
      #endif

    /* --- START Check for first boot --- */

    if(nvm_contains(WIFI_NVM, AP_SSID_NVM) == false)
    {
      nvm_write_string(WIFI_NVM, AP_SSID_NVM, "ESP_HomeController");
      nvm_write_string(WIFI_NVM, AP_PASSWORD_NVM, "123456789");
    }

    wl_status_t wifi_attempt = WL_NO_SHIELD;
    if(nvm_contains(WIFI_NVM, SSID_NVM) == true)
    {
      wifi_attempt = wifiInit_default();
      if(wifi_attempt == WL_CONNECTED)
      {
        serverInit(HOME_INIT);
      }
    }

    if(wifi_attempt != WL_CONNECTED)
    {
      APstartupMode();
      serverInit(SETUP_INIT);
    }

  /* --- END Check for first boot --- */

    /* Timer Setup */
    xAutoOffScreen_timer = xTimerCreate("AutoScreenOff", BACKLIGHT_TIMER, pdFALSE, ( void *) 0, vTimerCallback_AutoScreenOff);
    xTimerStart(xAutoOffScreen_timer, 0);

    shutterInit();
    //initTFT(&tft, &touch, TFT_ROTATION, TFT_TEXTSIZE, ILI9341_BLACK, FreeSansBold9pt7b);

    /* Task Setup & Startup */
    //xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    //xTaskCreateUniversal(touchTask, "touchTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &touchTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
    xTaskCreateUniversal(shutterTask, "shutterTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &shutterTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
    xTaskCreateUniversal(motionTask, "motionTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &motionTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);   
    xTaskCreateUniversal(timeTask, "timeTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &timeTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);


    xTaskCreateUniversal(deviceTask, "deviceTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &deviceTaskHandle, CONFIG_ARDUINO_RUNNING_CORE); 
    xTaskCreateUniversal(fauxmoTask, "fauxmoTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &fauxmoTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);

  #endif
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
