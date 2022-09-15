#include "gui.hpp"
#include <MCUFRIEND_kbv.h>

Slider::Slider(uint16_t x, uint16_t y, uint16_t minv, uint16_t maxv){
  ox = x;
  oy = y;
  min_val = minv;
  max_val = maxv;
  valx = x+150;
  valy = oy+wid+11;
  val = minv;
  pos = 0;
}

bool Slider::clicked(uint16_t xpos, uint16_t ypos){
  return xpos > ox-5 && xpos < ox+len+5 && ypos > oy && ypos < oy+wid;
}

void Slider::update(uint16_t xpos, MCUFRIEND_kbv scr){
  if(xpos > ox-5 && xpos < ox)              pos = 0;
  else if(xpos >= ox && xpos <= ox+len)     pos = xpos - ox;
  else if(xpos > ox+len && xpos < ox+len+5) pos = len;

  val = map(pos, 0, len, min_val, max_val);

  redraw(scr);
}

void Slider::update(uint16_t xpos, MCUFRIEND_kbv scr, byte step){
  if(xpos > ox-5 && xpos < ox)              pos = 0;
  else if(xpos >= ox && xpos <= ox+len)     pos = xpos - ox;
  else if(xpos > ox+len && xpos < ox+len+5) pos = len;

  val = map(pos, 0, len, min_val, max_val);
  val = val - val % step;

  redraw(scr);
}

void Slider::redraw(MCUFRIEND_kbv scr){
  scr.setTextSize(2);
  scr.fillRect(ox, oy, pos, wid, WHITE);
  scr.fillRect(ox+pos, oy, len-pos, wid, BLACK);
  
  scr.fillRect(valx, valy, ox+len-maxv_offset-valx, 16, BLACK);
  scr.setCursor(valx, valy);
  scr.print(val);
  scr.print(unit);
}

void Slider::draw_init(MCUFRIEND_kbv scr){  
  scr.setTextSize(2);
  scr.drawRect(ox-3, oy-3, len+6, wid+6, WHITE);
  scr.setCursor(ox-minv_offset, valy);  
  scr.print(min_val);
  scr.setCursor(ox+label_offset, valy);
  scr.print(label);
  scr.setCursor(valx, valy);
  scr.print(val);
  scr.print(unit);
  scr.setCursor(ox+len-maxv_offset, valy);
  scr.print(max_val);
}

void Slider::configure(const char lbl[], const char u[]){
  if(strlen(lbl) < 12)  strcpy(label, lbl);
  else                  strcpy(label, "error");

  if(strlen(u) < 4) strcpy(unit, u);
  else              strcpy(unit, "err");

  strcat(label, "=");
  byte len = strlen(label);
  label_offset = 118 - len*10;

  if(min_val < 10)  minv_offset = 8;
  else              minv_offset = 2;

  if(max_val < 10)        maxv_offset = 2;
  else if(max_val < 100)  maxv_offset = 8;
  else                    maxv_offset = 14;
}

uint16_t Slider::get_val(){
  return val;
}

Button::Button(uint16_t x, uint16_t y){
  ox = x;
  oy = y;
}

bool Button::clicked(uint16_t xpos, uint16_t ypos){
  return xpos > ox && xpos < ox+len && ypos > oy && ypos < oy+wid;
}

void Button::update(MCUFRIEND_kbv scr){
  (*cb)();
  delay(150);
}

void Button::configure(const char lbl[], void (*callback)()){
  if(strlen(lbl) < 12)  strcpy(label, lbl);
  else                  strcpy(label, "error");

  len = 19 * strlen(label) + 5;
  wid = 31;

  cb = callback;
}

void Button::draw_init(MCUFRIEND_kbv scr){
  scr.drawRect(ox-1, oy-1, len+2, wid+2, WHITE);
  scr.drawRect(ox, oy, len, wid, WHITE);
  scr.drawRect(ox+1, oy+1, len-2, wid-2, WHITE);
  scr.setCursor(ox+5, oy+5);
  scr.setTextSize(3);
  scr.print(label);
}

Timer::Timer(uint16_t x, uint16_t y){
  ox = x;
  oy = y;
}

void Timer::configure(uint16_t t){
  secs = t;
}

void Timer::print(char* buf){
  uint16_t remainder = secs;
  byte seconds = remainder % 60;
  remainder /= 60;
  byte minutes = remainder % 60;
  remainder /= 60;
  byte hours = remainder % 60;
  sprintf(buf, "%d:%d:%d", hours, minutes, seconds);
}

void Timer::draw_init(MCUFRIEND_kbv scr){
  scr.setCursor(ox, oy);
  scr.setTextSize(3);
  scr.setTextColor(RED);
  char buffer[10];
  print(buffer);
  scr.print(buffer);
}