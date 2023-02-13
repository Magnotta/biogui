#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <Arduino.h>
#include <MCUFRIEND_kbv.h>

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
    Widget(uint16_t _ox, uint16_t _oy);
    virtual bool clicked(uint16_t _xpos, uint16_t _ypos);
    virtual void update(MCUFRIEND_kbv *_scr);
    virtual void draw(MCUFRIEND_kbv *_scr);
    virtual void erase(MCUFRIEND_kbv *_scr);
    virtual void activate();
    virtual void deactivate();
    virtual bool is_active();
    virtual bool is_clicker();
protected:
    uint16_t ox;    // origin x coordinate
    uint16_t oy;    // origin y coordinate
    uint16_t len;   // total length of the slider bar in pixels
    uint16_t wid;   // width of the slider bar in pixels
    uint16_t cx;    // click x coordinate
    uint16_t cy;    // click y coordinate
    bool clicker;
    bool active;    
    bool clicking;  // True while the widget is being clicked on
};

class Slider : public Widget{
public:
    Slider(uint16_t x, uint16_t y, uint16_t minv, uint16_t maxv, uint8_t _step, const char lbl[], const char u[]);
    bool clicked(uint16_t _xpos, uint16_t _ypos);
    void update(MCUFRIEND_kbv *scr);  // updates the value and redraws a slider
    void draw(MCUFRIEND_kbv *scr);  // initial drawing of slider to screen, should only be called once
    void erase(MCUFRIEND_kbv *scr);
    void reset();
    
    uint16_t get_val(); //returns current value the slider holds
protected:
    uint16_t pos;           // current position of the slider bar in pixels
    uint16_t val;           // current value the slider represents
    uint16_t valx;          // x coord of where to write the current value
    uint16_t valy;          // y coord of where to write the current value
    uint16_t min_val;       // minimum value the slider can represent
    uint16_t max_val;       // maximum value the slider can represent
    uint8_t minv_offset;       // horizontal offset in pixels of where to write the minimum value
    uint8_t maxv_offset;       // horizontal offset in pixels of where to write the maximum value
    uint8_t label_offset;      // horizontal offset in pixels of where to write the slider label
    uint8_t step;
    uint8_t bar_wid;           // vertical width of the slider bar in pixels
    char label[13];         // usually ends with an =
    char unit[5];           // unit of measurement e.g. "%", "cm", "W/m2"
private:
    void redraw(MCUFRIEND_kbv *scr); // used internally for better code organization
};

class Button : public Widget{
public:
    Button(uint16_t x, uint16_t y, const char *lbl, void (*_callback)(), uint16_t delay);
    Button(uint16_t x, uint16_t y, const char *lbl, void (*_callback)()) : Button{x, y, lbl, _callback, 250}{}
    void update(MCUFRIEND_kbv *scr);  // executes the callback function
    void draw(MCUFRIEND_kbv *scr);  // initial drawing of button to screen, should only be called once
protected:
    void redraw(MCUFRIEND_kbv *scr);

    char label[10];
    void (*callback)();   // callback function pointer
    uint16_t debounce;  // debounce dedlay in milliseconds
    unsigned long mark; // For debouncing delay
};

class Label : public Widget{
public:
    Label(uint16_t x, uint16_t y, uint8_t size, uint16_t c, void (*setter)());
    void update(MCUFRIEND_kbv *scr);
    void draw(MCUFRIEND_kbv *scr);

    char text[20];
protected:
    void redraw(MCUFRIEND_kbv *scr);

    char cmp[20];
    uint16_t color;
    uint8_t fontsize;
    void (*set_text)();
};

class Timer : public Widget{
public:
    Timer(uint16_t x, uint16_t y, void (*_callback)());
    void update(MCUFRIEND_kbv *scr); // reduces remaining seconds by one and updates display
    void arming_event(uint16_t t); // set total countdown duration on the timer
    void hhmmss(char* buf);  // Prints remaining time into buffer, formatted (hh:mm:ss)
    void draw(MCUFRIEND_kbv *scr);  // Draws the timer on the screen, should only be called once
protected:
    uint16_t mx;    // minutes x coord
    uint16_t sx;    // seconds x coord
    uint16_t len2dig;   // length of each couple of digits
    uint16_t secs;  // countdown duration left in seconds
    unsigned long mark; // for detecting one second intervals with millis()

    void (*callback)();   // callback for when timer reaches 0 seconds
private:
    void redraw(MCUFRIEND_kbv *scr); // used internally for better code organization
};

#endif
