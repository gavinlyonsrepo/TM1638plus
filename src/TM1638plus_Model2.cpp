/*
* Project Name: TM1638plus 
* File: TM1638plus_Model2.cpp
* Description: Source cpp file for Arduino library for "Model 2" TM1638 module(16 KEY 16 pushbuttons).
* Author: Gavin Lyons.
* Created: August 2019
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include "TM1638plus_Model2.h"
#include "TM1638plus_font.h"

TM1638plus_Model2::TM1638plus_Model2(uint8_t strobe, uint8_t clock, uint8_t data,  bool swap_nibbles) {
  _STROBE_IO = strobe;
  _DATA_IO = data;
  _CLOCK_IO = clock;
  _SWAP_NIBBLES = swap_nibbles;
}

void TM1638plus_Model2::displayBegin(void)
{
  pinMode(_STROBE_IO, OUTPUT);
  pinMode(_DATA_IO, OUTPUT);
  pinMode(_CLOCK_IO, OUTPUT);
  sendCommand(ACTIVATE_TM);
  brightness(DEFAULT_BRIGHTNESS);
  reset();
}

void TM1638plus_Model2::sendCommand(uint8_t value)
{
  digitalWrite(_STROBE_IO, LOW);
  sendData(value);
  digitalWrite(_STROBE_IO, HIGH);
}

void TM1638plus_Model2::sendData(byte data)
{
  shiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, data);
}

void TM1638plus_Model2::reset() {
  sendCommand(WRITE_INC); // set auto increment mode
  digitalWrite(_STROBE_IO, LOW);
  sendData(SEG_ADR);   // set starting address to 0
  for (uint8_t position = 0; position < 16; position++)
  {
	sendData(0x00); //clear all segments
  }
   digitalWrite(_STROBE_IO, HIGH);
}

void TM1638plus_Model2::DisplaySegments(byte segment, byte digit)
{
   if (_SWAP_NIBBLES == true)
   {
   	  uint8_t upper , lower = 0;
   	  lower = (digit) & 0x0F;  // select lower nibble
   	  upper =  (digit >> 4) & 0X0F; //select upper nibble
   	  digit = lower << 4 | upper;
   }

  segment = (segment<<1);
  sendCommand(WRITE_LOC);
  digitalWrite(_STROBE_IO, LOW);
  sendData(SEG_ADR | segment);
  sendData(digit);
  digitalWrite(_STROBE_IO, HIGH);
}

void TM1638plus_Model2::brightness(uint8_t brightness)
{
	uint8_t  value = 0;
	value = BRIGHT_ADR + (BRIGHT_MASK & brightness);
	digitalWrite(_STROBE_IO, LOW); 
	shiftOut(_DATA_IO, _CLOCK_IO, LSBFIRST, value);
	digitalWrite(_STROBE_IO, HIGH); 
}

void TM1638plus_Model2::DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower, byte dots, boolean leadingZeros)
{
  char valuesUpper[DISPLAY_SIZE + 1];
  char valuesLower[DISPLAY_SIZE/2 + 1];
  snprintf(valuesUpper, DISPLAY_SIZE/2 + 1, leadingZeros ? "%04X" : "%X", numberUpper);
  snprintf(valuesLower, DISPLAY_SIZE/2 + 1, leadingZeros ? "%04X" : "%X", numberLower); 
  strcat(valuesUpper ,valuesLower);
  DisplayStr(valuesUpper, dots);
}

void TM1638plus_Model2::DisplayDecNum(unsigned long number, byte dots, boolean leadingZeros)
{
  char values[DISPLAY_SIZE + 1];
  snprintf(values, DISPLAY_SIZE + 1, leadingZeros ? "%08ld" : "%ld", number); 
  DisplayStr(values, dots);
}

void TM1638plus_Model2::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, byte dots, boolean leadingZeros)
{
  char valuesUpper[DISPLAY_SIZE + 1];
  char valuesLower[DISPLAY_SIZE/2 + 1];
  snprintf(valuesUpper, DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : "%d", numberUpper);
  snprintf(valuesLower, DISPLAY_SIZE/2 + 1, leadingZeros ? "%04d" : "%d", numberLower); 
  strcat(valuesUpper ,valuesLower);
  DisplayStr(valuesUpper, dots);
}

void TM1638plus_Model2::DisplayStr(const char* string, const word dots)
{
  byte values[DISPLAY_SIZE];
  boolean done = false;
  uint8_t Result  = 0; 
  memset(values, 0, DISPLAY_SIZE * sizeof(byte));

  for (uint8_t  i = 0; i < DISPLAY_SIZE; i++) 
  {
	   if (!done && string[i] != '\0') {
		 if (dots >> (7-i) & 1){  //if dots bit is set for that position apply the mask to turn on dot(0x80).
			Result = pgm_read_byte(&SevenSeg[string[i] - ASCII_OFFSET]);
			values[i] = (Result | DOT_MASK_DEC); //apply the Dot bitmask to value extracted from ASCII table
			}
		  else 
			values[i] = pgm_read_byte(&SevenSeg[string[i] - ASCII_OFFSET]) ;
		}
	  else {
		done = true;
		values[i] = (((dots >> (7 - i)) & 1) << 7);
		}
	  
  }
 ASCIItoSegment(values);
}

void TM1638plus_Model2::ASCIItoSegment(const byte values[])
{
  for (uint8_t  segment = 0; segment < DISPLAY_SIZE; segment++) {
  uint8_t  SegmentValue = 0;
  for (uint8_t  j = 0; j < DISPLAY_SIZE; j++) {
	SegmentValue |= ((values[j] >> segment) & 1) << (DISPLAY_SIZE - j - 1);
  }
  DisplaySegments(segment , SegmentValue);
  }
}

unsigned char TM1638plus_Model2::ReadKey16()
{
  unsigned char c[4], i, key_value=0;
  digitalWrite(_STROBE_IO, LOW);
  shiftOut(_DATA_IO, _CLOCK_IO, MSBFIRST, BUTTONS_MODE);
  pinMode(_DATA_IO, INPUT);
  for (i = 0; i < 4; i++)
  {
	 c[i] = shiftIn(_DATA_IO, _CLOCK_IO, LSBFIRST);
	 if (c[i] == 0x04) key_value = 1 + (2*i); //00000100 4 0x04
	 if (c[i] == 0x40) key_value = 2 + (2*i); //01000000 64 0x40
	 if (c[i] == 0x02) key_value = 9 + (2*i); //00000010 2 0x02
	 if (c[i] == 0x20) key_value = 10 + (2*i);  //00100000 32 0x20
  }
  pinMode(_DATA_IO, OUTPUT);
  digitalWrite(_STROBE_IO, HIGH); 
  return (key_value);
  // Data matrix for read key_value. 
  // c3 0110 0110  c2 0110 0110  c1 0110 0110  c0 0110 0110 :bytes read 
  //    8,16 7,15     6,14 5,13     4,12 3,11     2,10  1,9 :button value
}
