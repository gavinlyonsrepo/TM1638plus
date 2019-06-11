

Overview
--------------------------------------------
* Name: TM1638plus
* Description: An Arduino library to display data on a 8-digit TM1638 seven segment module
This module is for the (KEY & LED) variant which has 8 LED's and 8 Push buttons.
* Author: Gavin Lyons.
* Development platform: Arduino UNO


Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)

Features
----------------------


**TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)**

These Tm1638 modules are commonly available. 
They consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons.
I have seen two variants, this library is for the one marked LED & KEY.
It will not work with the push buttons only module without some modification,
I don't possess the push button only module at this point so cannot test at this time.

1. TM1638 LED & KEY , 8 LEDS and 8 Push Buttons
2. TM1638 KEY , 16 Push buttons

Two 4 digit 3461AS (common anode , RED , decimal point) are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).

The library memory footprint has been optimised and is just:
Sketch uses 2454 bytes (7%) of program storage space. Maximum is 32256 bytes.
Global variables use 54 bytes (2%) of dynamic memory. Maximum is 2048 bytes.


3 Connections to Arduino: 

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output


**Library Functions**

The commented functions can be found in library header file TM1638plus.h.
The library support ASCII ,text ,Hex and allows for setting individual segments,
and the decimal point of segment.
The TM1638plus_TEST.ino contains a set of tests demonstrating library functions.

For more information see commented headers in header file. 

1. Print an ASCII character.
2. Print an ASCII character with a dot/decimal point.
3. Print a Hexadecimal digit(0-15).
4. Print a text string(dots are replaced and dot is turned on preceding digit), 
"abc.def" becomes "abcdef" with c decimal point segment switched on.
5. Read buttons status. User may have to debounce buttons depending on application.
See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) for debounce example.
6. Switch LEDS on and off.
7. Reset and init module functions.
8. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change.
The DEFAULT_BRIGHTNESS define in header file. 


Installation
------------------------------

The library is included in the official Arduino library manger and the optimum way to install it 
is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 
Search "TM1638" in search bar of library manager to find it.

See link below for instruction for this and for the other installation methods too.

[Installing Additional Arduino Libraries guide](https://www.arduino.cc/en/Guide/Libraries)

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)


![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
