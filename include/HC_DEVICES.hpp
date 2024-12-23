/*
*
*
*
*
*
*/

#ifndef HC_DEVICES_H
#define HC_DEVICES_H

#include "self_arduino.hpp"

extern uint8_t HC_IP_DEVICE[];
extern String HC_IP_DEVICE_STR;
extern uint8_t HC_IP_GATEWAY[];
extern uint8_t HC_IP_SUBNET[];
extern uint8_t HC_IP_DNS[];
extern String HOSTNAME;

#define MOTION_ACTIVE true
#define MOTION_INACTIVE false

const String TASMOTA_IP_PRINTER = "192.168.0.34";
const String TASMOTA_IP_BCPU_GLOBE = "192.168.0.23";
const String TASMOTA_IP_NIGHTLAMP = "192.168.0.35";
const String TASMOTA_IP_DESKLAMP = "192.168.0.36";
const String TASMOTA_IP_LIVINGROOMLAMP = "192.168.0.24";
const String TASMOTA_IP_COMPUTER = "192.168.0.8";

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

enum device_id 
{
    NONE_ID = 0,
    PRINTER_ID,
    BCPU_ID,
    GLOBE_ID,
    NIGHTLAMP_ID,
    DESKLAMP_ID,
    LIVINGROOMLAMP_ID,
    COMPUTER_ID,
    END_ID
};
struct tasmota_device
{
    device_id id;
    String ip;
    String power;
    String status;
    boolean motion;
    String DeviceName;
};

extern tasmota_device NONE;
extern tasmota_device PRINTER;
extern tasmota_device BCPU;
extern tasmota_device GLOBE;
extern tasmota_device NIGHTLAMP;
extern tasmota_device DESKLAMP;
extern tasmota_device LIVINGROOMLAMP;
extern tasmota_device COMPUTER;

void init_deviceTask(int queSize);

String getTasmotaStatus(String body);

String requestDeviceStatus(tasmota_device device);
bool requestAllStatus(void);

tasmota_device name2device(String deviceName);
tasmota_device getNextDevice(device_id device = NONE_ID);

BaseType_t sendQueueStatusRequest(tasmota_device device);
void deviceTask(void *pvParameter);

#endif /* HC_DEVICES_H */