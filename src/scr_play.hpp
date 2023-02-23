#ifndef PLAY
#define PLAY

#include <nav.hpp>
#include <temperature.hpp>

extern MCUFRIEND_kbv tft;
extern Screen config;
extern Router sys;
extern Slider slider2, slider3;
extern const byte LED_pin;
extern TempSens temperature;

extern uint8_t cycles, cycles_i, pwm;
Screen play{&tft};

void btn2_cb();
void tmr1_cb();
void tmr2_cb();
void lbl1_set();
void lbl2_set();
void lbl3_set();

Button btn2{80, 380, "Encerrar", btn2_cb};
Timer tmr1{67, 240, tmr1_cb};	// For ON TIME
Timer tmr2{67, 240, tmr2_cb};	// For OFF TIME
Label lbl1{30, 30, 2, WHITE, lbl1_set};
Label lbl2{30, 60, 2, WHITE, lbl2_set};
Label lbl3{30, 90, 2, WHITE, lbl3_set};

void btn2_cb(){
	sys.goto_screen(&config);
	digitalWrite(LED_pin, LOW);
}

void tmr1_cb(){
	tmr1.deactivate();
	tmr1.erase(&tft);
	if(slider3.get_val()){
		tmr2.arming_event(slider3.get_val());
		tmr2.draw(&tft);
		digitalWrite(LED_pin, LOW);
	}else if(cycles_i < cycles){
		cycles_i++;
		tmr1.arming_event(slider2.get_val()*5);
		tmr1.draw(&tft);
	}
	else{
		sys.goto_screen(&config);
		digitalWrite(LED_pin, LOW);
	}
}

void tmr2_cb(){
	if(cycles_i < cycles){
		cycles_i++;
		tmr2.deactivate();
		tmr2.erase(&tft);
		tmr1.arming_event(slider2.get_val()*5);
		tmr1.activate();
		tmr1.draw(&tft);
		analogWrite(LED_pin, pwm);
	}else{
		sys.goto_screen(&config);
		digitalWrite(LED_pin, LOW);
	}
}

void lbl1_set(){ sprintf(lbl1.text, "Ciclo %d de %d", cycles_i+1, cycles+1); }
void lbl2_set(){ sprintf(lbl2.text, "Irrad.: 0 W/cm2"); }
void lbl3_set(){ sprintf(lbl3.text, "%s graus C", temperature.temperature_string); }

void play_add_widgets(){
	play.add_widget(&btn2);
	play.add_widget(&tmr1);
	play.add_widget(&tmr2);
	play.add_widget(&lbl1);
	play.add_widget(&lbl2);
	play.add_widget(&lbl3);
}

#endif
