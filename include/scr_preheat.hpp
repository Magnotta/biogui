#include "nav.hpp"
#include "sensors.hpp"
#include "head.hpp"
#include "datalog.hpp"

extern MCUFRIEND_kbv tft;
extern Page config, play;
extern Router sys;
extern LEDHead head;
extern Logger logger;
extern uint8_t pwm;

Page preheat{&tft};

namespace Preheat{

void tmr1_cb();
void btn1_cb();
void btn2_cb();

Timer tmr1{67, 240, tmr1_cb};
Button btn1{115, 60, "Parar", btn1_cb};
Button btn2{95, 360, "Iluminar", btn2_cb};

void tmr1_cb(){
	btn2.activate();
	btn2.draw(&tft);
}

void btn1_cb(){
	sys.goto_screen(&config);
	head.MOSFET_off();
}

void btn2_cb(){
	sys.goto_screen(&play);
	logger.stamp_file(";LED start\n");
	Play::btn2.deactivate();
	if(Config::toggle1.is_toggled())	Play::tmr1.arming_event(Config::slider4.get_val());
	else							Play::tmr1.arming_event(Config::slider2.get_val()*60);
	head.switch_relay();
}

void add_widgets(){
	preheat.add_widget(&tmr1);
	preheat.add_widget(&btn1);
	preheat.add_widget(&btn2);
}
}	// namespace Preheat