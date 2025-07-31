/*!
	@file   seven_segment_font_data.hpp
	@brief  Seven-segment font data class for LED segment modules.
*/

#ifndef SEVENSEG_COMMON_H
#define SEVENSEG_COMMON_H

/*!
	@class SevenSegmentFont
	@brief Class that provides access to a seven-segment font data table.
 */
class SevenSegmentFont {
protected:
	static const uint8_t* pFontSevenSegptr();
private:
	static const uint8_t fontData[];
};

#endif
