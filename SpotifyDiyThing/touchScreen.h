//#include <XPT2046_Touchscreen.h>
#include "CYD28_TouchscreenR.h"
#include <SPI.h>

//#define XPT2046_IRQ 36
//#define XPT2046_MOSI 32
//#define XPT2046_MISO 39
//#define XPT2046_CLK 25
//#define XPT2046_CS 33

#define CYD28_DISPLAY_HOR_RES_MAX 320
#define CYD28_DISPLAY_VER_RES_MAX 240

bool previousTrackStatus = false;
bool nextTrackStatus = false;
bool devicesStatus = false;
int deviceIndex = -1;

//SPIClass mySpi = SPIClass(HSPI);
//
//XPT2046_Touchscreen ts(XPT2046_CS, XPT2046_IRQ);  // Param 2 - Touch IRQ Pin - interrupt enabled polling
CYD28_TouchR ts(CYD28_DISPLAY_HOR_RES_MAX, CYD28_DISPLAY_VER_RES_MAX);

SpotifyArduino *spotify_touch;

void touchSetup(SpotifyArduino *spotifyObj) {
//  mySpi.begin(XPT2046_CLK, XPT2046_MISO, XPT2046_MOSI, XPT2046_CS);
//  ts.begin(mySpi);
  ts.begin();
  ts.setRotation(1);
  spotify_touch = spotifyObj;
}

bool handleTouched() {
  previousTrackStatus = false;
  nextTrackStatus = false;
  devicesStatus = false;
  //if (ts.tirqTouched() && ts.touched()) {
  if (ts.touched()) {
    CYD28_TS_Point p = ts.getPointScaled();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
    delay(30);
    Serial.println();
    if (p.x < 120) {
      previousTrackStatus = true;
      return true;
    } else if (p.x > 200) {
      if(p.y < 40) {
        devicesStatus = true;
        return true;
      } else if (p.y > 50) {
        nextTrackStatus = true;
        return true;
      }
    }
  }

  return false;

}

bool handleTouchedDevice() {
  deviceIndex = -1;

  if (ts.touched()) {
    CYD28_TS_Point p = ts.getPointScaled();
    Serial.print("Pressure = ");
    Serial.print(p.z);
    Serial.print(", x = ");
    Serial.print(p.x);
    Serial.print(", y = ");
    Serial.print(p.y);
    delay(30);
    Serial.println();

    int index = (p.y + 16) / 40.0;

    deviceIndex = index - 1;

    return true;
  }

  return false;
}