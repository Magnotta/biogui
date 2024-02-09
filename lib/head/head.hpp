#ifndef LIB_HEAD_HEAD_HPP_
#define LIB_HEAD_HEAD_HPP_

#include <Arduino.h>
#include "sensors.hpp"

class LEDHead{
public:
	LEDHead(uint8_t power_LED_pin, uint8_t relay_pin, uint8_t MOSFET_temp_sens_pin, uint8_t temp_sens_pin, uint8_t amp_sens_pin, double shut_off_temp);
	void init();
	void step();
	void preheat(uint8_t);
	void MOSFET_on(uint8_t);
	void MOSFET_off();
	void relay_on();
	void relay_off();
	bool temp_safe();
	bool LED_is_on();
	bool relay_is_on();

	char LED_temperature_string[7], MOSFET_temperature_string[7];
	char amps_string[7];
	uint8_t _pwm;
protected:
	TempSens _LED_temp_sens, _MOSFET_temp_sens;
	AmpSens _source_amp_sens;
	uint8_t _MOSFET_gate_pin, _relay_pin;
	double _shut_off_temp, _preheat_buf[10];
	bool _LED_on, _preheat_on, _relay_on;
private:
	void append(double);
};

#endif // LIB_HEAD_HEAD_HPP_
