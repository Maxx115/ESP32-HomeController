/*
*
*
*
*
*
*/

#ifndef HC_TFT_SCREEN
#define HC_TFT_SCREEN

/* ---------------- INCLUDES ---------------- */
#include <Adafruit_ILI9341.h>
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>
#include <Fonts/FreeSansBold9pt7b.h>
#include "usergraphics.h"

/* ---------------- DEFINES / CONSTANTs ---------------- */
/* TFT visual definitions */
#define TFT_ROTATION 1
#define TFT_TEXTSIZE 0
#define BACKLIGHT_TIMER 5000

/* TFT pin definitions */
#define AZ_TOUCH_MOD_BIG_TFT 
#define TFT_CS   5
#define TFT_DC   4
#define TFT_LED  15  
#define TFT_MOSI 23
#define TFT_CLK  18
#define TFT_RST  22
#define TFT_MISO 19
#define TFT_LED  15  
#define TOUCH_CS 14
#define TOUCH_IRQ 27

/* TFT pin adressing */
#define TFT_LED_OFF HIGH
#define TFT_LED_ON  LOW

/* touchscreen calibration definnitions */
#define MINPRESSURE 10
#define TS_MINX 370
#define TS_MINY 470
#define TS_MAXX 3700
#define TS_MAXY 3600
 
/* ---------------- FUNCTION PROTOTYPES ---------------- */
void initTFT(Adafruit_ILI9341 *tft, XPT2046_Touchscreen *touch, int rot, int textSize, int textColor, GFXfont textFont);
void tft_printStartScreen(Adafruit_ILI9341 *tft, IPAddress ipAdr);
void tft_printBox(Adafruit_ILI9341 *tft, int boxColor);

#endif /* HC_TFT_SCREEN */
