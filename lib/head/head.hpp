#ifndef LIB_HEAD_HEAD_HPP_
#define LIB_HEAD_HEAD_HPP_

#include <Arduino.h>
#include "sensors.hpp"

class LEDHead{
public:
	LEDHead(uint8_t, uint8_t, uint8_t, uint8_t, double);
	void init();
	void update();
	void preheat(uint8_t);
	void LED_start(uint8_t);
	void LED_stop();
	bool temp_safe();
	bool LED_is_on();

	char LED_temperature_string[6], MOSFET_temperature_string[6];
	char amps_string[10];
protected:
	TempSens _LED_temp_sens, _MOSFET_temp_sens;
	AmpSens _LED_amp_sens;
	uint8_t _LED_control_pin;
	double _shut_off_temp;
	bool _LED_on, _preheat_on;
};

#endif // LIB_HEAD_HEAD_HPP_
