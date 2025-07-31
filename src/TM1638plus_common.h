/*!
	@file     TM1638plus_common.h
	@author   Gavin Lyons
	@brief    Arduino library Tm1638plus, Header  file for common data and functions between model classes. 

	@note  See URL for full details. https://github.com/gavinlyonsrepo/TM1638plus
*/

#ifndef TM1638PLUS_COMMON_H
#define TM1638PLUS_COMMON_H

#if (ARDUINO >=100)
	#include "Arduino.h"
#else
	#include "WProgram.h"
#endif

#include "common_data.h"
#include "seven_segment_font_data.h"

/*! Alignment of text on display */
enum AlignTextType_e : uint8_t
{
	TMAlignTextRight = 1,   /**< Align text to the right on  display */
	TMAlignTextLeft = 2      /**< Align text to the left  on  display */
}; 

/*!
	@brief  The base Class , used to store common data & functions for all models types.
*/
class TM1638plus_common : public SevenSegmentFont , public CommonData
{

public:

	void displayBegin();	 
	void reset(void);  
	void brightness(uint8_t brightness);
	//getting and setter for _HFIN_DELAY
	uint8_t getHFIN_DELAY() const { return _HFIN_DELAY; }
	void setHFIN_DELAY(uint8_t delay) { _HFIN_DELAY = delay; }
	uint8_t getHFOUT_DELAY() const { return _HFOUT_DELAY; }
	void setHFOUT_DELAY(uint8_t delay) { _HFOUT_DELAY = delay; }
protected:

	uint8_t  HighFreqshiftin(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder) ;
	void HighFreqshiftOut(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder, uint8_t val);
	void sendCommand(uint8_t value);
	void sendData(uint8_t  data);

	uint8_t _STROBE_IO; /**<  GPIO connected to STB on Tm1638  */
	uint8_t _DATA_IO;	/**<  GPIO connected to DIO on Tm1638  */
	uint8_t _CLOCK_IO;	/**<  GPIO connected to CLk on Tm1638  */
	bool _HIGH_FREQ = false; 		/**< Set to true if running high freq CPU. */
	uint8_t _HFIN_DELAY = 1;  /**<  uS Delay used by shiftIn function for High-freq MCU  */
	uint8_t _HFOUT_DELAY = 1; /**<  uS Delay used by shiftOut function for High-freq MCU */

	// Commands list and defaults
	static constexpr uint8_t TM_ACTIVATE = 0x8F;		   /**< Start up device */
	static constexpr uint8_t TM_BUTTONS_MODE = 0x42;	   /**< Buttons mode */
	static constexpr uint8_t TM_WRITE_LOC = 0x44;		   /**< Write to a memory location */
	static constexpr uint8_t TM_WRITE_INC = 0x40;		   /**< Incremental write */
	static constexpr uint8_t TM_SEG_ADR = 0xC0;		   /**< Leftmost segment Address C0 C2 C4 C6 C8 CA CC CE */
	static constexpr uint8_t TM_LEDS_ADR = 0xC1;		   /**< Leftmost LED address C1 C3 C5 C7 C9 CB CD CF */
	static constexpr uint8_t TM_BRIGHT_ADR = 0x88;		   /**< Brightness address */
	static constexpr uint8_t TM_BRIGHT_MASK = 0x07;		   /**< Brightness mask */
	static constexpr uint8_t TM_DEFAULT_BRIGHTNESS = 0x02; /**< Brightness can be 0x00 to 0x07, 0x00 is least bright */
	static constexpr uint8_t TM_DISPLAY_SIZE = 8;		   /**< Size of display in digits */
	
private:

};

#endif
