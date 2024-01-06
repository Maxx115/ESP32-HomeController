/*
*
*
*
*
*
*/

#ifndef HC_SUN_TIME
#define HC_SUN_TIME

#include "HC_TIME_KEEPER.hpp"
#include "time.h"

tm get_suntimes(struct tm *timeinfo, struct geolocation *testLocation);

#endif /* HC_SUN_TIME */