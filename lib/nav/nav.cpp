#include "nav.hpp"

/// @brief Screen class for displaying a single screen
/// @param s Display screen
/// @param _entry The function to be called when this screen is loaded
/// @param _exit The function to be called when this screen is unloaded
/// @param _update The function to be called in loop while this screen is active
Screen::Screen(MCUFRIEND_kbv *s){
	scr = s;
	widget_count = 0;
}

/// @brief Cycle through all widgets added to a screen and update them
/// @param x X coordinate of screen click
/// @param y Y coordinate of screen click
void Screen::update(uint16_t x, uint16_t y){
	for(uint8_t i = 0; i < widget_count; i++){
		if(widgets[i]->is_active() && (!widgets[i]->is_clicker() || widgets[i]->clicked(x, y)))
				// Update a widget only if it is active, is not a clicker or has been clicked on
				widgets[i]->update(scr);
	}
}

/// @brief Add a widget to a screen, so that it is automatically drawn and updated
/// @param x Should be the memory address os a global Widget object
void Screen::add_widget(Widget *x){
	widgets[widget_count] = x;
	widget_count++;
}

/// @brief The routine for entering a screen: draws all active widgets
void Screen::entry(){
	for(uint8_t i = 0; i < widget_count; i++){
		if(widgets[i]->is_active())
			widgets[i]->draw(scr);
	}
}

/// @brief The routine for exiting a screen: erases all active widgets
void Screen::exit(){
	for(uint8_t i = 0; i < widget_count; i++){
		if(widgets[i]->is_active())
			widgets[i]->erase(scr);
	}
}

/*##########################################################################
* Router ###################################################################
##########################################################################*/

Router sys{nullptr};

Router::Router(Screen *_home){
	home = _home;
	current = _home;
	next = nullptr;
	jump_set = false;
	timestamp = 0;
}

/// @brief Sets home screen, to be accessed by the home button
/// @param _home Pointer to Screen object
void Router::set_home(Screen *_home){
	home = _home;
}

/// @brief Erase current screen, draw another
/// @param next Pointer to screen object
void Router::goto_screen(Screen *_next){
	if(current == _next)
		return;
	next = _next;
	jump_set = true;
}

void Router::jump(){
	current->exit();
	current = next;
	current->entry();
	jump_set = false;
}

/// @brief Updates all widgets linked to current screen
/// @param x X coordinate of screen click
/// @param y Y coordinate of screen click
/// @note To be called inside arduino lcoop()
void Router::loop(uint16_t x, uint16_t y){
	if(millis() - timestamp > 991){ // a little smaller than a sec empirically works best
		timestamp = millis();
		once_per_sec_set = true;
	}else{
		once_per_sec_set = false;
	}	
	current->update(x, y);
	if(jump_set)
		jump();
}

/// @brief Draws home screen on the display
/// @note To be called inside arduino setup()
void Router::enter(){
	// load_from_eeprom();
	signature();
	home->entry();
	timestamp = millis();
}

void Router::signature(){}

bool Router::once_per_sec(){
	return once_per_sec_set;
}
