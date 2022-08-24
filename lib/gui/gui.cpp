#include "gui.hpp"
#include <MCUFRIEND_kbv.h>

Slider::Slider(uint16_t x, uint16_t y, byte minv, byte maxv){
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

void Slider::redraw(MCUFRIEND_kbv scr){
  scr.setTextSize(2);
  scr.fillRect(ox, oy, pos, wid, WHITE);
  scr.fillRect(ox+pos, oy, len-pos, wid, BLACK);
  
  scr.fillRect(valx, valy, len/4, 15, BLACK);
  scr.setCursor(valx, valy);
  scr.print(val);
  scr.print(unit);
}

void Slider::draw_init(MCUFRIEND_kbv scr){
  active = true;
  
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