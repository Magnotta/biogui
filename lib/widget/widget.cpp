#include "widget.hpp"
#include "nav.hpp"

/*##########################################################################
* Widget ###################################################################
##########################################################################*/

extern Router sys;

/// @brief Initialize a widget
/// @param x origin x coordinate
/// @param y origin y coordinate
Widget::Widget(uint16_t ox, uint16_t oy){
  _ox = ox;
  _oy = oy;
  _len = 0;
  _wid = 0;
  _cx = 0;
  _cy = 0;
  _system = &sys;
  _active = true;
  _clicking = false;
  _clicker = true; // this default prevents Screen::update from updating invisible widgets
}

/// @brief Check whether the widget is active and was clicked on then update its clicking state
/// @param x The x coordinate of the click
/// @param y the y coordinate of the click
/// @return True if click is inside active widget area, false otherwise
bool Widget::clicked(uint16_t x, uint16_t y){
  _clicking = x > _ox && x < _ox+_len && y > _oy && y < _oy+_wid;
  if(_clicking){
    _cx = x;
    _cy = y;
  }
  return _clicking;
}

void Widget::erase(MCUFRIEND_kbv *scr){
  scr->fillRect(_ox, _oy, _len, _wid, BLACK);
}

void Widget::activate(){
  _active = true;
}

void Widget::deactivate(){
  _active = false;
}

bool Widget::is_active(){
  return _active;
}

bool Widget::is_clicker(){
  return _clicker;
}

uint16_t Widget::getWid(){
  return _wid;
}

uint16_t Widget::getLen(){
  return _len;
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
/// @param step Step size for the slider bar
/// @param label Label string
/// @param unit Unit string, for example "cm" or "W"
Slider::Slider(uint16_t x, uint16_t y, uint16_t minv, uint16_t maxv, uint8_t step, const char label[], const char unit[])
:Widget(x, y){
  _len = 240;
  _bar_wid = 20;
  _wid = _bar_wid+29;
  _min_val = minv;
  _max_val = maxv;
  _step = step;
  _valx = _ox+150;
  _valy = _oy+_bar_wid+11;
  _val = minv;
  _pos = 0;
  _clicker = true;

  if(strlen(label) < 12)  snprintf(_label, 12, "%s", label);
  else                  snprintf(_label, 12, "error");

  if(strlen(unit) < 4) snprintf(_unit, 4, "%s", unit);
  else              snprintf(_unit, 4, "err");

  strncat(_label, "=", 1);
  uint8_t len = strlen(_label);
  _label_offset = 118 - len*10;

  if(_min_val < 10)  _minv_offset = 8;
  else              _minv_offset = 2;

  if(_max_val < 10)        _maxv_offset = 2;
  else if(_max_val < 100)  _maxv_offset = 8;
  else                    _maxv_offset = 14;
}

bool Slider::clicked(uint16_t x, uint16_t y){
  _clicking = x > _ox && x < _ox+_len+3 && y > _oy && y < _oy+_wid;
  if(_clicking){
    _cx = x;
    _cy = y;
  }
  return _clicking;
}

/// @brief Update slider value and redraw it
/// @param scr Display screen
void Slider::update(MCUFRIEND_kbv *scr){
  if(_cx > _ox-3 && _cx <= _ox)              _pos = 0;
  else if(_cx >= _ox && _cx <= _ox+_len)      _pos = _cx - _ox;
  else if(_cx >= _ox+_len && _cx < _ox+_len+3) _pos = _len;

  _val = map(_pos, 0, _len, _min_val, _max_val);
  _val = _val - _val % _step;

  redraw(scr);
}

// Used internally for better code organization
void Slider::redraw(MCUFRIEND_kbv *scr){
  scr->setTextSize(2);
  scr->fillRect(_ox, _oy, _pos, _bar_wid, WHITE);
  scr->fillRect(_ox+_pos, _oy, _len-_pos, _bar_wid, BLACK);

  scr->fillRect(_valx, _valy, _ox+_len-_maxv_offset-_valx, 16, BLACK);
  scr->setCursor(_valx, _valy);
  scr->print(_val);
  scr->print(_unit);
}

/// @brief Draw slider on the display. Calling more than once is redundant
/// @param scr display screen
void Slider::draw(MCUFRIEND_kbv *scr){
  scr->drawRect(_ox-3, _oy-3, _len+6, _bar_wid+6, WHITE);
  scr->fillRect(_ox, _oy, _pos, _bar_wid, WHITE);

  scr->setTextSize(2);
  scr->setCursor(_ox-_minv_offset, _valy);
  scr->print(_min_val);
  scr->setCursor(_ox+_label_offset, _valy);
  scr->print(_label);
  scr->setCursor(_valx, _valy);
  scr->print(_val);
  scr->print(_unit);
  scr->setCursor(_ox+_len-_maxv_offset, _valy);
  scr->print(_max_val);
}

void Slider::erase(MCUFRIEND_kbv *scr){
  scr->drawRect(_ox-3, _oy-3, _len+6, _bar_wid+6, BLACK);
  scr->fillRect(_ox, _oy, _len, _bar_wid, BLACK);
  scr->fillRect(_ox-10, _oy+_bar_wid+11, _len+30, 16, BLACK);
}

void Slider::reset(){
  _val = _min_val;
  _pos = 0;
}

uint16_t Slider::get_val(){
  return _val;
}

/*##########################################################################
* Button ###################################################################
##########################################################################*/

/// @brief Button constructor
/// @param x Origin (top left corner) x coordinate
/// @param y Origin (top left corner) y coordinate
/// @param label Label to be written inside the button
/// @param callback Function pointer to callback function
Button::Button(uint16_t x, uint16_t y, const char label[], void (*callback)(), uint16_t delay)
:Widget(x, y){
  if(strlen(label) < 10) snprintf(_label, 10, "%s", label);
  else                    snprintf(_label, 10, "error");

  _len = 18*strlen(_label) + 7; // -3 from formula, +10 from text displacement
  _wid = 34; // 24 from formula, +10 from text displacement
  _callback = callback;
  _debounce = delay;
  _clicker = true;
}

/// @brief Run the callback and debounce
/// @param scr Display screen
void Button::update(MCUFRIEND_kbv *scr){
  if(millis() - _mark > _debounce){
    _mark = millis();
    (*_callback)();
    redraw(scr);
  }
}

/// @brief Draw button on the display. Calling more than once is redundant
/// @param scr Display screen
void Button::draw(MCUFRIEND_kbv *scr){
  scr->drawRect(_ox, _oy, _len, _wid, WHITE);
  scr->drawRect(_ox+1, _oy+1, _len-2, _wid-2, WHITE);
  scr->drawRect(_ox+2, _oy+2, _len-4, _wid-4, WHITE);
  scr->setCursor(_ox+5, _oy+5);
  scr->setTextSize(3);
  scr->print(_label);
}

void Button::redraw(MCUFRIEND_kbv *scr){
  scr->fillRect(_ox, _oy, _len, _wid, BLACK);
  if(_clicking){
    scr->drawRect(_ox, _oy, _len, _wid, RED);
    scr->drawRect(_ox+1, _oy+1, _len-2, _wid-2, RED);
    scr->drawRect(_ox+2, _oy+2, _len-4, _wid-4, RED);
    scr->setTextColor(RED);
  }else{
    scr->drawRect(_ox, _oy, _len, _wid, WHITE);
    scr->drawRect(_ox+1, _oy+1, _len-2, _wid-2, WHITE);
    scr->drawRect(_ox+2, _oy+2, _len-4, _wid-4, WHITE);
  }
  scr->setCursor(_ox+5, _oy+5);
  scr->setTextSize(3);
  scr->print(_label);
  scr->setTextColor(WHITE);
}

/*##########################################################################
* Label ####################################################################
##########################################################################*/

/// @brief Display dynamic text on screen
/// @param x Origin (top left corner) x coordinate
/// @param y Origin (top left corner) y coordinate
/// @param fontsize Text fontsize in pixels
/// @param color Text color
/// @param setter Function to set the label text, typically a wrapper around an snprintf call (buffer size 25)
Label::Label(uint16_t x, uint16_t y, uint8_t fontsize, uint16_t color, void (*setter)())
:Widget(x, y){
  _fontsize = fontsize;
  _color = color;
  _set_text = setter;
  _clicker = false;

  (*_set_text)();
  byte _l = strlen(text);

  switch(_fontsize){
  case 1:
    _len = 6*_l-1;
    _wid = 8;
    break;
  case 2:
    _len = 12*_l-2;
    _wid = 16;
    break;
  case 3:
    _len = 18*_l-3;
    _wid = 24;
    break;
  default:
    snprintf(text, 25, "Error");
    _set_text = nullptr;
    _fontsize = 2;
    _len = 58;
    _wid = 14;
  }
}

void Label::update(MCUFRIEND_kbv *scr){
  if(_system->once_per_sec()){
    if(_set_text != nullptr)
      (*_set_text)();
    if(strcmp(_cmp, text)){
      snprintf(_cmp, 25, "%s", text);
      _len = _fontsize * 6 * strlen(text) - _fontsize;
      redraw(scr);
    }
  }
}

void Label::draw(MCUFRIEND_kbv *scr){
  scr->setCursor(_ox, _oy);
  scr->setTextSize(_fontsize);
  scr->setTextColor(_color);
  scr->print(text);
  scr->setTextColor(WHITE);
}

void Label::redraw(MCUFRIEND_kbv *scr){
  erase(scr);
  draw(scr);
}

/*##########################################################################
* Timer ####################################################################
##########################################################################*/

/// @brief Initialize a timer
/// @param x origin x coordinate
/// @param y origin y coordinate
Timer::Timer(uint16_t x, uint16_t y, void (*callback)())
:Widget(x, y){
  _wid = 28;
  _len = 188;
  len2dig = 44;
  mx = _ox + 72;
  sx = mx + 72;
  _callback = callback;
  _clicker = false;
}

/// @brief Decrease timer seconds and update the display. When zero is reached, run the callback function
/// @param scr display screen
void Timer::update(MCUFRIEND_kbv *scr){
  if(_system->once_per_sec()){
    if(secs)  secs--; 
    if(!secs){
      (*_callback)();
      return;
    }

    redraw(scr);
  }
}

// Used internally for better code organization
void Timer::redraw(MCUFRIEND_kbv *scr){
  scr->fillRect(_ox, _oy, len2dig, _wid, BLACK);
  scr->fillRect(mx, _oy, len2dig, _wid, BLACK);
  scr->fillRect(sx, _oy, len2dig, _wid, BLACK);
  scr->setCursor(_ox, _oy);
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
    if(seconds < 10)  snprintf(buf, 10, "0%d:0%d:0%d", hours, minutes, seconds);
    else              snprintf(buf, 10, "0%d:0%d:%d", hours, minutes, seconds);
  }else{
    if(seconds < 10)  snprintf(buf, 10, "0%d:%d:0%d", hours, minutes, seconds);
    else              snprintf(buf, 10, "0%d:%d:%d", hours, minutes, seconds);
  }
}

/// @brief Draw timer to display. Calling more than once is redundant
/// @param scr Display screen
void Timer::draw(MCUFRIEND_kbv *scr){
  scr->setCursor(_ox, _oy);
  scr->setTextSize(4);
  scr->setTextColor(RED);
  char aux[10];
  hhmmss(aux);
  scr->print(aux);
  scr->setTextColor(WHITE);
}

/// @brief Activate a timer and set its countdown value
/// @param t Time span for the countdown in seconds
void Timer::arming_event(uint16_t t){
  activate();
  secs = t;
}

/*##########################################################################
* Navibutton ###############################################################
##########################################################################*/

/// @brief Constructor for Navigation Button
/// @param x Origin (top left corner) x coordinate
/// @param y Origin (top left corner) y coordinate
/// @param label Label to be written inside the button
/// @param dest Pointer to the destination screen
NaviButton::NaviButton(uint16_t x, uint16_t y, const char *label, Screen *dest)
:Widget(x, y){
  if(strlen(label) < 10) snprintf(_label, 10, "%s", label);
  else                    snprintf(_label, 10, "error");

  _len = 18*strlen(label) + 7; // -3 from formula, +10 from text displacement
  _wid = 34; // 24 from formula, +10 from text displacement
  _clicker = true;
}

/// @brief Jump to pointed screen
/// @param scr Display screen
void NaviButton::update(MCUFRIEND_kbv *scr){
  _system->goto_screen(_dest);
}

void NaviButton::draw(MCUFRIEND_kbv *scr){
  scr->drawRect(_ox, _oy, _len, _wid, WHITE);
  scr->drawRect(_ox+1, _oy+1, _len-2, _wid-2, WHITE);
  scr->drawRect(_ox+2, _oy+2, _len-4, _wid-4, WHITE);
  scr->setCursor(_ox+5, _oy+5);
  scr->setTextSize(3);
  scr->print(_label);
}