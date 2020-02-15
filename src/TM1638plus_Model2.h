/*
* Project Name: TM1638plus_Model2 
* File: TM1638plus_Model2.h
* Description: TM1638plus_Model2.h header file Arduino library for Model 2 TM1638 module(16 KEY 16 pushbuttons).
* Author: Gavin Lyons.
* Created August 2019
* URL: https://github.com/gavinlyonsrepo/TM1638plus
*/


#ifndef TM1638plus_Model2_h
#define TM1638plus_Model2_h

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#define ACTIVATE_TM 0x8F // Start up
#define BUTTONS_MODE 0x42 // Buttons mode
#define WRITE_LOC 0x44 // Write to a location
#define WRITE_INC 0x40 // Incremental write
#define SEG_ADR 0xC0  // leftmost segment Address C0 C2 C4 C6 C8 CA CC CE
#define BRIGHT_ADR 0x88 // Brightness address
#define BRIGHT_MASK 0x07 // Brightness mask 
#define DEFAULT_BRIGHTNESS 0x02 //can be 0x00 to 0x07 

#define DISPLAY_SIZE 8 //size of display
#define ASCII_OFFSET 32 // Ascii table offset to jump over first missing 32 chars   
#define DOT_MASK_DEC 128 // 0x80 Mask to  switch on decimal point in seven seg.  

class TM1638plus_Model2  {

public:
    // Constructor Init the module
    // strobe = GPIO connected to strobe line of module
    // clock = GPIO connected to clock line of module
    // data = GPIO connected to data line of module
    // reverse_nibbles = boolean default false, if true swaps nibbles on display byte
    TM1638plus_Model2(uint8_t strobe, uint8_t clock, uint8_t data, bool swap_nibbles= false);
    
    // Methods
    
    // Begin method , sets pinmodes , Call in setup
    void displayBegin(void);
    
    // Send a command to module
    void sendCommand(uint8_t value);

    // Send data to the module
    void sendData(unsigned char data);

    // Reset / Clear module 
    void reset(void);

    //Sets the brightness level on a scale of brightness = 0 to 7.
    //0 is not turned off, it's just the lowest brightness.
    //If user wishes to change the default brightness at start-up change.
     //The DEFAULT_BRIGHTNESS define in header file.
    void brightness(uint8_t brightness);

    //Read buttons returns a byte with value of buttons 1-16 (1 for S1 2 for S2 ... 16 for S16 )
    // Returns zero if no button pressed.
    //User may have to deb-ounce buttons depending on application.
    //See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
    //For de-bounce practical example(model 1)
    //model 2 example here in ADC file TM1638plus_ADC_TEST_Model2.ino.
    unsigned char ReadKey16(void);

    // Send seven segment value module
    // pass segment  0-7, byte of data corresponding to segments abcdefg(dp) 01234567.
    // a is 0 , dp is 7 , segmentValue is which segments are off or on for each digit. d7d6d5d54d3d2d1 
    // Have to call it 8 times in row to set all segments. 
    // to set all "a" on send (0x00,0xFF)
    // to set all segment "g" off (0x06,0X00)
    void DisplaySegments(uint8_t segment, uint8_t segmentValue);
    
    // Display a Hexadecimal number ,takes a number and byte for decimal point display, leading zeros optional
    // converts to string. 
    //void DisplayHexNum(unsigned long number, byte dots, boolean leadingZeros = true);
    void DisplayHexNum(uint16_t  numberUpper, uint16_t numberLower, byte dots, boolean leadingZeros = true);
    
    // Display a decimal number , takes a number and byte for decimal point display,  leading zeros optional 
    // converts to string. 
    void DisplayDecNum(unsigned long number, byte dots, boolean leadingZeros = true);

    // Display a string , takes a string and byte for decimal point display
    // Takes in string , converts it to ASCII using the font and masks for the decimal point
    // Then passes array of eight ASCII bytes to DisplayValues function
    void DisplayStr(const char* string, const word dots = 0);
   
    // Takes in Array of 8 ASCII bytes , Called from DisplayStr 
    // Scans each ASCII byte converts to array of 8 segment bytes where each byte represents a segment
    // i.e. byte 0 represents a in segment and then each bit represents the a segment in each digit.
    // So for "00000005" is converted by DisplayStr to hex "3F 3F 3F 3F 3F 3F 3F 6D" where left is first digit.
    // this is converted to hex "FF FE FF FF FE FF 01 00"  Where left is first segment
    // So "a" segment is turned on for all bytes and "b" is on for all except last digit.
    // The bits are  mapping below abcdefg(dp) = 01234567 ! 
    // see for mapping of seven segment to digit https://en.wikipedia.org/wiki/Seven-segment_display
    void ASCIItoSegment(const byte values[]);
	
	//Divides the display into two nibbles and displays a Decimal number in each.
	//takes in two numbers 0-9999 for each nibble ,  and byte for decimal point display,
	//and leading zeros optional
	void DisplayDecNumNibble(uint16_t numberUpper, uint16_t numberLower, byte dots, boolean leadingZeros = true);
	
private:
        uint8_t _STROBE_IO;
        uint8_t _DATA_IO;
        uint8_t _CLOCK_IO;
        bool _SWAP_NIBBLES;       
};
 
#endif
