/*!
	@file MAX7219plus.h
	@author Gavin Lyons
	@brief library header file to drive MAX7219 displays
*/

#ifndef MAX7219PLUS_COMMON_H
#define MAX7219PLUS_COMMON_H

// Libraries
#include "seven_segment_font_data.h"
#include <SPI.h>

// --- SPI settings section ---
#define MAX_P_SPI_FREQ 1000000              /**< hz SPI bus baud rate  */
#define MAX_P_SPI_CLOCK_DIV SPI_CLOCK_DIV8  /**< SPI bus baud rate, STM32 data use only */
#define MAX_P_SPI_DIRECTION  MSBFIRST       /**< SPI data bit order orientation */
#define MAX_P_SPI_MODE SPI_MODE0            /**< SPI Mode 0-3 */
//There is a pre-defined macro SPI_HAS_TRANSACTION in SPI library for checking 
 //whether the firmware of the Arduino board supports SPI.beginTransaction().
#ifdef SPI_HAS_TRANSACTION
    #define MAX_P_SPI_TRANSACTION_START SPI.beginTransaction(SPISettings(MAX_P_SPI_FREQ, MAX_P_SPI_DIRECTION, MAX_P_SPI_MODE)) 
    #define MAX_P_SPI_TRANSACTION_END SPI.endTransaction()
#else // SPI transactions likewise not present in MCU or lib
    #define MAX_P_SPI_TRANSACTION_START SPI.setClockDivider(MAX_P_SPI_CLOCK_DIV) // 72/8 = 9Mhz
    #define MAX_P_SPI_TRANSACTION_END  // Blank 
#endif

/*!
	@brief  Drive MAX7219 seven segment displays
*/
class MAX7219plus_Model6 : public SevenSegmentFont , public CommonData
{
public:
	MAX7219plus_Model6(uint8_t chipSelect, uint8_t clock, uint8_t data, uint16_t CommDelay, uint8_t totalDisplays);
	MAX7219plus_Model6(uint8_t chipSelect, uint8_t totalDisplays);

	/*! Alignment of text on display */
	enum TextAlignment_e : uint8_t
	{
		AlignLeft       = 0,  /**< Align text to the left on display */
		AlignRight      = 1,  /**< Align text to the right on display */
		AlignRightZeros = 2   /**< Add leading zeros to the right aligned text */
	};
	
	/*! The decode-mode register sets BCD code B or no-decode operation for each digit */
	enum DecodeMode_e : uint8_t
	{
		DecodeModeNone     = 0x00, /**< No decode for digits 7–0 */
		DecodeModeBCDOne   = 0x01, /**< Code B decode for digit 0, No decode for digits 7–1*/
		DecodeModeBCDTwo   = 0x0F, /**< Code B decode for digits 3–0, No decode for digits 7–4*/
		DecodeModeBCDThree = 0xFF  /**< Code B decode for digits 7–0 */
	};
	/*!  sets BCD code B font (0-9, E, H, L,P, and -) Built-in font */
	enum CodeBFont_e : uint8_t
	{
		CodeBFontZero    = 0x00, /**< Code B decode for Zero */
		CodeBFontOne     = 0x01, /**< Code B decode for One */
		CodeBFontTwo     = 0x02, /**< Code B decode for Two */
		CodeBFontThree   = 0x03, /**< Code B decode for Three */
		CodeBFontFour    = 0x04, /**< Code B decode for Four */
		CodeBFontFive    = 0x05, /**< Code B decode for Five */
		CodeBFontSix     = 0x06, /**< Code B decode for Six */
		CodeBFontSeven   = 0x07, /**< Code B decode for Seven */
		CodeBFontEight   = 0x08, /**< Code B decode for Eight */
		CodeBFontNine    = 0x09, /**< Code B decode for Nine */
		CodeBFontDash    = 0x0A, /**< Code B decode for Dash */
		CodeBFontE       = 0x0B, /**< Code B decode for letter E */
		CodeBFontH       = 0x0C, /**< Code B decode for letter H */
		CodeBFontL       = 0x0D, /**< Code B decode for letter L */
		CodeBFontP       = 0x0E, /**< Code B decode for letter P */
		CodeBFontSpace   = 0x0F  /**< Code B decode for Space */
	};
	/*! Set intensity/brightness of Display */
	enum Intensity_e : uint8_t
	{
		IntensityMin     = 0x00, /**< Minimum Intensity */
		IntensityDefault = 0x08, /**< Default Intensity */
		IntensityMax     = 0x0F  /**<  Maximum Intensity */
	};
	/*! The scan-limit register sets how many digits are displayed */
	enum ScanLimit_e : uint8_t
	{
		ScanOneDigit      = 0x00,  /**< Scan One digit */
		ScanTwoDigit      = 0x01,  /**< Scan Two digit*/
		ScanThreeDigit    = 0x02,  /**< Scan Three digit */
		ScanFourDigit     = 0x03,  /**< Scan Four digit */
		ScanFiveDigit     = 0x04,  /**< Scan Five digit*/
		ScanSixDigit      = 0x05,  /**< Scan Six digit */
		ScanSevenDigit    = 0x06,  /**< Scan Seven digit */
		ScanEightDigit    = 0x07   /**< Scan Eight digit*/
	};

	/*! Register opcodes of the MAZ7219 chip, Register Address Map */
	enum RegisterModes_e : uint8_t
	{
		MAX7219_REG_NOP          = 0x00, /**<  No operation */
		MAX7219_REG_DecodeMode   = 0x09, /**<  Decode-Mode Register */
		MAX7219_REG_Intensity    = 0x0A, /**<  Intensity Register, brightness of display */
		MAX7219_REG_ScanLimit    = 0x0B, /**<  Scan Limit,  The scan-limit register sets how many digits are displayed */
		MAX7219_REG_ShutDown     = 0x0C, /**<  When the MAX7219 is in shutdown mode, the scan oscillator is
												halted, all segment current sources are pulled to ground,
												and all digit drivers are pulled to V+, thereby blanking the
												display.  */
		MAX7219_REG_DisplayTest  = 0x0F  /**<  Display-test mode turns all LEDs on by
												overriding, but not altering, all controls and digit registers */
	};

	void InitDisplay(ScanLimit_e numDigits, DecodeMode_e decodeMode);
	void ClearDisplay(void);
	void SetBrightness(uint8_t brightness);
	void DisplayTestMode(bool OnOff);
	void ShutdownMode(bool OnOff);

	uint16_t GetCommDelay(void);
	void SetCommDelay(uint16_t commDelay);
	bool GetHardwareSPI(void);
	uint8_t GetCurrentDisplayNumber(void);
	void SetCurrentDisplayNumber(uint8_t);

	void DisplayChar(uint8_t digit, uint8_t value, DecimalPoint_e decimalPoint);
	int DisplayText(char *text, TextAlignment_e TextAlignment);
	int DisplayText(char *text);
	void DisplayIntNum(unsigned long number, TextAlignment_e TextAlignment);
	void DisplayDecNumNibble(uint16_t  numberUpper, uint16_t numberLower, TextAlignment_e TextAlignment);
	void DisplayBCDChar(uint8_t digit, CodeBFont_e value);
	int DisplayBCDText(char *text);
	void SetSegment(uint8_t digit, uint8_t segment);

protected:


private:

	uint8_t _Display_CS;     /**<  GPIO connected to  CS on MAX7219*/
	uint8_t _Display_SDATA;  /**<  GPIO connected to DIO on MAX7219*/
	uint8_t _Display_SCLK;   /**<  GPIO connected to CLK on MAX7219*/

	uint16_t _CommDelay = 0;    /**<  uS delay used in communications SW SPI, User adjust */
	uint8_t _NoDigits   = 8;    /**<  Number of digits in display */
	bool _HardwareSPI = false;  /**< Is the Hardware SPI on , true yes , false SW SPI*/

	DecodeMode_e CurrentDecodeMode; /**< Enum to store current decode mode  */

	uint8_t _CurrentDisplayNumber = 1; /**< Which display the user wishes to write to in a cascade of connected displays*/
	uint8_t _NoDisplays = 1;           /**< Number of displays in cascade, default is 1 */

	void HighFreqshiftOut(uint8_t value);
	void WriteDisplay(uint8_t RegisterCode, uint8_t data);
	uint8_t ASCIIFetch(uint8_t character,DecimalPoint_e decimalPoint);
	void SetDecodeMode(DecodeMode_e mode);
	void SetScanLimit(ScanLimit_e numDigits);
	uint8_t flipBitsPreserveMSB(uint8_t byte);
	void CommStart(void);
	void CommEnd(void);
};

#endif
