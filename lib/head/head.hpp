#ifndef LED_HEAD
#define LED_HEAD

#include <Arduino.h>
#include "sensors.hpp"

class LEDHead{
public:
	LEDHead(uint8_t _LED_control_pin, uint8_t _temp_sens_pin, uint8_t _amp_sens_pin, double _shut_off_temp);
	void init();
	void update();
	void LEDOn(uint8_t);
	void LEDOff();
	bool temp_safe();

	char temperature_string[6];
	char amps_string[10];
protected:
	TempSens temp_sens;
	AmpSens amp_sens;
	uint8_t LED_control_pin;
	double shut_off_temp;
	unsigned long timestamp;
};

#endif