#ifndef NAV_HPP
#define NAV_HPP

#include "widget.hpp"

class Screen{
public:
	Screen(MCUFRIEND_kbv *s);
	void update(uint16_t x, uint16_t y);
	void add_widget(Widget *x);
	void entry();
	void exit();

	Widget *widgets[20];
	uint8_t widget_count;
protected:
	MCUFRIEND_kbv *scr;
};

class Router{
public:
	Router(Screen *_home, Screen *cur);
	Router() : Router{nullptr, nullptr}{};
	void enter();
	void set_home(Screen *_home);
	void switch_screen(Screen *next);
	void loop(uint16_t x, uint16_t y);
protected:
	Screen *current;
	Screen *home;
};

#endif
