#ifndef LIB_NAV_NAV_HPP_
#define LIB_NAV_NAV_HPP_

#include "widget.fwd.hpp"
#include "widget.hpp"

class Screen{
public:
	explicit Screen(MCUFRIEND_kbv*);
	void update(uint16_t, uint16_t);
	void add_widget(Widget*);
	void entry();
	void exit();

	Widget *widgets[20];
	uint8_t widget_count;
protected:
	MCUFRIEND_kbv *_scr;
};

class Router{
public:
	explicit Router(Screen*);
	void enter();
	void set_home(Screen*);
	void goto_screen(Screen*);
	void signature();
	void loop(uint16_t, uint16_t);
	bool once_per_sec();
protected:
	void jump();

	Screen *_current;
	Screen *_home;
	Screen *_next;
	volatile bool _jump_set;
	bool _once_per_sec_set;
	uint32_t _timestamp;
};

#endif // LIB_NAV_NAV_HPP_
