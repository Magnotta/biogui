#ifndef LED_HEAD
#define LED_HEAD

#include <Arduino.h>
#include "temperature.hpp"

class LEDHead{
public:
	LEDHead(uint8_t _pinLED, uint8_t _pinSens, double _shutoffTemp);
	void init();
	void LEDOn(uint8_t dutyCycle);
	void LEDOff();
	bool verify();

	char temperature_string[6];
protected:
private:
	TempSens sensor;
	uint8_t LEDpin;
};

#endif