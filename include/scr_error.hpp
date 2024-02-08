#include <nav.hpp>
#include <sensors.hpp>
#include <head.hpp>

extern MCUFRIEND_kbv tft;
extern Router sys;
extern LEDHead head;

Screen error{&tft};

namespace Error{

void lbl1_set();
void lbl2_set();
void lbl3_set();
void lbl4_set();
void lbl5_set();

Label lbl1{20, 25, 2, WHITE, lbl1_set};
Label lbl2{50, 25+lbl1.getWid()+5, 2, WHITE, lbl2_set};
Label lbl3{37, 25+2*(lbl1.getWid()+5), 2, WHITE, lbl3_set};
Label lbl4{21, 25+3*(lbl1.getWid()+5), 2, WHITE, lbl4_set};
Label lbl5{64, 25+4*(lbl1.getWid()+5), 2, WHITE, lbl5_set};

void lbl1_set(){ snprintf(lbl1.text, 25, "Temperatura de seguranca"); }
void lbl2_set(){ snprintf(lbl2.text, 25, "excedida. Aguarde a"); }
void lbl3_set(){ snprintf(lbl3.text, 25, "temperatura abaixar e"); }
void lbl4_set(){ snprintf(lbl4.text, 25, "reinicie o equipamento."); }
void lbl5_set(){ snprintf(lbl5.text, 25, "Temp LED: %s C", head.LED_temperature_string); }

void add_widgets(){
	error.add_widget(&lbl1);
	error.add_widget(&lbl2);
	error.add_widget(&lbl3);
	error.add_widget(&lbl4);
	error.add_widget(&lbl5);
}

} // namespace Error
