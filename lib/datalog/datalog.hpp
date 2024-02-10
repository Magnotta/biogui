#ifndef LIB_DATALOG_DATALOG_HPP_
#define LIB_DATALOG_DATALOG_HPP_

#include <Arduino.h>
#include <SdFat.h>
#include "head.hpp"

class Logger{
public:
	/// @brief Interface over SPI to a microSD card.
	/// @param head_p Pointer to the LEDHead object. Needed for access to data from sensors.
	/// @param fail_indicator_led_pin Arduino GPIO pin connected to the failure indicator LED.
	/// @param spi_sck_mhz Frequency of the SPI bus communication in MHz. Default is 50.
	/// @param options SPI options. Default is DEDICATED_SPI.
	/// @param chip_select_pin Arduino GPIO pin connected to the microSD reader chip select. Default is SS.
	Logger(LEDHead *head_p, uint8_t fail_indicator_led_pin, uint8_t spi_sck_mhz=50, bool options=DEDICATED_SPI, uint8_t chip_select_pin=SS);
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