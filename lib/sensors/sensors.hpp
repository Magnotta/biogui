#ifndef LIB_SENSORS_SENSORS_HPP_
#define LIB_SENSORS_SENSORS_HPP_

#include <Arduino.h>

class Sens{
public:
	Sens(uint8_t, uint8_t);
	void init();
	virtual void update();
	double get_fdata();
protected:
	virtual double get_data();
	void append(double newValue);
	void copy(double *aux);
	double median_filter();

	uint8_t sensor_pin;
	uint8_t median_filter_window_size;
	double readings_array[15];
	double latest_filtered_datum;
};

class TempSens : public Sens{
public:
	TempSens(uint8_t _pin, uint8_t _window_size, double _conversion_coeff = 0.489);
	TempSens():TempSens{0, 0}{};
	void update();
protected:
	double get_data();
	double conversion_coeff;
};

class AmpSens : public Sens{
public:
	AmpSens(uint8_t _pin, uint8_t _window_size, uint16_t _shift = 310, double _conversion_coeff = 0.0264);
	AmpSens():AmpSens(0, 0){}
	void update();
protected:
	double get_data();
	uint16_t shift;
	double conversion_coeff;
};

#endif // LIB_SENSORS_SENSORS_HPP_
