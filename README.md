
Overview
--------------------------------------------
* Name: TM1638plus
* Description: An Arduino library to display data on a 8-digit TM1638 seven segment module
This module supports the (8 KEY & 8 LED) variant which has 8 LED's and 8 Push buttons,
and the (16 KEY QFY) variant which has 16 pushbuttons.
* Author: Gavin Lyons.
* Development platform: Arduino UNO and NANO


Table of contents
---------------------------

  * [Overview](#overview)
  * [Features](#features)

Features
----------------------

These TM1638 modules are commonly and widely available. 

Connections to Arduino: 

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output

This library supports two variants of the TM1638, which for purposes of the Documentation, I will refer to as Model 1 and Model 2. Keys = Push buttons.

| Model No | Module Name | LEDS | KEYS | 
| ------ | ------ |  ------ | ------ |
| One | TM1638 LED & KEY | 8 | 8 |
| Two | TM1638 KEYS QYF  | 0 | 16 |

There are two sets of files to support each model. I kept them separate as the models are wired quite different
Model 1 address by digit, while Model 2 address by segment. So the code is quite different for both.
The same ASCII font file is used by both sets of files.

| Model | Header | Code file | Example file | 
| ------ | ------ |  ------ | ------ |
| One | TM1638plus.h       | TM1638plus.cpp | TM1638plus_TEST.ino |
| Two | TM1638plus_Model2.h  | TM1638plus_Model2.cpp | TM1638plus_TEST_Model2.ino |


**Model 1: TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)**

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)

![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
 
This variant consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons.

Two 4 digit 3461AS-1 (34 inch, 4 digit ,common Cathode,  decimal point, RED) are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).


**Model 1 Library Functions**

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
9. Manually set segments to create custom patterns.

**========================================================================**

**Model 2: TM1638 Module 16 Push buttons (16 KEY) (QYF)**

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16381.jpg)

![ sch ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16382.jpg)


They consist of an 8-digit seven segment display with decimal points,
and 16Push buttons.

Two 4 digit 3461BS-1 (34 inch, 4 digit ,common Anode,  decimal point, RED)are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.


NOTE A : Swapped display Issue:
For Some users using this library the nibbles in information display byte 
where swapped around. This is because there are different modules on market with different wiring. 
See issue #3 on github called Swapped display :: "helloU2" becomes "loU2 hel". 
If you test library and you see this issue, in order to fix this when you declare the 
Object, set the fourth parameter "swap_nibbles" to True, The default is false.

| PCB Model Label | Operation | Object constructor |
| ------ | ------ | ------ | 
| QYF-TM1638 | Normal operation |     TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, false) | 
| QYF-TM1638 -Ver 1.0 | Swapped display Fix | TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, true)  | 

NOTE B : ESP32 timing issue:
See issue #2 on github. Some users have reported timing issues on ESP32, I have not tested nor do I possess at the  time of writing an ESP32. See issue comments for possible fix. The library is tested on UNO and a NANO.

**Model 2 Library Functions**

The commented functions can be found in library header file TM1638plus_Model2.h.
The library support Strings , decimal ,Hex and allows for setting individual segments,
and the decimal point of segment.
The TM1638plus_TEST_Model2.ino contains a set of tests demonstrating library functions.

For more detailed information see commented headers in header file(.h).

1. Print an Hexadecimal number with or without leading zeros
2. Print an decimal number with or without leading zeros
3. Manually set segments to create custom patterns.
4. Print a text string, dot function supported. 
5. Read buttons status. User may have to debounce buttons depending on application.
See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) for debounce example( model1 )
6. Reset and init module functions.
7. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change.
The DEFAULT_BRIGHTNESS define in header file. 
