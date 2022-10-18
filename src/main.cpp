#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <gui.hpp>

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

Slider slider1 = Slider(40, 60);
Slider slider2 = Slider(40, 120);
Slider slider3 = Slider(40, 180);
Slider slider4 = Slider(40, 240);
Slider slider5 = Slider(40, 300);

Button btn1 = Button(90, 380);  //Start button
Button btn2 = Button(btn1);     //Stop button

Timer tmr1 = Timer(89, 440);

bool play = false;
bool jss = true; //Just Switched States, begins as true because startup is a state swtich from off to on
uint16_t ID;
uint8_t Orientation = 0;    //PORTRAIT
uint8_t cycles;

void sw2on();
void sw2off();
void timer_callback();

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

  slider1.configure(0, 100, "Potencia", "%", 240, 25);
  slider2.configure(0, 30, "Tempo on", "min", 240, 25);
  slider3.configure(0, 300, "Tempo off", "seg", 240, 25);
  slider4.configure(1, 10, "N Ciclos", "", 240, 25);
  slider5.configure(5, 15, "Distancia", "cm", 240, 25);

  btn1.configure("Iniciar", sw2on);
  btn2.configure("Encerrar", sw2off);

  tmr1.configure(0, sw2off);

  slider1.draw_init(tft);
  slider2.draw_init(tft);
  slider3.draw_init(tft);
  slider4.draw_init(tft);
  slider5.draw_init(tft);
}

void loop(){
  uint16_t xpos, ypos;  //screen coordinates
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

  if(!play){  // CONFIG STATE?
    if(jss){  // JUST SWITCHED STATES?
      tmr1.erase(tft);
      btn2.erase(tft);
      btn1.draw_init(tft);
      jss = false;
    }

    // Are we in the slider1 area?
    if(slider1.clicked(xpos, ypos)){
      slider1.update(xpos, tft);
    }

    // Are we in the slider2 area?
    if(slider2.clicked(xpos, ypos)){
      slider2.update(xpos, tft);
    }

    // Are we in the slider3 area?
    if(slider3.clicked(xpos, ypos)){
      slider3.update(xpos, tft, 10);
    }

    // Are we in the slider4 area?
    if(slider4.clicked(xpos, ypos)){
      slider4.update(xpos, tft);
    }

    // Are we in the slider5 area?
    if(slider5.clicked(xpos, ypos)){
      slider5.update(xpos, tft);
    }

    // Are we in the button1 area?
    if(btn1.clicked(xpos, ypos)){
      btn1.update(true);
      tmr1.configure(slider2.get_val()*60);
    }
  }else{  // PLAY STATE?
    if(jss){  // JUST SWITCHED STATES?
      btn1.erase(tft);
      tmr1.draw_init(tft);
      btn2.draw_init(tft);
      jss = false;
    }
    
    if(btn2.clicked(xpos, ypos)){
      btn2.update(true);
      tmr1.erase(tft);
    }

    if(millis() - tmr1.mark > 1000){
      tmr1.mark = millis();
      tmr1.update(tft);
    }
  }
}

void sw2on(){
  play = true;
  jss = true;
}

void sw2off(){
  play = false;
  jss = true;
}

void timer_callback(){
  
}