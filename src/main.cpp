#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>
#include <gui.hpp>

void redraw();
void drawWindow();

MCUFRIEND_kbv tft;

// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from TouchScreen_Calibr_native.ino
char name[] = {'n','d','d','2','\0'};
const int XP=6,XM=A2,YP=A1,YM=7; //320x480 ID=0x9486
const int TS_LEFT=934,TS_RT=156,TS_TOP=980,TS_BOT=118;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;

#define MINPRESSURE 200
#define MAXPRESSURE 1000
#define REDRAWDELAY 5

Slider slider1, slider2, slider3;

uint16_t ID;
uint8_t Orientation = 0;    //PORTRAIT

unsigned long redrawFlag;

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

void setup(void){
  tft.reset();
  ID = tft.readID();
  tft.begin(ID);
  Serial.begin(115200);
  delay(10);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  sliderInit(slider1, 40, 120, 0, 100);
  sliderInit(slider2, 40, 200, 0, 30);
  sliderInit(slider3, 40, 280, 5, 15);
  delay(1000);

  drawWindow();
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

    // Are we in the slider1 Y area?
    if(ypos > slider1.oy && ypos < slider1.oy+slider1.wid){
      slider1.redraw = true;
      if(xpos > slider1.ox-5 && xpos < slider1.ox)                                slider1.pos = 0;
      else if(xpos >= slider1.ox && xpos <= slider1.ox+slider1.len)               slider1.pos = xpos - slider1.ox;
      else if(xpos > slider1.ox+slider1.len && xpos < slider1.ox+slider1.len+5)   slider1.pos = slider1.len;
      else slider1.redraw = false;

      slider1.val = map(slider1.pos, 0, slider1.len, slider1.minVal, slider1.maxVal);
    }

    // Are we in the slider2 Y area?
    if(ypos > slider2.oy && ypos < slider2.oy+slider2.wid){
      slider2.redraw = true;
      if(xpos > slider2.ox-5 && xpos < slider2.ox)                                slider2.pos = 0;
      else if(xpos >= slider2.ox && xpos <= slider2.ox+slider2.len)               slider2.pos = xpos - slider2.ox;
      else if(xpos > slider2.ox+slider2.len && xpos < slider2.ox+slider2.len+5)   slider2.pos = slider2.len;
      else slider2.redraw = false;

      slider2.val = map(slider2.pos, 0, slider2.len, slider2.minVal, slider2.maxVal);
    }

    // Are we in the slider3 Y area?
    if(ypos > slider3.oy && ypos < slider3.oy+slider3.wid){
      slider3.redraw = true;
      if(xpos > slider3.ox-5 && xpos < slider3.ox)                                slider3.pos = 0;
      else if(xpos >= slider3.ox && xpos <= slider3.ox+slider3.len)               slider3.pos = xpos - slider3.ox;
      else if(xpos > slider3.ox+slider3.len && xpos < slider3.ox+slider3.len+5)   slider3.pos = slider1.len;
      else slider3.redraw = false;

      slider3.val = map(slider3.pos, 0, slider3.len, slider3.minVal, slider3.maxVal);
    }
  }
  
  if(millis() - redrawFlag > REDRAWDELAY){
    redraw();
    redrawFlag = millis();
  }
}

void redraw(){
  if(slider1.redraw){
    tft.fillRect(slider1.ox, slider1.oy, slider1.pos, slider1.wid, WHITE);
    tft.fillRect(slider1.ox+slider1.pos, slider1.oy, slider1.len-slider1.pos, slider1.wid, BLACK);
    
    tft.fillRect(slider1.valx, slider1.valy, slider1.len/4, slider1.wid, BLACK);
    tft.setCursor(slider1.valx, slider1.valy);
    tft.print(slider1.val);
    tft.print('%');
    
    slider1.redraw = false;
  }

  if(slider2.redraw){
    tft.fillRect(slider2.ox, slider2.oy, slider2.pos, slider2.wid, WHITE);
    tft.fillRect(slider2.ox+slider2.pos, slider2.oy, slider2.len-slider2.pos, slider2.wid, BLACK);

    tft.fillRect(slider2.valx, slider2.valy, slider2.len/4, slider2.wid, BLACK);
    tft.setCursor(slider2.valx, slider2.valy);
    tft.print(slider2.val);
    tft.print("min");
    
    slider2.redraw = false;
  }

  if(slider3.redraw){
    tft.fillRect(slider3.ox, slider3.oy, slider3.pos, slider3.wid, WHITE);
    tft.fillRect(slider3.ox+slider3.pos, slider3.oy, slider3.len-slider3.pos, slider3.wid, BLACK);

    tft.fillRect(slider3.valx, slider3.valy, slider3.len/4, slider3.wid, BLACK);
    tft.setCursor(slider3.valx, slider3.valy);
    tft.print(slider3.val);
    tft.print("cm");
    
    slider3.redraw = false;
  }
}

void drawWindow(){
  tft.setTextSize(2);
  
  tft.drawRect(slider1.ox-3, slider1.oy-3, slider1.len+6, slider1.wid+6, WHITE);
  tft.setCursor(slider1.ox-8, slider1.valy);
  tft.print(slider1.minVal);
  tft.setCursor(slider1.ox+28, slider1.valy);
  tft.print("Potencia=");
  tft.setCursor(slider1.valx, slider1.valy);
  tft.print(slider1.val);
  tft.print('%');
  tft.setCursor(slider1.ox+slider1.len-15, slider1.valy);
  tft.print(slider1.maxVal);

  tft.drawRect(slider2.ox-3, slider2.oy-3, slider2.len+6, slider2.wid+6, WHITE);
  tft.setCursor(slider2.ox-8, slider2.valy);
  tft.print(slider2.minVal);
  tft.setCursor(slider2.ox+38, slider2.valy);
  tft.print("Duracao=");
  tft.setCursor(slider2.valx, slider2.valy);
  tft.print(slider2.val);
  tft.print("min");
  tft.setCursor(slider2.ox+slider2.len-9, slider2.valy);
  tft.print(slider2.maxVal);
  
  
  tft.drawRect(slider3.ox-3, slider3.oy-3, slider3.len+6, slider3.wid+6, WHITE);
  tft.setCursor(slider3.ox-8, slider3.valy);
  tft.print(slider3.minVal);
  tft.setCursor(slider3.ox+18, slider3.valy);
  tft.print("Distancia=");
  tft.setCursor(slider3.valx, slider3.valy);
  tft.print(slider3.val);
  tft.print("cm");
  tft.setCursor(slider3.ox+slider3.len-9, slider3.valy);
  tft.print(slider3.maxVal);
}