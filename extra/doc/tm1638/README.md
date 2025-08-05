# TM1638 README

## Examples files

| List No | Example ino file name  | Description | Model |
| ------ | ------ |   ------ | ------ |
| 1 |TM1638plus_HELLOWORLD_Model1| basic use case |1|
| 2 |TM1638plus_TEST_Model1| Test sequence |1|
| 3 |TM1638plus_BUTTON_Model1| Demo showing how to de-bounce buttons |1|
| 4 |TM1638plus_HELLOWORLD_Model2| basic use case |2|
| 5 |TM1638plus_TEST_Model2| Test sequence |2|
| 6 |TM1638plus_ADC_Model2| Demo ADC |2|
| 7 |TM1638plus_MQTT_Model2| Demo MQTT by [wunderbaum](https://github.com/wunderbaum) |2|
| 8 |TM1638plus_TEST_Model3| Test sequence |3|

## Connections

TM1638 Connections to MCU: 

	* GPIO = STB = Strobe
	* GPIO = CLK = Clock
	* GPIO = DIO = Data input / output
	* GND
	* VCC 

## Model Types

### Model One

TM1638 Module 8 Push buttons 8 LEDS (LED & KEY)

![ module ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638.jpg)

![ sch ](https://github.com/gavinlyonsrepo/pic_16F18446_projects/blob/master/images/TM1638_2.jpg)
 
This variant consist of an 8-digit seven segment display with decimal points,
8 Leds and 8 Push buttons. Two 4 digit 3461AS-1 (.34 inch, 4 digit ,common Cathode,  decimal point, RED) are used in this module giving a total of 8 digits. A TM1638 controller chip drives the unit.
The unit is marked (LED & KEY).

The library support ASCII ,text ,Hex and allows for setting individual segments,
and the decimal point of segment. User may have to "debounce" the button read  method depending on application.
debouncing left out to minimise library size. Example files show some methods to do this.
User can Adjust brightness of module. Support 8 degree brightness adjustment.
It is possible to manually set segments to create custom patterns without reference to the ASCII font.

### Model Two

TM1638 Module 16 Push buttons (16 KEY) (QYF).

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16381.jpg)

![ sch ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16382.jpg)

They consist of an 8-digit seven segment display with decimal points,
and 16 Push buttons.Two 4 digit 3461BS-1 (.34 inch, 4 digit ,common Anode,  decimal point, RED)are used in this module giving a total of 8 digits. A TM1638 controller chip drives the unit.
NB :  If your display shows "56781234" for "12345678" see Notes section. Note 1.

Two different functions to read buttons. 
Raw ASCII data can be printed without reference to font file.

### Model Three

There are different PCB's of these modules on market, 
This library was tested on no 3 below. I think this software will work for all of them 
and the differences in PCB are related to connectors, layout and component placement. 
This module is a variant of Model 1. The differences are the LEDs are bigger and bi-color
both red and green, The seven segment display is larger and extra connectors are added for Daisy chaining. 
Two 4 digit KYX-5461AS-7.3 (.54 inch, 4 digit ,common cathode,  decimal point, RED)are used in this module
giving a total of 8 digits. 

1. LKM1638 v1.1
2. LKM1638 v1.2
3. TM1638 V1.3 

![ module ](https://github.com/gavinlyonsrepo/TM1638plus/blob/master/extra/images/tm16384.jpg)

*Model 3 LED Functions*

The code is the same as model 1 and there is one unique model 3 example file. 
setLED and setLEDs functions behaviour is the only difference in code base between 1 and 3.
SetLED: The difference is when you call the setLED function you pass the following to get LEDs to change colour. For more detailed information on functions see commented headers in header file(.h).

| Model | setLED Value | result | 
|    ----   |    ----   |    ----   |
|    1 & 3  |    0   |    LED off   |
|    3   |    1   |    Led green on  |
|    3   |    2   |    LED red on  |
|    1   |    1   |    LED on   |

SetLEDs: When you pass call the setLEDs function you can pass a word pattern where upper byte is turns LEDs green on and lower byte turns LEDs red on . Model one ignores lower byte, Set to 0x00 always. 

1. Model 3 setLEDs(word) = 0xGGRR
3. Model 1 setLEDs(word) = 0xRR00

## Notes

### Swapped data nibbles
 
Model 2
For Some users using this library the nibbles in information display byte 
where swapped around. This is because there are different versions of modules on market with different wiring.  See issue #3 on github called Swapped display :: "12345678" becomes "56781234". 
If you test library and you see this issue, in order to fix this when you declare the 
Object, set the fourth parameter "swap_nibbles" to True, The default is false.

| PCB Model noted Label | Operation | Object constructor 4th parameter |
| ------ | ------ | ------ | 
| QYF-TM1638 | default operation | false | 
| QYF-TM1638 -Ver 1.0 | Swapped display Fix |  true  | 

### Read buttons anomaly

Read buttons anomaly on High frequency micro-controllers.
This library uses a software SPI-like protocol and may not work fully on 
micro-controllers running at a very high frequency, without some adjustments to timing.
It is a SPI-like interface with a single bidirectional data wire DIO.
The TM1638 is basically a slow SPI device (< 500kHz) in DIO mode. The clock uses the equivalent of SPI mode 3 (normally high, clocks data on the rising edge). The problem is that the native Arduino shiftIn()/shiftOut() wire functions are simply too fast for this device (technically the clock signalling for the TM1638 is inverted but as it triggers on a rising edge still it is tolerant of that).
To make this work with fast devices, the shift clocking is slowed with a small delay (on the order of a microsecond).  

HighFreqshiftin  function:
 
As of version 1.6 a new parameter *(_HIGH_FREQ)* has been introduced to constructor it is false by default. Set to true for high frequency MCU ~> 100Mhz.  This should fix the issue of HF MCU not reading buttons correctly(ESP-Xs). The High_Freq parameter causes a custom shift-in function to be used. The delay in this function is fixed at 1 uS, it can be changed manually by adjusted  the defines in common header file.

The  Teensy results have been sent in by email, I don't have these MCU's them at time of writing. 

| IC |  frequency | Status | 
| ------ | ------ | ------ | 
| ATtiny85 | 1Mhz internal | Working | 
| Arduino UNO  |  16 MHz  | Working | 
| Arduino Nano  | 16 MHz  | Working |
| STM32 "blue pill" STM32F103C8T6 | 72Mhz | Working |
| ESP8266 | 160Mhz | Working |
| ESP 32  |   240 MHz  | Working, with high_freq set to  true | 
| Teensy 4.0| 150Mhz | Working model 1,  no Data rest of models |
| Teensy 4.0| 396Mhz | Not working on  m1 pre v1.6, no data after,  no Data rest of models |

### Multiple displays

Driving multiple displays.
It is possible to drive multiple modules. Share the DIO and CLK lines and use a unique
STB line for each device. see issue number 10 at github for example code.


### Multiple buttons pressed together

Detecting multiple buttons pressed together.

Model 1 and Model 3 CAN detect multiple buttons pressed.

Model 2 has two different functions:

1. ReadKey16 returns a byte with decimal value 1-16 this function cannot 
detect multiple buttons pressed.

2. ReadKey16Two returns a 16 bit integer where each bit corresponds to the 16 switch's.
However due to the wiring  of the module, see SG-X lines  on schematic,
Pressing Certain combinations of buttons will cause the  data on Seven Segments to 
change. So the simultaneous use of multiple key presses and the seven segments display 
is problematic. See issue 12 on github for more details.
