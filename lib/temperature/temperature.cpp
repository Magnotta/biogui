#include <Arduino.h>
#include "temperature.hpp"

TempSens::TempSens(uint8_t pin, uint8_t winSize, int _delay_ms, double _shutoff_temperature, double multiplier){
	sensor_pin = pin;
	median_filter_array_size = winSize;
  delay_ms = _delay_ms;
  shutoff_temperature = _shutoff_temperature;
	conversion_factor = multiplier;
}

void TempSens::init(){
	pinMode(sensor_pin, INPUT);
	for(int i=0; i < median_filter_array_size; i++){
    	temperature_array[i] = analogRead(sensor_pin)*conversion_factor;
	}
	timestamp = millis();
}

double TempSens::median_filter(){
  double aux;
  double tempSortArr[median_filter_array_size];
  copy(tempSortArr);
  
  for(int i = 1; i < median_filter_array_size; i++){
    for(int j = 0; j < median_filter_array_size - i; j++){
      if(tempSortArr[j] > tempSortArr[j+1]){
        aux = tempSortArr[j];
        tempSortArr[j] = tempSortArr[j+1];
        tempSortArr[j+1] = aux;
      }
    }
  }

  return tempSortArr[median_filter_array_size/2];
}

void TempSens::update(){
	if(millis() - timestamp > 1000){
		append(analogRead(sensor_pin)*conversion_factor);
    temperature = median_filter();
    timestamp = millis();
	}
}

double TempSens::getTemp(){
  return temperature;
}

void TempSens::setPin(uint8_t pin){
  sensor_pin = pin;
}

void TempSens::setSOT(double temp){
  shutoff_temperature = temp;
}

bool TempSens::safe(){
  return temperature < shutoff_temperature;
}

void TempSens::append(double newValue){
	for(int i = median_filter_array_size-1; i >= 0; i--){
    if(i == 0){
      temperature_array[i] = newValue;
    }
    else{
      temperature_array[i] = temperature_array[i - 1];
    }
  }
}

void TempSens::copy(double* aux){
  for(int i = 0; i < median_filter_array_size; i++){
    aux[i] = temperature_array[i];
  }
}
