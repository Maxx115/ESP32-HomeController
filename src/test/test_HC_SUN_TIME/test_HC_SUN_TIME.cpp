#include "test/test_HC_SUN_TIME/test_HC_SUN_TIME.hpp"

#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_SUN_TIME.hpp"

#include "time.h"
#include "HC_TIME_KEEPER.hpp"

/*
Calculations to implement:

j_transit = j_year_ref + j_mean_solar_time + equ_of_time_simple

j_year_ref = 2_451_545
j_mean_solar_time = ceil(j_date - j_year_ref) - longitude_west / 360°
equ_of_time_simple = 0.0053 * sin(solar_mean_anomaly) - 0.0069 * sin(2 * exliptic_longitude)
solar_mean_anomaly = (357.5291 + 0.98560028 * j_mean_solar_time) mod 360
ecliptic_longitude = (solar_mean_anomaly + equ_of_center + 180 + 102.9372) mod 360
equ_of_center = 1.9148 * sin(solar_mean_anomaly) + 0.02 * sin(2 * solar_mean_anomaly) + 0.0003 * sin(3 * solar_mean_anomaly)

w_0 = arccos((sin(-0.833°) - sin(latitude) * sin(declination)) / (cos(latitude * cos(declination))))
declination = arcsin(sin(ecliptic_longitude) * sin(23.4397°))

j_rise = j_transit - w_0 / 360°
j_set = j_transit + w_0 / 360°
*/

void testing_get_j_rise(void);

void test_HC_SUN_TIME(void)
{
    testing_get_j_rise();
}

void testing_get_j_rise(void)
{
    struct tm timeinfo;
    const char* ntpServer = "ptbtime1.ptb.de";
    const long gmtOffset_sec = 0;
    const int daylightOffset_sec = 0;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    setenv("TZ", "CET-1CEST,M3.5.0,M10.5.0/3", 1);
    tzset();
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_year = 2024 - 1900;
    timeinfo.tm_mon = 01;
    timeinfo.tm_mday = 07;

    struct geolocation testLocation;
    
    Tester test_unit_getTasmotaStatus("test_unit_get_j_rise", "Returns the time of sunrise for date & geolocation");
    test_unit_getTasmotaStatus.begin();
    test_unit_getTasmotaStatus.tester_assert(get_suntimes(&timeinfo, &testLocation).tm_hour, 06);
    test_unit_getTasmotaStatus.end();
}
