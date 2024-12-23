#include "HC_SUN_TIME.hpp"
#include "math.h"

long double degree2radiant(long double degree)
{
    long double radiant = degree * 3.14159265358979323846 / 180.0;
    return radiant;
}
long double radiant2degrees(long double radiant)
{
    long double degree = radiant * 180.0 / 3.14159265358979323846;
    return degree;
}
long double timestamp2julian(long double timestamp)
{
    long double julian = timestamp / 86400.0 + 2440587.5;
    return julian;
}
long double julian2timestamp(long double julian)
{
    long double timestamp = (julian - 2440587.5) * 86400;
    return timestamp;
}

/*--------------------------------------------------------------*/

long double calc_equation_of_the_center(long double m_radians)
{
    long double c_degrees = 1.9148 * sin(m_radians) + 0.02 * sin(2 * m_radians) + 0.0003 * sin(3 * m_radians);
    return c_degrees;
}

long double calc_ecliptic_longitude(long double m_degrees, long double m_radians)
{
    long double c_degrees = calc_equation_of_the_center(m_radians);
    long double l_degrees = fmod(m_degrees + c_degrees + 180.0 + 102.9372, 360);
    return l_degrees;
}

long double calc_solar_mean_anomaly(long double j)
{
    long double m_degrees = 357.5291 + 0.98560028 * j;
    return m_degrees;
}

long double calc_julian_day(long double j_date)
{
    long double n = ceil(j_date - (2451545.0 + 0.0009) + 69.184 / 86400.0);
    return n;
}

long double calc_mean_solar_time(long double j_day, long double longitude_w)
{
    long double j = j_day + 0.0009 - longitude_w / 360.0;
    return j;
}

long double calc_solar_transit(long double j, long double m_radians, long double l_radians)
{
    long double j_transit = 2451545.0 + j + 0.0053 * sin(m_radians) - 0.0069 * sin(2 * l_radians);
    return j_transit;
}

long double calc_declination_of_the_sun(long double l_radians)
{
    long double sin_d = sin(l_radians) * sin(degree2radiant(23.4397));
    return sin_d;
}

long double calc_hour_angle(long double l_radians, long double latitude_n)
{
    long double elevation = 0.0;
    long double sin_d = calc_declination_of_the_sun(l_radians);
    long double cos_d = cos(asin(sin_d));

    long double some_cos = (sin(degree2radiant(-0.833 - 2.076 * sqrt(elevation) / 60.0)) - sin(degree2radiant(latitude_n)) * sin_d) / (cos(degree2radiant(latitude_n)) * cos_d);
    long double w0_radians = acos(some_cos);

    return w0_radians;
}

long double calc_sunrise(long double ts, long double longitude_w, long double latitude_n)
{
    long double j_date = timestamp2julian(ts);
    long double j_day = calc_julian_day(j_date);
    long double j = calc_mean_solar_time(j_day, longitude_w);
    long double m_degrees = calc_solar_mean_anomaly(j);
    long double m_radians = degree2radiant(m_degrees);
    long double l_degrees = calc_ecliptic_longitude(m_degrees, m_radians);
    long double l_radians = degree2radiant(l_degrees);
    long double w0_radians = calc_hour_angle(l_radians, latitude_n);
    long double w0_degrees = radiant2degrees(w0_radians);

    long double j_transit = calc_solar_transit(j, m_degrees, l_degrees);
    long double j_rise = j_transit - w0_degrees / 360;

    return j_rise;
}

struct data
{
    long double j_date;
    long double j_day;
    long double j;
    long double m_degrees;
    long double m_radians;
    long double l_degrees;
    long double l_radians;
    long double w0_radians;
    long double w0_degrees;
};
struct data j_data; 

long double calc_j_transit(long double ts, long double longitude_w, long double latitude_n)
{
    j_data.j_date = timestamp2julian(ts);
    j_data.j_day = calc_julian_day(j_data.j_date);
    j_data.j = calc_mean_solar_time(j_data.j_day, longitude_w);
    j_data.m_degrees = calc_solar_mean_anomaly(j_data.j);
    j_data.m_radians = degree2radiant(j_data.m_degrees);
    j_data.l_degrees = calc_ecliptic_longitude(j_data.m_degrees, j_data.m_radians);
    j_data.l_radians = degree2radiant(j_data.l_degrees);
    j_data.w0_radians = calc_hour_angle(j_data.l_radians, latitude_n);
    j_data.w0_degrees = radiant2degrees(j_data.w0_radians);

    long double j_transit = calc_solar_transit(j_data.j, j_data.m_degrees, j_data.l_degrees);

    return j_transit;
}

long double calc_j_rise(long double j_transit, long double w0_degrees)
{
    long double j_rise = j_transit - w0_degrees / 360;
    return j_rise;
}
long double calc_j_set(long double j_transit, long double w0_degrees)
{
    long double j_rise = j_transit + w0_degrees / 360;
    return j_rise;
}

/*--------------------------------------------------------------*/

tm get_suntimes(struct tm *timeinfo, struct geolocation *testLocation)
{    
    long double ts = (long double)mktime(timeinfo);
    long double latitude_n = 52.2689;
    long double longitude_w = 10.5268;

    /*------------------------------------------------------------*/

    long double j_transit = calc_j_transit(ts, longitude_w, latitude_n);
    long double j_rise = calc_j_rise(j_transit, j_data.w0_degrees);
    long double j_set = calc_j_set(j_transit, j_data.w0_degrees);

    long double ts_transit = julian2timestamp(j_transit);
    Serial.printf("j_transit = %.20Lf", j_transit);
    Serial.printf(" | ts_transit = %.20Lf\n", ts_transit);

    long double ts_rise = julian2timestamp(j_rise);
    Serial.printf("j_rise = %.20Lf", j_rise);
    Serial.printf(" | ts_rise = %.20Lf\n", ts_rise);

    long double ts_set = julian2timestamp(j_set);
    Serial.printf("j_set = %.20Lf", j_set);
    Serial.printf(" | ts_set = %.20Lf\n", ts_set);

    time_t ts_convert = (time_t) ts_transit;
    struct tm *seunnoon = localtime(&ts_convert);
    Serial.println(String(seunnoon->tm_year + 1900) + "." + String(seunnoon->tm_mon + 1) + "." + String(seunnoon->tm_mday) + " | " + String(seunnoon->tm_hour) + ":" + String(seunnoon->tm_min) + ":" + String(seunnoon->tm_sec));

    ts_convert = (time_t) ts_rise;
    struct tm *sunrise = localtime(&ts_convert);
    Serial.println(String(sunrise->tm_year + 1900) + "." + String(sunrise->tm_mon + 1) + "." + String(sunrise->tm_mday) + " | " + String(sunrise->tm_hour) + ":" + String(sunrise->tm_min) + ":" + String(sunrise->tm_sec));

    ts_convert = (time_t) ts_set;
    struct tm *sunset = localtime(&ts_convert);
    Serial.println(String(sunset->tm_year + 1900) + "." + String(sunset->tm_mon + 1) + "." + String(sunset->tm_mday) + " | " + String(sunset->tm_hour) + ":" + String(sunset->tm_min) + ":" + String(sunset->tm_sec));


    return *sunrise;
}
