#include "head.hpp"
#include <Arduino.h>

LEDHead::LEDHead(uint8_t LED_control_pin, uint8_t jtemp_sens, uint8_t temp_sens_pin, uint8_t amp_sens_pin, double shut_off_temp){
	_MOSFET_temp_sens = TempSens{jtemp_sens, 5};
	_LED_temp_sens = TempSens{temp_sens_pin, 5};
	_LED_amp_sens = AmpSens{amp_sens_pin, 5};
	_LED_control_pin = LED_control_pin;
	_shut_off_temp = shut_off_temp;

	_LED_on = false;
	_preheat_on = false;
}

void LEDHead::init(){
	_LED_temp_sens.init();
	pinMode(_LED_control_pin, OUTPUT);
}

void LEDHead::update(){
	_LED_temp_sens.update();
	dtostrf(_LED_temp_sens.get_fdata(), 5, 2, LED_temperature_string);

	_MOSFET_temp_sens.update();
	dtostrf(_MOSFET_temp_sens.get_fdata(), 5, 2, MOSFET_temperature_string);

	_LED_amp_sens.update();
	dtostrf(_LED_amp_sens.get_fdata(), 9, 2, amps_string);
}

void LEDHead::preheat(uint8_t duty_cycle){

}

void LEDHead::LED_start(uint8_t duty_cycle){
	if(_MOSFET_temp_sens.get_fdata() > 40.0){
		if(_LED_temp_sens.get_fdata() < _shut_off_temp){
			analogWrite(_LED_control_pin, duty_cycle);
			_LED_on = true;
		}
	}else{
		preheat(duty_cycle);
	}
}

void LEDHead::LED_stop(){
	digitalWrite(_LED_control_pin, LOW);
	_LED_on = false;
}

bool LEDHead::temp_safe(){
	return _LED_temp_sens.get_fdata() < _shut_off_temp;
}

bool LEDHead::LED_is_on(){
	return _LED_on;
}
