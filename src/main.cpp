#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <nav.hpp>
#include <temperature.hpp>
#include <head.hpp>
#include "scr_config.hpp"
#include "scr_play.hpp"
#include "scr_error.hpp"

MCUFRIEND_kbv tft;

Router sys{&config, &config};

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const char name[] = "ndd2";
const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
const int TS_LEFT=934,TS_RT=156,TS_TOP=980,TS_BOT=118;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

uint16_t ID;

LEDHead head{44, A9, 30.0};

void setup(void){
  head.init();

  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  tft.setRotation(2);   // Portrait orientation
  tft.fillScreen(BLACK);

  delay(1000);

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
  if(tp.z > MINPRESSURE && tp.z < MAXPRESSURE){
    xpos = map(tp.x, TS_RT, TS_LEFT, 0, tft.width());
    ypos = map(tp.y, TS_BOT, TS_TOP, 0, tft.height());
  }

  sys.loop(xpos, ypos);
  
  if(!head.verify()){
    head.LEDOff();
    sys.goto_screen(&error);
  }
}