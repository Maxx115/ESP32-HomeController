/*
*
*
*
*
*
*/

#ifndef HC_SHUTTER_CONTROL
#define HC_SHUTTER_CONTROL

/* --------------- INCLUDE SECTION ---------------- */
#include "self_arduino.hpp"

/* ---------------- DEFINES / CONSTANTs ---------------- */
#define pinShutterUP    32
#define pinShutterDOWN  25
#define SHUTTER_OFF HIGH
#define SHUTTER_ON LOW

struct shutterMsg
{
    char dir = '0';
    uint8_t val = SHUTTER_OFF;
};

/* ---------------- FUNCTION PROTOTYPES ---------------- */
void shutterUP(uint8_t status);
void shutterDOWN(uint8_t status);
void shutterOFF(void);
void shutterControl(char dir, uint8_t val);

#endif /* HC_SHUTTER_CONTROL */
