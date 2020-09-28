/*
  Project Name: TM1638
  File: TM1638plus_ADC_Model2.ino
  Description: demo file library for model 2 TM1638 module(16 KEY 16 pushbuttons).
  
  Carries out tests demonstrating arduino library TM1638plus displaying ADC data.
  
  Three functions / testmodes where "->" represenst delay between display modes 
  1. ADC1.ADC0 -> DATA.DATA , Demo:  DisplayDecNumNibble function
  2. ADC1.DATA -> DATA.ADC0 , Demo: mixing text label and integer number data
  3. ADC1.DA.TA -> DA.TA.VOLT, Demo: how to use floats
   
  Also demo buttons as button presses is used to switch between 
  testmodes 
  
  Board tested: UNO NANO
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
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object Init the module
// strobe = GPIO connected to strobe line of module
// clock = GPIO connected to clock line of module
// data = GPIO connected to data line of module
// swap_nibbles = boolean default false, if true swaps nibbles on display byte
// high_freq = boolean default false,, If using a high freq CPU > ~100 MHZ set to true. 

TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);

//Control Vars &  defines.
#define POT0 A0
#define POT1 A1
uint16_t sensorValue0 = 0;
uint16_t sensorValue1 = 0;
boolean DisplayMode = true;
uint8_t testMode = 1;
unsigned long previousMillis_display = 0;        // will store last time ADCs was updated
unsigned long previousMillis_button = 0;
const long interval_display = 3000;      // interval at which to read ADCs (milliseconds)
const long interval_button = 225;      // interval to read button

void setup() {
  Serialinit();
  tm.displayBegin();

}

//Function to setup serial called from setup FOR debug
void Serialinit()
{
  Serial.begin(9600);
  delay(100);
  Serial.println("--Comms UP--TM1638plus_ADC_TEST_Model2.ino--");
}


void loop() {
  unsigned long currentMillis = millis();
  uint8_t buttons = 0;

  // Read the  Pots and toggle display mode every interval_display delay
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
    DisplayMode = !DisplayMode;  // toggle displaymode
  }

  // Read the button everyinterval_button delay
  if (currentMillis - previousMillis_button >= interval_button)
  {
    previousMillis_button = currentMillis;
    buttons = tm.ReadKey16();
  }


  if (buttons != 0) {
    Serial.println(buttons);
    testMode = buttons;
  }

  switch (testMode)
  {
    case  1 : ModelTwoADC_One(); break; // ADC1.ADC0 -> DATA.DATA , Demo:  DisplayDecNumNibble function
    case  2 : ModelTwoADC_Two(); break; // ADC1.DATA -> DATA.ADC0 , Demo: mixing text label and data
    case  3 : ModelTwoADC_Three(); break; // ADC1.DA.TA DA.TA.VOLT, Demo: how to use non-integers
    default : tm.reset();
  }

}

// Test function to display data like 
//  ADC1.ADC0 -> DATA.DATA , Demo:  DisplayDecNumNibble function
void ModelTwoADC_One() {

  if (DisplayMode == true)
  {
    // "AAAA.BBBB" where AAAA is POT1 value and BBB is POT0 value
    tm.DisplayDecNumNibble(sensorValue1, sensorValue0, 1 << 4 , true);
  } else {
    tm.DisplayStr("ADC1ADC0", 1 << 4); // "ADC1.ADC0"
  }
}

// Test function to display data like 
// ADC1.DATA -> DATA.ADC0 , Demo: mixing text label and data
void ModelTwoADC_Two()
{
  char workstr[9];
  if (DisplayMode == true)
  {
    sprintf(workstr, "ADC1%04d", sensorValue1);
    tm.DisplayStr(workstr, 1 << 4 ); // ADC1.AAAA where AAAA is POT1 value
  } else {
    sprintf(workstr, "%04dADC0", sensorValue0);
    tm.DisplayStr(workstr, 1 << 4 ); // AAAA.ADC0 where BBBB is POT0 value
  }
}

// Test function to display data like 
// ADC1.DA.TA DA.TA.VOLT, Demo: how to use non-integers
void ModelTwoADC_Three()
{
  char workstr[9];
  uint16_t voltage;

  if (DisplayMode == true)
  {
    // ADC1.AA.AA where AAAA is POT0 converted to voltage value
    // To avoid using float we convert the ADC to a number represnting voltage ~ 500 = 5.00 volts
    //  5/1024 = .00488, .0048 * 10000 = 49,
    //  2.5 volt ~ 500ADC , so (500*49)/100 = 250,  parse 250 to make look like 02.50
    voltage = ((sensorValue1 * 49) / 100);
    sprintf(workstr, "ADC1%04d", voltage); //combine voltage with string
    tm.DisplayStr(workstr, 0x14 );

  } else {
    //Displays 12.45Volt
    float volts = 12.45;
    volts = volts * 100; // 1245
    dtostrf(volts, 4, 0, workstr);
    strcat(workstr, "VOLT"); // 1245Volt
    tm.DisplayStr(workstr, 0x50); //12.45.VOLT
  }


}
