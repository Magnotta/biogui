#include <Arduino.h>
#include <sensors.hpp>
#include <head.hpp>
#include <max6675.h>

char msgByte, cmdBuff[10];
byte cmd_i = 0;
unsigned long timestamp;
const int sampleT_ms = 1000;

LEDHead head{46, 47, A10, A9, A8, 5, 0.489, 5, 0.00727, 75.0};

MAX6675 thermocouple{SCK, SS, MISO};

void clearArray(char*, int);
void getCmd();
void printData();
void cmdExe();

void setup(void){
	Serial.begin(115200);
	head.init();
}

void loop(){
	getCmd();
  
	if(millis() - timestamp > sampleT_ms){
		timestamp = millis();
		head.step();
		printData();
	}

}

void clearArray(char* arr, int len){
  for( int i = 0; i < len; i++ ){
    arr[i] = '\0';
  }
}

void getCmd(){
  while(Serial.available() > 0){
    msgByte = Serial.read();
    if(msgByte != '\n'){
      cmdBuff[cmd_i] = msgByte;
      cmd_i++;
    }
    else{
      cmdExe();
      clearArray(cmdBuff, 10);
      cmd_i = 0;
    }
  }
}

void printData(){  
	Serial.print(head._pwm);
	Serial.print(",");
	Serial.print(head.amps_string);
	Serial.print(",");
	Serial.println(thermocouple.readCelsius());
}

void cmdExe(){
	switch(cmdBuff[0]){
	case 'd':
		uint8_t pwm = atoi(cmdBuff+1);
		head.MOSFET_on(pwm);
		break;
	case 'r':
		if(head.relay_is_on())	head.relay_off();
		else					head.relay_on();

		break;
	case 's':
		head.MOSFET_off();
		delay(15);
		if(head.relay_is_on())	head.relay_off();
		else					head.relay_on();
		delay(35);
		head.MOSFET_on(255);
		break;
	case 'o':
		head.MOSFET_off();
		break;
	default:
		Serial.println("Command unrecognized. No action invoked.");
		break;
	}
}