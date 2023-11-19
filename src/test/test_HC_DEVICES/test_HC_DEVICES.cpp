#include "test/test_HC_DEVICES/test_HC_DEVICE.hpp"
#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_DEVICES.hpp"

/*
Function to test:

String requestDeviceStatus(tasmota_device device);
void requestAllStatus();

void sendQueueStatusRequest(tasmota_device device);
void deviceTask(void *pvParameter);

*/

void testing_getTasmotaStatus(void);

void test_HC_DEVICE(void)
{
    testing_getTasmotaStatus();
}

void testing_getTasmotaStatus(void)
{
    Tester test_unit_getTasmotaStatus("test_unit_getTasmotaStatus", "Returns one of three: OFF, ON, ERROR");
    test_unit_getTasmotaStatus.begin();
    test_unit_getTasmotaStatus.tester_assert(getTasmotaStatus(""), "ERROR");
    test_unit_getTasmotaStatus.tester_assert(getTasmotaStatus("{\"POWER1\":\"qw121aderrorERROROnOff\"}"), "ERROR");
    test_unit_getTasmotaStatus.tester_assert(getTasmotaStatus("{\"POWER1\":\"OFF\"}"), "OFF");
    test_unit_getTasmotaStatus.tester_assert(getTasmotaStatus("{\"POWER1\":\"ON\"}"), "ON");
    test_unit_getTasmotaStatus.end();
}