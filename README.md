![ module pics ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16383.jpg)

Overview
--------------------------------------------
* Name: TM1638plus
* Description: 
An Arduino library to display data on a 8-digit TM1638 seven segment module.
This library supports the (8 KEY & 8 LED) variant which has 8 LED's and 8 Push buttons,
and the (16 KEY QFY) variant which has 16 pushbuttons.
* Main Author: Gavin Lyons.
* Development platform: See Note B in notes section below.
* History: see CHANGELOG.md in extra folder
* Contributors: [gabormay](https://github.com/gabormay)  

Table of contents
---------------------------

  * [Overview](#overview)
  * [Installation](#installation)
  * [Features](#features)
  * [Model One](#model-one)
  * [Model Two](#model-two)
  * [Notes](#notes)
  * [Memory](#memory)

Installation
------------------------------

The library is included in the official Arduino library manger and the optimum way to install it 
is using the library manager which can be opened by the *manage libraries* option in Arduino IDE. 
Search "tm1638" in search bar of library manager to find it.

See link below for instruction for this and for the other methods too.

[Installing Additional Arduino Libraries guide](https://www.arduino.cc/en/Guide/Libraries)


Features
----------------------

Connections to Arduino: 

1. GPIO = STB = Strobe
2. GPIO  = CLK  = Clock
3. GPIO = DIO = Data input / output
4. GND

This library supports two variants of the TM1638, which for purposes of this documentation, 
will be refered to as Model 1 and Model 2 henceforth. Keys = Push buttons.

| Model No | Module Name | LEDS | KEYS | 
| ------ | ------ |  ------ | ------ |
| One | TM1638 LED & KEY | 8 | 8 |
| Two | TM1638 KEYS QYF  | 0 | 16 |

There are two sets of files to support each model. I kept them separate as the models are wired quite different
Model 1 address by digit, while Model 2 address by segment. So the code is quite different for both.
The same ASCII font file is used by both sets of files.

| Model | Header | Code file | Example files | 
| ------ | ------ |  ------ | ------ |
| 1 | TM1638plus.h       | TM1638plus.cpp | TM1638plus_BASIC_TEST.ino TM1638plus_TEST.ino |
| 2 | TM1638plus_Model2.h  | TM1638plus_Model2.cpp | TM1638plus_BASIC_TEST_Model2 TM1638plus_TEST_Model2.ino TM1638plus_ADC_TEST_Model2.ino TM1638plus_SCROLL_TEST_Model2.ino |


Model One
--------------------------------------

TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)

![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
 
This variant consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons.

Two 4 digit 3461AS-1 (34 inch, 4 digit ,common Cathode,  decimal point, RED) are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).

**Model 1 Library Functions and example files**

The commented functions can be found in library header file TM1638plus.h.
The library support ASCII ,text ,Hex and allows for setting individual segments,
and the decimal point of segment.
The TM1638plus_TEST.ino contains a set of tests demonstrating library functions.

For more information see the commented headers in header file. 

1. Print an ASCII character.
2. Print an ASCII character with a dot/decimal point.
3. Print a Hexadecimal digit(0-15).
4. Print a long integer number with or without leading zeros.
5. Print two integer numbers (0-9999) to each nibble with or without leading zeros.
6. Print a text string(dots are replaced and dot is turned on preceding digit), 
"abc.def" becomes "abcdef" with c decimal point segment switched on.
7. Read buttons status. User may have to debounce buttons depending on application.
debouncing left out to minimise library size. see url below for debounce example.
8. Switch the 8 LEDS on and off.
9. Reset and init module functions.
10. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change.
The DEFAULT_BRIGHTNESS define in header file. 
11. Manually set segments to create custom patterns.


See [URL LINK](https://github.com/gavinlyonsrepo/Arduino_Clock_3) 
for a practical real world example of using this library,
including a example of debouncing the key presses.

Model Two
-----------------------------------------

TM1638 Module 16 Push buttons (16 KEY) (QYF).

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16381.jpg)

![ sch ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16382.jpg)


They consist of an 8-digit seven segment display with decimal points,
and 16 Push buttons.

Two 4 digit 3461BS-1 (34 inch, 4 digit ,common Anode,  decimal point, RED)are used in this module
giving a total of 8 digits. A TM1638 controller chip drives the unit.

NB :  If your display shows "56781234" for "12345678" see Notes section. Note A.

**Model 2 Library Functions and example files**

The commented functions can be found in library header file TM1638plus_Model2.h.
The library support Strings,decimal ,Hex ,raw ASCII data, setting individual segments,
and the decimal point.

For more detailed information on functions see commented headers in header file(.h).

1. Print a Hexadecimal number with or without leading zeros
2. Print a decimal number with or without leading zeros
3. Manually set segments to create custom patterns.
4. Print two 4 digit decimal to each nibble with or without leading zeros.
5. Print a text string, dot function supported. 
6. Read buttons status. User may want/have to debounce buttons depending on application.
See TM1638plus_ADC_TEST_Model2.ino for debounce button example.
7. Reset and init module functions.
8. Adjust brightness of module. Support 8 degree brightness adjustment.
If user wishes to change the default brightness at start-up change.
The "DEFAULT_BRIGHTNESS" define in header file. 
9. Print raw ASCII data without reference to font file.
10. Print two Decimal number(0-9999) in each nibble.
11. Example file general contains a set of general function tests: TM1638plus_TEST_model2.ino 
12. Example file ADC shows some practical data examples : TM1638plus_ADC_TEST_Model2.ino 
13. Example file for one method of scrolling text: TM1638plus_SCROLL_TEST_Model2.ino  

Notes
--------------------------

*NOTE A* : Swapped display Issue: Model 2 only

For Some users using this library the nibbles in information display byte 
where swapped around. This is because there are different modules on market with different wiring. 
See issue #3 on github called Swapped display :: "12345678" becomes "56781234". 
If you test library and you see this issue, in order to fix this when you declare the 
Object, set the fourth parameter "swap_nibbles" to True, The default is false.

| PCB Model Label | Operation | Object constructor |
| ------ | ------ | ------ | 
| QYF-TM1638 | default operation | TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, false) | 
| QYF-TM1638 -Ver 1.0 | Swapped display Fix | TM1638plus_Model2 tm(STROBE_TM, CLOCK_TM , DIO_TM, true)  | 


*NOTE B* :  High frequency micro-controllers.

This library uses Software SPI or "bit banging" and may not work fully on 
micro-controllers running at a very high frequency, without some adjustments to timing.
See issue 1 & 2 in github issues section for ESP32 untested possible fix.
The ESP32 and Teensy results have been sent to me, I don't have these MCU's them at time of writing.

| IC |  frequency | Model 1 | Model 2 |
| ------ | ------ | ------ | ------ | 
| Arduino UNO  |  32 MHz  | Working | Working |
| Arduino Nano  | 32 MHz  | Working| Working |
| ESP 32  |   240 MHz  | Issues with buttons function | no Data |
| ESP-12E ESP8266 | 80 and 160 MHZ  | Working | Working |
| Teensy 4.0| 150Mhz | Working | no Data |
| Teensy 4.0| 396Mhz | buttons not working , display issues | no Data |

*NOTE C* :  MicroChip PIC XC8 forks.

I have forked this library to the PIC for the XC8 compiler the forks can be found at [Link](https://github.com/gavinlyonsrepo/pic_16F18446_projects)

Memory
-------------------------------

1. Model 1 memory usage, basic hello world sketch.

Sketch uses 1488 bytes (4%) of program storage space. 
Global variables use 22 bytes (1%) of dynamic memory.

2. Model 2 memory usage, basic hello world sketch.

Sketch uses 1536 bytes (5%) of program storage space. 
Global variables use 23 bytes (1%) of dynamic memory.
