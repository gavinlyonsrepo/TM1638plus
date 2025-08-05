/*!
	@file MAX7219_BCD_Model6.ino
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
		Carries out tests on BCD decode mode 3. BCD mode uses the built in font Code B
		this font has BCD code B (0-9, E, H, L, P, and -) 
		Hardware SPI , The ASCII font is NOT referenced and can be discarded when using this mode
		to save memory 
	@details See USER OPTION SPI SELECTION , comment out define 'hardwareSPI' for software SPI.
	@test
		-# Test 1000 BCD character
		-# Test 1001 BCD string
*/

// Libraries 
#include "MAX7219plus.h" 

/// @cond

// *** USER OPTION SPI SELECTION ***
#define myhardwareSPI //hardware SPI , comment out for software SPI
// ***
uint8_t totalDisplays = 1; // Total number of displays in cascade

#ifdef myhardwareSPI // Hardware SPI setup
	//Data  Pin 11 on UNO R4  
	//Clock Pin 13 on UNO R4 
	uint8_t CS_PIN   = 2;  
	// Constructor object 
	MAX7219plus_Model6 myMAX(CS_PIN, totalDisplays);
#else // Software SPI setup
	uint8_t SDIN_PIN = 3; 
	uint8_t SCLK_PIN = 4; 
	uint8_t CS1_PIN   = 2 ;  
	uint16_t CommDelay = 0; // uS software SPI delay.
	// Constructor object 
	MAX7219plus_Model6 myMAX(CS1_PIN, SCLK_PIN,SDIN_PIN, CommDelay, totalDisplays);
#endif

// Setup test
void setup()
{
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeBCDThree);
	myMAX.ClearDisplay();
}

// Main loop
void loop() 
{
	myTest();
	while(1){}; // wait here forever.
}
// End of main

// Function Space
void myTest(void)
{
	// Test BCD Char
	myMAX.DisplayBCDChar(7, myMAX.CodeBFontH); // Digit 7 LHS of display
	myMAX.DisplayBCDChar(6, myMAX.CodeBFontE);
	myMAX.DisplayBCDChar(5, myMAX.CodeBFontL);
	myMAX.DisplayBCDChar(4, myMAX.CodeBFontP);
	myMAX.DisplayBCDChar(3, myMAX.CodeBFontSpace);
	myMAX.DisplayBCDChar(2, myMAX.CodeBFontFour);
	myMAX.DisplayBCDChar(1, myMAX.CodeBFontDash);
	myMAX.DisplayBCDChar(0, myMAX.CodeBFontOne); // Digit 0 RHS
	delay(5000);
	myMAX.ClearDisplay();
	
	// Test BCD string 
	char teststring1[] = "PH  3-14";
	myMAX.DisplayBCDText(teststring1);
	delay(5000);
	myMAX.ClearDisplay();
}

/// @endcond
