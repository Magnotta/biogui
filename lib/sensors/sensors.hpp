#ifndef LIB_SENSORS_SENSORS_HPP_
#define LIB_SENSORS_SENSORS_HPP_

#include <Arduino.h>

class Sens{
public:
	Sens(uint8_t, uint8_t, double);
	Sens() : Sens{0, 5, 0.0}{};
	void init();
	virtual void update();
	double get_fdata();
protected:
	double get_data();
	void append(double);
	void copy(double*);
	double median_filter();

	uint8_t _sensor_pin;
	uint8_t _median_filter_window_size;
	double _conversion_coeff;
	double _readings_array[15];
	double _latest_filtered_datum;
};

class TempSens : public Sens{
public:
	TempSens(uint8_t, uint8_t, double=0.489);
	TempSens() : TempSens{0, 5}{};
protected:
};

class AmpSens : public Sens{
public:
	AmpSens(uint8_t, uint8_t, double=0.0264);
	AmpSens() : AmpSens{0, 5}{};
protected:
};

#endif // LIB_SENSORS_SENSORS_HPP_
