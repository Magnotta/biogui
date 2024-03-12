#include <nav.hpp>
#include <sensors.hpp>
#include <head.hpp>

extern MCUFRIEND_kbv tft;
extern Screen config;
extern Slider slider2;
extern Router sys;
extern LEDHead head;
extern uint8_t pwm;

Screen play{&tft};

namespace Play{

void play_end_cb();
void lbl1_set();
void lbl2_set();

Button btn1{80, 380, "Encerrar", play_end_cb};
Timer tmr1{67, 240, play_end_cb};	// For ON TIME
Label lbl1{30, 60, 2, WHITE, lbl1_set};
Label lbl2{30, 90, 2, WHITE, lbl2_set};

void play_end_cb(){
	sys.goto_screen(&config);
	head.MOSFET_off();
}

void lbl1_set(){ snprintf(lbl1.text, 25, "Temp LED: %s C", head.LED_temperature_string); }
void lbl2_set(){ snprintf(lbl2.text, 25, "Corrente: %s A", head.amps_string); }

void add_widgets(){
	play.add_widget(&btn1);
	play.add_widget(&tmr1);
	play.add_widget(&lbl1);
	play.add_widget(&lbl2);
}

} // namespace Play
