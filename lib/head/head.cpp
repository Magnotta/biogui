#include "head.hpp"
#include <Arduino.h>

LEDHead::LEDHead(uint8_t _LED_control_pin, uint8_t _temp_sens_pin, uint8_t _amp_sens_pin, double _shut_off_temp){
	temp_sens = TempSens{_temp_sens_pin, 5};
	amp_sens = AmpSens{_amp_sens_pin, 5};
	LED_control_pin = _LED_control_pin;
	shut_off_temp = _shut_off_temp;
}

void LEDHead::init(){
	temp_sens.init();
	pinMode(LED_control_pin, OUTPUT);
	timestamp = millis();
}

void LEDHead::update(){
	if(millis() - timestamp > 1000){
		timestamp = millis();
		temp_sens.update();
		dtostrf(temp_sens.get_fdata(), 5, 2, temperature_string);

		amp_sens.update();
		dtostrf(amp_sens.get_fdata(), 9, 2, amps_string);
	}
}

void LEDHead::LEDOn(uint8_t _duty_cycle){
	if(temp_sens.get_fdata() < shut_off_temp){
		analogWrite(LED_control_pin, _duty_cycle);
	}
}

void LEDHead::LEDOff(){
	digitalWrite(LED_control_pin, LOW);
}

bool LEDHead::temp_safe(){
	return temp_sens.get_fdata() < shut_off_temp;
}
