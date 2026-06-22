# Keyboard Creature  
An interactive ESP32-powered project made with salvaged electronics, sensors, motors, LEDs, and sound. The project combines a DC motor, servo motor, passive buzer, ultrasonic sesnor, button input, and 8 LEDs driven through a shift register to make one reactive embedded system. All designed to help you time your work and become more productive.  
  
![Keyboard Creature Demo](media/keyboardcreature.gif)  
  
## Purpose & Overview  
  
Keyboard Creature is a small interactive electronics project that lives under your keyboard or on your desk. It is designed to react to nearby movement and user input in order to help you work in timed intervals. Once turned on, the user has the option to select from 5,10,15 or 20 minutes of focused time. Once started, the keyboard creature will keep track of how much time left, but it will only count down if you are actually at your keyboard.  
  
It uses an ESP32 as the central microcontroller, reads distance data from an ultrasonic sensor, accepts button input, and controls multiple output systems includeing a pencil-sharpener DC motor, SG90 servo, passive buzzer, and 8 LEDs through an SN74HC595 shift register.  
  
The overall goal of this project learning-wise was to practice embedded systems integration: wiring multiple components, debugging power/control issues, using transistor switching and capacitors, driving LEDs through a shift register, and documenting the final product.  
  
- Short demo video: https://streamable.com/knix4x (Demo is done with the timer at 10 seconds)  
- Demo GIF: shown above  
- Final build photos: 'media/build-photos'  
  
## Features  
  
- ESP32-based control  
- Ultrasonic distance-based activation, to detect user presence  
- Button-controlled mode switching  
- DC motor control through transistor switching  
- SG90 servo movement  
- Passive buzzer sound effects  
- 8 LED animation system using SN74HC595 shift register  
- Modular test sketches for each subsystem  
- Fully documented wiring, pinout, and troubleshooting process  
  
## Hardware  
  
Found in 'hardware/BOM.csv'  
  
## Pinout  
  
Found in 'hardware/pinout.md'  

## System Architecture  
  
Button --------------- ESP32  
Ultrasonic sensor -------^  
  
ESP32  
|  
|----- DC Motor  
|  
|----- Servo  
|  
|------ Buzzer  
|  
------ Shift Register ----- LEDs  

  
