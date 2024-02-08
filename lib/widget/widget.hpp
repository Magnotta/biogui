#ifndef LIB_WIDGET_WIDGET_HPP_
#define LIB_WIDGET_WIDGET_HPP_

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include "nav.fwd.hpp"
#include "nav.hpp"

// Assign human-readable names to some common 16-bit color values:
#define BLACK   0x0000
#define BLUE    0x001F
#define RED     0xF800
#define GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF

class Widget{
public:
    Widget(uint16_t, uint16_t);
    virtual bool clicked(uint16_t, uint16_t);
    virtual void update(MCUFRIEND_kbv*);
    virtual void draw(MCUFRIEND_kbv*);
    virtual void erase(MCUFRIEND_kbv*);
    virtual void activate();
    virtual void deactivate();
    virtual bool is_active();
    virtual bool is_clicker();
    virtual uint16_t getWid();
    virtual uint16_t getLen();

protected:
    uint16_t _ox;    // origin x coordinate
    uint16_t _oy;    // origin y coordinate
    uint16_t _len;   // total length of the slider bar in pixels
    uint16_t _wid;   // width of the slider bar in pixels
    uint16_t _cx;    // click x coordinate
    uint16_t _cy;    // click y coordinate
    bool _clicker;
    bool _active;    
    bool _clicking;  // true while the widget is being clicked on
    Router *_system;
};

class Slider : public Widget{
public:
    Slider(uint16_t, uint16_t, uint16_t, uint16_t, uint8_t, const char[], const char[]);
    bool clicked(uint16_t, uint16_t);
    void update(MCUFRIEND_kbv*);  // updates the value and redraws a slider
    void draw(MCUFRIEND_kbv*);  // initial drawing of slider to screen, should only be called once
    void erase(MCUFRIEND_kbv*);
    void reset();

    uint16_t get_val(); // returns current value the slider holds

protected:
    uint16_t _pos;           // current position of the slider bar in pixels
    uint16_t _val;           // current value the slider represents
    uint16_t _valx;          // x coord of where to write the current value
    uint16_t _valy;          // y coord of where to write the current value
    uint16_t _min_val;       // minimum value the slider can represent
    uint16_t _max_val;       // maximum value the slider can represent
    uint8_t _minv_offset;       // horizontal offset in pixels of where to write the minimum value
    uint8_t _maxv_offset;       // horizontal offset in pixels of where to write the maximum value
    uint8_t _label_offset;      // horizontal offset in pixels of where to write the slider label
    uint8_t _step;
    uint8_t _bar_wid;           // vertical width of the slider bar in pixels
    char _label[13];         // usually ends with an =
    char _unit[5];           // unit of measurement e.g. "%", "cm", "W/m2"

private:
    void redraw(MCUFRIEND_kbv*); // used internally for better code organization
};

class Button : public Widget{
public:
    Button(uint16_t, uint16_t, const char[], void (*)(), uint16_t);
    void update(MCUFRIEND_kbv *);  // executes the callback function
    void draw(MCUFRIEND_kbv *);  // initial drawing of button to screen, should only be called once

protected:
    void redraw(MCUFRIEND_kbv *);

    char _label[10];
    void (*_callback)();   // callback function pointer
    uint16_t _debounce;  // debounce delay in milliseconds
    uint32_t _mark; // for debouncing delay
};

class Label : public Widget{
public:
    Label(uint16_t, uint16_t, uint8_t, uint16_t, void (*)());
    void update(MCUFRIEND_kbv*);
    void draw(MCUFRIEND_kbv*);

    char text[25];

protected:
    void redraw(MCUFRIEND_kbv*);

    char _cmp[25];
    uint16_t _color;
    uint8_t _fontsize;
    void (*_set_text)();
};

class Timer : public Widget{
public:
    Timer(uint16_t, uint16_t, void (*)());
    void update(MCUFRIEND_kbv*); // reduces remaining seconds by one and updates display
    void arming_event(uint16_t); // set total countdown duration on the timer
    void hhmmss(char[]);  // prints remaining time into buffer, formatted (hh:mm:ss)
    void draw(MCUFRIEND_kbv*);  // draws the timer on the screen, should only be called once

protected:
    uint16_t mx;    // minutes x coord
    uint16_t sx;    // seconds x coord
    uint16_t len2dig;   // length of each couple of digits
    uint16_t secs;  // countdown duration left in seconds

    void (*_callback)();   // _callback for when timer reaches 0 seconds

private:
    void redraw(MCUFRIEND_kbv*); // used internally for better code organization
};

class NaviButton : public Widget{
public:    
    NaviButton(uint16_t, uint16_t, const char*, Screen*);
    void update(MCUFRIEND_kbv*);
    void draw(MCUFRIEND_kbv*);

protected:
    char _label[12];
    Screen *_dest;
};

#endif // LIB_WIDGET_WIDGET_HPP_
