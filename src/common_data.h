/*!
	@file   common_data.h
	@brief  common data class for LED segment modules.
*/

#ifndef DATALIB_COMMON_H
#define DATALIB_COMMON_H


/*!
	@class CommonData
	@brief Class that provides access to common data
 */
class CommonData{
public: 

	/*! @brief Switch on or off  decimal point on a digit */
	enum DecimalPoint_e : uint8_t
	{
		DecPointOff  = 0, /**< Decimal point segment off */
		DecPointOn  = 1  /**< Decimal point segment on */
	};
	
	static constexpr uint16_t displaylib_LED_VersionNum = 210; /**< library version number */
	bool displaylib_LED_debug = false; /**< debug flag, true = debug mode on, extra infomation written to console */

protected:
	// Font offsets
	static constexpr uint8_t _ASCII_FONT_OFFSET     = 0x20; /**< Offset in the ASCII table for font Start position */
	static constexpr uint8_t _ASCII_FONT_END        = 0x7B; /**< End of ASCII Table + 1*/
	static constexpr uint8_t _ASCII_FONT_HEX_OFFSET = 0x10; /**< ASCII table offset to reach the number position */
	// Decimal point masks
	static constexpr uint8_t  DEC_POINT_7_MASK =    0x80; /**< Mask to switch on 7 seg decimal point */
};

#endif
