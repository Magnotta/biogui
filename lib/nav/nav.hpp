#ifndef NAV_HPP
#define NAV_HPP

#include <MCUFRIEND_kbv.h>
#include "widget.hpp"

class Screen{
public:
	Screen(MCUFRIEND_kbv *s, void (*_entry)(), void (*_exit)());
	void update(uint16_t x, uint16_t y);
	void add_widget(Widget *x);

	void (*entry)();
	void (*exit)();

	Widget *head[20];
	byte widget_i;
protected:
	MCUFRIEND_kbv *scr;
};

class Mngr{
public:
	Mngr();
protected:
	Screen *current;
};

#endif
