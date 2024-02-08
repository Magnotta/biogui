#include "datalog.hpp"
#include "head.hpp"

Logger::Logger(LEDHead *_head_p, uint8_t _fail_indicator_led_pin, uint8_t _spi_sck_mhz, bool _options, uint8_t _chip_select_pin){
	head_p = _head_p;
	fail_indicator_led_pin = _fail_indicator_led_pin;
	spi_sck_mhz = _spi_sck_mhz;
	options = _options;
	chip_select_pin = _chip_select_pin;
	active = false;
}

void Logger::init(const char* _filename){
	pinMode(fail_indicator_led_pin, OUTPUT);
	
	if(!sd.begin(SdSpiConfig(chip_select_pin, options, SD_SCK_MHZ(spi_sck_mhz))))
    	digitalWrite(fail_indicator_led_pin, HIGH);
	else
    	active = true;
	
	char *dot = strchr(_filename, '.');
	uint8_t len = strlen(_filename);
	if(len > 12 ||						// 8.3 filename format has no more than 12 chars
		!dot ||							// must have a dot
		dot - _filename > 8 ||			// must have no more than 8 chars file title
		_filename + len - dot > 3)		// must have no more than 3 chars file extension
		snprintf(filename, 13, _filename);
	else
		snprintf(filename, 13, "data.bin");
}

bool Logger::is_active(){
	return active;
}

void Logger::log_to_file(){
	if(!file.open(filename, O_WRONLY))
		return;

	snprintf(row_buf, 30, "%s, %s, %s\n", head_p->LED_temperature_string,
										  head_p->MOSFET_temperature_string,
										  head_p->amps_string);

	while (sd.card()->isBusy()){}
	file.write(row_buf, 29);
	
	file.truncate();
	file.sync();
}
