# Design decisions  
  
This document explains my thought process on multiple aspects of the project.  
  
## Using ESP32  
  
I chose to build with the ESP32 because of the multiple pins it has for input and PWM capabilities for the servo and passive buzzer.  
Also, I plan on developing an app that is used on the user's computer in the future. Fortunately, ESP32 has bluetooth capabilities which makes this possible.  
  
## Using a DC salvaged motor.  
  
The DC motor was salvaged from an electric pencil sharpener. Originally, I wanted to open the pencil sharpener to see how it worked on the inside. After doing so, I actually  
gained inspiration to use the motor to make something cooler, which inspired this project. I think it's super cool reusing existing parts giving the project a "Frankenstein" feel. I plan on doing this more  
in the future.  
  
## Using LED shift register  
  
Instead of hooking up 8 LEDs to 8 pins on the ESP32, I decided to get outside of my comfort zone. By using the SN74HC595N chip, I am able to control all 8 LEDs with 3 GPIO pins and learn  
a cool new chip in the process.  
  
## Using capacitors  
  
Initially, the DC motor would use too much current upon startup causing the LEDs to flicker when they weren't supposed to. So, after doing some research, I discovered  
the power of using capacitors across the positive and negative terminals in order to make the startup smoother. Afterwards, two more were placed across the motor and shift register to reduce noise.  
In this process I learned intuitively how capacitors work and physically how they can help balance load upon startup.  
  
## Shoebox enclosure  
  
The whole project is housed inside of a shoebox as an effective but low-cost container. It is also a decent size for living under a keyboard. There is a giant flap on the top of the shoebox which makes debugging and  
looking at the circuit extremely easy. Something I could add in the future would be a 3D printed or laser-cut enclosure to more accurately fit the size of a keyboard. Finally, it gives the project a scrappy, home-made aesthetic.  
