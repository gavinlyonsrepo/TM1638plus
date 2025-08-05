/*!
	@file MAX7219_CASCADE_Model6.ino
	@author Gavin Lyons
	@brief A demo file library for Max7219 seven segment displays
			Shows simple example with two cascaded displays
	@test
		-# Test 410 Cascade demonstration 
*/

// Libraries 
#include "MAX7219plus.h" 

/// @cond

// Delays for testing
#define TEST_DELAY5 5000
#define TEST_DELAY2 2000
#define TEST_DELAY1 1000

// *** USER OPTION SPI SELECTION ***
#define myhardwareSPI //hardware SPI , comment out for software SPI
// ***
uint8_t totalDisplays = 2; // Total number of displays in cascade

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
	uint16_t CommDelay = 0; //uS software SPI delay.
	// Constructor object 
	MAX7219plus_Model6 myMAX(CS1_PIN, SCLK_PIN,SDIN_PIN, CommDelay, totalDisplays);
#endif

void setup(){
  Serial.begin(38400);
	delay(TEST_DELAY1);
	Serial.println("Test Begin");
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();
}

// Main loop
void loop() 
{
	// Some test data defined
	char teststr1[] = "Display1";
	char teststr2[] = "Display2";
	// Init the displays
	// -Init Display one 
	myMAX.SetCurrentDisplayNumber(1);
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();
	// -Init Display Two
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();

	// Write to Display one 
	myMAX.SetCurrentDisplayNumber(1);
	Serial.println("Display 1");
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	delay(5000);
	
	// Write to Display Two 
	myMAX.SetCurrentDisplayNumber(2);
	Serial.println("Display 2");
	myMAX.DisplayText(teststr2, myMAX.AlignRight);
	delay(5000);
	
	// Write to Display one again
	myMAX.SetCurrentDisplayNumber(1);
	Serial.println("Display 1 again, 111");
	myMAX.DisplayIntNum(111, myMAX.AlignRight);
	delay(5000);

	// Write to Display two again
	myMAX.SetCurrentDisplayNumber(2);
	Serial.println("Display 2 again, 222");
	myMAX.DisplayIntNum(222, myMAX.AlignRight);
	delay(5000);
	
	// Clear the displays 
	Serial.println("Clear the displays");
	// -Clear Display one 
	myMAX.SetCurrentDisplayNumber(1);
	myMAX.ClearDisplay();
	delay(2500);
	// -Clear DisplayTwo 
	myMAX.SetCurrentDisplayNumber(2);
	myMAX.ClearDisplay();
	delay(500);
	
	// End operations
	Serial.println("Test End");
  while(1){};
} 
// End of main

/// @endcond
