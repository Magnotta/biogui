#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <nav.hpp>
#include <sensors.hpp>
#include <head.hpp>
#include "scr_config.hpp"
#include "scr_play.hpp"
#include "scr_error.hpp"

MCUFRIEND_kbv tft;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const char name[] = "ndd2";
const int XP = 6, XM = A2, YP = A1, YM = 7; //320x480 ID=0x9486
const int TS_LEFT = 934, TS_RT = 156, TS_TOP = 980, TS_BOT = 118;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
uint16_t ID;

LEDHead head{46, A9, A8, 65.0};

void setup(void){
  head.init();
  sys.set_home(&config);

  delay(100);

  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(0);   // Portrait orientation
  tft.fillScreen(BLACK);

  tft.setCursor(51, 465);
  tft.print("Andre Mariano e Paulo Souza - IF UnB");

  Config::add_widgets();
  Play::add_widgets();
  Error::add_widgets();

  sys.enter();
}

void loop(){
  uint16_t xpos = 0, ypos = 0;  //screen coordinates
  tp = ts.getPoint();   //tp.x, tp.y are ADC values

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);

  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if(tp.z > 200 && tp.z < 1000){
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
  }

  sys.loop(xpos, ypos);

  head.update();
  if(!head.temp_safe()){
    head.LEDOff();
    sys.goto_screen(&error);
  }
}
