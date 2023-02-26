#include "head.hpp"
#include <Arduino.h>

LEDHead::LEDHead(uint8_t _pinLED, uint8_t _pinSens, double _shutoffTemp){
	sensor.setPin(_pinSens);
	sensor.setSOT(_shutoffTemp);
	LEDpin = _pinLED;
}

void LEDHead::init(){
	sensor.init();
	pinMode(LEDpin, OUTPUT);
}

void LEDHead::LEDOn(uint8_t dutyCycle){
	if(sensor.safe()){
		analogWrite(LEDpin, dutyCycle);
	}
}

void LEDHead::LEDOff(){
	digitalWrite(LEDpin, LOW);
}

bool LEDHead::verify(){
	sensor.update();
	dtostrf(sensor.getTemp(), 5, 2, temperature_string);
	return sensor.safe();
}