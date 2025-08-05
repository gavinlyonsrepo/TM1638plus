/*!
	@file MAX7219_TEST_Model6.ino
	@author Gavin Lyons
	@brief A demo file for Max7219 seven segment displays ,
			Carries out series of tests to test the library. Hardware or software SPI
	@details See USER OPTION SPI SELECTION , comment out define 'hardwareSPI' for software SPI.
	@test
		-# Test 1 Text strings display 
		-# Test 2 Brightness
		-# Test 3 Test Display Mode
		-# Test 4 ShutDown Mode
		-# Test 5 Character display 
		-# Test 6 Set Segment
		-# Test 7 Decimal number
		-# Test 8 Multiple Decimal points + Display Overflow  
		-# Test 9 Floating point
		-# Test 10 Counter
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
	uint16_t CommDelay = 0; //uS software SPI delay.
	// Constructor object 
	MAX7219plus_Model6 myMAX(CS1_PIN, SCLK_PIN,SDIN_PIN, CommDelay, totalDisplays);
#endif

void setup(){
  //Serial.begin(38400);
	delay(TEST_DELAY1);
	//Serial.println("Test Begin");
	myMAX.InitDisplay(myMAX.ScanEightDigit, myMAX.DecodeModeNone);
	myMAX.ClearDisplay();
}

// Main loop
void loop()
{
	Test1();
	Test2();
	Test3();
	Test4();
	Test5();
	Test6();
	Test7();
	Test8();
	Test9();
	Test10();
	EndTest();
}
// End of main


// Functions
void EndTest(void)
{
	//Serial.println("Test End");
  while(1){};
}

void Test1(void)
{
	//Serial.println("Test 1:: DisplayText ");
	// Hello world test on MAX7219
	char teststr1[] = "Start";
	char teststr2[] = "-62.95";
	char teststr3[] = "37.45";
	
	char teststr4[] = "12-44-21";
	char teststr5[] = "07/31/19";
	char teststr6[] = "11.12.2023";
	
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	delay(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr1, myMAX.AlignRight);
	delay(TEST_DELAY2);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr2, myMAX.AlignRight);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr3, myMAX.AlignLeft);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr4, myMAX.AlignLeft);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr5, myMAX.AlignLeft);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	myMAX.DisplayText(teststr6, myMAX.AlignRight);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test2(void)
{
		//Serial.println("Test 2:: Brightness ");
  char teststr1[] = "Bright";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	myMAX.SetBrightness(myMAX.IntensityMin);
	delay(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityMax);
	delay(TEST_DELAY5);
	myMAX.SetBrightness(myMAX.IntensityDefault);
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test3(void) 
{
	//Serial.println("Test 3:: Display Test Mode ");
	myMAX.DisplayTestMode(true);
	delay(TEST_DELAY5);
	myMAX.DisplayTestMode(false);
}

void Test4(void) 
{
	//Serial.println("Test 4:: Shutdown Mode");
	char teststr1[] = "shutdown";
	myMAX.DisplayText(teststr1, myMAX.AlignLeft);
	delay(TEST_DELAY2);
	myMAX.ShutdownMode(true);
	delay(TEST_DELAY2);
	myMAX.ShutdownMode(false);
	delay(TEST_DELAY2);
	myMAX.ClearDisplay();
}

void Test5(void)
{
	//Serial.println("Test 5:: Display characters ab.cde1.23. ");
	myMAX.DisplayChar(7, 'a', myMAX.DecPointOff);  // Digit 7 is LHS of display
	delay(TEST_DELAY1);
	myMAX.DisplayChar(6, 'b', myMAX.DecPointOn);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(5, 'c', myMAX.DecPointOff);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(4, 'd', myMAX.DecPointOff);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(3, 'e', myMAX.DecPointOff);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(2, '1', myMAX.DecPointOn);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(1, '2', myMAX.DecPointOff);
	delay(TEST_DELAY1);
	myMAX.DisplayChar(0, '3', myMAX.DecPointOn);
	delay(TEST_DELAY1);
	myMAX.ClearDisplay();
}

void Test6(void)
{
	//Serial.println("Test 6:: Set digit to Segments, 76543210 = dpabcdefg. ");
	for (uint8_t digit = 0; digit <8 ; digit++)
	{ 
		myMAX.SetSegment(digit, 1<<digit);
		delay(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}

void Test7(void)
{
	//Serial.println("Test 7:: Decimal number ");
	myMAX.DisplayIntNum(45, myMAX.AlignRight); // "        45"
	delay(TEST_DELAY5);
	
	myMAX.ClearDisplay();
	myMAX.DisplayIntNum(798311, myMAX.AlignLeft); // "798311  "
	delay(TEST_DELAY5);
	
	myMAX.DisplayIntNum(93391, myMAX.AlignRightZeros); // "00093391"
	delay(TEST_DELAY5);
	
	myMAX.DisplayIntNum(12345678, myMAX.AlignRight); // "12345678"
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7d tm.DisplayDecNumNIbble right aligned
	myMAX.DisplayDecNumNibble(134, 78, myMAX.AlignRight); // " 134" 78"
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7e tm.DisplayDecNumNIbble left aligned
	myMAX.DisplayDecNumNibble(123, 662, myMAX.AlignLeft); // "123 662 "
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	
	// TEST 7f tm.DisplayDecNumNIbble leading zeros
	myMAX.DisplayDecNumNibble(493, 62, myMAX.AlignRightZeros); // "04930062"
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test8(void)
{
	//TEST 8 Multiple dots test
	//Serial.println("Test 8: Multiple dots test ");
	char teststr1[] = "Hello...";
	char teststr2[] = "...---...";
	myMAX.DisplayText(teststr1);   
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
	myMAX.DisplayText(teststr2);//SOS in morse
	delay(TEST_DELAY5);

	//TEST8b user overflow
	//Serial.println("Test 8B: overflow test ");
	char teststr3[] = "1234567890abc";
	myMAX.DisplayText(teststr3); //should display just 12345678
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test9(void)
{
	//Serial.println("Test 9:  Float ");
	float voltage = 12.45;
	char workStr[11];
	sprintf(workStr, "ADC=%.2f", voltage);
	
	myMAX.DisplayText(workStr); // ADC=12.45
	delay(TEST_DELAY5);
	myMAX.ClearDisplay();
}

void Test10(void)
{
	//Serial.println("Test 10: counter Demo ");
	char workStr[10];
	for (float counter = 0; counter < 3.0; counter += 0.2) 
	{
		sprintf(workStr, "%.1f", counter);
		myMAX.DisplayText(workStr, myMAX.AlignRight); 
		delay(TEST_DELAY1);
	}
	myMAX.ClearDisplay();
}

/// @endcond
