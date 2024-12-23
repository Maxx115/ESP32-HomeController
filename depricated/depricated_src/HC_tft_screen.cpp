/*
*
*
*
*
*
*/

/* --------------- INCLUDE SECTION ---------------- */
#include "HC_tft_screen.hpp"

#include "self_arduino.hpp"

/* ---------------- TFT FUNCTION SECTION ---------------- */

void initTFT(Adafruit_ILI9341 *tft, XPT2046_Touchscreen *touch, int rot, int textSize, int textColor, GFXfont textFont)
{
  pinMode(TFT_LED, OUTPUT);
  digitalWrite(TFT_LED, LOW);
  tft->begin();
  touch->begin();
  tft->setRotation(rot);
  tft->setTextSize(textSize);
  tft->setTextColor(textColor);
  tft->setFont(&textFont);
}

void tft_printStartScreen(Adafruit_ILI9341 *tft, IPAddress ipAdr)
{
  tft->fillRect(0, 0, ILI9341_TFTHEIGHT, ILI9341_TFTWIDTH, ILI9341_WHITE);
  // (ILI9341_TFTWIDTH-DS_LOGO_WIDTH)/2
  tft->drawRGBBitmap((ILI9341_TFTHEIGHT-DS_LOGO_HEIGHT)/2, 0, ds_logo, DS_LOGO_HEIGHT, DS_LOGO_WIDTH);
  tft->setCursor((ILI9341_TFTHEIGHT-DS_LOGO_HEIGHT)/2+20, (ILI9341_TFTWIDTH-DS_LOGO_WIDTH)/2+DS_LOGO_HEIGHT);
  tft->println("IP: 192.168.0.200");
}

void tft_printBox(Adafruit_ILI9341 *tft, int boxColor)
{
    tft->fillRect(0, 0, ILI9341_TFTHEIGHT, ILI9341_TFTWIDTH, boxColor);
}
