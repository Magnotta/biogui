#include <math.h>
#include "nav.hpp"
#include "head.hpp"
#include "datalog.hpp"

extern MCUFRIEND_kbv tft;
extern Router sys;
extern Screen preheat;
extern LEDHead head;
extern Logger logger;
uint8_t pwm;

namespace Preheat{
	extern Timer tmr1;
	extern Button btn2;
}

Screen config{&tft};

namespace Config{

void lbl1_set();
void lbl2_set();
void btn1_cb();

Slider slider1{40, 160, "Potencia", 0, 100, 1, "%"};
Slider slider2{40, 220, "Tempo on", 0, 60, 1, "min"};
Slider slider3{40, 280, "Distancia", 9, 9, 1, "cm"};
Label lbl1{26, 346, 2, YELLOW, lbl1_set};
Label lbl2{26, 365, 2, YELLOW, lbl2_set};
Button btn1{90, 410, "Iniciar", btn1_cb};  //Start button

void lbl1_set(){
	double irradiance = 1.2*static_cast<double>(slider1.get_val());
	char buf[7];
	snprintf(lbl1.text, 25, "Irrad.= %s mW/cm2", dtostrf(irradiance, 6, 2, buf));
}

void lbl2_set(){
	double irradiance = 1.2*static_cast<double>(slider1.get_val());
	double fluence = irradiance*static_cast<double>(slider2.get_val())*0.06;
	char buf[9];
	snprintf(lbl2.text, 25, "Fluencia= %s J/cm2", dtostrf(fluence, 7, 2, buf));
}

void btn1_cb(){
	if(slider2.get_val() == 0)
		return;
	head.relay_off();
	delay(35);
	pwm = map(slider1.get_val(), 0, 100, 0, 255);
	Preheat::tmr1.arming_event(300);
	Preheat::btn2.deactivate();
	sys.goto_screen(&preheat);
	logger.stamp_file(";preheat start\n");
	head.MOSFET_on(pwm);
}

void add_widgets(){
	config.add_widget(&slider1);
	config.add_widget(&slider2);
	config.add_widget(&slider3);
	config.add_widget(&lbl1);
	config.add_widget(&lbl2);
	config.add_widget(&btn1);
}

} // namespace Config
