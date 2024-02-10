#ifndef LIB_HEAD_HEAD_HPP_
#define LIB_HEAD_HEAD_HPP_

#include <Arduino.h>
#include "sensors.hpp"

class LEDHead{
public:
	/// @brief Abstraction class for most of the circuitry in the main board, not including the SD card.
	/// @param MOSFET_gate_pin Arduino GPIO pin connected to the MOSFET gate. Must be a PWM enabled pin.
	/// @param relay_pin Arduino GPIO pin connected to the relay signal.
	/// @param MOSFET_temp_sens_pin Arduino ADC pin connected to LM35DZ at MOSFET's heatsink.
	/// @param temp_sens_pin Arduino ADC pin connected to LM35DZ at the main LED.
	/// @param amp_sens_pin Arduino ADC pin connected to amplified shunt current sensor in the LED path.
	/// @param temp_window_size Median filter window size for the temperature sensors.
	/// @param temp_multiplier Conversion multiplier between ADC output (0 ~ 1023) and expected value in degrees centigrade.
	/// @param amps_window_size Median filter window size for the electric current sensor.
	/// @param amps_multiplier Conversion multiplier between ADC output (0 ~ 1023) and expected value in amperes.
	/// @param shut_off_temp Maximum safe LED operating temperature.
	LEDHead(uint8_t MOSFET_gate_pin, uint8_t relay_pin,
			uint8_t MOSFET_temp_sens_pin, uint8_t temp_sens_pin, uint8_t amp_sens_pin,
			uint8_t temp_window_size, double temp_multiplier, uint8_t amps_window_size,
			double amps_multiplier, double shut_off_temp);

	void init();

	
	void step();

	void preheat(uint8_t duty_cycle);

	/// @brief Send a 1kHz PWM signal to the MOSFET's gate.
	/// @param duty_cycle 
	void MOSFET_on(uint8_t duty_cycle);

	void MOSFET_off();
	
	void relay_on();
	
	void relay_off();
	
	bool temp_safe();
	
	bool LED_is_on();
	
	bool relay_is_on();

	char LED_temperature_string[7], MOSFET_temperature_string[7];
	char amps_string[7];
	uint8_t _pwm;
protected:
	ADCSens _LED_temp_sens, _MOSFET_temp_sens, _source_amp_sens;
	uint8_t _MOSFET_gate_pin, _relay_pin;
	double _shut_off_temp, _preheat_buf[10];
	bool _LED_on, _preheat_on, _relay_on;
private:
	void append(double);
};

#endif // LIB_HEAD_HEAD_HPP_
