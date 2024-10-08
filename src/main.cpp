#include <Arduino.h>
#include <MCUFRIEND_kbv.h>
#include <TouchScreen.h>

#include "widget.hpp"
#include "nav.hpp"
#include "sensors.hpp"
#include "head.hpp"
#include "datalog.hpp"

#include "scr_config.hpp"
#include "scr_play.hpp"
#include "scr_error.hpp"
#include "scr_preheat.hpp"

MCUFRIEND_kbv tft;
// ALL Touch panels and wiring is DIFFERENT
// copy-paste results from MCUFRIEND_kbv/examples/TouchScreen_Calibr_native.ino
const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486
const int TS_LEFT=149,TS_RT=923,TS_TOP=954,TS_BOT=90;


// const int XP=8,XM=A2,YP=A3,YM=9; //320x480 ID=0x9486
// const int TS_LEFT=111,TS_RT=927,TS_TOP=935,TS_BOT=89;

TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);
TSPoint tp;
uint16_t ID;

LEDHead head{46, 47, A10, A9, A8, 5, 0.489, 5, 0.003612, 75.0};
Logger logger{&head, 49};

void setup(void){
	head.init();
	
	logger.init("pdt660a.bin");

	sys.set_home(&config);
	tft.reset();
	ID = tft.readID();
	tft.begin(ID);
	tft.setRotation(0);   // Portrait orientation
	tft.fillScreen(BLACK);
	tft.setCursor(51, 465);
	tft.print("Andre Mariano e Paulo Souza - IF UnB");

	Config::add_widgets();
	Play::add_widgets();
	Error::add_widgets();
	Preheat::add_widgets();

	sys.enter();
}

void loop(){
	uint16_t xpos = 0, ypos = 0;  //screen coordinates
	tp = ts.getPoint();   //tp.x, tp.y are ADC values
	// if sharing pins, you'll need to fix the directions of the touchscreen pins
	pinMode(XM, OUTPUT);
	pinMode(YP, OUTPUT);
	// we have some minimum pressure we consider 'valid'
	// pressure of 0 means no pressing!
	if(tp.z > 200 && tp.z < 1000){
		xpos = map(tp.x, TS_LEFT, TS_RT, 0, tft.width());
		ypos = map(tp.y, TS_TOP, TS_BOT, 0, tft.height());
	}


	sys.loop(xpos, ypos);
	head.step();
	if(!head.temp_safe()){
		head.MOSFET_off();
		sys.goto_screen(&error);
	}
	if(sys.once_per_sec() && head.MOSFET_is_on() && logger.is_active()){
		logger.log_to_file();
	}
}
