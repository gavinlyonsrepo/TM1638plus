/*
  Project Name: TM1638
  File: TM1638plus_SCROLL_TEST_Model2.ino
  Description: demo file library for model 2 TM1638 module(16 KEY 16 pushbuutons).
  Carries out test demonstrating arduino library TM1638plus.

  Shows ONE possible method of scrolling text and data on the  display  uisng the string class.
  
  Author: Gavin Lyons.
  Created Jan 2020
  URL: https://github.com/gavinlyonsrepo/TM1638plus
*/

#include <TM1638plus_Model2.h>

// GPIO I/O pins on the Arduino connected to strobe, clock, data,
// pick on any I/O pin you want.
#define  STROBE_TM 4
#define  CLOCK_TM 6
#define  DIO_TM 7
bool swap_nibbles = false; //Optional , Default is false if left out, see note in readme at URL

// Constructor object Init the module
// strobe = GPIO connected to strobe line of module
// clock = GPIO connected to clock line of module
// data = GPIO connected to data line of module
// swap_nibbles = boolean default false, if true swaps nibbles on display byte
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles);

//Control Vars &  defines.
#define POT0 A0
#define POT1 A1
uint16_t sensorValue0 = 0;
uint16_t sensorValue1 = 0;

unsigned long previousMillis_display = 0;        // will store last time ADC was updated
const long interval_display = 1000;      // interval at which to read ADCs (milliseconds)

String textScroll;

void setup() {
  Serialinit();
}


//Function to setup serial called from setup FOR debug
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--Comms UP--TM1638plus_ADC_TEST_Model2.ino--");
  tm.reset();
}

void loop() {
  unsigned long currentMillis = millis();

  // Read the  Pots and update string every interval_display delay
  if (currentMillis - previousMillis_display >= interval_display)
  {
    previousMillis_display = currentMillis;
    // read the input on analog pin 0 and 1
    sensorValue0 = analogRead(POT0);
    sensorValue1 = analogRead(POT1);
    
    // Send ADC data to serial port DEBUG
    Serial.print("POT0 : ");
    Serial.println(sensorValue0);
    Serial.print("POT1 : ");
    Serial.println(sensorValue1);
    
    textScroll = "";
    textScroll = textScroll + "   ADC1 = " + sensorValue1 + " ADC0 = "+ sensorValue0; 
  }
  
   ModelTwo_Scroll(); 
}

//Function to scroll data passes back to program when finished. 
void ModelTwo_Scroll()
{
  char charbuf[9];
  while(textScroll.length()> 0)
  {
  
  //convert the string object to character array  
  textScroll.toCharArray(charbuf, 9);
  //display the character buffer
  tm.DisplayStr(charbuf);
  // decrement the string
   textScroll.remove(0, 1);
  delay(500);
  }  
}
