/*
*
*
*
*
*
*/

#ifndef HC_TIME_KEEPER
#define HC_TIME_KEEPER

/* ---------------- INCLUDES ---------------- */
#include "self_arduino.hpp"

/* ---------------- DEFINES / CONSTANTs ---------------- */

/* ---------------- STRUCTS ---------------- */
struct timeStruct
{
  int hour;
  int minute;
  int second;

  int year;
  int month;
  int day;
};

struct geolocation
{
  long longitude = 13.404954;
  char longitude_dir = 'N';
  long latitude =  52.520008;
  char latitude_dir = 'E';
};

/* ---------------- FUNCTION PROTOTYPES ---------------- */
void initTime();
void updateTime(void);
int getTimeSeconds(void);
void setTimeSeconds(int sec);
void uptickTime(void);
void trackEvent(void);
String getTimeString(void);

void timeTask(void *pvParameter);

#endif /* HC_TIME_KEEPER */