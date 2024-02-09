#include <Arduino.h>
#include <sensors.hpp>
#include <head.hpp>
#include <datalog.hpp>

char msgByte, cmdBuff[10];
byte cmd_i = 0;
unsigned long timestamp;
const int sampleT_ms = 1000;

LEDHead head{46, 47, A10, A9, A8, 75.0};
Logger logger{&head, 49};

void clearArray(char*, int);
void getCmd();
void printData();
void cmdExe();

void setup(void){
	Serial.begin(115200);
	head.init();
	logger.init("pdt660a.bin");
}

void loop(){
	getCmd();
  
	if(millis() - timestamp > sampleT_ms){
		timestamp = millis();
		head.step();
		if(head.LED_is_on() && logger.is_active())
			logger.log_to_file();
		printData();
	}

}

void clearArray( char* arr, int len ){
  for( int i = 0; i < len; i++ ){
    arr[i] = '\0';
  }
}

void getCmd(){
  while( Serial.available() > 0 ){
    msgByte = Serial.read();
    if( msgByte != '\n' ){
      cmdBuff[cmd_i] = msgByte;
      cmd_i++;
    }
    else{
      cmdExe();
      clearArray( cmdBuff, 10 );
      cmd_i = 0;
    }
  }
}

void printData(){  
	Serial.print(head._pwm);
	Serial.print(",");
	Serial.print(head.amps_string);
	Serial.print(",");
	Serial.print(head.MOSFET_temperature_string);
	Serial.print(",");
	Serial.println(head.LED_temperature_string);
}

void cmdExe(){
	uint8_t pwm;
	switch( cmdBuff[0] ){
	case 'd':
		pwm = atoi(cmdBuff+1);
		head.MOSFET_on(pwm);
		break;
	case 'r':
		if(head.relay_is_on())	head.relay_off();
		else					head.relay_on();

		break;
	case 'o':
		head.MOSFET_off();
		break;
	default:
		Serial.println("Command unrecognized. No action invoked.");
		break;
	}
}