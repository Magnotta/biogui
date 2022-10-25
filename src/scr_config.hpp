#ifndef CONFIG
#define CONFIG

#include <nav.hpp>

extern MCUFRIEND_kbv tft;
extern Screen play;
extern Router sys;
extern Timer tmr1, tmr2;

uint8_t cycles;

Screen config = Screen(&tft);

void btn1_cb();

Slider slider1{40, 60, 240, 25, 0, 100, 1, "Potencia", "%"};
Slider slider2{40, 120, 240, 25, 0, 30, 1, "Tempo on", "min"};
Slider slider3{40, 180, 240, 25, 0, 300, 10, "Tempo off", "seg"};
Slider slider4{40, 240, 240, 25, 1, 10, 1, "N Ciclos", ""};
Slider slider5{40, 300, 240, 25, 5, 15, 1, "Distancia", "cm"};
Button btn1{90, 380, "Iniciar", btn1_cb, 250};  //Start button

void btn1_cb(){
	tmr1.activate();
	tmr2.deactivate();
	tmr1.arming_event(slider2.get_val()*5);
	sys.switch_screen(&play);
	cycles = slider4.get_val();
}

void config_add_widgets(){
	config.add_widget(&slider1);
	config.add_widget(&slider2);
	config.add_widget(&slider3);
	config.add_widget(&slider4);
	config.add_widget(&slider5);
	config.add_widget(&btn1);
}

#endif
