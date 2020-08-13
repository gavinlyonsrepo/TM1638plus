/*
  Project Name: TM1638
  File: TM1638plus_TEST_Model2.ino
  Description: demo file library for model 2 TM1638 module(16 KEY 16 pushbuutons).
  Carries out series of tests demonstrating arduino library TM1638plus.

  Tests

  0 = Test 0 reset test  
  
1A = Test 1A decimal numbers
  1B = Test 1B  decimal numbers with the DisplayDecNumNibble function
  
  2 = Test 2 Hexadecimal number
 
  3 Test 3 manually set segments abcdefg(dp) = 01234567
  3A = Display 1
  3B = Display 00000005
  3C = Display scrolling g segment , "-" countup to "--------"

  4 = Display 4 strings
  
  5 = ASCII to segments takes an array of 8 bytes and displays them
      without ref to the ASCII font table, direct data to digits to displays. i.e. 3F 3F 3F 6D 3F 3F 3F 6D = 00050005
       (DP)gfedcba = 0011 1111 = 3F = zero https://en.wikipedia.org/wiki/Seven-segment_display
  
  6 = Brightness control 
  
  7 = buttons, no debounce see notes at URL for Example to debounce.
      returns 0-16 , 0 for nothing pressed. data sent to serial.
  
  Author: Gavin Lyons.
  Created: August 2019
  URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include <TM1638plus_Model2.h>

#define  myTestDelay 5000
uint8_t  testcount = 0;

// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM 4  // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 6  // clock = GPIO connected to clock line of module
#define  DIO_TM 7 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see note in readme at URL
bool high_freq = false; //default false,, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);

void setup() {
  Serialinit();
  tm.displayBegin(); // Init the module
  delay(myTestDelay);

  // Test 0 reset test
  tm.reset();
}

void loop() {
  testcount++;
  
    switch (testcount)
  {
    case 1: Test1(); break; // Test 1 decimal numbers
    case 2: Test2(); break; // Test 2 Hexadecimal number
    case 3: Test3(); break; // Test 3a 3b & 3C using DisplaySegments method
    case 4: Test4(); break; // Test 4 strings
    case 5: Test5(); break; //  Test 5 ASCIItoSegment method 
    case 6: Test6(); break; //  Test 6  Brightness and reset
    case 7: Test7(); break;  //  Test 7 Buttons
  }
}

//Function to setup serial called from setup
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("-------------Comms UP------------");
}

void Test1(void)
{
  // Test 1 decimal numbers
  tm.DisplayDecNum(5, 1 << 6); // 00.000005
  delay(myTestDelay);
  tm.DisplayDecNum(99991111, 1 << 4); // 9999.1111
  delay(myTestDelay);
  tm.DisplayDecNum(-2888, 0 );  // -0002888
  delay(myTestDelay);
  tm.DisplayDecNum(33, 0 ); // 00000033
  delay(myTestDelay);
  tm.DisplayDecNum(-33, 0 , false); // "-33        "
  delay(myTestDelay);
  // Test 1b  decimal numbers with the DisplayDecNumNibble function divides display into two nibbles.
  tm.DisplayDecNumNibble(1234 , 78, 1<<4 , true); // "1234.0078"
  delay(myTestDelay);
}

void Test2(void)
{
  // Test 2 Hexadecimal number
  tm.DisplayHexNum(0x0000, 0x456E, 0x00, true); // 0000456E
  delay(myTestDelay);
  tm.DisplayHexNum(0xABCD, 0xEF23, 0, true); // ABCDEF23
  delay(myTestDelay);
  tm.DisplayHexNum(0x0000, 0x00FF, 1 << 4); // 0000.00FF
  delay(myTestDelay);
}

void Test3(void)
{
  // Test 3 manually set segments abcdefg(dp) = 01234567
  // display a one in position one "       1"
  tm.DisplaySegments(0, 0x00); //a
  tm.DisplaySegments(1, 0x01); //b, for b turn on digit one only
  tm.DisplaySegments(2, 0x01); //c, for c turn on digit one only
  tm.DisplaySegments(3, 0x00); //d
  tm.DisplaySegments(4, 0x00); //e
  tm.DisplaySegments(5, 0x00);
  tm.DisplaySegments(6, 0x00); //g
  tm.DisplaySegments(7, 0x00); //DP
  delay(myTestDelay);

  // Test 3b manually set segments
  // Display "00000005"
  tm.DisplaySegments(0, 0xFF); //a, turn a on for all digits
  tm.DisplaySegments(1, 0xFE); //b
  tm.DisplaySegments(2, 0xFF); //c
  tm.DisplaySegments(3, 0xFF); //d
  tm.DisplaySegments(4, 0xFE); //e
  tm.DisplaySegments(5, 0xFF); //f
  tm.DisplaySegments(6, 0x01); //g , for g middle segment, digit one only on
  tm.DisplaySegments(7, 0x00); //decimal point, turn off all decmial points
  delay(myTestDelay);

  // Test 3c manually set segments scroll g 
  // Display "-" countup to "--------"
  tm.reset();
  uint8_t dashvalue = 1;
  for (uint8_t j = 0; j < 8;  j++)
  {
    
    tm.DisplaySegments(6, dashvalue); //g scrolling 
    dashvalue = (dashvalue*2)+1; //1 to 256
    delay(1000);
  }
  delay(myTestDelay);

}

void Test4(void)
{
  // Test 4 strings
  tm.DisplayStr("helloYOU", 1); // "helloYOU."
  delay(myTestDelay);
  tm.DisplayStr("      Hi", 0x08); // "     . Hi"
  delay(myTestDelay);
  tm.DisplayStr("        ", 0Xff); // ". . . . . . . ."
  delay(myTestDelay);
  tm.DisplayStr(" helloU2", 0); // " helloU2"
  delay(myTestDelay);
  tm.DisplayStr("hello", 0);  // "hello   "
  delay(myTestDelay);
}

void Test5(void)
{
  // Test 5 ASCII to segments takes an array of bytes and displays them
  // without ref to the ASCII font table direct data to digits to displays 3F 3F 3F 6D 3F 3F 3F 6D = 00050005
  // gfedcba = 3F for zero https://en.wikipedia.org/wiki/Seven-segment_display
  
  const byte values[8] = {0x3F, 0x3F, 0x3F, 0x6D, 0x3F, 0x3F, 0x3F, 0x6D}; // for ascii to segment test 00050005
  
  tm.ASCIItoSegment(values);
  
  delay(myTestDelay);
}

void Test6(void)
{
  // Test 6  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.DisplayStr("brightnes", 1);
    delay(1000);
  }
  tm.reset();
}

void Test7(void)
{
  while(1)
  {
      // Test 7 , buttons, no debounce see notes at URL for example to debounce.
      // returns 0-16 , 0 for nothing pressed.
      unsigned char buttons = tm.ReadKey16();
      Serial.println(buttons);
      delay(250);
    }
}
