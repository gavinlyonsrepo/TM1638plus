# MAX7219 README


## Description

An Arduino library to display data on a 8-digit MAX7219 seven segment module.
Support for BCD font, hardware or software SPI, Shutdown mode, test mode and Brightness control.

## Example files 

| List No | Example file name  | Description |
| ------ | ------ |   ------ |
| 1 | MAX7219_TEST_Model6.ino |Test sequence |
| 2 | MAX7219_BCD_Model6.ino | Shows use of BCD built-in font, if the BCD font is used the seven segment font can be discarded to save memory |
| 3 | MAX7219_CASCADE_Model6.ino | Simple demo showing use of cascaded displays |

## Hardware

Switch between software SPI or hardware SPI control by choosing relevant constructor.

### Software SPI

For Software SPI pick any GPIO you want.
User may need to change thebCommDelay variable (uS Communication delay) 
depending on speed of CPU on system by default its 0 uS.

### Hardware SPI

For Hardware SPI the User must use 2 fixed SPI pins, MOSI and SCLK. User can choice any GPIO for
for chip select pin. SPI settings(baudrate etc) can be changed in the header file.

### Connections 

| Pin no  | SW SPI | HW SPI  | Pin function |
| --- | --- | --- | --- |
| 1 | any GPIO output | SCLK |  CLK = Clock |
| 2 | any GPIO output | any GPIO Output | CS = Chip select |
| 3 | any GPIO output | SDATA |  DIN = Data in |

## Notes and Issues

### Cascaded Displays

Support for Cascaded Displays is shown in one of examples files. 
Multiple displays can be connected together. 
Cascaded Displays are displays connected together.  CS and CLK lines are tied together.
The DIN line is connected to the first display only. 
The DOUT line of the first display is then connected to the DIN line of the next 
display and so on. Diagram below shows how to connect two displays together. 
Specify number of displays in constructor parameter.

![img Cascade](https://github.com/gavinlyonsrepo/displaylib_LED_PICO/blob/main/extra/image/max7219cascade.png)
