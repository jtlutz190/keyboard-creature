#include <Arduino.h>

// SN74HC595 pins
#define DATA_PIN   19
#define CLOCK_PIN  25
#define LATCH_PIN  32

void writeLEDs(byte value) {
  digitalWrite(LATCH_PIN, LOW);

  // LSBFIRST worked with Q7 LED included
  shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);

  digitalWrite(LATCH_PIN, HIGH);
}

void setup() {
  pinMode(DATA_PIN, OUTPUT);
  pinMode(CLOCK_PIN, OUTPUT);
  pinMode(LATCH_PIN, OUTPUT);

  writeLEDs(0x00);
}

void loop() {
  // One LED at a time
  for (int i = 0; i < 8; i++) {
    writeLEDs(1 << i);
    delay(150);
  }

  // Fill up LEDs one by one
  for (int i = 0; i < 8; i++) {
    writeLEDs((1 << (i + 1)) - 1);
    delay(150);
  }

  // Empty down LEDs one by one
  for (int i = 7; i >= 0; i--) {
    writeLEDs((1 << i) - 1);
    delay(150);
  }

  // Blink all LEDs
  for (int i = 0; i < 3; i++) {
    writeLEDs(0xFF);
    delay(250);

    writeLEDs(0x00);
    delay(250);
  }
}
