#include <MCUFRIEND_kbv.h>
#include "nav.hpp"
#include "widget.hpp"

/// @brief Constructor for Screen class
/// @param s Display screen
/// @param _entry The function to be called when this screen is loaded
/// @param _exit The function to be called when this screen is unloaded
/// @param _update The function to be called in loop while this screen is active
Screen::Screen(MCUFRIEND_kbv *s, void (*_entry)(), void (*_exit)()){
	scr = s;
	entry = _entry;
	exit = _exit;
	head[0] = nullptr;
	widget_i = 0;
}

void Screen::update(uint16_t x, uint16_t y){
	for(byte i = 0; i < widget_i; i++){
		if(head[i]->clicked(x, y))
			head[i]->update(scr);
	}
}

void Screen::add_widget(Widget *x){
	head[widget_i] = x;
	widget_i++;
	head[widget_i] = nullptr;
}

Mngr::Mngr(){
	current = nullptr;
}