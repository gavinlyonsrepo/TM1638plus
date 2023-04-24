/*!
	@file     TM1638plus_Model2.h
	@author   Gavin Lyons
	@brief Arduino library Tm1638plus, Header file for TM1638 module(16 KEY 16 pushbuttons. Model 2.
*/

#ifndef TM1638PLUS_MODEL2_H
#define TM1638PLUS_MODEL2_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include "TM1638plus_common.h"

/*!
	@brief Class for Model 2
*/
class TM1638plus_Model2 : public TM1638plus_common {

public:

	TM1638plus_Model2(uint8_t strobe, uint8_t clock, uint8_t data, bool swap_nibbles= false,bool high_freq = false);

	// Methods
	uint8_t ReadKey16(void);
	uint16_t ReadKey16Two(void);

	void DisplaySegments(uint8_t segment, uint8_t segmentValue);
	void DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower, uint8_t dots, boolean leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	void DisplayDecNum(unsigned long number, uint8_t dots, boolean leadingZeros = true, AlignTextType_e = TMAlignTextLeft);
	void DisplayStr(const char* string, const uint16_t  dots = 0);
	void ASCIItoSegment(const uint8_t values[]);
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, uint8_t dots, boolean leadingZeros = true, AlignTextType_e = TMAlignTextLeft);

private:

	bool _SWAP_NIBBLES= false;

};

#endif
