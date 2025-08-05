/*!
	@file   seven_segment_font_data.h
	@brief  Seven-segment font data class for LED segment modules.
*/

#ifndef SEVENSEG_COMMON_H
#define SEVENSEG_COMMON_H

#if (ARDUINO >=100)
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif
#include <stdint.h>

/*!
	@class SevenSegmentFont
	@brief Class that provides access to a seven-segment font data table.
 */
class SevenSegmentFont {
protected:
	static const uint8_t* pFontSevenSegptr();
protected:
	// Font offsets
	static constexpr uint8_t _ASCII_FONT_OFFSET     = 0x20; /**< Offset in the ASCII table for font Start position */
	static constexpr uint8_t _ASCII_FONT_END        = 0x7B; /**< End of ASCII Table + 1*/
	static constexpr uint8_t _ASCII_FONT_HEX_OFFSET = 0x10; /**< ASCII table offset to reach the number position */
	// Decimal point masks
	static constexpr uint8_t  DEC_POINT_7_MASK =    0x80; /**< Mask to switch on 7 seg decimal point */
private:
	static const uint8_t fontData[];
};

/*!
	@class CommonData
	@brief Class that provides access to common data
 */
class CommonData{
public: 

	/*! @brief Decimal point on digit control */
	enum DecimalPoint_e : uint8_t
	{
		DecPointOff  = 0, /**< Decimal point segment off */
		DecPointOn  = 1  /**< Decimal point segment on */
	};
	
	static constexpr uint16_t displaylib_LED_VersionNum = 220; /**< library version number */
	bool displaylib_LED_debug = false; /**< debug flag, true = debug mode on, extra infomation written to console */


};

#endif
