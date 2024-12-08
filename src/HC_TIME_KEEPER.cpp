/* --------------- INCLUDE SECTION ---------------- */
#include "HC_TIME_KEEPER.hpp"

#include "HC_DEVICES.hpp"
#include "HC_SERVER_APPLICATION.hpp"
#include "HC_WIFI_INTERFACE.hpp"
#include "time.h"

/* ---------------- DEFINES / CONSTANTs ---------------- */
timeStruct localTime;

/* https://ipinfo.io/json */

/* ---------------- LOCAL VARIABLE SECTION ---------------- */

/* ---------------- HELP FUNCTION SECTION ---------------- */

/* ---------------- TIME FUNCTION SECTION ---------------- */

void initTime()
{
  const char* ntpServer = "ptbtime1.ptb.de";
  const long gmtOffset_sec = 0;
  const int daylightOffset_sec = 0;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
  tzset();
}

void updateTime(void)
{
  struct tm timeinfo;

  while(!getLocalTime(&timeinfo))
  {
    /* Wait for local time */
  }

  localTime.hour = timeinfo.tm_hour;
  localTime.minute = timeinfo.tm_min;
  localTime.second = timeinfo.tm_sec;

  localTime.year = timeinfo.tm_year;
  localTime.month = timeinfo.tm_mon;
  localTime.day = timeinfo.tm_mday;
}

int getTimeSeconds(void)
{
  return localTime.second;
}

void setTimeSeconds(int sec)
{
  localTime.second = sec;
}

void uptickTime(void)
{
  localTime.minute = (localTime.minute + 1) % 60;
  if(localTime.minute == 0)
  {
    localTime.hour = (localTime.hour + 1) % 24;
    if(localTime.hour == 4)
    {
      updateTime();
    }
  }
}

void trackEvent(void)
{
  if(localTime.hour == 5 && localTime.minute == 0)
  {
    sendDeviceRequest(NIGHTLAMP, TASMOTA_ON);
    sendDeviceRequest(NIGHTLAMP, TASMOTA_BLINK);
  }
  else if(localTime.hour == 8 && localTime.minute == 0)
  {
    sendDeviceRequest(NIGHTLAMP, TASMOTA_OFF);
  }
  else if(localTime.hour == 16 && localTime.minute == 0)
  {
    //sendDeviceRequest(DESKLAMP, TASMOTA_ON);
    DESKLAMP.motion = true;
  }
  else if(localTime.hour == 20 && localTime.minute == 0) 
  {
    DESKLAMP.motion = false;
    sendDeviceRequest(DESKLAMP, TASMOTA_OFF);
  }
}

String getTimeString(void)
{
  String string_time = (String(localTime.hour) + ":" + String(localTime.minute));
  return string_time;
}

/* 
* Keeps track of time & time-events
*/
void timeTask(void *pvParameter)
{
  initTime();
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