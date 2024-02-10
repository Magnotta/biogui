#include "datalog.hpp"
#include "head.hpp"

Logger::Logger(LEDHead *head_p, uint8_t fail_indicator_led_pin, uint8_t spi_sck_mhz, bool options, uint8_t chip_select_pin){
	_head_p = head_p;
	_fail_indicator_led_pin = fail_indicator_led_pin;
	_spi_sck_mhz = spi_sck_mhz;
	_options = options;
	_chip_select_pin = chip_select_pin;
	_active = false;
}

void Logger::init(const char* filename){
	pinMode(_fail_indicator_led_pin, OUTPUT);
	
	if(!_sd.begin(SdSpiConfig(_chip_select_pin, _options, SD_SCK_MHZ(_spi_sck_mhz))))
    	digitalWrite(_fail_indicator_led_pin, HIGH);
	else
    	_active = true;
	
	char *dot = strchr(filename, '.');
	uint8_t len = strlen(filename);
	if(len > 12 ||						// 8.3 filename format has no more than 12 chars
		!dot ||							// must have a dot
		dot - filename > 8 ||			// must have no more than 8 chars file title
		filename + len - dot > 3)		// must have no more than 3 chars file extension
		snprintf(_filename, 13, filename);
	else
		snprintf(_filename, 13, "data.bin");
}

bool Logger::is_active(){
	return _active;
}

void Logger::log_to_file(){
	if(!_file.open(_filename, O_WRITE)){
		fail_led_on();
		return;
	}
	fail_led_off();

	snprintf(_row_buf, 30, "%s, %s, %s\n", _head_p->LED_temperature_string,
										  _head_p->MOSFET_temperature_string,
										  _head_p->amps_string);

	while (_sd.card()->isBusy()){}
	_file.write(_row_buf, 29);
	
	_file.truncate();
	_file.sync();

	_file.close();
}

void Logger::fail_led_off(){
	digitalWrite(_fail_indicator_led_pin, LOW);
}

void Logger::fail_led_on(){
	digitalWrite(_fail_indicator_led_pin, HIGH);
}