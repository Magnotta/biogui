#include "widget.hpp"

/*##########################################################################
* Widget ###################################################################
##########################################################################*/

/// @brief Initialize a widget
/// @param x origin x coordinate
/// @param y origin y coordinate
Widget::Widget(uint16_t _ox = 0, uint16_t _oy = 0, uint16_t _len = 0, uint16_t _wid = 0){
  ox = _ox;
  oy = _oy;
  len = _len;
  wid = _wid;
  active = true;
  clicking = false;
}

/// @brief Check whether the widget was clicked  on
/// @param _x The x coordinate of the click
/// @param _y the y coordinate of the click
/// @return True if click is inside widget area, false otherwise
bool Widget::clicked(uint16_t _x, uint16_t _y){
  return active && _x > ox && _x < ox+len && _y > oy && _y < oy+wid;
}

void Widget::erase(MCUFRIEND_kbv *_scr){
  _scr->fillRect(ox, oy, len, wid, BLACK);
}

void Widget::activate(){
  active = true;
}

void Widget::deactivate(){
  active = false;
}

bool Widget::is_active(){
  return active;
}

/*##########################################################################
* Slider ###################################################################
##########################################################################*/

/// @brief Slider constructor
/// @param x Origin (top left corner) x coordinate
/// @param y Origin (top left corner) y coordinate
/// @param l Horizontal length of the slider in pixels
/// @param w Vertixal width of the slider in pixels
/// @param minv Minimum value the slider should represent
/// @param maxv Maximum value the slider should represent
/// @param _step Step size for the slider bar
/// @param lbl Label string
/// @param u Unit string, for example "cm" or "W"
Slider::Slider(uint16_t x, uint16_t y, uint16_t l, uint16_t w, uint16_t minv, uint16_t maxv, uint8_t _step, const char lbl[], const char u[]){
  ox = x;
  oy = y;
  len = l;
  bar_wid = w;
  wid = bar_wid+29;
  min_val = minv;
  max_val = maxv;
  step = _step;
  valx = ox+150;
  valy = oy+bar_wid+11;
  val = minv;
  pos = 0;

  if(strlen(lbl) < 12)  strcpy(label, lbl);
  else                  strcpy(label, "error");

  if(strlen(u) < 4) strcpy(unit, u);
  else              strcpy(unit, "err");

  strcat(label, "=");
  uint8_t len = strlen(label);
  label_offset = 118 - len*10;

  if(min_val < 10)  minv_offset = 8;
  else              minv_offset = 2;

  if(max_val < 10)        maxv_offset = 2;
  else if(max_val < 100)  maxv_offset = 8;
  else                    maxv_offset = 14;
}

/// @brief Check wheter a click is inside slider area. If it is, saves the x coordinate of the click.
/// @param xpos The x coordinate of the click
/// @param ypos The y coordinate of the click
/// @return True or false
bool Slider::clicked(uint16_t xpos, uint16_t ypos){
  bool aux = xpos > ox-3 && xpos < ox+len+3 && ypos > oy && ypos < oy+bar_wid;
  if(aux) cx = xpos;
  return aux;
}

/// @brief Update slider value and redraw it
/// @param scr Display screen
void Slider::update(MCUFRIEND_kbv *scr){
  if(cx > ox-3 && cx <= ox)              pos = 0;
  else if(cx >= ox && cx <= ox+len)      pos = cx - ox;
  else if(cx >= ox+len && cx < ox+len+3) pos = len;

  val = map(pos, 0, len, min_val, max_val);
  val = val - val % step;

  redraw(scr);
}

// Used internally for better code organization
void Slider::redraw(MCUFRIEND_kbv *scr){
  scr->setTextSize(2);
  scr->fillRect(ox, oy, pos, bar_wid, WHITE);
  scr->fillRect(ox+pos, oy, len-pos, bar_wid, BLACK);
  
  scr->fillRect(valx, valy, ox+len-maxv_offset-valx, 16, BLACK);
  scr->setCursor(valx, valy);
  scr->print(val);
  scr->print(unit);
}

/// @brief Draw slider on the display. Calling more than once is redundant
/// @param scr display screen
void Slider::draw(MCUFRIEND_kbv *scr){  
  scr->setTextSize(2);
  scr->drawRect(ox-3, oy-3, len+6, bar_wid+6, WHITE);
  scr->setCursor(ox-minv_offset, valy);  
  scr->print(min_val);
  scr->setCursor(ox+label_offset, valy);
  scr->print(label);
  scr->setCursor(valx, valy);
  scr->print(val);
  scr->print(unit);
  scr->setCursor(ox+len-maxv_offset, valy);
  scr->print(max_val);
}

void Slider::erase(MCUFRIEND_kbv *scr){
  scr->drawRect(ox-3, oy-3, len+6, bar_wid+6, BLACK);
  scr->fillRect(ox, oy, len, bar_wid, BLACK);
  scr->fillRect(ox-10, oy+bar_wid+11, len+30, 16, BLACK);
}

uint16_t Slider::get_val(){
  return val;
}

/*##########################################################################
* Button ###################################################################
##########################################################################*/

/// @brief Button constructor
/// @param x Origin (top left corner) x coordinate
/// @param y Origin (top left corner) y coordinate
/// @param lbl Label to be written inside the button
/// @param _callback Function pointer to callback function
Button::Button(uint16_t x, uint16_t y, const char lbl[], void (*_callback)(), uint16_t delay){
  ox = x;
  oy = y;

  if(strlen(lbl) < 12)  strcpy(label, lbl);
  else                  strcpy(label, "error");

  len = 19 * strlen(label) + 5;
  wid = 31;

  callback = _callback;

  debounce = delay;
}

bool Button::clicked(uint16_t _xpos, uint16_t _ypos){
  clicking = active && _xpos > ox && _xpos < ox+len && _ypos > oy && _ypos < oy+wid;
  return clicking;
}

/// @brief Run the callback function
/// @param scr Display screen
void Button::update(MCUFRIEND_kbv *scr){
  if(millis() - mark > 250){
    mark = millis();
    (*callback)();
    redraw(scr);
  }
}

/// @brief Draw button on the display. Calling more than once is redundant
/// @param scr Display screen
void Button::draw(MCUFRIEND_kbv *scr){
  scr->drawRect(ox, oy, len, wid, WHITE);
  scr->drawRect(ox+1, oy+1, len-2, wid-2, WHITE);
  scr->drawRect(ox+2, oy+2, len-4, wid-4, WHITE);
  scr->setCursor(ox+5, oy+5);
  scr->setTextSize(3);
  scr->print(label);
}

void Button::redraw(MCUFRIEND_kbv *scr){
  scr->fillRect(ox, oy, len, wid, BLACK);
  if(clicking){
    scr->drawRect(ox, oy, len, wid, RED);
    scr->drawRect(ox+1, oy+1, len-2, wid-2, RED);
    scr->drawRect(ox+2, oy+2, len-4, wid-4, RED);
    scr->setTextColor(RED);
  }else{
    scr->drawRect(ox, oy, len, wid, WHITE);
    scr->drawRect(ox+1, oy+1, len-2, wid-2, WHITE);
    scr->drawRect(ox+2, oy+2, len-4, wid-4, WHITE);
  }
  scr->setCursor(ox+5, oy+5);
  scr->setTextSize(3);
  scr->print(label);
  scr->setTextColor(WHITE);
}

/*##########################################################################
* Label ####################################################################
##########################################################################*/

Label::Label(char txt[]){
  if(strlen(txt) < 12)  strcpy(text, txt);
}

/*##########################################################################
* Timer ####################################################################
##########################################################################*/

/// @brief Initialize a timer
/// @param x origin x coordinate
/// @param y origin y coordinate
Timer::Timer(uint16_t x, uint16_t y, uint16_t t, void (*_callback)()){
  ox = x;
  oy = y;
  wid = 28;
  len = 188;
  len2dig = 44;
  mx = ox + 72;
  sx = mx + 72;
  secs = t;

  callback = _callback;
}

bool Timer::clicked(uint16_t x, uint16_t y){
  return active;
}

/// @brief Decrease timer seconds and update the display. When zero is reached, run the callback function
/// @param scr display screen
void Timer::update(MCUFRIEND_kbv *scr){
  if(millis() - mark > 1000){
    mark = millis();
    if(secs)  secs--; 
    if(!secs){
      (*callback)();
      return;
    }

    redraw(scr);
  }
}

// Used internally for better code organization
void Timer::redraw(MCUFRIEND_kbv *scr){
  scr->fillRect(ox, oy, len2dig, wid, BLACK);
  scr->fillRect(mx, oy, len2dig, wid, BLACK);
  scr->fillRect(sx, oy, len2dig, wid, BLACK);
  scr->setCursor(ox, oy);
  scr->setTextSize(4);
  scr->setTextColor(RED);
  char aux[10];
  hhmmss(aux);
  scr->print(aux);
  scr->setTextColor(WHITE);
}

/// @brief Get formatted representation (hh:mm:ss) of the time left in the timer
/// @param buf Buffer where the string will be printed.
void Timer::hhmmss(char* buf){
  uint16_t remainder = secs;
  uint8_t seconds = remainder % 60;
  remainder /= 60;
  uint8_t minutes = remainder % 60;
  remainder /= 60;
  uint8_t hours = remainder % 60;

  if(minutes < 10){
    if(seconds < 10)  sprintf(buf, "0%d:0%d:0%d", hours, minutes, seconds);
    else              sprintf(buf, "0%d:0%d:%d", hours, minutes, seconds);
  }else{
    if(seconds < 10)  sprintf(buf, "0%d:%d:0%d", hours, minutes, seconds);
    else              sprintf(buf, "0%d:%d:%d", hours, minutes, seconds);
  }
}

/// @brief Draw timer to display. Calling more than once is redundant
/// @param scr Display screen
void Timer::draw(MCUFRIEND_kbv *scr){
  scr->setCursor(ox, oy);
  scr->setTextSize(4);
  scr->setTextColor(RED);
  char aux[10];
  hhmmss(aux);
  scr->print(aux);
  scr->setTextColor(WHITE);
}

void Timer::arming_event(uint16_t t){
  secs = t;
  mark = millis();
}