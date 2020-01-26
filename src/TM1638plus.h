/*
* Project Name: TM1638plus 
* File: TM1638plus.h
* Description: TM1638plus.h header file arduino library for TM1638 module(LED & KEY).
* Author: Gavin Lyons.
* Created May 2019
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/


#ifndef TM1638plus_h
#define TM1638plus_h

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif


#define ACTIVATE_TM 0x8F // Start up
#define BUTTONS_MODE 0x42 // Buttons mode
#define WRITE_LOC 0x44 // Write to a location
#define WRITE_INC 0x40 // Incremental write
#define SEG_ADR 0XC0  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
#define LEDS_ADR 0xC1  // Leftmost LED address C1 C3 C5 C7 C9 CB CD CF
#define BRIGHT_ADR 0x88 // Brightness address
#define BRIGHT_MASK 0x07 // Brightness mask 
#define DEFAULT_BRIGHTNESS 0x02 //can be 0x00 to 0x07 
#define DISPLAY_SIZE 8 //size of display

#define ASCII_OFFSET 32 // Ascii table offset to jump over first missing 32 chars
#define HEX_OFFSET   16 // Ascii table offset to reach number position
#define DOT_MASK_DEC    128 // 0x80 Mask to  switch on decimal point in seven seg.   

class TM1638plus  {

public:
	// Constructor 
	// Init the module
	TM1638plus(uint8_t strobe, uint8_t clock, uint8_t data);
	// Methods
	// Send a command to module
	void sendCommand(uint8_t value);

	// Reset module 
	void reset(void);

	//Sets the brightness level on a scale of brightness = 0 to 7.
	//0 is not turned off, it's just the lowest brightness.
	//If user wishes to change the default brightness at start-up change.
	 //The DEFAULT_BRIGHTNESS define in header file.
	void brightness(uint8_t brightness);

	//Read buttons returns a byte with value of buttons 1-8 b7b6b5b4b3b2b1b0
	// 1 pressed, zero not pressed. 
	//User may have to debounce buttons depending on application.
	//See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
	// for de-bonce example.
	uint8_t readButtons(void);

	// Set an LED, pass it LED position 0-7 and value 0 or 1
	void setLED(uint8_t position, uint8_t value);

	// Send Text to Seven segments, passed char array pointer
	// dots are removed from string and dot on preceding digit switched on
	// "abc.def" will be shown as "abcdef" with c decimal point turned on.
	void displayText(const char *text);

	// Send ASCII value to seven segment, pass position 0-7 and ASCII value byte
	void displayASCII(uint8_t position, uint8_t ascii);

	// Same as displayASCII function but turns on dot/decimal point  as well 
	void displayASCIIwDot(uint8_t position, uint8_t ascii) ;

	// Send HEX value to seven segment, pass position 0-7 and hex value(DEC) 0-15
	void displayHex(uint8_t position, uint8_t hex);

	// Send seven segment value to seven segment
	//  pass position 0-7 byte of data corresponding to segments (dp)gfedcba
	// i.e 0b01000001 will set g and a on. 
	void display7Seg(uint8_t position, uint8_t value);
	
	//Display an integer and leading zeros optional
	void displayIntNum(unsigned long number, boolean leadingZeros = true);
	
	//Divides the display into two nibbles and displays a Decimal number in each.
	//takes in two numbers 0-9999 for each nibble ,  and byte for decimal point display,
	//and leading zeros optional
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, boolean leadingZeros = true);
	
private:
		uint8_t _STROBE_IO;
		uint8_t _DATA_IO;
		uint8_t _CLOCK_IO;

};

#endif
