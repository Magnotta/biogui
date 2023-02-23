#include <Arduino.h>
#include "temperature.hpp"

TempSens::TempSens(uint8_t pin, uint8_t winSize, double multiplier){
	sensor_pin = pin;
	median_filter_array_size = winSize;
	conversion_factor = multiplier;
}

void TempSens::initialize(){
	pinMode(sensor_pin, INPUT);
	for(int i=0; i < median_filter_array_size; i++){
    	temperature_array[i] = analogRead(sensor_pin)*conversion_factor;
	}
	time_stamp = millis();
}

void TempSens::update(){
	if(millis() - time_stamp > 1000){
		append(analogRead(sensor_pin)*conversion_factor);
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
    dtostrf(tempSortArr[median_filter_array_size/2], 5, 2, temperature_string);
    time_stamp = millis();
	}
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
