#include "HC_DEVICES.hpp"

#include "HC_WIFI_INTERFACE.hpp"

tasmota_device PRINTER = {PRINTER_ID, TASMOTA_IP_PRINTER, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE};
tasmota_device BCPU = {BCPU_ID, TASMOTA_IP_BCPU_GLOBE, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE};
tasmota_device GLOBE = {GLOBE_ID, TASMOTA_IP_BCPU_GLOBE, TASMOTA_POWER_2, "UNKNOWN", MOTION_INACTIVE};
tasmota_device NIGHTLAMP = {NIGHTLAMP_ID, TASMOTA_IP_NIGHTLAMP, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE};
tasmota_device DESKLAMP = {DESKLAMP_ID, TASMOTA_IP_DESKLAMP, TASMOTA_POWER_1, "UNKNOWN", MOTION_INACTIVE};

QueueHandle_t xQueueStatusRequest;
device_id queueDevice_id;

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

void requestAllStatus()
{
    PRINTER.status = requestDeviceStatus(PRINTER);
    BCPU.status = requestDeviceStatus(BCPU);
    GLOBE.status = requestDeviceStatus(GLOBE);
    NIGHTLAMP.status = requestDeviceStatus(NIGHTLAMP);
    DESKLAMP.status = requestDeviceStatus(DESKLAMP);    
}

void sendQueueStatusRequest(tasmota_device device)
{
    queueDevice_id = device.id;
    xQueueSend(xQueueStatusRequest, (void *) &queueDevice_id, 0);
}

void deviceTask(void *pvParameter)
{
    xQueueStatusRequest = xQueueCreate(20, sizeof(queueDevice_id));
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