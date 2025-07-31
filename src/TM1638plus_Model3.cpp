/*!
	@file     tm1638plus_model3.cpp
	@author   Gavin Lyons
	@brief    library Header file for TM1638 module(LED & KEY). Model 3.
*/

#include "TM1638plus_Model3.h"

/*!
	@brief Constructor for class TM1638plus
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
	@param highfreq  Changes the value of parameter _HIGH_FREQ which is default false.
*/
TM1638plus_model3::TM1638plus_model3(uint8_t strobe, uint8_t clock, uint8_t data, bool highfreq) : TM1638plus(strobe, clock, data, highfreq) {
 // Blank constructor
}

/*!
	@brief Set ONE LED on or off  Model 1  & 3
	@param position  0-7  == L1-L8 on PCB
	@param  value  0 off 1 on
*/
void TM1638plus_model3::setLED(uint8_t position, uint8_t value)
{
	pinMode(_DATA_IO, OUTPUT);
	sendCommand(TM_WRITE_LOC);
	digitalWrite(_STROBE_IO, LOW);
	sendData(TM_LEDS_ADR + (position << 1));
	sendData(value);
	digitalWrite(_STROBE_IO, HIGH);
}

/*!
	@brief Set all  LED's  on or off  Model 1 & 3
	@param  ledvalues see note behaviour differs depending on Model.
	@note
	1. Model 3
		-# Upper byte for the green LEDs, Lower byte for the red LEDs (0xgreenred) (0xGGRR)
		-# ie. 0xE007   1110 0000 0000 0111   results in L8-L0  GGGX XRRR,  L8 is RHS on display
	2. MODEL 1:
		-# Upper byte 1 for  LED data , Lower byte n/a set to 0x00 (0xleds, 0x00)
		-# i.e 0xF100  1111 0000 L8-L0 RRRR XXXX , L8 is RHS on display
*/
void TM1638plus_model3::setLEDs(uint16_t ledvalues)
{
	for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
		uint8_t colour = 0;

		if ((ledvalues & (1 << LEDposition)) != 0) {
			colour |= TM_RED_LED; //scan lower byte, set Red if one
		} 
		if ((ledvalues & (1 << (LEDposition + 8))) != 0) {
			colour |= TM_GREEN_LED; //scan upper byte, set green if one
		}

		setLED(LEDposition, colour);
	}
}



