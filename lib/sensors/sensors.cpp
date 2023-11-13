#include <Arduino.h>
#include "sensors.hpp"

/// @brief Sensor constructor
/// @param _pin The ADC pin on the arduino the sensor is connected to
/// @param _window_size Median filter window size, can only be 15 or smaller
Sens::Sens(uint8_t _pin, uint8_t _window_size){
  sensor_pin = _pin;
  if(_window_size > 15) median_filter_window_size = 15;
  else                  median_filter_window_size = _window_size;
}

void Sens::init(){
  pinMode(sensor_pin, INPUT);
	for(int i = 0; i < median_filter_window_size; i++){
    	readings_array[i] = get_data();
	}
}

void Sens::copy(double *aux){
  for(int i = 0; i < median_filter_window_size; i++){
    aux[i] = readings_array[i];
  }
}

void Sens::append(double val){
  for(int i = median_filter_window_size-1; i > 0; i--){
    readings_array[i] = readings_array[i - 1];
  }
  readings_array[0] = val;
}

double Sens::median_filter(){
  double aux_sort_arr[15];
  copy(aux_sort_arr);

  double aux;
  for(int i = 1; i < median_filter_window_size; i++){
    for(int j = 0; j < median_filter_window_size - i; j++){
      if(aux_sort_arr[j] > aux_sort_arr[j+1]){
        aux = aux_sort_arr[j];
        aux_sort_arr[j] = aux_sort_arr[j+1];
        aux_sort_arr[j+1] = aux;
      }
    }
  }

  return aux_sort_arr[median_filter_window_size/2];
}

double Sens::get_fdata(){
  return latest_filtered_datum;
}

/// @brief Temperature sensor class for LM35DZ
/// @param _pin The ADC pin on the arduino the sensor is connected to
/// @param _window_size Median filter window size, can only be 15 or smaller
/// @param _conversion_coeff Multiplier to convert from 0-1023 ADC output to floating point data
TempSens::TempSens(uint8_t _pin, uint8_t _window_size, double _conversion_coeff)
:Sens(_pin, _window_size){
  conversion_coeff = _conversion_coeff;
}

double TempSens::get_data(){
  return analogRead(sensor_pin)*conversion_coeff;
}

void TempSens::update(){
  append(get_data());
  latest_filtered_datum = median_filter();
}

AmpSens::AmpSens(uint8_t _pin, uint8_t _window_size, uint16_t _shift, double _conversion_coeff)
:Sens(_pin, _window_size){
  conversion_coeff = _conversion_coeff;
  shift = _shift;
}

void AmpSens::update(){
  append(get_data());
  latest_filtered_datum = median_filter();
}

double AmpSens::get_data(){
  return static_cast<double>(shift - analogRead(sensor_pin))*conversion_coeff;
}
