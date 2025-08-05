/*!
	@file   MAX7219plus.cpp
	@author Gavin Lyons
	@brief  library source file to drive MAX7219 displays
*/

#include "MAX7219plus.h"


// Public methods

/*!
	@brief Constructor for class MAX7219plus_Model6 software SPI
	@param clock CLk pin
	@param chipSelect CS pin
	@param data DIO pin
	@param CommDelay Software SPI communications delay in uS
	@param totalDisplays Total number of displays in cascade
	@note overloaded this one is for Software SPI
*/
MAX7219plus_Model6::MAX7219plus_Model6(uint8_t chipSelect, uint8_t clock, uint8_t data, uint16_t CommDelay, uint8_t totalDisplays)
{
	_Display_CS  = chipSelect;
	_Display_SCLK = clock;
	_Display_SDATA = data;
	_CommDelay = CommDelay;
	_NoDisplays = totalDisplays;
	_HardwareSPI = false;
}

/*!
	@brief Constructor for class MAX7219plus_Model6 hardware SPI
	@param chipSelect CS pin
	@param totalDisplays Total number of displays in cascade
	@note overloaded this one is for Hardware SPI 
*/
MAX7219plus_Model6::MAX7219plus_Model6(uint8_t chipSelect, uint8_t totalDisplays)
{
	_Display_CS  = chipSelect;
	_NoDisplays = totalDisplays;
	_HardwareSPI = true;
}


/*!
	@brief get value of _HardwareSPI , true hardware SPI on , false off.
	@return _HardwareSPI , true hardware SPI on , false off.
*/
bool MAX7219plus_Model6::GetHardwareSPI(void)
{return _HardwareSPI;}


/*!
	@brief Init the display
	@param numDigits scan limit set to 8 normally , advanced use only
	@param decodeMode Must users will use 0x00 here
	@note when cascading supplies init display one first always!
*/
void MAX7219plus_Model6::InitDisplay(ScanLimit_e numDigits, DecodeMode_e decodeMode)
{
	if (_CurrentDisplayNumber == 1)
	{

		if (_HardwareSPI == false)
		{
			// Set software SPI specific pin outputs.
			pinMode(_Display_SCLK, OUTPUT);
			pinMode(_Display_SDATA, OUTPUT);
			pinMode(_Display_CS, OUTPUT);
			digitalWrite(_Display_CS, HIGH);
		}else
		{
			pinMode(_Display_CS, OUTPUT);
			digitalWrite(_Display_CS, HIGH);
			SPI.begin();
		}
	}
	delay(50); // small init delay before commencing transmissions
	_NoDigits = numDigits+1;
	CurrentDecodeMode = decodeMode;
	SetScanLimit(numDigits);
	SetDecodeMode(decodeMode);
	ShutdownMode(false);
	DisplayTestMode(false);
	ClearDisplay();
	SetBrightness(IntensityDefault);
}

/*!
	@brief Clear the display
*/
void MAX7219plus_Model6::ClearDisplay(void)
{
	switch(CurrentDecodeMode)
	{
	case DecodeModeNone: // Writes zero to blank display
		for(uint8_t digit = 0; digit<_NoDigits ; digit++)
		{
			CommStart();
			WriteDisplay(digit+1, 0x00);
			CommEnd();
		}
	break;
	case DecodeModeBCDOne:  // Mode BCD on digit 0 , rest of display write Zero
		DisplayBCDChar(0, CodeBFontSpace);
		for(uint8_t digit=1; digit<_NoDigits ; digit++)
		{
			CommStart();
			WriteDisplay(digit+1, 0x00);
			CommEnd();
		}
	break;
	case DecodeModeBCDTwo: // Mode BCD on digit 0-3 , rest of display write  Zero
		for(uint8_t digitBCD = 0; digitBCD<_NoDigits-4 ; digitBCD++)
		{
			CommStart();
			DisplayBCDChar(digitBCD, CodeBFontSpace);
			CommEnd();
		}
		for(uint8_t digit=4; digit<_NoDigits ; digit++)
		{
			CommStart();
			WriteDisplay(digit+1, 0x00);
			CommEnd();
		}
	break;
	case DecodeModeBCDThree: // BCD digit 7-0
		for(uint8_t digit=0; digit<_NoDigits ; digit++)
		{
			CommStart();
			DisplayBCDChar(digit, CodeBFontSpace);
			CommEnd();
		}
	break;
	} // end of switch

	
}

/*!
	@brief Displays a character on display using MAX7219 Built in BCD code B font
	@param digit The digit to display character in, 7-0 ,7 = LHS 0 =RHS
	@param value  The BCD character to display
	@note sets BCD code B font (0-9, E, H, L,P, and -) Built-in font
*/
void MAX7219plus_Model6::DisplayBCDChar(uint8_t digit, CodeBFont_e value)
{
	CommStart();
	WriteDisplay(digit+1, value);
	CommEnd();
}

/*!
	@brief Displays a character on display
	@param digit The digit to display character in, 7-0 ,7 = LHS 0 =RHS
	@param character  The ASCII character to display
	@param decimalPoint Is the decimal point(dp) to be set or not.
*/
void MAX7219plus_Model6::DisplayChar(uint8_t digit, uint8_t character , DecimalPoint_e decimalPoint)
{
	CommStart();
	WriteDisplay(digit+1,ASCIIFetch(character , decimalPoint));
	CommEnd();
}

/*!
	@brief Set a seven segment LED ON
	@param digit The digit to set segment in, 7-0 ,7 = LHS 0 =RHS
	@param segment The segment of seven segment to set dpabcdefg
*/
void MAX7219plus_Model6::SetSegment(uint8_t digit, uint8_t segment)
{
	CommStart();
	WriteDisplay(digit+1, segment);
	CommEnd();
}

/*!
	@brief Displays a text string on display
	@param text pointer to character array containg text string
	@param TextAlignment left or right alignment
	@details AlignRightZeros option for Text alignment not supported in this function.
	@note This method is overloaded, see also DisplayText(char *)
	@return error -2 if string is null. -3 if option AlignRightZeros entered , 0 for success
*/
int MAX7219plus_Model6::DisplayText(char* text, TextAlignment_e TextAlignment){

	if (text == nullptr) 
	{
		if(displaylib_LED_debug)
			Serial.println("Error: DisplayText 1: String is null.");
		return -2;
	}
	char character;
	char pos =0;

	// We need the length of the string - no of decimal points set
	uint8_t LengthOfStr;
	LengthOfStr=strlen(text);
	for(uint8_t index =0; text[index]; index++)
	{
		if(text[index] == '.') LengthOfStr--; // decrement string for dp's
	}
	if (LengthOfStr > (_NoDigits)) LengthOfStr = (_NoDigits);

	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			switch (TextAlignment) // Display a character with dp set
			{
				case AlignLeft  : DisplayChar((_NoDigits-1)- pos ,character, DecPointOn); break;
				case AlignRight : DisplayChar((LengthOfStr-1)- pos ,character, DecPointOn); break;
				case AlignRightZeros: return -3; break;
			}
			pos++;
			text++;
		}  else
		{
			switch (TextAlignment) // Display a character without dp set
			{
				case AlignLeft  : DisplayChar((_NoDigits-1) -pos, character, DecPointOff); break;
				case AlignRight : DisplayChar((LengthOfStr-1) - pos, character, DecPointOff); break;
				case AlignRightZeros : return -3; break;
			}
			pos++;
		}
	}
	return 0;
}


/*!
	@brief Displays a text string on display
	@param text  pointer to character array containing text string
	@note This method is overloaded, see also DisplayText(char *, TextAlignment_e )
	@return error -2 if string is null , 0 for success
*/
int MAX7219plus_Model6::DisplayText(char* text){

	if (text == nullptr) 
	{
		if(displaylib_LED_debug)
			Serial.println("Error: DisplayText 2: String is null.");
		return -2;
	}
	char character;
	char pos = _NoDigits-1;

	while ((character = (*text++)) && pos < _NoDigits)
	{
		if (*text == '.' && character != '.')
		{
			DisplayChar(pos  ,character, DecPointOn);
			pos--;
			text++;
		}  else
		{
			DisplayChar(pos  ,character, DecPointOff);
			pos--;
		}
	}
	return 0;
}

/*!
	@brief Displays a BCD text string on display using MAX7219 Built in BCD code B font
	@param text  pointer to character array containing text string
	@note sets BCD code B font (0-9, E, H, L,P, and -) Built-in font
		  Non supported characters printed as space ' '
	@return error -2 if string is null , 0 for success
*/
int MAX7219plus_Model6::DisplayBCDText(char* text){

	if (text == nullptr) 
	{
		if(displaylib_LED_debug)
			Serial.println("Error: DisplayBCDText  1: String is null.");
		return -2;
	}
	char character;
	char pos =_NoDigits-1;

	while ((character = (*text++)) )
	{
		switch (character)
		{
			case '0' : DisplayBCDChar(pos,CodeBFontZero);  break;
			case '1' : DisplayBCDChar(pos,CodeBFontOne);   break;
			case '2' : DisplayBCDChar(pos,CodeBFontTwo);   break;
			case '3' : DisplayBCDChar(pos,CodeBFontThree); break;
			case '4' : DisplayBCDChar(pos,CodeBFontFour);  break;
			case '5' : DisplayBCDChar(pos,CodeBFontFive);  break;
			case '6' : DisplayBCDChar(pos,CodeBFontSix);   break;
			case '7' : DisplayBCDChar(pos,CodeBFontSeven); break;
			case '8' : DisplayBCDChar(pos,CodeBFontEight); break;
			case '9' : DisplayBCDChar(pos,CodeBFontNine);  break;
			case '-' : DisplayBCDChar(pos,CodeBFontDash);  break;
			case 'E' :
			case 'e' :
				DisplayBCDChar(pos,CodeBFontE);
			break;
			case 'H' :
			case 'h' :
				DisplayBCDChar(pos,CodeBFontH);
			break;
			case 'L' :
			case 'l' :
				DisplayBCDChar(pos,CodeBFontL);
			break;
			case 'P' :
			case 'p' :
				DisplayBCDChar(pos,CodeBFontP);
			break;
			case ' ' : DisplayBCDChar(pos,CodeBFontSpace); break;
			default  : DisplayBCDChar(pos,CodeBFontSpace); break;
		}
	pos--;
	}
	return 0;
}

/*!
	@brief sets the brightness of display
	@param brightness rang 0x00 to 0x0F , 0x00 being least bright.
*/
void MAX7219plus_Model6::SetBrightness(uint8_t brightness)
{
	brightness &= IntensityMax;
	CommStart();
	WriteDisplay(MAX7219_REG_Intensity, brightness);
	CommEnd();
}


/*!
	@brief Turn on and off the Shutdown Mode
	@param OnOff true = Shutdown mode on , false shutdown mode off
	@note power saving mode
*/
void MAX7219plus_Model6::ShutdownMode(bool OnOff)
{
	CommStart();
	OnOff ? WriteDisplay(MAX7219_REG_ShutDown, 0) : WriteDisplay(MAX7219_REG_ShutDown, 1);
	CommEnd();
}
/*!
	@brief Turn on and off the Display Test Mode
	@param OnOff true = display test mode on , false display Test Mode off
	@note Display-test mode turns all LEDs on
*/
void MAX7219plus_Model6:: DisplayTestMode(bool OnOff)
{
	CommStart();
	OnOff ? WriteDisplay(MAX7219_REG_DisplayTest, 1) : WriteDisplay(MAX7219_REG_DisplayTest, 0);
	CommEnd();
}


/*!
	@brief Set the communication delay value
	@param commDelay Set the communication delay value uS software SPI
*/
void MAX7219plus_Model6::SetCommDelay(uint16_t commDelay) {_CommDelay = commDelay;}

/*!
	@brief Get the communication delay value
	@return Get the communication delay value uS Software SPi
*/
uint16_t  MAX7219plus_Model6::GetCommDelay(void) {return _CommDelay;}

/*!
	@brief Get the Current Display Number
	@return Get the Current Display Number
*/
uint8_t MAX7219plus_Model6::GetCurrentDisplayNumber(void){return _CurrentDisplayNumber; }

/*!
	@brief Set the Current Display Number
	@param DisplayNum Set the Current Display Number
*/
void MAX7219plus_Model6::SetCurrentDisplayNumber(uint8_t DisplayNum )
{
if (DisplayNum == 0 ) DisplayNum = 1; // Zero user error check

_CurrentDisplayNumber  = DisplayNum  ;
}

/*!
	@brief Display an integer and leading zeros optional
	@param number  integer to display 2^32
	@param TextAlignment enum text alignment, left or right alignment or leading zeros
*/
void  MAX7219plus_Model6::DisplayIntNum(unsigned long number, TextAlignment_e TextAlignment)
{
	char values[_NoDigits+1];
	char TextDisplay[6] = "%";
	char TextRight[4] = "8ld";
	char TextLeft[3] = "ld";
	char TextLeadZero[5] = "08ld";

	switch(TextAlignment)
	{
		case AlignRight:
			strcat(TextDisplay ,TextRight); // %8ld
		break;
		case AlignLeft:
			strcat(TextDisplay ,TextLeft);  // %ld
		break;
		case AlignRightZeros:
			strcat(TextDisplay ,TextLeadZero);  // %08ld
		break;
	}
	snprintf(values, _NoDigits+1, TextDisplay, number);
	DisplayText(values);
}


/*!
	@brief Display an integer in a nibble (4 digits on display)
	@param numberUpper   upper nibble integer 2^16
	@param numberLower   lower nibble integer 2^16
	@param TextAlignment  left or right alignment or leading zeros
	@note
		Divides the display into two nibbles and displays a Decimal number in each.
		takes in two numbers 0-9999 for each nibble.
*/
void MAX7219plus_Model6::DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment)
{
	char valuesUpper[_NoDigits+ 1];
	char valuesLower[_NoDigits/2 + 1];
	char TextDisplay[5] = "%";
	char TextRight[3] = "4d";
	char TextLeft[4] = "-4d";
	char TextLeadZero[4] = "04d";

	switch(TextAlignment)
	{
		case AlignLeft: strcat(TextDisplay ,TextLeft); break;  // %-4d
		case AlignRight: strcat(TextDisplay ,TextRight); break; // %4d
		case AlignRightZeros: strcat(TextDisplay ,TextLeadZero); break; // %04d
	}

	snprintf(valuesUpper, _NoDigits/2 + 1, TextDisplay, numberUpper);
	snprintf(valuesLower, _NoDigits/2 + 1, TextDisplay, numberLower);
	strcat(valuesUpper ,valuesLower);

	DisplayText(valuesUpper);
}



// Private methods

 /*!
	@brief Shifts out a uint8_t of data on to the MAX7219 SPI-like bus, MSBFIRST
	@param value The uint8_t of data to shift out
	@note _CommDelay microsecond delay may have to be adjusted depending on processor, for software SPI
*/
void MAX7219plus_Model6::HighFreqshiftOut(uint8_t value)
{
	for (uint8_t i = 0; i < 8; i++)
	{
		!!(value & (1 << (7 - i))) ? digitalWrite(_Display_SDATA, HIGH):  digitalWrite(_Display_SDATA, LOW);
		digitalWrite(_Display_SCLK, HIGH);
		delayMicroseconds(_CommDelay);
		digitalWrite(_Display_SCLK, LOW);
		delayMicroseconds(_CommDelay);
	}
}

/*!
	@brief Fetch's the seven segment code for a given ASCII code from the font
	@param character The ASCII character to  lookup
	@param decimalPoint Is the decimal point(dp) to be set or not.
	@return The seven segment representation of the ASCII character in a byte dpabcdefg
*/
uint8_t MAX7219plus_Model6::ASCIIFetch(uint8_t character, DecimalPoint_e decimalPoint)
{
	if (character < _ASCII_FONT_OFFSET || character >= _ASCII_FONT_END )
	{
		if (displaylib_LED_debug){
			Serial.print("Warning : ASCIIFetch : ASCII character is outside font range :");
			Serial.println(character);
		}
		character = '0';
	} 
	uint8_t returnCharValue =0;
	const uint8_t *font = SevenSegmentFont::pFontSevenSegptr();
	returnCharValue = flipBitsPreserveMSB(font[character - _ASCII_FONT_OFFSET]);
	switch (decimalPoint)
	{
		case DecPointOn  :  returnCharValue |= DEC_POINT_7_MASK; break;
		case DecPointOff :  break;
	}

	return returnCharValue;
}

/*!
	@brief Write to the MAX7219 display register
	@param RegisterCode the register to write to
	@param data The data byte to send to register
*/
void MAX7219plus_Model6::WriteDisplay( uint8_t RegisterCode, uint8_t data)
{
	if (_HardwareSPI == false)
	{
		// Loop over all displays, from last to first in chain
		for (int8_t i = _NoDisplays; i >= 1; i--)
		{
			if (i == _CurrentDisplayNumber)
			{
				HighFreqshiftOut(RegisterCode);
				HighFreqshiftOut(data);
			}else{
				HighFreqshiftOut(MAX7219_REG_NOP);
				HighFreqshiftOut(0x00);
			}
		}
	}else{
		uint8_t TransmitBuffer[_NoDisplays * 2];
		// Fill all with NOPs
		for (uint8_t i = 0; i < _NoDisplays; i++) {
			TransmitBuffer[i * 2]     = MAX7219_REG_NOP;
			TransmitBuffer[i * 2 + 1] = 0x00;
		}
		// Place real command for the target display
		uint8_t displayIndex = _NoDisplays - _CurrentDisplayNumber;
		TransmitBuffer[displayIndex * 2]     = RegisterCode;
		TransmitBuffer[displayIndex * 2 + 1] = data;

		SPI.transfer(TransmitBuffer, sizeof(TransmitBuffer));
	}
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param mode Set to 0x00 for most users
*/
void MAX7219plus_Model6::SetDecodeMode(DecodeMode_e mode)
{
	CommStart();
	WriteDisplay(MAX7219_REG_DecodeMode , mode);
	CommEnd();
}

/*!
	@brief Set the decode mode of the  MAX7219 decode mode register
	@param numDigits Usually set to 7(digit 8) The scan-limit register sets how many digits are displayed,
	from 1 to 8.
	@note Advanced users only , read datasheet
*/
void MAX7219plus_Model6::SetScanLimit(ScanLimit_e numDigits)
{
	CommStart();
	WriteDisplay(MAX7219_REG_ScanLimit, numDigits);
	CommEnd();
}

/*!
	@brief Flips the positions of bits in a byte while preserving the MSB bit
	@param byte A byte of data, ASCII character
	@return A byte from font representing LED segment data with bits positions flipped and MSB bit value preserved 
	@details The reason for this function is that the MAX7219 requires ASCII segment data
	in following order : dp-abcdefg but the font we use is dp-gfedcba where
	letters represent seven segment LEDS, and dp represents decimal point.. 
	We flip the bits in the code rather than change the font data in font file because
	the font data is used by other modules(TM1638 + TM1637) and they use dp-gfedcba order.
	Thus we can share same font file between all seven segment modules.
*/
uint8_t MAX7219plus_Model6::flipBitsPreserveMSB(uint8_t byte) 
{
	uint8_t msb = byte & DEC_POINT_7_MASK; // Extract the most significant bit (MSB)
	uint8_t flipped = 0;       // Variable to store the flipped result
	for (int i = 0; i < 7; ++i) 
	{
		// Shift the i-th bit from the rightmost 7 bits to the new position
		if (byte & (1 << i)) 
		{
			flipped |= (1 << (6 - i));
		}
	}
	return msb | flipped; // Combine the saved MSB with the flipped bits
}

/*!
	@brief Start the communication with the MAX7219 display
	@note This function is used to set the CS pin low before sending data
*/
void MAX7219plus_Model6::CommStart(void)
{
	if (_HardwareSPI) {MAX_P_SPI_TRANSACTION_START;}
	digitalWrite(_Display_CS, LOW);
}

/*!
	@brief End the communication with the MAX7219 display
	@note This function is used to set the CS pin high after sending data 
*/
void MAX7219plus_Model6::CommEnd(void)
{
	digitalWrite(_Display_CS, HIGH);
	if (_HardwareSPI) {MAX_P_SPI_TRANSACTION_END;}
}
// == EOF ==
