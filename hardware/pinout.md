# Pinout  
This document lists the final ESP32 pin assignments for the project.  
  
| Subsystem | ESP32 GPIO | Direction | Purpose / Notes |  
-----------------------------------------------------------  
| DC Motor | GPIO 18 | Output | Controls salvaged pencil sharpener motor via PN2222A transistor |  
| Servo Motor | GPIO27 | Output | Controls SG90 servo movement |  
| Passive buzzer | GPIO33 | Output | Driven with PWM using LEDC |  
| Button | GPIO4 | Input | Wired to gnd using INPUT_PULLUP, pressed = LOW |  
| Ultrasonic Echo | GPIO35 | Input | Reads echo signal through resistor divider |  
| Ultrasonic Trig | GPIO14 | Output | Sends trigger to ultrasonic sensor |  
| Shift register DATA | GPIO19 | Output | Sends LED data to SN74 chip |  
| Shift Reigster Clock | GPIO25 | Output | Clocks data into SN74 chip |  
| Shift Register Latch | GPIO32 | Output | Updates LED outputs |  
  
## Button  

GPIO 4 ===== Button  
Button ===== GND  
  
The button uses the ESP32 internal pull-up resistor.  
  
Pressed -> LOW  
Released -> HIGH  
  
## Ultrasonic Sensor  
  
VCC ===== Power rail (+)  
GND ===== GND  
Trig ===== GPIO14  
Echo ===== Voltage divider into GPIO35  
  
Voltage divider setup:  
  
Echo ==== 1kO  
1kO ==== GPIO35  
1kO ==== 2kO  
2kO ==== GND  
  
## Shift Register  
  
Pin16 / VCC ==(Side 2 of capacitor)=== 3.3v (ESP32 3V3)  
Pin8 / GND ==(Side 1 of capacitor)== GND  
Pin14 / DATA ==== GPIO19  
Pin11 / Clock ==== GPIO25  
Pin12 / Latch ==== GPIO32  
Pins 1-7 + Pin 15 / Q0-Q7 ==== LEDs through 330O resistors  
Pin13 / Output enable ==== GND  
Pin10 / SRCLR ==== 3.3v  
  
Pin16 is wired through the ESP32's 3.3v regulator to maintain the 3.3v logic the chip works under.  
A ceramic capacitor across + and - is used to reduce noise.  
  
## Servo  
  
VCC ==== Power rail (+)  
GND ==== GND  
Signal ==== GPIO27  
  
## DC Motor  
  
Motor(+) ==(Striped side of diode)==(Side 1 of capacitor)== Power rail(+)  
Motor(-) ==(Non-striped side of diode)==(Side 2 of capacitor)== Transistor (Collector)  
Transistor (Base) ===== Resistor 1kO ====== GPIO18  
Transistor (Emitter) ===== GND  
A ceramic capacitor is used across + and - in order to reduce noise.  
  
## Buzzer (passive)  
  
GPIO33 ===== Buzzer(+)  
Buzzer(-) ===== GND  
  
PWM is utilized on GPIO33 in order to create tones.  
  
## Battery pack  
  
AA Battery Holder A (+) ==== AA Battery Holder B (-)  
AA Battery Holder B(+) ===== Power rail (+)  
AA Battery Holder A (-) ===== Power rail (-)  
  
Two 2x AA Batteries in series to add their voltages by series rule for voltage.  
