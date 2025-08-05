/*!
	@file     TM1638plus_BUTTON_Model1.ino
	@author   Gavin Lyons
	@brief    TM1638plus Model 1 button debouncing.
	@details  Short, long and release detection. Combo detection on buttons 0 + 1 only(more can be added)
*/

#include <TM1638plus.h>

// GPIO pins
#define STROBE_TM 4
#define CLOCK_TM  6
#define DIO_TM    7

TM1638plus tm(STROBE_TM, CLOCK_TM, DIO_TM, false);

// Config
constexpr uint8_t NUM_BUTTONS = 8;
constexpr unsigned long DEBOUNCE_MS = 50; // time taken to debounce a button in mS
constexpr unsigned long LONG_PRESS_MS = 800; // time taken to deinfe a long press on a button held done in mS

struct ButtonState {
	bool stableState = false;
	bool lastRaw = false;
	bool wasPressed = false;
	bool longPressReported = false;
	unsigned long lastChange = 0;
	unsigned long pressStart = 0;
};

ButtonState buttons[NUM_BUTTONS];

void setup() {
	tm.displayBegin();
	tm.displayText("--------");
	Serial.begin(38400);
	delay(1000);
}

void loop() {
	updateButtons();
}

void updateButtons() {
	uint8_t rawStates = tm.readButtons();
	unsigned long now = millis();
	for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
		bool raw = rawStates & (1 << i);
		if (raw != buttons[i].lastRaw) {
			buttons[i].lastChange = now;
			buttons[i].lastRaw = raw;
		}
		if ((now - buttons[i].lastChange) > DEBOUNCE_MS) {
			if (raw != buttons[i].stableState) {
				buttons[i].stableState = raw;
				// Button just pressed
				if (raw) {
					buttons[i].pressStart = now;
					buttons[i].longPressReported = false;
					buttonPressed(i);
				} else {
					if (!buttons[i].longPressReported)
						buttonReleased(i);
					buttons[i].wasPressed = false;
				}
			}
			// Long press detection
			if (raw && !buttons[i].longPressReported && (now - buttons[i].pressStart >= LONG_PRESS_MS)) {
				buttonLongPress(i);
				buttons[i].longPressReported = true;
			}
		}
	}
}

// Single button press
void buttonPressed(uint8_t id) {
	Serial.print("Short press on button ");
	Serial.println(id);
	tm.displayIntNum(1 << id, true, TMAlignTextLeft);
	buttons[id].wasPressed = true;
	// Check for combos
	checkCombos();
}

// Long press
void buttonLongPress(uint8_t id) {
	Serial.print("Long press on button ");
	Serial.println(id);
}

// Button released (only if not long pressed)
void buttonReleased(uint8_t id) {
	Serial.print("Released button ");
	Serial.println(id);
}

// Combo detection (example: buttons 0 + 1)
void checkCombos() {
	uint8_t state = 0;
  uint8_t buttons_0_1 = 0b00000011;
	for (uint8_t i = 0; i < NUM_BUTTONS; ++i) {
		if (buttons[i].stableState)
			state |= (1 << i);
	}
	if ((state & buttons_0_1) == buttons_0_1) {
		Serial.println("Combo: Buttons 0 + 1 pressed");
    tm.displayIntNum(buttons_0_1, true, TMAlignTextLeft);
	}
}
