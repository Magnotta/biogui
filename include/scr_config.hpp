#ifndef SCREENCONFIG
#define SCREENCONFIG

#include <nav.hpp>
#include <head.hpp>

extern MCUFRIEND_kbv tft;
extern Router sys;
extern Screen play;
extern LEDHead head;
uint8_t cycles, cycles_i, pwm;

namespace Play{
	extern Timer tmr1;
	extern Timer tmr2;
}

Screen config{&tft};

namespace Config{

void btn1_cb();

Slider slider1{40, 60, 0, 100, 1, "Potencia", "%"};
Slider slider2{40, 120, 0, 60, 1, "Tempo on", "min"};
Slider slider3{40, 180, 0, 300, 10, "Tempo off", "seg"};
Slider slider4{40, 240, 1, 10, 1, "N Ciclos", ""};
Slider slider5{40, 300, 5, 15, 1, "Distancia", "cm"};
Button btn1{90, 380, "Iniciar", btn1_cb};  //Start button

void btn1_cb(){
	if(slider2.get_val() == 0)
		return;
	pwm = map(slider1.get_val(), 0, 100, 0, 255);
	Play::tmr1.arming_event(slider2.get_val()*60);
	Play::tmr2.deactivate();
	cycles = slider4.get_val() - 1;
	cycles_i = 0;
	sys.goto_screen(&play);
	head.LEDOn(pwm);
}

void add_widgets(){
	config.add_widget(&slider1);
	config.add_widget(&slider2);
	config.add_widget(&slider3);
	config.add_widget(&slider4);
	config.add_widget(&slider5);
	config.add_widget(&btn1);
}

}

#endif
