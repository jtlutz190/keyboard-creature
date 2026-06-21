#include <Arduino.h>

#define DC_MOTOR_PIN 18

void setup() {
  pinMode(DC_MOTOR_PIN, OUTPUT);
  digitalWrite(DC_MOTOR_PIN, LOW);
}

void loop() {
  digitalWrite(DC_MOTOR_PIN, HIGH); 
  delay(1000);

  digitalWrite(DC_MOTOR_PIN, LOW);  
  delay(1000);
}

// simply turning on and off the motor about a constant interval
