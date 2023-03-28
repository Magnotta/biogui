#ifndef SCREENPLAY
#define SCREENPLAY

#include <nav.hpp>
#include <temperature.hpp>
#include <head.hpp>

extern MCUFRIEND_kbv tft;
extern Screen config;
extern Slider slider2, slider3;
extern Router sys;
extern LEDHead head;
extern uint8_t cycles, cycles_i, pwm;

Screen play{&tft};

namespace Play{

void btn1_cb();
void tmr1_cb();
void tmr2_cb();
void lbl1_set();
void lbl2_set();

Button btn1{80, 380, "Encerrar", btn1_cb};
Timer tmr1{67, 240, tmr1_cb};	// For ON TIME
Timer tmr2{67, 240, tmr2_cb};	// For OFF TIME
Label lbl1{30, 30, 2, WHITE, lbl1_set};
Label lbl2{30, 60, 2, WHITE, lbl2_set};

void btn1_cb(){
	sys.goto_screen(&config);
	head.LEDOff();
}

void tmr1_cb(){
	tmr1.deactivate();
	tmr1.erase(&tft);
	if(Config::slider3.get_val()){
		tmr2.arming_event(Config::slider3.get_val());
		tmr2.draw(&tft);
		head.LEDOff();
	}else if(cycles_i < cycles){
		cycles_i++;
		tmr1.arming_event(Config::slider2.get_val()*60);
		tmr1.draw(&tft);
	}
	else{
		sys.goto_screen(&config);
		head.LEDOff();
	}
}

void tmr2_cb(){
	if(cycles_i < cycles){
		cycles_i++;
		tmr2.deactivate();
		tmr2.erase(&tft);
		tmr1.arming_event(Config::slider2.get_val()*60);
		tmr1.activate();
		tmr1.draw(&tft);
		head.LEDOn(pwm);
	}else{
		sys.goto_screen(&config);
		head.LEDOff();
	}
}

void lbl1_set(){ sprintf(lbl1.text, "Ciclo %d de %d", cycles_i+1, cycles+1); }
void lbl2_set(){ sprintf(lbl2.text, "Temp LED: %s C", head.temperature_string); }

void add_widgets(){
	play.add_widget(&btn1);
	play.add_widget(&tmr1);
	play.add_widget(&tmr2);
	play.add_widget(&lbl1);
	play.add_widget(&lbl2);
}

}

#endif
