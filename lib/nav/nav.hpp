#ifndef LIB_NAV_NAV_HPP_
#define LIB_NAV_NAV_HPP_

#include "widget.fwd.hpp"
#include "widget.hpp"

class Page{
public:
	/// @brief A collection of widgets and an automated way to update them.
	/// @param scr Pointer to the MCUFRIEND_kbv object. Needed for access to function calls for drawing on the display.
	Page(MCUFRIEND_kbv *scr, void (*enter)()=nullptr);
	void update(uint16_t, uint16_t);
	void add_widget(Widget*);
	void entry();
	void exit();
	void set_enter(void (*enter)());

	Widget *widgets[20];
	uint8_t widget_count;
protected:
	MCUFRIEND_kbv *_scr;
	void (*_enter)();
};

class Router{
public:
	/// @brief Orchestrates screen transitions and exposes some useful interfaces.
	explicit Router(Page *home);
	void enter();
	void set_home(Page*);
	void goto_screen(Page*);
	void signature();
	void loop(uint16_t, uint16_t);
	bool once_per_sec();
protected:
	void jump();

	Page *_current;
	Page *_home;
	Page *_next;
	volatile bool _jump_set;
	bool _once_per_sec_set;
	uint32_t _timestamp;
};

#endif // LIB_NAV_NAV_HPP_
