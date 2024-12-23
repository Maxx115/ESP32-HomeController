#include "test_main.hpp"
#include "test/Tester.hpp"


#include "self_arduino.hpp"

#include "test_tester/test_tester.hpp"
#include "test_HC_DEVICES/test_HC_DEVICE.hpp"
#include "test_HC_WIFI_INTERFACE/test_HC_WIFI_INTERFACE.hpp"
#include "test_HC_SUN_TIME/test_HC_SUN_TIME.hpp"

void test_main(void)
{
    Serial.println("Starting Unit Tests...");

    //test_tester();
    test_HC_DEVICE();
    test_HC_WIFI_INTERFACE();
    //test_HC_SUN_TIME();

    for(;;){delay(1000);}
}

