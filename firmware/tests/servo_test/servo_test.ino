#include <Arduino.h>
#include <ESP32Servo.h>

#define SERVO_PIN 27

Servo testServo;

void setup() {
  testServo.attach(SERVO_PIN, 500, 2400);
  testServo.write(90); // stop/center
}

void loop() {
  testServo.write(180); // max one direction / 180 position
  delay(1000);

  testServo.write(90);  // stop/center
  delay(1000);
}
