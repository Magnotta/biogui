#ifndef PLAY
#define PLAY

#include <nav.hpp>

extern MCUFRIEND_kbv tft;
extern Screen config;
extern Router sys;

extern uint8_t cycles;

Screen play{&tft};

void btn2_cb(){
	sys.switch_screen(&config);
}

void tmr1_cb(){

}

Button btn2{90, 380, "Encerrar", btn2_cb};
Timer tmr1{79, 240, 0, btn2_cb};

void play_add_widgets(){
	play.add_widget(&btn2);
	play.add_widget(&tmr1);
}

#endif
