#ifndef LIB_HEAD_HEAD_HPP_
#define LIB_HEAD_HEAD_HPP_

#include <Arduino.h>
#include "sensors.hpp"

class LEDHead{
public:
	LEDHead(uint8_t _LED_control_pin, uint8_t _temp_sens_pin, uint8_t _amp_sens_pin, double _shut_off_temp);
	void init();
	void update();
	void LED_start(uint8_t duty_cycle);
	void LED_stop();
	bool temp_safe();
	bool LED_is_on();

	char LED_temperature_string[6], MOSFET_temperature_string[6];
	char amps_string[10];
protected:
	TempSens LED_temp_sens, MOSFET_temp_sens;
	AmpSens LED_amp_sens;
	uint8_t LED_control_pin;
	double shut_off_temp;
	bool LED_on;
};

#endif // LIB_HEAD_HEAD_HPP_
