#include <Arduino.h>
#include "sensors.hpp"

/// @brief Sensor constructor
/// @param sensor_pin The ADC pin on the arduino the sensor is connected to
/// @param window_size Median filter window size, can only be 15 or smaller
Sens::Sens(uint8_t sensor_pin, uint8_t window_size, double conversion_coeff){
	_sensor_pin = sensor_pin;
	if(window_size > 15)		_median_filter_window_size = 15;
	else if(window_size < 3)	_median_filter_window_size = 3;
	else						_median_filter_window_size = window_size;
}

void Sens::init(){
	pinMode(_sensor_pin, INPUT);
	for(int i = 0; i < _median_filter_window_size; i++){
		_readings_array[i] = get_data();
	}
}

void Sens::update(){
	append(get_data());
	_latest_filtered_datum = median_filter();
}

void Sens::copy(double *aux){
	for(int i = 0; i < _median_filter_window_size; i++){
		aux[i] = _readings_array[i];
	}
}

void Sens::append(double val){
	for(int i = _median_filter_window_size-1; i > 0; i--){
		_readings_array[i] = _readings_array[i - 1];
	}
	_readings_array[0] = val;
}

double Sens::median_filter(){
	double aux_sort_arr[15];
	copy(aux_sort_arr);

	double aux;
	for(int i = 1; i < _median_filter_window_size; i++){
		for(int j = 0; j < _median_filter_window_size - i; j++){
			if(aux_sort_arr[j] > aux_sort_arr[j+1]){
				aux = aux_sort_arr[j];
				aux_sort_arr[j] = aux_sort_arr[j+1];
				aux_sort_arr[j+1] = aux;
			}
		}
	}

	return aux_sort_arr[_median_filter_window_size/2];
}

double Sens::get_fdata(){
	return _latest_filtered_datum;
}

double Sens::get_data(){
	return analogRead(_sensor_pin)*_conversion_coeff;
}

/// @brief Temperature sensor class for LM35DZ
/// @param sensor_pin The ADC pin on the arduino the sensor is connected to
/// @param window_size Median filter window size, can only be 15 or smaller
/// @param conversion_coeff Multiplier to convert from 0-1023 ADC output to floating point data
TempSens::TempSens(uint8_t sensor_pin, uint8_t window_size, double conversion_coeff)
:Sens(sensor_pin, window_size, conversion_coeff){}

AmpSens::AmpSens(uint8_t sensor_pin, uint8_t window_size, double conversion_coeff)
:Sens(sensor_pin, window_size, conversion_coeff){}
