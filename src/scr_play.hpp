#ifndef PLAY
#define PLAY

#include <nav.hpp>

extern MCUFRIEND_kbv tft;
extern Screen config;
extern Router sys;

extern uint8_t cycles;

Screen play{&tft};

void btn2_cb();
void tmr1_cb();
void tmr2_cb();

Button btn2{90, 380, "Encerrar", btn2_cb, 250};
Timer tmr1{67, 240, 0, tmr1_cb};	// For ON TIME
Timer tmr2{67, 240, 0, tmr2_cb};	// For OFF TIME

void btn2_cb(){
	sys.switch_screen(&config);
}

void tmr1_cb(){
	tmr1.deactivate();
	tmr1.erase(&tft);
	if(slider3.get_val()){
		tmr2.arming_event(slider3.get_val());
		tmr2.activate();
		tmr2.draw(&tft);
	}else if(cycles-1){
		cycles--;
		tmr1.arming_event(slider2.get_val()*5);
		tmr1.activate();
		tmr1.draw(&tft);
	}
	else
		sys.switch_screen(&config);
}

void tmr2_cb(){
	if(cycles-1){
		cycles--;
		tmr2.deactivate();
		tmr2.erase(&tft);
		tmr1.arming_event(slider2.get_val()*5);
		tmr1.activate();
		tmr1.draw(&tft);
	}else{
		sys.switch_screen(&config);
	}
}

void play_add_widgets(){
	play.add_widget(&btn2);
	play.add_widget(&tmr1);
	play.add_widget(&tmr2);
}

#endif
