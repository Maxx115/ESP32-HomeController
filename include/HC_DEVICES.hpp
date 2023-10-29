/*
*
*
*
*
*
*/

#ifndef TASMOTA_CMD_H
#define TASMOTA_CMD_H

#include "self_arduino.hpp"

const String TASMOTA_PRINTER = "192.168.0.34";
const String TASMOTA_BCPU = "192.168.0.23";
const String TASMOTA_GLOBE = "192.168.0.23";
const String TASMOTA_NIGHTLAMP = "192.168.0.35";
const String TASMOTA_DESKLAMP = "192.168.0.33";

const String TASMOTA_CMD_START = "/cm?cmnd=";

const String TASMOTA_POWER_1 = TASMOTA_CMD_START + "Power1";
const String TASMOTA_POWER_2 = TASMOTA_CMD_START + "Power2";
const String TASMOTA_POWER_3 = TASMOTA_CMD_START + "Power3";
const String TASMOTA_POWER_4 = TASMOTA_CMD_START + "Power4";

const String TASMOTA_TOGGLE = "%20TOGGLE";
const String TASMOTA_ON = "%20ON";
const String TASMOTA_OFF = "%20OFF";
const String TASMOTA_BLINK = "%20BLINK";
const String TASMOTA_BLINKOFF = "%20BLINKOFF";

const String TASMOTA_BLINK_COUNT = "BlinkCount%20";
const String TASMOTA_BLINK_TIME = "BlinkTime%20";

#endif /* TASMOTA_CMD_H */