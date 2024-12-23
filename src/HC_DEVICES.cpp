#include "HC_DEVICES.hpp"

#if UNIT_TEST==1
#include "test/test_HC_DEVICES/test_HC_DEVICE.hpp"
#define sendDeviceStatusRequest(device) stub_sendDeviceStatusRequest(device)
#else
#include "HC_WIFI_INTERFACE.hpp"
#endif

uint8_t HC_IP_DEVICE[] = {192, 168, 0, 200};
String HC_IP_DEVICE_STR = "192.168.0.200";
uint8_t HC_IP_GATEWAY[] = {192, 168, 0, 1};
uint8_t HC_IP_SUBNET[] = {255, 255, 255, 0};
uint8_t HC_IP_DNS[] = {8, 8, 8, 8};
String HOSTNAME = "ESP32_Home_Controller";

tasmota_device NONE = {NONE_ID, "", "", "UNKNOWN", MOTION_INACTIVE, ""};
tasmota_device PRINTER = {PRINTER_ID, TASMOTA_IP_PRINTER, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "Printer"};
tasmota_device BCPU = {BCPU_ID, TASMOTA_IP_BCPU_GLOBE, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "BCPU"};
tasmota_device GLOBE = {GLOBE_ID, TASMOTA_IP_BCPU_GLOBE, TASMOTA_POWER_2, "UNKNOWN", MOTION_INACTIVE, "Globe"};
tasmota_device NIGHTLAMP = {NIGHTLAMP_ID, TASMOTA_IP_NIGHTLAMP, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "Nightlamp"};
tasmota_device DESKLAMP = {DESKLAMP_ID, TASMOTA_IP_DESKLAMP, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "Desklamp"};
tasmota_device LIVINGROOMLAMP = {LIVINGROOMLAMP_ID, TASMOTA_IP_LIVINGROOMLAMP, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "Standlicht"};
tasmota_device COMPUTER = {COMPUTER_ID, TASMOTA_IP_COMPUTER, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE, "Computer"};

#define COUNT_DEVICES 8
tasmota_device arrayDevices[COUNT_DEVICES] = {NONE, PRINTER, BCPU, GLOBE, NIGHTLAMP, DESKLAMP, LIVINGROOMLAMP, COMPUTER};

QueueHandle_t xQueueStatusRequest;
device_id queueDevice_id;

#define DEFAULT_QUEUE_SIZE 20

String getTasmotaStatus(String body)
{
    if(body.indexOf("OFF") > 0)
    {
        return "OFF";
    }
    else if(body.indexOf("ON") > 0)
    {
        return "ON";
    }
    else
    {
        return "ERROR";
    }
}

String requestDeviceStatus(tasmota_device device)
{
    String body = sendDeviceStatusRequest(device);
    return getTasmotaStatus(body);
}

bool requestAllStatus(void)
{
    String checkReturn = "";
    bool ret = false;

    checkReturn += PRINTER.status = requestDeviceStatus(PRINTER);
    checkReturn += BCPU.status = requestDeviceStatus(BCPU);
    checkReturn += GLOBE.status = requestDeviceStatus(GLOBE);
    checkReturn += NIGHTLAMP.status = requestDeviceStatus(NIGHTLAMP);
    checkReturn += DESKLAMP.status = requestDeviceStatus(DESKLAMP);

    if(checkReturn.indexOf("ERROR") < 0)
    {
        ret = true;
    }

    return ret;
}

void init_deviceTask(int queSize)
{
    xQueueStatusRequest = xQueueCreate(queSize, sizeof(queueDevice_id));
}

BaseType_t sendQueueStatusRequest(tasmota_device device)
{
    queueDevice_id = device.id;
    return xQueueSend(xQueueStatusRequest, (void *) &queueDevice_id, 0);
}

tasmota_device name2device(String deviceName)
{
    for(int idx = 0; idx < COUNT_DEVICES; idx++)
    {
        if(deviceName == arrayDevices[idx].DeviceName)
        {
            return arrayDevices[idx];
        }
    }

    return NONE;
}

tasmota_device getNextDevice(device_id deviceID)
{
    if(deviceID == END_ID)
    {
        return NONE;
    }
    else
    {
        int idx = deviceID + 1;
        if(idx < COUNT_DEVICES)
        {
            if(idx > COUNT_DEVICES)
            {
                return NONE;
            }
            else
            {
                return arrayDevices[idx];
            }
        }
    }

    return NONE;
}

void deviceTask(void *pvParameter)
{
    init_deviceTask(DEFAULT_QUEUE_SIZE);
    vTaskDelay(10000);
    requestAllStatus();

    for(;;)
    {
        xQueueReceive(xQueueStatusRequest, &queueDevice_id, portMAX_DELAY);
        switch(queueDevice_id)
        {
            case PRINTER_ID: PRINTER.status = requestDeviceStatus(PRINTER); break;
            case BCPU_ID: BCPU.status = requestDeviceStatus(BCPU); break;
            case GLOBE_ID: GLOBE.status = requestDeviceStatus(GLOBE); break;
            case NIGHTLAMP_ID: NIGHTLAMP.status = requestDeviceStatus(NIGHTLAMP); break;
            case DESKLAMP_ID: DESKLAMP.status = requestDeviceStatus(DESKLAMP); break;
            dafault: break;
        }
    }
}