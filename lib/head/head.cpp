#include "head.hpp"
#include <Arduino.h>

LEDHead::LEDHead(uint8_t MOSFET_gate_pin, uint8_t relay_pin,
			uint8_t MOSFET_temp_sens_pin, uint8_t temp_sens_pin, uint8_t amp_sens_pin,
			uint8_t temp_window_size, double temp_multiplier, uint8_t amps_window_size,
			double amps_multiplier, double shut_off_temp){
	_MOSFET_temp_sens = ADCSens{MOSFET_temp_sens_pin, temp_window_size, temp_multiplier};
	_LED_temp_sens = ADCSens{temp_sens_pin, temp_window_size, temp_multiplier};
	_source_amp_sens = ADCSens{amp_sens_pin, amps_window_size, amps_multiplier};
	_MOSFET_gate_pin = MOSFET_gate_pin;
	_relay_pin = relay_pin;
	_pwm = 0;
	_shut_off_temp = shut_off_temp;

	_LED_on = false;
	_preheat_on = false;
	_relay_on = false;

	for(uint8_t i = 0; i < 10; i++){
		_preheat_buf[i] = 0.0;
	}

}

void LEDHead::init(){
	_LED_temp_sens.init();
	_MOSFET_temp_sens.init();
	_source_amp_sens.init();
	pinMode(_MOSFET_gate_pin, OUTPUT);
	pinMode(_relay_pin, OUTPUT);
	digitalWrite(_relay_pin, HIGH);
}

void LEDHead::step(){
	_LED_temp_sens.update();
	dtostrf(_LED_temp_sens.get_filtered_datum(), 6, 2, LED_temperature_string);

	_MOSFET_temp_sens.update();
	dtostrf(_MOSFET_temp_sens.get_filtered_datum(), 6, 2, MOSFET_temperature_string);

	_source_amp_sens.update();
	dtostrf(_source_amp_sens.get_filtered_datum(), 6, 2, amps_string);

	if(_LED_on){
		if(_LED_temp_sens.get_filtered_datum() > _shut_off_temp){
			MOSFET_off();
		}
	}
}

void LEDHead::preheat(uint8_t duty_cycle){
	_pwm = duty_cycle;
	_preheat_on = true;
	delay(25);
	analogWrite(_MOSFET_gate_pin, _pwm);
}

void LEDHead::MOSFET_on(uint8_t duty_cycle){
	_pwm = duty_cycle;
	analogWrite(_MOSFET_gate_pin, _pwm);
	_LED_on = true;
}

void LEDHead::MOSFET_off(){
	digitalWrite(_MOSFET_gate_pin, LOW);
	_pwm = 0;
	_LED_on = false;
}

bool LEDHead::temp_safe(){
	return _LED_temp_sens.get_filtered_datum() < _shut_off_temp;
}

bool LEDHead::LED_is_on(){
	return _LED_on;
}

void LEDHead::append(double val){
	for(uint8_t i = 9; i > 0; i--){
		_preheat_buf[i] = _preheat_buf[i - 1];
	}
	_preheat_buf[0] = val;
}

void LEDHead::relay_on(){
	digitalWrite(_relay_pin, LOW);
	_relay_on = true;
}

void LEDHead::relay_off(){
	digitalWrite(_relay_pin, HIGH);
	_relay_on = false;
}

bool LEDHead::relay_is_on(){
	return _relay_on;
}