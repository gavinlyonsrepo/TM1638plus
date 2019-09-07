/*
  Project Name: TM1638
  File: TM1638plus_TEST_Model2.ino
  Description: demo file library for model 2 TM1638 module(16 KEY 16 pushbuutons).
  Carries out series of tests demonstrating arduino library TM1638plus.
  Author: Gavin Lyons.
  Created August 2019
  URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include <TM1638plus_Model2.h>

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
//pick on any I/O you want.
#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

//Constructor object
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM);


void setup() {
  Serialinit();

  // Test 0 reset test
  tm.reset();

  // Test 1 decimal numbers
  tm.DisplayDecNum(5, 1 << 6); // 00.000005
  delay(2000);
  tm.DisplayDecNum(99991111, 1 << 6); // 99.991111
  delay(2000);
   tm.DisplayDecNum(2222, 1 );  // 00002222.
  delay(2000);
   tm.DisplayDecNum(33, 0 ); // 00000033
  delay(2000);
  tm.DisplayDecNum(33, 0 ,false); // "33        "
  delay(2000);
  
  // Test 2 Hexadecimal number
  tm.DisplayHexNum(0x456E, 1 << 3, false); //456E .  "
  delay(2000);
  tm.DisplayHexNum(0xFF, 1 << 4,false); // "FF  .   "
  delay(2000);
   tm.DisplayHexNum(0xFF, 1 << 4); // 0000.00FF
  delay(2000);
  
  // Test 3 manually set segments
  // display a one in position one "       1"
  tm.DisplaySegments(0, 0x00); //a
  tm.DisplaySegments(1, 0x01); //b for b turn on digit one only 
  tm.DisplaySegments(2, 0x01); //c for c turn on digit one only
  tm.DisplaySegments(3, 0x00);
  tm.DisplaySegments(4, 0x00);
  tm.DisplaySegments(5, 0x00);
  tm.DisplaySegments(6, 0x00); //g
  tm.DisplaySegments(7, 0x00);
  delay(2000);

  // Test 3b manually set segments
  // Display "00000005"  
  tm.DisplaySegments(0, 0xFF); //a
  tm.DisplaySegments(1, 0xFE);
  tm.DisplaySegments(2, 0xFF);
  tm.DisplaySegments(3, 0xFF);
  tm.DisplaySegments(4, 0xFE);
  tm.DisplaySegments(5, 0xFF);
  tm.DisplaySegments(6, 0x01); //g
  tm.DisplaySegments(7, 0x00); //decimal point
  delay(2000);


  // Test 4 strings 
  tm.DisplayStr("helloYOU", 1); // "helloYOU."
  delay(2000);
  tm.DisplayStr("      Hi", 0x08); // "     . Hi"
  delay(2000);
  tm.DisplayStr("        ", 0Xff); // ". . . . . . . ."
  delay(2000);
   tm.DisplayStr(" helloU2", 0); // " helloU2"
  delay(2000);
   tm.DisplayStr("hello", 0);  // "hello   "
  delay(2000);
  
  // Test 5  Brightness and reset
  for (uint8_t brightness = 0; brightness < 8; brightness++)
  {
    tm.brightness(brightness);
    tm.DisplayStr("brightnes", 1);
    delay(1000);
  }
  tm.reset();

}

void loop() {
  // Test 6 buttons 
  unsigned char buttons = tm.ReadKey16();
  Serial.println(buttons);
  delay(250);
}



//Function to setup serial called from setup
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("-------------Comms UP------------");
}
