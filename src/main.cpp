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

Slider slider1 = Slider(40, 60, 0, 100);
Slider slider2 = Slider(40, 120, 0, 30);
Slider slider3 = Slider(40, 180, 5, 15);
Slider slider4 = Slider(40, 240, 0, 300);

Button btn1 = Button(90, 320);

Timer tmr1 = Timer(110, 380);

bool play = false;
uint16_t ID;
uint8_t Orientation = 0;    //PORTRAIT
unsigned long timer;

void btn1_callback();

void setup(void){
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  Serial.begin(115200);
  delay(10);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  delay(1000);

  slider1.configure("Potencia", "%");
  slider2.configure("Duracao", "min");
  slider3.configure("Distancia", "cm");
  slider4.configure("Descanso", "seg");

  btn1.configure("Iniciar", btn1_callback);

  tmr1.configure(0);

  slider1.draw_init(tft);
  slider2.draw_init(tft);
  slider3.draw_init(tft);
  slider4.draw_init(tft);

  btn1.draw_init(tft);
}

void loop(){
  uint16_t xpos, ypos;  //screen coordinates
  tp = ts.getPoint();   //tp.x, tp.y are ADC values

  // if sharing pins, you'll need to fix the directions of the touchscreen pins
  pinMode(XM, OUTPUT);
  pinMode(YP, OUTPUT);
  
  // we have some minimum pressure we consider 'valid'
  // pressure of 0 means no pressing!
  if (tp.z > MINPRESSURE && tp.z < MAXPRESSURE) {
    // most mcufriend have touch (with icons) that extends below the TFT
    // screens without icons need to reserve a space for "erase"
    // scale the ADC values from ts.getPoint() to screen values e.g. 0-239
    //
    // Calibration is true for PORTRAIT. tp.y is always long dimension 
    // map to your current pixel orientation
    xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
    ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());

    if(!play){

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
        slider3.update(xpos, tft);
      }

      // Are we in the slider4 area?
      if(slider4.clicked(xpos, ypos)){
        slider4.update(xpos, tft, 10);
      }

      // Are we in the button1 area?
      if(btn1.clicked(xpos, ypos)){
        btn1.update(tft);
        tmr1.configure(slider2.get_val()*60);
      }
    }else{
      tmr1.draw_init(tft);
    }
  }
}

void btn1_callback(){
  play = true;
}