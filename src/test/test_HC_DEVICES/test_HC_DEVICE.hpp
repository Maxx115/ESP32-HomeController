
#ifndef TEST_HC_DEVICE
#define TEST_HC_DEVICE

#include "HC_DEVICES.hpp"

void test_HC_DEVICE(void);
#if UNIT_TEST==1
/*STUB*/String stub_sendDeviceStatusRequest(tasmota_device device);
#endif

#endif /* TEST_HC_DEVICE */