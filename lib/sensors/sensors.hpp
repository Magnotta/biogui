#ifndef LIB_SENSORS_SENSORS_HPP_
#define LIB_SENSORS_SENSORS_HPP_

#include <Arduino.h>

class ADCSens{
public:
	/// @brief Interface for the Arduino ADC input pins. It incorporates unit conversion and moving median filtering. 
	/// @param sensor_pin Arduino ADC pin connected to the sensor's output.
	/// @param window_size Moving median filter window size. Must be an odd number from 3 to 15 inclusive.
	/// @param conversion_coeff Conversion multiplier between ADC output (0 ~ 1023) and expected value in given units.
	ADCSens(uint8_t sensor_pin, uint8_t window_size, double conversion_coeff);
	ADCSens():ADCSens{0, 3, 0.0}{};
	void init();
	void update();
	double get_filtered_datum();
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

#endif // LIB_SENSORS_SENSORS_HPP_
