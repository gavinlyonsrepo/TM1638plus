#include <TM1638plus.h>

// I/O pins on the Arduino connected to strobe, clock, data
#define  STROBE_TM 4
#define  CLOCK_TM 7
#define  DIO_TM 8


TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM);

void setup() {
   // Test 0 reset test
   tm.setLED(0, 1);
   delay(1000);
   tm.reset();
  
  // Test 1  Brightness and reset
   for (uint8_t brightness = 0; brightness < 8; brightness++)
   {
          tm.brightness(brightness);
          tm.displayText("00000000");
          delay(1500);
   }
   tm.reset();
   
   // restore default brightness
   tm.brightness(0x02);
  
   //Test 2 ASCII display 2.348
    
   tm.displayASCIIwDot(0, '2');
   tm.displayASCII(1, '3');
   tm.displayASCII(2, '4');
   tm.displayASCII(3, '8');
   delay(2000);
  
   //TEST 3 single segment
   //In this case  segment g (middle dash) of digit position 7
   tm.display7Seg(7,0b01000000);
   delay(2000);
    
   // Test 4 Hex digits.
   tm.displayHex(0, 1);
   tm.displayHex(1, 2);
   tm.displayHex(2, 3);
   tm.displayHex(3, 4);
   tm.displayHex(4, 5);
   tm.displayHex(5, 6);
   tm.displayHex(6, 7);
   tm.displayHex(7, 8);
   delay(2000);
   tm.displayHex(0, 8);
   tm.displayHex(1, 9);
   tm.displayHex(2, 10);
   tm.displayHex(3, 11);
   tm.displayHex(4, 12);
   tm.displayHex(5, 13);
   tm.displayHex(6, 14);
   tm.displayHex(7, 15);
   delay(2000);

   // Test 5 TEXT  with dec point
   // abcdefgh with decimal point for c and d 
   tm.displayText("abc.d.efgh");
   delay(3000);
   
   // Test6  TEXT + ASCII combo 
   // ADC=2.548
   char text1[]= "ADC=.";
   tm.displayText(text1);
   tm.displayASCIIwDot(4, '2');
   tm.displayASCII(5, '5');
   tm.displayASCII(6, '4');
   tm.displayASCII(7, '8');
   delay(2000);
  
   //Test 7 buttons and LED test
   tm.displayText("buttons ");
}


void loop() {
   uint8_t buttons = tm.readButtons();
   doLEDs(buttons);
}

// scans the individual bits of value
void doLEDs(uint8_t value) {
  for (uint8_t position = 0; position < 8; position++) {
    tm.setLED(position, value & 1);
    value = value >> 1;
  }
}
