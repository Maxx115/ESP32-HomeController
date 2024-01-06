#include "test/test_HC_SUN_TIME/test_HC_SUN_TIME.hpp"

#include "test/Tester.hpp"
#include "self_arduino.hpp"

#include "HC_SUN_TIME.hpp"

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

void test_HC_SUN_TIME(void)
{
    
}

