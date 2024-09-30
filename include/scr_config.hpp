#include <math.h>
#include "nav.hpp"
#include "head.hpp"
#include "datalog.hpp"

extern MCUFRIEND_kbv tft;
extern Router sys;
extern Page preheat;
extern LEDHead head;
extern Logger logger;
uint8_t pwm;

namespace Preheat{
	extern Timer tmr1;
	extern Button btn2;
}

Page config{&tft};

namespace Config{

void tgl1_cb();
void lbl1_set();
void lbl2_set();
void btn1_cb();

Toggle toggle1{108, 40, "Curto", tgl1_cb};
Toggle toggle2{90, 95, "Segurar", nullptr};
Slider slider1{40, 160, "Potencia", 0, 100, 1, "%"};
Slider slider2{40, 220, "Tempo on", 0, 60, 1, "min"};
// Slider slider3{40, 280, "Distancia", 9, 9, 1, "cm"};
Slider slider4{40, 220, "Tempo on", 0, 30, 1, "s"};
Label lbl1{26, 346, 2, YELLOW, lbl1_set};
Label lbl2{70, 365, 2, YELLOW, lbl2_set};
Button btn1{90, 410, "Iniciar", btn1_cb};

void tgl1_cb(){
	if(slider2.is_active()){
		slider2.deactivate();
		slider2.erase(&tft);
		slider4.activate();
		slider4.draw(&tft);
	}else{
		slider4.deactivate();
		slider4.erase(&tft);
		slider2.activate();
		slider2.draw(&tft);
	}
}

void lbl1_set(){
	snprintf(lbl1.text, 25, "Irrad. a 9cm em mW/cm2");
}

void lbl2_set(){
	double irradiance = 1.1*static_cast<double>(slider1.get_val());
	char buf[7];
	snprintf(lbl2.text, 25, "%s", dtostrf(irradiance, 6, 2, buf));
}

void btn1_cb(){
	if((slider2.get_val() == 0 && !toggle1.is_toggled()) || (slider4.get_val() == 0 && toggle1.is_toggled()))
		return;
	head.relay_off();
	delay(35);
	pwm = map(slider1.get_val(), 0, 100, 0, 255);
	Preheat::tmr1.arming_event(120);
	Preheat::btn2.deactivate();
	sys.goto_screen(&preheat);
	logger.stamp_file(";preheat start\n");
	head.MOSFET_on(pwm);
}

void add_widgets(){
	config.add_widget(&toggle1);
	config.add_widget(&toggle2);
	config.add_widget(&slider1);
	config.add_widget(&slider2);
	// config.add_widget(&slider3);
	config.add_widget(&slider4);
	config.add_widget(&lbl1);
	config.add_widget(&lbl2);
	config.add_widget(&btn1);
	
	slider4.deactivate();
}

} // namespace Config
