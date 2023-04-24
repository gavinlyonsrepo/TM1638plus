/*!
	@file     TM1638plus_HELLOWORLD_Model2.ino
	@author   Gavin Lyons
	@brief 
		A Basic test file for model 2, TM1638 module(16 KEY 16 pushbuttons).
		Carries out series most basic test , "hello world" helo wrld
*/

#include <TM1638plus_Model2.h>

// GPIO I/O pins on the Arduino connected to strobe, clock, data, pick on any I/O pin you want.
#define  STROBE_TM 4  // strobe = GPIO connected to strobe line of module
#define  CLOCK_TM 6  // clock = GPIO connected to clock line of module
#define  DIO_TM 7 // data = GPIO connected to data line of module
bool swap_nibbles = false; //Default is false if left out, see issues section in readme at URL
bool high_freq = false; //default false, If using a high freq CPU > ~100 MHZ set to true. 

// Constructor object Init the module
TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, swap_nibbles, high_freq);


void setup() {
 tm.displayBegin();
}

void loop() {
	tm.DisplayStr("helowrld", 0);  // Display "helowrld" in seven segments with zero decimal points set.
	delay(5000);
}
