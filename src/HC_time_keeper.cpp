/* --------------- INCLUDE SECTION ---------------- */
#include "HC_time_keeper.hpp"

#include "HC_wifi_server.hpp"
#include "time.h"

/* ---------------- DEFINES / CONSTANTs ---------------- */
timeStruct localTime;

/* ---------------- LOCAL VARIABLE SECTION ---------------- */

/* ---------------- HELP FUNCTION SECTION ---------------- */

/* ---------------- TIME FUNCTION SECTION ---------------- */

void initTime()
{
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = 3600;
  const int daylightOffset_sec = 3600;
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
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
    if(localTime.hour == 0)
    {
      updateTime();
    }
  }
}

void trackEvent(void)
{
  if(localTime.hour == 5 && localTime.minute == 0)
  {
    loopback_request("/plug?item=night_lamp");
  }
}

String getTimeString(void)
{
  String string_time = (String(localTime.hour) + ":" + String(localTime.minute));
  return string_time;
}