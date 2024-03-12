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
    /// @brief Base class for widgets on the screen.
    /// @param ox Origin (top left) x-coordinate in pixels.
    /// @param oy Origin (top left) y-coordinate in pixels.
    Widget(uint16_t ox, uint16_t oy);
    virtual bool clicked(uint16_t, uint16_t);
    virtual void update(MCUFRIEND_kbv*) = 0;
    virtual void draw(MCUFRIEND_kbv*) = 0;
    virtual void erase(MCUFRIEND_kbv*);
    virtual void activate();
    virtual void deactivate();
    virtual bool is_active();
    virtual bool is_clicker();
    virtual uint16_t get_wid();
    virtual uint16_t get_len();

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
    /// @brief Slider for setting some parameter's value.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels.
    /// @param label Name of the parameter as a C string.
    /// @param minv Minimum (leftmost) value the parameter can take.
    /// @param maxv Maximum (rightmost) value the parameter can take.
    /// @param step Step size on the parameter's value. It is helpful when the parameter's value interval is large and fine grained control becomes impossible.
    /// @param unit In what unit is the parameter's value, as a C string.
    Slider(uint16_t x, uint16_t y, const char label[], uint16_t minv, uint16_t maxv, uint8_t step, const char unit[]);
    bool clicked(uint16_t, uint16_t) override;
    void update(MCUFRIEND_kbv*) override;  // updates the value and redraws a slider
    void draw(MCUFRIEND_kbv*) override;  // initial drawing of slider to screen, should only be called once
    void erase(MCUFRIEND_kbv*) override;
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

class ButtonBase : public Widget{
public:
    /// @brief Base class for buttons.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels.
    /// @param label Button label as a C string.
    ButtonBase(uint16_t x, uint16_t y, const char label[]);
	virtual void update(MCUFRIEND_kbv*) = 0;
    void draw(MCUFRIEND_kbv*) override;  // initial drawing of button to screen, should only be called once

protected:
    void redraw(MCUFRIEND_kbv*);

    char _label[10];
};

class Button : public ButtonBase{
public:
	/// @brief Button widget that triggers a user-defined callback function.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels. 
    /// @param label Button label as a C string.
	/// @param callback Pointer to a void function that's triggered by a button press.
	/// @param delay_ms Debounce delay in ms.
	Button(uint16_t x, uint16_t y, const char label[], void (*callback)(), uint16_t delay_ms=250);
	void update(MCUFRIEND_kbv*) override; 
protected:
	void (*_callback)();   // callback function pointer
    uint16_t _debounce;  // debounce delay in milliseconds
    uint32_t _mark; // for debouncing delay
};

class NaviButton : public ButtonBase{
public:
    /// @brief Button that triggers a screen transition, aka navigation.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels. 
    /// @param label Button label as a C string.
    /// @param dest Pointer to the destination screen. 
    NaviButton(uint16_t x, uint16_t y, const char *label, Screen *dest);
    void update(MCUFRIEND_kbv*) override;

protected:
    Screen *_dest;
};

class Label : public Widget{
public:
    /// @brief Display dynamic text on the screen.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels. 
    /// @param fontsize Fontsize (line thickness) in pixels.
    /// @param color 16-bit RGB (5-6-5).
    /// @param setter Pointer to a function that sets the label text. Typically an snprintf call that writes 25 chars to the label.text variable.
    Label(uint16_t x, uint16_t y, uint8_t fontsize, uint16_t color, void (*setter)());
    void update(MCUFRIEND_kbv*) override;
    void draw(MCUFRIEND_kbv*) override;

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
    /// @brief Timer that triggers a user-defined function after a countdown.
	/// @param x Origin (top left) x-coordinate in pixels.
    /// @param y Origin (top left) y-coordinate in pixels. 
    /// @param callback Pointer to a void function that's triggered when the countdown ends.
    Timer(uint16_t x, uint16_t y, void (*callback)());
    void update(MCUFRIEND_kbv*) override; // reduces remaining seconds by one and updates display
    void draw(MCUFRIEND_kbv*) override;  // draws the timer on the screen, should only be called once
    void arming_event(uint16_t); // set total countdown duration on the timer
    void hhmmss(char*);  // prints remaining time into buffer, formatted (hh:mm:ss)

protected:
    uint16_t mx;    // minutes x coord
    uint16_t sx;    // seconds x coord
    uint16_t len2dig;   // length of each couple of digits
    uint16_t secs;  // countdown duration left in seconds

    void (*_callback)();   // _callback for when timer reaches 0 seconds

private:
    void redraw(MCUFRIEND_kbv*); // used internally for better code organization
};

#endif // LIB_WIDGET_WIDGET_HPP_
