#include "test_main.hpp"
#include "test/Tester.hpp"


#include "self_arduino.hpp"

#include "test_tester/test_tester.hpp"
#include "test_HC_DEVICES/test_HC_DEVICE.hpp"
#include "test_HC_WIFI_INTERFACE/test_HC_WIFI_INTERFACE.hpp"

void test_main(void)
{
    Serial.begin(9600);
    Serial.println("Starting Unit Tests...");

    //test_tester();
    //test_HC_DEVICE();
    test_HC_WIFI_INTERFACE();

    for(;;){delay(1000);}
}

