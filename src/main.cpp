#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include "widget.hpp"
#include "nav.hpp"

MCUFRIEND_kbv tft;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
const char name[] = "ndd2";
const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
const int TS_LEFT=934,TS_RT=156,TS_TOP=980,TS_BOT=118;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000

Slider slider1 = Slider(40, 60, 240, 25, 0, 100, 1, "Potencia", "%");
Slider slider2 = Slider(40, 120, 240, 25, 0, 30, 1, "Tempo on", "min");
Slider slider3 = Slider(40, 180, 240, 25, 0, 300, 10, "Tempo off", "seg");
Slider slider4 = Slider(40, 240, 240, 25, 1, 10, 1, "N Ciclos", "");
Slider slider5 = Slider(40, 300, 240, 25, 5, 15, 1, "Distancia", "cm");

void sw2on();
void sw2off();

Button btn1 = Button(90, 380, "Iniciar", sw2on);  //Start button
//Button btn2 = Button(90, 380, "Encerrar", sw2off);     //Stop button

//Timer tmr1 = Timer(89, 440, 0, sw2off);

//void play_entry();
//void play_exit();

void config_entry();
void config_exit();

Screen config = Screen(&tft, config_entry, config_exit);

bool play = false;
bool jss = true; //Just Switched States, begins as true because startup is a state swtich from off to on
uint16_t ID;
uint8_t Orientation = 0;    //PORTRAIT
uint8_t cycles;

void sw2on();
void sw2off();

void setup(void){
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  Serial.begin(115200);
  delay(10);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  delay(1000);

  tft.setCursor(51, 465);
  tft.print("Andre Mariano e Paulo Souza - IF UnB");

  config.add_widget(&slider1);
  config.add_widget(&slider2);
  config.add_widget(&slider3);
  config.add_widget(&slider4);
  config.add_widget(&slider5);
  config.add_widget(&btn1);

  config.entry();
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
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
  }

  config.update(xpos, ypos);
}

void sw2on(){
  //play = true;
  jss = true;
}

void sw2off(){
  //play = false;
  jss = true;
}

void play_entry(){

}

void play_exit(){

}

/*void play_update(int xpos, int ypos){
  if(btn2.clicked(xpos, ypos)){
    btn2.update(true);
    tmr1.erase(tft);
  }
}*/

void config_entry(){
  slider1.draw(&tft);
  slider2.draw(&tft);
  slider3.draw(&tft);
  slider4.draw(&tft);
  slider5.draw(&tft);

  btn1.draw(&tft);
}

void config_exit(){}
/************* SCREEN PLAY UPDATE ***************
    if(btn2.clicked(xpos, ypos)){
      btn2.update(true);
      tmr1.erase(tft);
    }

    if(millis() - tmr1.mark > 1000){
      tmr1.mark = millis();
      tmr1.update(tft);
    }
*/

/********** SCREEN SWITCHING ****************
  if(!play){  // CONFIG STATE?
    if(jss){  // JUST SWITCHED STATES?
      tmr1.erase(tft);
      btn2.erase(tft);
      btn1.draw_init(tft);
      jss = false;
    }
  }else{  // PLAY STATE?
    if(jss){  // JUST SWITCHED STATES?
      btn1.erase(tft);
      tmr1.draw_init(tft);
      btn2.draw_init(tft);
      jss = false;
    }
  }
*/