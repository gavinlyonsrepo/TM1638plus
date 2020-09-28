/*
  Project Name: TM1638
  File: TM1638plus_TEST.ino
  Description: demo file library for  TM1638 module(8 bicolour green and red LEDs & 8 pushbuttons). 
  Called Model 3 in this library.  This model is labelled LKM1638 or tm1638 v1.3
  Carries out series of tests demonstrating arduino library TM1638plus.

  TESTS
  TEST 0 Reset
  TEST 1 Brightness
  TEST 2 ASCII display
  TEST 3 Set a single segment
  TEST 4 Hex digits
  TEST 5 Text String with Decimal point
  TEST 6 TEXT + ASCII combo
  TEST 7 Integer Decimal number
  TEST 8 Text String + Float
  TEST 9 Text String + decimal number
  TEST 10 Multiple dots
  TEST 11 Display Overflow
  TEST 12 Scrolling text
  TEST 13 Green + red LEDS, setLED and setLEDs functions.
  TEST 14 Buttons to serial monitor

  Author: Gavin Lyons.
  Created May 2019
  URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include <TM1638plus.h>

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

//Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU default false)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);


// Some vars and defines for the tests.
#define myTestDelay  5000
#define myTestDelay1 1000
uint8_t  testcount = 1;


void setup()
{
  Serialinit();
  tm.displayBegin();
  delay(myTestDelay1);

  //Test 0 reset
  Test0();
}

void loop()
{
  switch (testcount)
  {
    case 1: Test1(); break; // Brightness
    case 2: Test2(); break; // ASCII display
    case 3: Test3(); break; // TEST 3 Set a single segment
    case 4: Test4(); break; // Hex digits
    case 5: Test5(); break; // Text String with Decimal point
    case 6: Test6(); break; // TEXT + ASCII combo
    case 7: Test7(); break; // Integer Decimal number
    case 8: Test8(); break; // Text String + Float hack
    case 9: Test9(); break; // Text String + decimal number
    case 10: Test10(); break; // Multiple Decimal points
    case 11: Test11(); break; // Display Overflow
    case 12: Test12(); break; // Scrolling text
    case 13: Test13(); break; // Bi colour LEDS, setLED and setLEDS function.
    case 14: Test14(); break; // Buttons to serial monitor
  }
  testcount++;
}

void Test0()
{
  // Test 0 reset test
  tm.setLED(0, 1);
  delay(myTestDelay);
  tm.reset();
}

void Test1() {
  // Test 1  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.displayText("00000000");
    delay(myTestDelay1);
  }
  tm.reset();
  // restore default brightness
  tm.brightness(0x02);
}

void Test2() {
  //Test 2 ASCII display 2.342

  tm.displayASCIIwDot(0, '2');
  tm.displayASCII(1, '3');
  tm.displayASCII(2, '4');
  tm.displayASCII(3, '2');
  delay(myTestDelay);
  tm.reset();
}

void Test3() {
  //TEST 3 single segment (pos, (dp)gfedcba)
  //In this case  segment g (middle dash) of digit position 7
  tm.display7Seg(7, 0b01000000);
  delay(myTestDelay);
}

void Test4() {
  // Test 4 Hex digits.
  tm.displayHex(0, 1);
  tm.displayHex(1, 2);
  tm.displayHex(2, 3);
  tm.displayHex(3, 4);
  tm.displayHex(4, 5);
  tm.displayHex(5, 6);
  tm.displayHex(6, 7);
  tm.displayHex(7, 8);
  delay(myTestDelay);

  tm.displayHex(0, 8);
  tm.displayHex(1, 9);
  tm.displayHex(2, 10);
  tm.displayHex(3, 11);
  tm.displayHex(4, 12);
  tm.displayHex(5, 13);
  tm.displayHex(6, 14);
  tm.displayHex(7, 15);
  delay(myTestDelay);
}

void Test5() {
  // Test 5 TEXT  with dec point
  // abcdefgh with decimal point for c and d
  tm.displayText("abc.d.efgh");
  delay(myTestDelay);
}

void Test6() {
  // Test6  TEXT + ASCII combo
  // ADC=.2.541
  char text1[] = "ADC=.";
  tm.displayText(text1);
  tm.displayASCIIwDot(4, '2');
  tm.displayASCII(5, '5');
  tm.displayASCII(6, '4');
  tm.displayASCII(7, '1');
  delay(myTestDelay);
  tm.reset();
}

void Test7() {
  // TEST 7a Integer
  tm.displayIntNum(72, false); // "72      "
  delay(myTestDelay);
  // TEST 7b Integer
  tm.displayIntNum(92345, true); // "00092345"
  delay(myTestDelay);
  tm.reset();
  // TEST 7b tm.DisplayDecNumNIbble
  tm.DisplayDecNumNibble(1488, 5678, false); // "14885678"
  delay(myTestDelay);
  tm.DisplayDecNumNibble(123, 998, true); // "01230998"
  delay(myTestDelay);
}

void Test8() {
  // TEST 8  TEXT STRING + integer SSSSIIII
  char workStr[11];
  uint16_t  data = 294;
  sprintf(workStr, "ADC=.%04d", data); // "ADC=.0294"
  tm.displayText(workStr);
  delay(myTestDelay);
}

void Test9() {
  // TEST 9 Text String + Float  SSSSFFFF ,  just one possible method.
  float voltage = 12.45;
  uint16_t temp = 0;
  char workStr[11];
  uint8_t  digit1, digit2, digit3 , digit4;
  voltage =  voltage * 100; // 1245
  temp = (uint16_t)voltage;
  digit1 = (temp / 1000) % 10;
  digit2 = (temp / 100) % 10;
  digit3 = (temp / 10) % 10;
  digit4 =  temp % 10;

  sprintf(workStr, "ADC=.%d%d.%d%d", digit1, digit2, digit3, digit4);
  tm.displayText(workStr); //12.45.VOLT
  delay(myTestDelay);
  tm.reset();
}

void Test10()
{
  //TEST 10 Multiple dots test
  tm.displayText("Hello...");
  delay(myTestDelay);
  tm.displayText("...---..."); //SOS in morse
  delay(myTestDelay);
}

void Test11()
{
  //TEST11 user overflow
  tm.displayText("1234567890abc"); //should display just 12345678
  delay(myTestDelay);
  tm.reset();
}


void Test12() {
  //TEST 12 scrolling text, just one possible method 
    char textScroll[17] = " Hello world 123";
  unsigned long previousMillis_display = 0;  // will store last time display was updated
  const long interval_display = 1000;            //   interval at which to update display (milliseconds)

  while(1)
  {
  tm.displayText(textScroll);
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis_display >= interval_display)
  {
    previousMillis_display = currentMillis;
    if (strlen(textScroll) > 0)
    {
      memmove(textScroll, textScroll+1, strlen(textScroll));
      tm.displayText("        "); //Clear display or last character will drag across screen
    }else
    {
      return;
    }
   }
  }

}


void Test13()
{
  //Test 13 LED display
  uint8_t LEDposition = 0;

  // Test 13A Turn on green leds with setLED
  for (LEDposition = 0; LEDposition < 8; LEDposition++) {
    tm.setLED(LEDposition, TM_GREEN_LED);
    delay(500);
    tm.setLED(LEDposition, TM_OFF_LED);
  }

  // Test 13b turn on red LEDs with setLED
  for (LEDposition = 0; LEDposition < 8; LEDposition++) {
    tm.setLED(LEDposition, TM_RED_LED);
    delay(500);
    tm.setLED(LEDposition, TM_OFF_LED);
  }

  // TEST 13c 
  // test setLEDs function (0xgreenred) (0xGGRR) (LED8-LED1, LED8-LED1)
  // Upper byte switch LED green colour ON, lower byte = switch LED red colour ON
  // NB Note on the unit, LED8 is onthe right hand side so result is mirrored.
  // Example:
  // E0 = green on 07 = red on 
  // E0  = 1110 0000 , 07 = 0000 0111 = 11100111 = GGGXXRRR = LED8-LED1
  // Shows on display as  LED1-LED8 turns on RRRXXGGG as LED 8 is on right hand side.
   
  tm.setLEDs(0xE007); //L1-L8 turns on RRRXXGGG on display
  delay(3000);
  
  tm.setLEDs(0xF00F); // L1-L8 turns on RRRRGGGG on display
  delay(3000);
  tm.setLEDs(0xFE01); // L1-L8 turns on RGGGGGGG on display
  delay(3000);
  tm.setLEDs(0x00FF); //all red   RRRRRRR
  delay(3000);
  tm.setLEDs(0xFF00); //all green GGGGGGG
  delay(3000);
  tm.setLEDs(0x0000); //all off
  delay(3000);

}

void Test14() {
  //Test 14 buttons a press switch number S-X where x is 1-8.
  //The HEx value of switch is also sent to Serial port  and 7seg
  tm.displayText("buttons ");
  delay(2000);
  tm.displayText("        ");
  while (1) // Loop here forever
  {
    uint8_t buttons = tm.readButtons();
    /* buttons contains a byte with values of button s8s7s6s5s4s3s2s1
      HEX  :  Switch no : Binary
      0x01 : S1 Pressed  0000 0001
      0x02 : S2 Pressed  0000 0010
      0x04 : S3 Pressed  0000 0100
      0x08 : S4 Pressed  0000 1000
      0x10 : S5 Pressed  0001 0000
      0x20 : S6 Pressed  0010 0000
      0x40 : S7 Pressed  0100 0000
      0x80 : S8 Pressed  1000 0000
    */
    Serial.println(buttons, HEX);
    tm.displayIntNum(buttons, true);
    delay(500);
  }
}



//Function to setup serial called from setup FOR debug
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--Comms UP--TM1638plus_TEST_Model1.ino--");
}
