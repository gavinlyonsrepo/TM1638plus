/*!
	@file     TM1638plus.cpp
	@author   Gavin Lyons
	@brief    Arduino library Tm1638plus, Source file for TM1638 module(LED & KEY). Model 1 & Model 3.
*/

#include "TM1638plus.h"

/*!
	@brief Constructor for class TM1638plus
	@param strobe  GPIO STB pin
	@param clock  GPIO CLK pin
	@param data  GPIO DIO pin
	@param highfreq  Changes the value of parameter _HIGH_FREQ which is default false.
	@note _HIGH_FREQ is used when running high freq MCU CPU (~>100Mhz) because of issues with button function. Pass true to turn on.
*/
TM1638plus::TM1638plus(uint8_t strobe, uint8_t clock, uint8_t data, bool highfreq) {
	_STROBE_IO = strobe;
	_DATA_IO = data;
	_CLOCK_IO = clock;
	_HIGH_FREQ = highfreq;
}


/*!
	@brief Set ONE LED on or off  Model 1  & 3
	@param position  0-7  == L1-L8 on PCB
	@param  value  0 off 1 on
*/
void TM1638plus::setLED(uint8_t position, uint8_t value)
{
	pinMode(_DATA_IO, OUTPUT);
	sendCommand(TM_WRITE_LOC);
	digitalWrite(_STROBE_IO, LOW);
	sendData(TM_LEDS_ADR + (position << 1));
	sendData(value);
	digitalWrite(_STROBE_IO, HIGH);
}


/*!
	@brief Set all LED's  on or off  Model 1 & 3
	@param ledvalues 1 on , 0 off , 0xXXLL where LL = L8-L1
	@note MODEL 1:
		-# Upper byte ignored this byte/method is used by sub-class Model 3 
		-# Lower byte LED data model 1
		-# setLEDs(0xF0) Displays as XXXX LLLL (L1-L8),NOTE on display L8 is on right hand side.
*/
void TM1638plus::setLEDs(uint16_t ledvalues)
{
	uint8_t colour = 0;
	ledvalues = ledvalues & 0x00FF;
	for (uint8_t LEDposition = 0;  LEDposition < 8; LEDposition++) {
		if ((ledvalues & (1 << LEDposition)) != 0) {
			colour |= 0x01; 
		} 
		setLED(LEDposition, colour);
		colour = 0;
	}
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32
	@param leadingZeros  leading zeros set, true on , false off
	@param TextAlignment  left or right text alignment on display
*/
void TM1638plus::displayIntNum(unsigned long number, boolean leadingZeros, AlignTextType_e TextAlignment)
{
	char values[TM_DISPLAY_SIZE + 1];
	char TextDisplay[5] = "%";
	char TextLeft[3] = "ld";
	char TextRight[4] = "8ld";

	if (TextAlignment == TMAlignTextLeft)
		{
			strcat(TextDisplay ,TextLeft);  // %ld
		}else if ( TextAlignment == TMAlignTextRight)
		{
			strcat(TextDisplay ,TextRight); // %8ld
		}

	snprintf(values, TM_DISPLAY_SIZE + 1, leadingZeros ? "%08ld" : TextDisplay, number);
	displayText(values);
}

/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param leadingZeros  leading zeros set, true on , false off
	@param TextAlignment  left or right text alignment on display
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void TM1638plus::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, boolean leadingZeros, AlignTextType_e TextAlignment )
{
	char valuesUpper[TM_DISPLAY_SIZE + 1];
	char valuesLower[TM_DISPLAY_SIZE/2 + 1];
	char TextDisplay[5] = "%";
	char TextLeft[4] = "-4d";
	char TextRight[3] = "4d";

	 if (TextAlignment == TMAlignTextLeft)
	{
			strcat(TextDisplay ,TextLeft);  // %-4d
	}else if ( TextAlignment == TMAlignTextRight)
	{
			strcat(TextDisplay ,TextRight); // %4d
	}

	snprintf(valuesUpper, TM_DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : TextDisplay, numberUpper);
	snprintf(valuesLower, TM_DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : TextDisplay, numberLower);

	 strcat(valuesUpper ,valuesLower);
	 displayText(valuesUpper);
}

/*!
	@brief Display a text string  on display
	@param text    pointer to a character array
	@note 
		Dots are removed from string and dot on preceding digit switched on
		"abc.def" will be shown as "abcdef" with c decimal point turned on.
*/
void TM1638plus::displayText(const char *text) {
	char c, pos;
	pos = 0;
		while ((c = (*text++)) && pos < TM_DISPLAY_SIZE)  {
		if (*text == '.' && c != '.') {
			displayASCII(pos++, c, DecPointOn);
			text++;
		}  else {
			displayASCII(pos++, c, DecPointOff);
		}
		}
}

/*!
	@brief Display an ASCII character on display
	@param position The position on display 0-7  
	@param ascii The ASCII value from font table  to display 
	@param decimalPoint decimal point or off on the digit.
*/
void TM1638plus::displayASCII(uint8_t position, uint8_t ascii, DecimalPoint_e decimalPoint) {
	if (ascii < _ASCII_FONT_OFFSET || ascii >= _ASCII_FONT_END )
	{
		if (displaylib_LED_debug) 
			Serial.println("Warning : displayASCII : ASCII character is outside font range");
		ascii = '0';
	} // check ASCII font bounds
	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	switch (decimalPoint)
	{
		case DecPointOff: 
			display7Seg(position, font[ascii - _ASCII_FONT_OFFSET]); 
			break;
		case DecPointOn: // add 128 or 0x080 0b1000000 to turn on decimal point/dot in seven seg
			display7Seg(position, font[ascii- _ASCII_FONT_OFFSET] + DEC_POINT_7_MASK); 
			break;
	}
}

/*!
	@brief  Send seven segment value to seven segment
	@param position The position on display 0-7  
	@param value  byte of data corresponding to segments (dp)gfedcba 
	@note 	0b01000001 in value will set g and a on.
*/
void TM1638plus::display7Seg(uint8_t position, uint8_t value) { 
	sendCommand(TM_WRITE_LOC);
	digitalWrite(_STROBE_IO, LOW);
	sendData(TM_SEG_ADR + (position << 1));
	sendData(value);
	digitalWrite(_STROBE_IO, HIGH);
}

 /*!
	@brief  Send Hexadecimal value to seven segment
	@param position The position on display 0-7  
	@param hex  hexadecimal  value (DEC) 0-15  (0x00 - 0x0F)
*/
void TM1638plus::displayHex(uint8_t position, uint8_t hex)
{
	uint8_t offset = 0;
	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	hex = hex % 16;
	if (hex <= 9)
	{
		display7Seg(position, font[hex + _ASCII_FONT_HEX_OFFSET]);
		// 16 is offset in reduced ASCII table for number 0
	}else if ((hex >= 10) && (hex <=15))
	{
		// Calculate offset in reduced ASCII table for AbCDeF
		switch(hex)
		{
		 case 10: offset = 'A'; break;
		 case 11: offset = 'b'; break;
		 case 12: offset = 'C'; break;
		 case 13: offset = 'd'; break;
		 case 14: offset = 'E'; break;
		 case 15: offset = 'F'; break;
		}
		display7Seg(position, font[offset-_ASCII_FONT_OFFSET]);
	}
}

 /*!
	@brief  Read buttons values from display
	@return byte with value of buttons 1-8 b7b6b5b4b3b2b1b0 1 pressed, 0 not pressed.
	@note User may have to debounce buttons depending on application.
*/
uint8_t TM1638plus::readButtons()
{
	uint8_t buttons = 0;
	uint8_t v =0;

	digitalWrite(_STROBE_IO, LOW);
	sendData(TM_BUTTONS_MODE);
	pinMode(_DATA_IO, INPUT);

	for (uint8_t i = 0; i < 4; i++)
	{

		if  (_HIGH_FREQ == false)
			v = shiftIn(_DATA_IO, _CLOCK_IO, LSBFIRST) << i;
		else
			v = HighFreqshiftin(_DATA_IO, _CLOCK_IO, LSBFIRST) << i;

		buttons |= v;
	}

	pinMode(_DATA_IO, OUTPUT);
	digitalWrite(_STROBE_IO, HIGH);
	return buttons;
}
