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
    Widget(uint16_t _ox, uint16_t _oy, uint16_t _len, uint16_t _wid);
    virtual bool clicked(uint16_t _xpos, uint16_t _ypos);
    virtual void update(MCUFRIEND_kbv *_scr);
    virtual void draw(MCUFRIEND_kbv *_scr);
protected:
    uint16_t ox;    // origin x coordinate
    uint16_t oy;    // origin y coordinate
    uint16_t len;   // total length of the slider bar in pixels
    uint16_t wid;   // width of the slider bar in pixels
};

class Slider : public Widget{
public:
    Slider(uint16_t x, uint16_t y, uint16_t l, uint16_t w, uint16_t minv, uint16_t maxv, byte _step, const char lbl[], const char u[]);
    bool clicked(uint16_t xpos, uint16_t ypos); // returns whether (xpos, ypos) is inside slider area
    void update(MCUFRIEND_kbv *scr);  // updates the value and redraws a slider
    void draw(MCUFRIEND_kbv *scr);  // initial drawing of slider to screen, should only be called once
    uint16_t get_val(); //returns current value the slider holds
protected:
    uint16_t pos;           // current position of the slider bar in pixels
    uint16_t val;           // current value the slider represents
    uint16_t valx;          // x coord of where to write the current value
    uint16_t valy;          // y coord of where to write the current value
    uint16_t min_val;       // minimum value the slider can represent
    uint16_t max_val;       // maximum value the slider can represent
    uint16_t cx;            // Click x coordinate
    byte minv_offset;       // horizontal offset in pixels of where to write the minimum value
    byte maxv_offset;       // horizontal offset in pixels of where to write the maximum value
    byte label_offset;      // horizontal offset in pixels of where to write the slider label
    byte step;
    char label[13];         // usually ends with an =
    char unit[5];           // unit of measurement e.g. "%", "cm", "W/m2"

    void redraw(MCUFRIEND_kbv *scr); // used internally for better code organization
};

class Button : public Widget{
public:
    Button(uint16_t x, uint16_t y, const char lbl[], void (*_callback)()); 
    void update(MCUFRIEND_kbv *scr);  // executes the callback function
    void draw(MCUFRIEND_kbv *scr);  // initial drawing of button to screen, should only be called once
    void erase(MCUFRIEND_kbv *scr);   // draws a black rectangle over the button
protected:
    char label[10];
    void (*callback)();   // callback function pointer
};

class Label : public Widget{
public:
    Label(char txt[]);
    Label() : Label{err}{}

protected:
    char text[20];
private:
    char err[4] = {'e','r','r', '\0'};
};

class Timer : public Label{
public:
    Timer(uint16_t x, uint16_t y, uint16_t t, void (*_callback)());
    bool clicked(uint16_t x, uint16_t y);
    void update(MCUFRIEND_kbv *scr); // reduces remaining seconds by one and updates display
    void configure(uint16_t t); // set total countdown duration on the timer
    void configure();
    void hhmmss(char* buf);  // prints remaining time into buf
    void draw(MCUFRIEND_kbv *scr);  // initial drawing of timer to the screen, should only be called once
    void erase(MCUFRIEND_kbv *scr);  // draws a black rectangle over the timer

    unsigned long mark; // for detecting one second intervals with millis()
protected:
    uint16_t mx;    // minutes x coord
    uint16_t sx;    // seconds x coord
    const uint16_t len2dig = 44;   // length of each couple of digits
    const uint16_t wid = 22;  // width of fontsize 3 digit
    uint16_t secs;  // countdown duration left in seconds
    void (*callback)();   // callback for when timer reaches 0 seconds
private:
    void redraw(MCUFRIEND_kbv *scr); // used internally for better code organization
};

#endif
