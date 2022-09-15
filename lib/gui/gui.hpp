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
    Slider(uint16_t x, uint16_t y, uint16_t minv, uint16_t maxv);
    bool clicked(uint16_t xpos, uint16_t ypos); // returns whether (xpos, ypos) is inside slider area
    void update(uint16_t xpos, MCUFRIEND_kbv scr);  // updates the value and redraws a slider
    void update(uint16_t xpos, MCUFRIEND_kbv scr, byte step);   // updates the value in steps and redraws a slider
    void draw_init(MCUFRIEND_kbv scr);  // initial drawing of slider to screen, should only be called once
    void configure(const char lbl[], const char u[]);   // adds label and dimensional unit to slider
    uint16_t get_val(); //returns current value the slider holds
private:
    uint16_t ox;            // origin x coord
    uint16_t oy;            // origin y coord
    uint16_t pos;           // current position of the slider bar in pixels
    uint16_t len = 240;     // total length of the slider bar in pixels
    uint16_t wid = 25;      // width of the slider bar in pixels  
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

    void redraw(MCUFRIEND_kbv scr);
};

class Button{
public:
    Button(uint16_t x, uint16_t y);   //
    bool clicked(uint16_t xpos, uint16_t ypos); //
    void update(MCUFRIEND_kbv scr);  //
    void draw_init(MCUFRIEND_kbv scr);  // 
    void configure(const char lbl[], void (*callback)());   //

private:
    uint16_t ox;
    uint16_t oy;
    uint16_t len;
    uint16_t wid;
    char label[10];
    void (*cb)();
};

class Timer{
public:
    Timer(uint16_t x, uint16_t y);
    void update(MCUFRIEND_kbv scr);
    void configure(uint16_t t);
    void print(char* buf);  //Prints out remaining time into buf
    void draw_init(MCUFRIEND_kbv scr);
private:
    uint16_t ox;
    uint16_t oy;
    uint16_t secs;
};

#endif