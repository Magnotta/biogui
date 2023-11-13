#ifndef LIB_NAV_NAV_HPP_
#define LIB_NAV_NAV_HPP_

#include "widget.fwd.hpp"
#include "widget.hpp"

class Screen{
public:
	explicit Screen(MCUFRIEND_kbv *s);
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
	explicit Router(Screen *_home);
	void enter();
	void set_home(Screen *_home);
	void goto_screen(Screen *next);
	void signature();
	void loop(uint16_t x, uint16_t y);
	bool once_per_sec();
protected:
	void jump();

	Screen *current;
	Screen *home;
	Screen *next;
	volatile bool jump_set;
	bool once_per_sec_set;
	uint32_t timestamp;
};

#endif // LIB_NAV_NAV_HPP_
