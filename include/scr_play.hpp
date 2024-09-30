#include <nav.hpp>
#include <sensors.hpp>
#include <head.hpp>

extern MCUFRIEND_kbv tft;
extern Page config;
extern Slider slider2;
extern Router sys;
extern LEDHead head;
extern uint8_t pwm;

namespace Config{
extern Toggle toggle2;
} // namespace Config


Page play{&tft};

namespace Play{

void btn1_cb();
void btn2_cb();
void tmr1_cb();
void lbl1_set();
void lbl2_set();
void lbl3_set();

Button btn1{80, 380, "Encerrar", btn1_cb};
Button btn2{80, 180, "Iluminar", btn2_cb};
Timer tmr1{67, 240, tmr1_cb};	// For ON TIME
Label lbl1{30, 60, 2, WHITE, lbl1_set};
Label lbl2{30, 90, 2, WHITE, lbl2_set};
Label lbl3{30, 120, 2, WHITE, lbl3_set};

void btn1_cb(){
	sys.goto_screen(&config);
	head.MOSFET_off();
}

void btn2_cb(){
	btn2.erase(&tft);
	btn2.deactivate();
	head.switch_relay();
	if(Config::toggle1.is_toggled())	tmr1.arming_event(Config::slider4.get_val());
	else							tmr1.arming_event(Config::slider2.get_val()*60);
	tmr1.draw(&tft);
}


void tmr1_cb(){
	if(Config::toggle2.is_toggled()){
		btn2.activate();
		head.switch_relay();
		btn2.draw(&tft);
	}else{
		sys.goto_screen(&config);
		head.MOSFET_off();
	}
}

void lbl1_set(){ snprintf(lbl1.text, 25, "Temp LED: %s C", head.LED_temperature_string); }
void lbl2_set(){ snprintf(lbl2.text, 25, "Temp MOSFET: %s C", head.MOSFET_temperature_string); }
void lbl3_set(){ snprintf(lbl3.text, 25, "Corrente: %s A", head.amps_string); }

void add_widgets(){
	play.add_widget(&btn1);
	play.add_widget(&btn2);
	play.add_widget(&tmr1);
	play.add_widget(&lbl1);
	play.add_widget(&lbl2);
	play.add_widget(&lbl3);
}

} // namespace Play
