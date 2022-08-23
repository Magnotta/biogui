#ifndef GUI_HPP
#define GUI_HPP

#include <Arduino.h>

typedef struct{
    uint16_t ox;          // origin x coord
    uint16_t oy;          // origin y coord
    uint16_t len = 240;   // total length of the slider bar in pixels
    uint16_t wid = 25;    // width of the slider bar in pixels  
    uint16_t val;         // current value the slider represents
    uint16_t valx;        // x coord of where to write the current value
    uint16_t valy;        // y coord of where to write the current value
    byte minVal;          // minimum value the slider can represent
    byte maxVal;          // maximum value the slider can represent
    uint16_t pos;         // current position of the slider bar in pixels
    bool redraw;          // has the slider position changed?
} Slider;

/* typedef struct{
    uint16_t ox;
    uint16_t oy;
    uint16_t len;
    uint16_t wid;
    char* text;
    bool redraw;
} Button; */


void sliderInit(Slider& s, uint16_t x, uint16_t y, int minv, int maxv);

#endif