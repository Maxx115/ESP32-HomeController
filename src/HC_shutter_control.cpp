/*
*
*
*
*
*
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "HC_shutter_control.hpp"

/* ---------------- FUNCTION SECTION ---------------- */
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
