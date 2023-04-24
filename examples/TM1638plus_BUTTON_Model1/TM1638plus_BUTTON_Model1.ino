/*!
	@file     TM1638plus_BUTTON_Model1.ino
	@author   Gavin Lyons
	@brief    tm1638_plus ,Model 1 , Simple example of one possible method to "debounce" buttonsRead() method
  @note     Debouncing may or may not be necessary depending on application and the best way to achieve it will vary

*/

#include <TM1638plus.h> // include the library

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
// pick on any I/O you want.
#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7

bool high_freq = false; // default false, If using a high freq CPU > ~100 MHZ set to true. 

const long intervalButton = 200; // interval to read button (milliseconds)
const long intervalDisplay = 1000; // interval at which to change display (milliseconds)

// Constructor object (GPIO STB , GPIO CLOCK , GPIO DIO, use high freq MCU)
TM1638plus tm(STROBE_TM, CLOCK_TM , DIO_TM, high_freq);

void setup()
{
  tm.displayBegin(); 
  tm.displayText("00000000");
  Serial.begin(38400);
  delay(100);
  Serial.println("--Comms UP--TM1638plus_BUTTON_Model1.ino--");
  delay(5000);
}

void loop()
{
  uint8_t buttonsValue = buttonsRead();
  if (buttonsValue != 0){
    updateDisplay(buttonsValue);
  }

} // end of loop()

// Read and debounce the buttons from TM1638  every interval 
uint8_t buttonsRead(void)
{
  uint8_t buttons = 0;
  static unsigned long previousMillisButton = 0;  // executed once 
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillisButton >= intervalButton) {
    previousMillisButton = currentMillis;
    buttons = tm.readButtons();
  }
  return buttons;
}

// Change display every interval 
void updateDisplay(uint8_t buttonsValue)
{
  unsigned long currentMillis = millis();
  static unsigned long previousMillisDisplay = 0;  // executed once 
  if (currentMillis - previousMillisDisplay >= intervalDisplay)
  {
    previousMillisDisplay = currentMillis;
    tm.displayIntNum(buttonsValue, true, TMAlignTextLeft);
    Serial.println(buttonsValue, HEX);
  }
}