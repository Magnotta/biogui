#ifndef TEMPERATURE
#define TEMPERATURE

#include <Arduino.h>

class TempSens{
public:
	TempSens(uint8_t pin, uint8_t window_size, double multiplier=0.489);
	void initialize();
	void update();
	double getTemp();
	
	char temperature_string[6];
protected:
	void append(double newValue);
	void copy(double *aux);
private:
	unsigned long time_stamp;
	byte sensor_pin;
	uint8_t median_filter_array_size;
	double temperature_array[15];
	double conversion_factor;
};

#endif
