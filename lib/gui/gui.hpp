#ifndef GUI_HPP
#define GUI_HPP

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

class Slider{
public:
    Slider(uint16_t x, uint16_t y);
    bool clicked(uint16_t xpos, uint16_t ypos); // returns whether (xpos, ypos) is inside slider area
    void update(uint16_t xpos, MCUFRIEND_kbv scr);  // updates the value and redraws a slider
    void update(uint16_t xpos, MCUFRIEND_kbv scr, byte step);   // updates the value in steps and redraws a slider
    void draw_init(MCUFRIEND_kbv scr);  // initial drawing of slider to screen, should only be called once
    void configure(uint16_t minv, uint16_t maxv, const char lbl[], const char u[], uint16_t l, uint16_t w);   // adds label, unit and dimensions to slider
    uint16_t get_val(); //returns current value the slider holds
private:
    uint16_t ox;            // origin x coord.
    uint16_t oy;            // origin y coord.
    uint16_t pos;           // current position of the slider bar in pixels
    uint16_t len;     // total length of the slider bar in pixels
    uint16_t wid;      // width of the slider bar in pixels  
    uint16_t val;           // current value the slider represents
    uint16_t valx;          // x coord of where to write the current value
    uint16_t valy;          // y coord of where to write the current value
    uint16_t min_val;           // minimum value the slider can represent
    uint16_t max_val;           // maximum value the slider can represent
    byte minv_offset;       // horizontal offset in pixels of where to write the minimum value
    byte maxv_offset;       // horizontal offset in pixels of where to write the maximum value
    byte label_offset;      // horizontal offset in pixels of where to write the slider label
    char label[13];         // usually ends with an =
    char unit[5];           // unit of measurement e.g. "%", "cm", "W/m2"

    void redraw(MCUFRIEND_kbv scr); // used internally for better code organization
};

class Button{
public:
    Button(uint16_t x, uint16_t y); 
    bool clicked(uint16_t xpos, uint16_t ypos); // returns whether (xpos, ypos) is inside slider area
    void update(bool debounce);  // executes the callback function
    void draw_init(MCUFRIEND_kbv scr);  // initial drawing of button to screen, should only be called once
    void configure(const char lbl[], void (*callback)());   // adds label and callback function to button
    void erase(MCUFRIEND_kbv scr);   // draws a black rectangle over the button
private:
    uint16_t ox;    // origin x coord.
    uint16_t oy;    // origin y coord.
    uint16_t len;   // length in pixels along x axis
    uint16_t wid;   // width in pixels along y axis
    char label[10];
    void (*cb)();   // callback function pointer
};

class Timer{
public:
    Timer(uint16_t x, uint16_t y);
    void update(MCUFRIEND_kbv scr); // reduces remaining seconds by one and updates display
    void configure(uint16_t t); // set total countdown duration on the timer
    void configure(uint16_t t, void (*callback)());
    void hhmmss(char* buf);  // prints remaining time into buf
    void draw_init(MCUFRIEND_kbv scr);  // initial drawing of timer to the screen, should only be called once
    void erase(MCUFRIEND_kbv scr);  // draws a black rectangle over the timer

    unsigned long mark; // for detecting one second intervals with millis()
private:
    uint16_t ox;    // origin x coord
    uint16_t oy;    // origin y coord
    uint16_t mx;    // minutes x coord
    uint16_t sx;    // seconds x coord
    const uint16_t len2dig = 44;   // length of each couple of digits
    const uint16_t wid = 22;  // width of fontsize 3 digit
    uint16_t secs;  // countdown duration left in seconds
    void (*cb)();   // callback for when timer reaches 0 seconds

    void redraw(MCUFRIEND_kbv scr);
};

struct widget_t{
    void* widget;
    void* next = nullptr;
};

class Screen{
public:
    Screen();
    void add_widget(Timer t);
    void add_widget(Button b);
    void add_widget(Slider s);

    widget_t* WLH;  // widget list head pointer
private:

};

#endif