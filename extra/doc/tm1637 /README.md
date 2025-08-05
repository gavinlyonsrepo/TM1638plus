# TM1637 README


## Connections

TM1637 Connections to MCU: 

	* GPIO = CLK = Clock
	* GPIO = DIO = Data input / output
	* VCC 
	* GND

## Model Types

### Model Four

4 digits, there is a centered semi-colon. Example file = TM1637_TEST_Model4.
 
### Model Five

4 digits, each with a decimal point on each digit. Example file = TM1637_TEST_Model5.

## Comms delay

The Serial Communications delay used is set 
by default to 75 uS user can change this in constructor parameters.
