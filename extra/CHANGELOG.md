# Changelog

* version 1.0.0 June 2019
	* First release 
	* An Arduino library to display data on a 8-digit TM1638 seven segment module. This version is for the (KEY & LED) variant which has 8 LED's and 8 Push buttons.
	* Fork of "Tm1638lite" 
	* Much Improved memory footprint.
	* Support for Decimal point, Segment select and Brightness control added.

* Version 1.1.0 August 2019
	* Added support for "model 2" Tm1638 module(the one with no LEDS and 16 push buttons )
	* This model is marked QFY-TM1638.

* Version 1.2.0 November 2019
	* Patch to deal with issue number 3 on github dealing 
	with swapped displays on some modules(model 2) with different wiring, See Note A in readme
	* Removed ASCII font to a separate file for efficiency purposes as it was duplicated in both header files.
	* Memory footprint decrease for Model 2 library by using Progmem for the ASCII font file data as per Model 1. 

* Version 1.3.0 Jan 2020
	* Two new example files for Model2, demonstrating practical examples and scrolling text
	* Example file for model 1 extended, demonstrating practical examples and scrolling text
	* New function displayIntNum(model 1 only ) For displaying integer numbers
	* New function  DisplayDecNumNibble,  allow two separate numbers to be sent to each nibble
	* Model 2 ReadKey16() button function optimised.
	* ASCII font reduced in size  { | } ~ characters removed. 
 
* Version 1.4.0 Feb 2020
	* "beginDisplay" method added.
	* Two new example files added.
	* Tested on Nodemcu ESP-12E ESP2866 , it works.
	
* Version 1.5.0 May 2020
	* Minor update to bring in pull request 7 to deal with github issues 6 and 7.
	* Display overflow check and dealing with multiple dots. 
	* Changes are for Model 1 only and "displayText" function only.
	* Pull request 1 by [gabormay](https://github.com/gabormay) 

* Version  1.6.0 August 2020
	* Pull request  2 by [centic9](https://github.com/centic9) Model 1 constructor duplicate code removed.
	* Closed  Issue 1 and 2 on github by adding support for high frequency MCU, such as ESP32
	The constructor of all models has a new parameter if set to true the code uses a custom  "shiftin" function
	* Closed Issue 8 on github, LKM1638 bi-colour LED module support added (named Model 3 for purposes of library )
	* New function setLEDs added to turn on all LEDs at once, for model 1 and 3.
	
* Version 1.7.0 October 2020
	* Added ReadKey16Two function to model  2  to detect multiple key presses see issue 12 on github.
	* TM1638_common files created for common code and settings between the different models
	* Example file added by [wunderbaum](https://github.com/wunderbaum) see  pull request 3.
	* Optimisations  to example files. 
	
