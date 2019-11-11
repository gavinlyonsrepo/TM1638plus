# Changelog

* version 1.0.0 June 2019
	* First release 
	* An Arduino library to display data on a 8-digit TM1638 seven segment module. This version is for the (KEY & LED) variant which has 8 LED's and 8 Push buttons.
	* Fork of "Tm1638lite" 
	* Much Improved memory footprint.
	* Support for Decimal point, Segment select and Brightness control added.

* Version 1.0.1 June 10 2019
	* Minor update
	* Compiler warnings of using non-constant strings in "displayText" methods
	 fixed with by declaring consts. 
	* Improvements to documentation and comments

* Version 1.1.1 August 2019
	* Added support for "model 2" Tm1638 module(the one with no LEDS and 16 push buttons )
	* This model is marked QFY-TM1638.

* Version 1.2.0 November 2019
	* Patch to deal with issue number 3 on github dealing 
	with swapped displays on some modules(model 2) with different wiring, See Note A in readme
	* Removed ASCII font to a separate file for efficiency purposes as it was duplicated in both header files.
	* Memory footprint decrease for Model 2 library by using Progmem for the ASCII font file data as per Model 1. 
