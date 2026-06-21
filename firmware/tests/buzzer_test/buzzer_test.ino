#include <Arduino.h>

#define BUZZER_PIN 33

void setup() {
  ledcAttach(BUZZER_PIN, 2000, 8); // pin, frequency, resolution
  ledcWriteTone(BUZZER_PIN, 0);
}

void loop() {
  ledcWriteTone(BUZZER_PIN, 2000); // buzzer ON
  delay(1000);

  ledcWriteTone(BUZZER_PIN, 0);    // buzzer OFF
  delay(1000);
}
