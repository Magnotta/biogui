#include <Arduino.h>
#include "sensors.hpp"

/// @brief Sensor constructor
/// @param sensor_pin The ADC pin on the arduino the sensor is connected to
/// @param window_size Median filter window size, can only be 15 or smaller
ADCSens::ADCSens(uint8_t sensor_pin, uint8_t window_size, double conversion_coeff){
	_sensor_pin = sensor_pin;
	_conversion_coeff = conversion_coeff;
	if(window_size > 15)		_median_filter_window_size = 15;
	else if(window_size < 3)	_median_filter_window_size = 3;
	else						_median_filter_window_size = window_size;
}

void ADCSens::init(){
	pinMode(_sensor_pin, INPUT);
	for(int i = 0; i < _median_filter_window_size; i++){
		_readings_array[i] = get_data();
	}
}

void ADCSens::update(){
	append(get_data());
	_latest_filtered_datum = median_filter();
}

void ADCSens::copy(double *aux){
	for(int i = 0; i < _median_filter_window_size; i++){
		aux[i] = _readings_array[i];
	}
}

void ADCSens::append(double val){
	for(int i = _median_filter_window_size-1; i > 0; i--){
		_readings_array[i] = _readings_array[i - 1];
	}
	_readings_array[0] = val;
}

double ADCSens::median_filter(){
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

double ADCSens::get_filtered_datum(){
	return _latest_filtered_datum;
}

double ADCSens::get_data(){
	return analogRead(_sensor_pin)*_conversion_coeff;
}
