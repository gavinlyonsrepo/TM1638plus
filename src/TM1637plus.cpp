/*!
	@file     TM1637plus.cpp
	@author   Gavin Lyons
	@brief    Soucre file for for TM1637 module. Model 4 & 5
*/

#include "TM1637plus.h"

/*!
	@brief Initialize a TM1637 object
	@param clock GPIO connected to the clock pin of the module
	@param data GPIO connected to the DIO pin of the module
	@param delay microseconds delay, between bit transition on the serial
			bus connected to the display
	@param displaySize number of digits in display.
*/
TM1637plus_modelX::TM1637plus_modelX(uint8_t clock, uint8_t data, int delay, int displaySize)
{
	_DATA_IO = data;
	_CLOCK_IO = clock;
	_BitDelayUS = delay;
	_DisplaySize = displaySize;
}

/*! 
	@brief Clears the display
*/
void  TM1637plus_modelX::displayClear()
{
	uint8_t data[_DisplaySize] = {0};
	setSegments(data, _DisplaySize, 0);
}
/*!
	@brief Begin method , set and claims GPIO
	@note Call in Setup
*/
void TM1637plus_modelX::displayBegin(void)
{
	pinMode(_DATA_IO, INPUT);
	pinMode(_CLOCK_IO , INPUT);
}

/*!
	@brief Sets the brightness of the display.
	@param brightness A number from 0 to 7 (highest brightness)
	@param on Turn display on or off
	@note The setting takes effect when a command is given to change the data being
	displayed.
*/
void TM1637plus_modelX::setBrightness(uint8_t brightness, bool on)
{
	_brightness = (brightness & 0x7) | (on? 0x08 : 0x00);
}

/*!
	@brief Display data on the module
	@details This function receives segment values as input and displays them. The segment data
	is given as a byte array, each byte corresponding to a single digit. Within each byte,
	bit 0 is segment A, bit 1 is segment B etc.
	The function may either set the entire display or any desirable part on its own. The first
	digit is given by the reference position argument with 0 being the leftmost digit. The reference length
	argument is the number of digits to be set. Other digits are not affected.
	@param segments An array of size length containing the raw segment values, check font in seven_segment_font_data.hpp
	@param length The number of digits to be modified
	@param position The position from which to start the modification (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_modelX::setSegments(const uint8_t segments[], uint8_t length, uint8_t position)
{
	// Write Command 1
	CommStart();
	writeByte(_TM1637_COMMAND_1);
	CommStop();

	// Write Command 2 + first digit address
	CommStart();
	writeByte(_TM1637_COMMAND_2 + (position & 0x03));
	// Write the data
	for (uint8_t i=0; i < length; i++)
		writeByte(segments[i]);
	CommStop();

	// Write Command 3 + brightness
	CommStart();
	writeByte(_TM1637_COMMAND_3 + (_brightness & 0x0F));
	CommStop();
}


/*!
	@brief Displays a decimal number
	@details Displays the given argument as a decimal number
	@param number The number to be shown
	@param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are blank
	@param length The number of digits to set. The user must ensure that the number to be shown
			fits to the number of digits requested (for example, if two digits are to be displayed,
			the number must be between 0 to 99)
	@param position The position most significant digit (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_modelX::DisplayDecimal(int number,  bool leading_zero ,uint8_t length, uint8_t position)
{
	DisplayDecimalwDot(number, 0, leading_zero, length, position);
}

/*!
	@brief Displays a decimal number, with decimal point control 
	@details Displays the given argument as a decimal number. The dots between the digits (or colon)
	can be individually controlled
	@param number The number to be shown
	@param dots decimal point enable. The argument is a bitmask, with each bit corresponding to a dot
		  between the digits (or colon mark, as implemented by each module). The MSB is the 
		  leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
		  will correspond the dot near digit no. 2 from the left. Dots are updated only on
		  those digits actually being update (that is, no more than len digits)
		  For model 4 semi-colon display set to 0x40 to turn on the colon.
	@param leading_zero When true, leading zeros are displayed. Otherwise unnecessary digits are
		  blank
	@param length The number of digits to set. The user must ensure that the number to be shown
		  fits to the number of digits requested (for example, if two digits are to be displayed,
		  the number must be between 0 to 99)
	@param position The position least significant digit (0 - leftmost, 3 - rightmost)
*/
void TM1637plus_modelX::DisplayDecimalwDot(int number, uint8_t dots,  bool leading_zero ,uint8_t length, uint8_t position)
{
	// Array to store the encoded digits for the display
	uint8_t digits[_DisplaySize];
	// Array of divisors used to extract each digit from the number
	const static int divisors[] = { 1, 10, 100, 1000, 10000, 100000 };
	bool leading = true;

	// Loop through each digit position
	for (int8_t i = 0; i < _DisplaySize; i++) 
	{
		// Determine the divisor for the current digit position
		int divisor = divisors[_DisplaySize - 1 - i];
		// Extract the current digit from the number
		int character = number / divisor;
		uint8_t digit = 0;
		// Handle leading zeros and actual digits
		if (character == 0) 
		{
			// If leading zero is enabled or we're no longer in leading zeros
			if (leading_zero || !leading || (i == (_DisplaySize-1))) 
				digit = encodeCharacter('0' + character);
			else
				digit = 0;                // Leave the segment blank
		} else 
		{
			digit = encodeCharacter('0' + character);
			number -= character * divisor;   // Remove the processed digit from the number
			leading = false;                 // Leading zeros end as soon as a non-zero digit is found
		}
		digit |= (dots & DEC_POINT_7_MASK);
		dots <<= 1;
		digits[i] = digit;
	}
	// Set the segments on the display starting at the specified position
	// Use only the relevant portion of the `digits` array based on `length`
	setSegments(digits + (_DisplaySize - length), length, position);
}

/*!
	@brief Displays a character array
	@param numStr The character array to be shown
	@param dots decimal point enable. The argument is a bitmask, with each bit corresponding to a dot
		  between the digits (or colon mark, as implemented by each module). The MSB is the 
		  leftmost dot of the digit being update. For example, if pos is 1, the MSB of dots
		  will correspond the dot near digit no. 2 from the left. Dots are updated only on
		  those digits actually being update (that is, no more than len digits)
		  For model 4 semi-colon display set to 0x40 to turn on the colon.
	@param length The number of digits to set. The user must ensure that the number to be shown
		  fits to the number of digits requested
	@param position The position most significant digit (0 - leftmost, 3 - rightmost)
	@return Zero for success , -2 for nullptr, -3 input string size not equal to specified length.
*/
int TM1637plus_modelX::DisplayString(const char* numStr, uint8_t dots, uint8_t length, uint8_t position)
{
	if (numStr == nullptr) 
	{
		if (displaylib_LED_debug) 
			Serial.println("Error: DisplayString 1: String is null.");
		return -2;
	}
	if (strlen(numStr) != length) 
	{
		if (displaylib_LED_debug) 
			Serial.println("Error: DisplayString 2: Text array length is not equal to specifed length parameter");
		return -3;
	}
	
	uint8_t digits[_DisplaySize] = {0};
	uint8_t digit = 0;
	
	for (int8_t i = 0; i < length; i++) 
	{
		char currentChar = numStr[i];
		digit = encodeCharacter(currentChar);
		// Add the decimal point/colon to the digit if specified in `dots`
		digit |= (dots & DEC_POINT_7_MASK); // Check the MSB of `dots` and add it to the current digit
		dots <<= 1;             // Shift the `dots` value to apply the next dot/colon to the next digit
		digits[i] = digit;
	}
	setSegments(digits, length, position);
	return 0;
}


/*!
	@brief Translate a single Character into 7 segment code
	@details The method accepts a ASCII character and converts it to the
		seven segment code required to display the number on a 7 segment display.
		by referencing the ASCII font table on file.
	@param digit An ASCII character
	@return A code representing the 7 segment image of the digit (LSB - segment A;
			bit 6 - segment G; bit 7 - always zero) from the font.
			Will return zero(0x3F) if ASCII digit not in font.
*/
unsigned char TM1637plus_modelX::encodeCharacter(unsigned char digit)
{
	if (digit < _ASCII_FONT_OFFSET || digit >= _ASCII_FONT_END )
	{
		if (displaylib_LED_debug) 
			Serial.println("Warning : encodeCharacter : ASCII character is outside font range");
		return 0x3F;
	} // check ASCII font bounds
	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	unsigned char ascii = font[digit- _ASCII_FONT_OFFSET];
	return ascii;
}

/*!
	@brief Close method 
*/
void TM1637plus_modelX::displayClose(void)
{
	pinMode(_DATA_IO, INPUT);
	pinMode(_CLOCK_IO , INPUT);
}


/*!
	@brief Sets the delay, in microseconds, between bit transition on the serial
		bus connected to the display
*/
void TM1637plus_modelX::CommBitDelay(void)
{
	delayMicroseconds(_BitDelayUS);
}

/*! 
	@brief Starts the communication sequence 
*/
void TM1637plus_modelX::CommStart(void)
{
	pinMode(_DATA_IO, OUTPUT);
	CommBitDelay();
}

/*! 
	@brief Stops the communication sequence
*/
void TM1637plus_modelX::CommStop(void)
{
	
	pinMode(_DATA_IO, OUTPUT);
	CommBitDelay();
	pinMode(_CLOCK_IO, INPUT);
	CommBitDelay();
	pinMode(_DATA_IO, INPUT);
	CommBitDelay();
}

/*! 
	@brief Writes a byte to the Display
	@param byte the Byte to write
	@return status of acknowledge bit
*/
bool TM1637plus_modelX::writeByte(uint8_t byte)
{
	uint8_t data = byte;

	// 8 Data Bits
	for(uint8_t i = 0; i < 8; i++) 
	{
		// Set clock low
		pinMode(_CLOCK_IO, OUTPUT);
		CommBitDelay();
		// Set data bit
		if (data & 0x01)
		{
			pinMode(_DATA_IO, INPUT);
		}else{
			pinMode(_DATA_IO, OUTPUT);
		}
		CommBitDelay();
		// Set Clock high
		pinMode(_CLOCK_IO, INPUT);
		CommBitDelay();
		data = data >> 1;
	}

	// Wait for acknowledge
	// Set Clock to Low
	pinMode(_CLOCK_IO, OUTPUT);
	pinMode(_DATA_IO, INPUT);
	CommBitDelay();
	// Set clock to high
	pinMode(_CLOCK_IO, INPUT);
	CommBitDelay();
	// uint8_t acknowledge = gpio_get(_DATA_IO);
	uint8_t acknowledge = shiftIn(_DATA_IO, _CLOCK_IO, LSBFIRST);
	if (acknowledge == 0)
	{
		pinMode(_DATA_IO, OUTPUT);
	}
	CommBitDelay();
	pinMode(_CLOCK_IO, OUTPUT);
	CommBitDelay();
	return acknowledge;
}
