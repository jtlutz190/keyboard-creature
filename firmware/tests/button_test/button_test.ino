#include <Arduino.h>

// Button
#define BUTTON_PIN 4

// SN74HC595 pins
#define DATA_PIN   19
#define CLOCK_PIN  25
#define LATCH_PIN  32

void writeLEDs(byte value) {
  digitalWrite(LATCH_PIN, LOW);
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);
  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  writeLEDs(0x00);
}

void loop() {
  bool buttonPressed = digitalRead(BUTTON_PIN) == LOW;

  if (buttonPressed) {
    writeLEDs(0xFF); // all LEDs ON
  } else {
    writeLEDs(0x00); // all LEDs OFF
  }
}
