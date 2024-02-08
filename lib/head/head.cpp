#include "head.hpp"
#include <Arduino.h>

LEDHead::LEDHead(uint8_t _LED_control_pin, uint8_t _temp_sens_pin, uint8_t _amp_sens_pin, double _shut_off_temp){
	LED_temp_sens = TempSens{_temp_sens_pin, 5};
	LED_amp_sens = AmpSens{_amp_sens_pin, 5};
	LED_control_pin = _LED_control_pin;
	shut_off_temp = _shut_off_temp;
}

void LEDHead::init(){
	LED_temp_sens.init();
	pinMode(LED_control_pin, OUTPUT);
}

void LEDHead::update(){
	LED_temp_sens.update();
	dtostrf(LED_temp_sens.get_fdata(), 5, 2, LED_temperature_string);

	MOSFET_temp_sens.update();
	dtostrf(MOSFET_temp_sens.get_fdata(), 5, 2, MOSFET_temperature_string);

	LED_amp_sens.update();
	dtostrf(LED_amp_sens.get_fdata(), 9, 2, amps_string);
}

void LEDHead::LED_start(uint8_t duty_cycle){
	if(LED_temp_sens.get_fdata() < shut_off_temp){
		analogWrite(LED_control_pin, duty_cycle);
		LED_on = true;
	}
}

void LEDHead::LED_stop(){
	digitalWrite(LED_control_pin, LOW);
	LED_on = false;
}

bool LEDHead::temp_safe(){
	return LED_temp_sens.get_fdata() < shut_off_temp;
}

bool LEDHead::LED_is_on(){
	return LED_on;
}
