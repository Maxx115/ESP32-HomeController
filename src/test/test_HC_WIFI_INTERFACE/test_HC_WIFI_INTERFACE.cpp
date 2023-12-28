#include "test_HC_WIFI_INTERFACE.hpp"
#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_WIFI_INTERFACE.hpp"
#include "WiFi.h"
#include "WIFI_CONFIG.hpp"

void test_wifiInit(void);
void test_wakeMyPC(void);
void test_sendClientRequest(void);
void test_sendDeviceRequest(void);
void test_loopback_request(void);

void test_HC_WIFI_INTERFACE(void)
{
    test_wifiInit();
    test_wakeMyPC();
}

void test_wifiInit(void)
{
    uint8_t ip_device[] = {192, 168, 0, 255};
    uint8_t ip_gateway[] = {192, 168, 0, 1};
    uint8_t ip_subnet[] = {255, 255, 255, 0};
    uint8_t ip_dns[] = {192, 168, 0, 90};
    String hostname = "ESP32_Home_Controller";

    Tester test_unit_wifiInit("test_wifiInit", "Returns wl_status_t");
    test_unit_wifiInit.begin();
    test_unit_wifiInit.tester_assert(WL_DISCONNECTED, wifiInit(ip_device, ip_gateway, ip_subnet, ip_dns, hostname, SSID_CONFIG, "123456"));
    WiFi.disconnect();
    test_unit_wifiInit.tester_assert(WL_CONNECTED, wifiInit(ip_device, ip_gateway, ip_subnet, ip_dns, hostname, SSID_CONFIG, PASSWORD_CONFIG));
    WiFi.disconnect();
    test_unit_wifiInit.tester_assert((String)"192.168.0.255", WiFi.localIP().toString());
    test_unit_wifiInit.end();

}

void test_wakeMyPC(void)
{
    uint8_t ip_device[] = {192, 168, 0, 255};
    uint8_t ip_gateway[] = {192, 168, 0, 1};
    uint8_t ip_subnet[] = {255, 255, 255, 0};
    uint8_t ip_dns[] = {192, 168, 0, 90};
    String hostname = "ESP32_Home_Controller";

    Tester test_unit_wakeMyPC("test_unit_getTasmotaStatus", "Returns one of three: OFF, ON, ERROR");
    test_unit_wakeMyPC.begin();
    test_unit_wakeMyPC.tester_assert(false, wakeMyPC());
    wifiInit(ip_device, ip_gateway, ip_subnet, ip_dns, hostname, SSID_CONFIG, PASSWORD_CONFIG);
    test_unit_wakeMyPC.tester_assert(true, wakeMyPC());
    WiFi.disconnect();
    test_unit_wakeMyPC.end();
}

void test_sendDeviceRequest(void)
{
    uint8_t ip_device[] = {192, 168, 0, 255};
    uint8_t ip_gateway[] = {192, 168, 0, 1};
    uint8_t ip_subnet[] = {255, 255, 255, 0};
    uint8_t ip_dns[] = {192, 168, 0, 90};
    String hostname = "ESP32_Home_Controller";

    Tester test_unit_wakeMyPC("test_unit_getTasmotaStatus", "Returns one of three: OFF, ON, ERROR");
    test_unit_wakeMyPC.begin();
    test_unit_wakeMyPC.tester_assert((String)"", sendDeviceRequest(GLOBE, TASMOTA_TOGGLE, false));
    wifiInit(ip_device, ip_gateway, ip_subnet, ip_dns, hostname, SSID_CONFIG, PASSWORD_CONFIG);
    WiFi.disconnect();
    test_unit_wakeMyPC.end();
}

void test_loopback_request(void)
{
    
}