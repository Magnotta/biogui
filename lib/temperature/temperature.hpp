#ifndef TEMPERATURE
#define TEMPERATURE

#include <Arduino.h>

class TempSens{
public:
	TempSens(uint8_t pin, uint8_t window_size, int delay_ms, double shutoff_temperature, double multiplier=0.489);
	TempSens():TempSens{0, 7, 1000, 50.0}{};
	void init();
	void update();
	double getTemp();
	void setPin(uint8_t pin);
	void setSOT(double temp);
	bool safe();
protected:
	void append(double newValue);
	void copy(double *aux);
	double median_filter();
private:
	byte sensor_pin;
	unsigned long timestamp;
	uint8_t median_filter_array_size;
	double temperature_array[15];
	double conversion_factor;
	double temperature;
	double shutoff_temperature;
	int delay_ms;
};

#endif
