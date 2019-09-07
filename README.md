

Overview
--------------------------------------------
* Name: TM1638plus
* Description: An Arduino library to display data on a 8-digit TM1638 seven segment module
This module supports the (8 KEY & 8 LED) variant which has 8 LED's and 8 Push buttons,
and the (16 KEY)(QFY) variant which has 16 pushbuttons.
* Author: Gavin Lyons.
* Development platform: Arduino UNO


Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)

Features
----------------------


These TM1638 modules are commonly available. Connections to Arduino: 

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output

This library  supports two variants of the TM1638.

1. TM1638  8 KEY & 8 LED , 8 LEDS and 8 Push Buttons (MODEL 1)
2. TM1638 16 KEY , (QYF) 16 Push buttons (MODEL 2)

There are two sets of  files(header source and example) to support each 
due to the different way they are wired and driven.
The files for Model 2 are marked Model2.

**Model 1: TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)**

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)

![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
 
This variant consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons.

Two 4 digit 3461AS-1 (34 inch, 4 digit ,common Cathode,  decimal point, RED) are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).

The library memory footprint has been optimized and is just:
Sketch uses 2454 bytes (7%) of program storage space. Maximum is 32256 bytes.
Global variables use 54 bytes (2%) of dynamic memory. Maximum is 2048 bytes.

**Model 1 Library Functions**

The commented functions can be found in library header file *TM1638plus.h*.
The library support ASCII ,text ,Hex and allows for setting individual segments,
and the decimal point of segment.
The *TM1638plus_TEST.ino* contains a set of tests demonstrating library functions.

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
9. Manually set segments to create custom patterns.

**========================================================================**

**Model 2: TM1638 Module 16 Push buttons (16 KEY) (QYF)**

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16381.jpg)

![ sch ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16382.jpg)


They consist of an 8-digit seven segment display with decimal points,
and 16Push buttons.

Two 4 digit 3461BS-1 (34 inch, 4 digit ,common Anode,  decimal point, RED)are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.


**Model 2 Library Functions**

The commented functions can be found in library header file *TM1638plus_model2.h*.
The library support Strings , decimal ,Hex and allows for setting individual segments,
and the decimal point of segment.
The *TM1638plus_TEST_Model2.ino* contains a set of tests demonstrating library functions.

For more detailed information see commented headers in header file(.h).

1. Print an Hexadecimal number with or without leading zeros
2. Print an decimal number with or without leading zeros
3. Manually set segments to create custom patterns.
4. Print a text string
5. Read buttons status. User may have to debounce buttons depending on application.
See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) for debounce example(model1)
6. Reset and init module functions.
7. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change.
The DEFAULT_BRIGHTNESS define in header file. 
