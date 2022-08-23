#include "gui.hpp"

// 
void sliderInit(Slider& s, uint16_t x, uint16_t y, int minv, int maxv){
  s.ox = x;
  s.oy = y;

  s.minVal = minv;
  s.maxVal = maxv;
  s.valx = x+150;
  s.valy = s.oy+s.wid+11;
  s.val = minv;
  s.pos = 0;
  s.redraw = true;
}