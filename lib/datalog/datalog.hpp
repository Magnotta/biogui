#ifndef LIB_DATALOG_DATALOG_HPP_
#define LIB_DATALOG_DATALOG_HPP_

#include <Arduino.h>
#include <SdFat.h>
#include "head.hpp"

class Logger{
public:
	Logger(LEDHead*, uint8_t, uint8_t=50, bool=DEDICATED_SPI, uint8_t=SS);
	void init(const char*);
	bool is_active();
	void log_to_file();

	void fail_led_on();
	void fail_led_off();

protected:
	SdFat32 _sd;
	File32 _file;
	char _row_buf[30];
	LEDHead* _head_p;

	uint8_t _spi_sck_mhz;
	uint8_t _chip_select_pin;
	uint8_t _options;
	uint8_t _fail_indicator_led_pin;
	bool _active;
	char _filename[13];
};

#endif // LIB_DATALOG_DATALOG_HPP_