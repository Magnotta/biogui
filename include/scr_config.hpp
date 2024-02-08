#include <math.h>
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

void lbl1_set();
void lbl2_set();
void btn1_cb();

Slider slider1{40, 40, 0, 100, 1, "Potencia", "%"};
Slider slider2{40, 100, 0, 60, 1, "Tempo on", "min"};
Slider slider3{40, 160, 0, 300, 10, "Tempo off", "seg"};
Slider slider4{40, 220, 1, 10, 1, "N Ciclos", ""};
Slider slider5{40, 280, 5, 15, 1, "Distancia", "cm"};
Label lbl1{26, 346, 2, YELLOW, lbl1_set};
Label lbl2{32, 365, 2, YELLOW, lbl2_set};
Button btn1{90, 410, "Iniciar", btn1_cb};  //Start button

void lbl1_set(){
	double fluence;
	fluence = (0.1394+1897.7/(1+pow(slider5.get_val()/1.9726, 2.0892)))/(0.1394+1897.7/(1+pow(10/1.9726, 2.0892)));
	fluence = fluence*73.6*static_cast<double>(slider1.get_val())/100.0;
	char buf[7];
	snprintf(lbl1.text, 25, "Fluencia= %s mW/cm2", dtostrf(fluence, 6, 2, buf));
}

void lbl2_set(){
	double fluence;
	fluence = (0.1394+1897.7/(1+pow(slider5.get_val()/1.9726, 2.0892)))/(0.1394+1897.7/(1+pow(10/1.9726, 2.0892)));
	fluence = fluence*73.6*static_cast<double>(slider1.get_val())/100.0;
	double irradiance;
	irradiance = fluence*static_cast<double>(slider2.get_val()*slider4.get_val()*60)/1000.0;
	char buf[9];
	snprintf(lbl2.text, 25, "Irrad.= %s J/cm2", dtostrf(irradiance, 8, 2, buf));
}

void btn1_cb(){
	if(slider2.get_val() == 0)
		return;
	pwm = map(slider1.get_val(), 0, 100, 0, 255);
	Play::tmr1.arming_event(slider2.get_val()*60);
	Play::tmr2.deactivate();
	cycles = slider4.get_val() - 1;
	cycles_i = 0;
	sys.goto_screen(&play);
	head.LED_start(pwm);
}

void add_widgets(){
	config.add_widget(&slider1);
	config.add_widget(&slider2);
	config.add_widget(&slider3);
	config.add_widget(&slider4);
	config.add_widget(&slider5);
	config.add_widget(&lbl1);
	config.add_widget(&lbl2);
	config.add_widget(&btn1);
}

} // namespace Config
