/*!
	@file     TM1638plus_common.cpp
	@author   Gavin Lyons
	@brief    Arduino library Tm1638plus, Source file for common data and functions between model classes.
*/

#include "TM1638plus_common.h"


/*!
	@brief Begin method , sets pin modes and activate display.
	@note Call in Setup() loop
*/
void TM1638plus_common::displayBegin() {
	pinMode(_STROBE_IO , OUTPUT);
	pinMode(_DATA_IO, OUTPUT);
	pinMode(_CLOCK_IO , OUTPUT);
	sendCommand(TM_ACTIVATE);
	brightness(TM_DEFAULT_BRIGHTNESS);
	reset();
}

/*!
	@brief Send command to display
	@param value command byte to send
*/
void TM1638plus_common::sendCommand(uint8_t value)
{
	digitalWrite(_STROBE_IO, LOW);
	sendData(value);
	digitalWrite(_STROBE_IO, HIGH);
}

/*!
	@brief Send Data to display
	@param data  Data byte to send
*/
void TM1638plus_common::sendData(uint8_t data)
{
	if (_HIGH_FREQ == false)
		shiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, data);
	else
		HighFreqshiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, data);
}

/*!
	@brief Reset / clear  the  display
	@note The display is cleared by writing zero to all data segment  addresses.
*/
void TM1638plus_common::reset() {
	sendCommand(TM_WRITE_INC); // set auto increment mode
	digitalWrite(_STROBE_IO, LOW);
	sendData(TM_SEG_ADR);  // set starting address to
	for (uint8_t position = 0; position < 16; position++)
	{
		sendData(0x00);
	}
	 digitalWrite(_STROBE_IO, HIGH);
}

  /*!
	@brief  Sets the brightness level of segments in display on a scale of brightness
	@param brightness byte with value 0 to 7 The DEFAULT_BRIGHTNESS = 0x02
*/
void  TM1638plus_common::brightness(uint8_t brightness)
{
	uint8_t  value = 0;
	value = TM_BRIGHT_ADR + (TM_BRIGHT_MASK & brightness);
	sendCommand(value);
}

/*!
	@brief    Shifts in a byte of data from the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@param bitOrder Data byte bit order LSBFIRST OR MSBFIRST, Set to LSBFIRST for Tm1638
	@return  Data byte
	@note Used instead of arduino function "shiftin" when _HIGH_FREQ is set to true, for high frequency CPU's
*/
uint8_t  TM1638plus_common::HighFreqshiftin(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
	uint8_t value = 0;
	uint8_t i = 0;

	for(i = 0; i < 8; ++i) {

		digitalWrite(clockPin, HIGH);
		delayMicroseconds(_HFIN_DELAY);

		if(bitOrder == LSBFIRST)
			value |= digitalRead(dataPin) << i;
		else
			value |= digitalRead(dataPin) << (7 - i);

		digitalWrite(clockPin, LOW);
		delayMicroseconds(_HFIN_DELAY);
	}
	return value;
}

 /*!
	@brief    Shifts out a byte of data on to the Tm1638 SPI-like bus
	@param dataPin Tm1638 Data GPIO
	@param clockPin Tm1638 Clock GPIO
	@param bitOrder Data byte bit order LSBFIRST OR MSBFIRST, Set to LSBFIRST for Tm1638
	@param val The byte of data to shift out 
	@note Used instead of arduino function "shiftOut" when _HIGH_FREQ is set to true, for high frequency CPU's
*/
void TM1638plus_common::HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val)
{
	uint8_t i;

	for (i = 0; i < 8; i++)  {
		if (bitOrder == LSBFIRST)
			digitalWrite(dataPin, !!(val & (1 << i)));
		else
			digitalWrite(dataPin, !!(val & (1 << (7 - i))));

		digitalWrite(clockPin, HIGH);
		delayMicroseconds(_HFOUT_DELAY);
		digitalWrite(clockPin, LOW);
		delayMicroseconds(_HFOUT_DELAY);
	}
}
