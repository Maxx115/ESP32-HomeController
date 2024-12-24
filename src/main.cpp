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
#include "HC_time_keeper.hpp"

#include "HC_DEVICES.hpp"
#include "HC_SERVER_APPLICATION.hpp"
#include "HC_WIFI_INTERFACE.hpp"
#include "HC_NVM_MAN.hpp"
#include "HC_ALEXA_MAN.hpp"

/* ---------------- DEFINES / CONSTANTS ---------------- */


/* ---------------- FUNCTIONS ---------------- */

/* ---------------- GLOBEL VARIABLE ---------------- */

/* ---------------- RTOS FUNCTION DECLARATION ---------------- */
void loopTask(void *pvParameters);

/* ---------------- TIMER FUNCTION DECLARATION ---------------- */

/* ---------------- RTOS TIMER DECLARATION ---------------- */

/* ---------------- RTOS QUEUE DECLARATION ---------------- */

/* ---------------- RTOS SETUP ---------------- */
#ifndef CONFIG_ARDUINO_LOOP_STACK_SIZE
#define CONFIG_ARDUINO_LOOP_STACK_SIZE 8192
#endif
TaskHandle_t loopTaskHandle = NULL;
TaskHandle_t deviceTaskHandle = NULL;
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
      nvm_write_string(WIFI_NVM, AP_SSID_NVM, SETUP_AP_SSID);
      nvm_write_string(WIFI_NVM, AP_PASSWORD_NVM, SETUP_AP_PASSWORD);
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

    /* Task Setup & Startup */  
    xTaskCreateUniversal(loopTask, "loopTask", CONFIG_ARDUINO_LOOP_STACK_SIZE, NULL, 1, &loopTaskHandle, CONFIG_ARDUINO_RUNNING_CORE);
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
