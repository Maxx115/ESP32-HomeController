#include "test/test_HC_DEVICES/test_HC_DEVICE.hpp"
#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_DEVICES.hpp"

/*
Function to test:

String getTasmotaStatus(String body);

String requestDeviceStatus(tasmota_device device);
-> Not tested: covered by other Unit Tests

void requestAllStatus();

void sendQueueStatusRequest(tasmota_device device);

void deviceTask(void *pvParameter);
-> cannot be tested as its a inf loop

*/

void testing_getTasmotaStatus(void);
void testing_sendQueueStatusRequest(void);
void testing_requestAllStatus(void);
/*STUB*/String stub_sendDeviceStatusRequest(tasmota_device device);

void test_HC_DEVICE(void)
{
    testing_getTasmotaStatus();
    testing_sendQueueStatusRequest();
    testing_requestAllStatus();
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

void testing_sendQueueStatusRequest(void)
{
    init_deviceTask(5);

    Tester test_unit_sendQueueStatusRequest("test_unit_sendQueueStatusRequest", "Returns one of two: pdTRUE, errQUEUE_FULL");
    test_unit_sendQueueStatusRequest.begin();
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(PRINTER), pdTRUE);
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(BCPU), pdTRUE);
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(GLOBE), pdTRUE);
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(NIGHTLAMP), pdTRUE);
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(DESKLAMP), pdTRUE);
    test_unit_sendQueueStatusRequest.tester_assert(sendQueueStatusRequest(DESKLAMP), errQUEUE_FULL);
    test_unit_sendQueueStatusRequest.end();
}

const String stub_array_deviceStatus[] = {
    "{\"POWER1\":\"OFF\"}",
    "{\"POWER1\":\"ON\"}",
    "{\"POWER1\":\"ERROR\"}",
    "{\"POWER1\":\"qw121aderrorERROROnOff\"}"
};

int stub_array_index = 0;

/*STUB*/String stub_sendDeviceStatusRequest(tasmota_device device)
{
    String stubReturn = stub_array_deviceStatus[stub_array_index];
    return stubReturn;
}

void testing_requestAllStatus(void)
{
    Tester test_unit_requestAllStatus("test_unit_requestAllStatus", "Returns one of three: OFF, ON, ERROR");
    test_unit_requestAllStatus.begin();
    test_unit_requestAllStatus.tester_assert(requestAllStatus(), true);
    stub_array_index = 2;
    test_unit_requestAllStatus.tester_assert(requestAllStatus(), false);
    test_unit_requestAllStatus.end();
}