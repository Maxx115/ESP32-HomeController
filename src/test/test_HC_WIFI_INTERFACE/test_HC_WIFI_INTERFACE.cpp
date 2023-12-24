#include "test_HC_WIFI_INTERFACE.hpp"
#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_WIFI_INTERFACE.hpp"

void test_wifiInit(void);
void test_wakeMyPC(void);
void test_sendClientRequest(void);
void test_sendDeviceRequest(void);
void test_loopback_request(void);

void test_HC_WIFI_INTERFACE(void)
{
    test_wifiInit();
}

void test_wifiInit(void)
{
    Tester test_unit_getTasmotaStatus("test_unit_getTasmotaStatus", "Returns one of three: OFF, ON, ERROR");
    test_unit_getTasmotaStatus.begin();
    test_unit_getTasmotaStatus.tester_assert();
    test_unit_getTasmotaStatus.end();
}

void test_wakeMyPC(void)
{

}

void test_sendClientRequest(void)
{
    
}

void test_sendDeviceRequest(void)
{
    
}

void test_loopback_request(void)
{
    
}