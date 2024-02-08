#ifndef LIB_DATALOG_DATALOG_HPP_
#define LIB_DATALOG_DATALOG_HPP_

#include <Arduino.h>
#include <SdFat.h>
#include "head.hpp"

class Logger{
public:
	Logger(LEDHead *_head_p, uint8_t _fail_indicator_led_pin, uint8_t _spi_sck_mhz=50, bool _options=DEDICATED_SPI, uint8_t _chip_select_pin=SS);
	void init(const char* _filename);
	bool is_active();
	void log_to_file();

	void fail_led_on();
	void fail_led_off();

protected:
	SdFat32 sd;
	File32 file;
	char row_buf[30];
	LEDHead* head_p;

	uint8_t spi_sck_mhz;
	uint8_t chip_select_pin;
	uint8_t options;
	uint8_t fail_indicator_led_pin;
	bool active;
	char filename[13];
};

#endif // LIB_DATALOG_DATALOG_HPP_