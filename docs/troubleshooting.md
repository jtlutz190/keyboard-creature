# Troubleshooting  
  
This document lists the problems faced creating the project and how they were solved.  
  
## DC motor flyback  
  
Initially, I had set up the DC motor with just the transistor switch and no flyback diode. This caused me to lose a transistor during testing. After doing some  
research, I added a diode across the motor terminals and the motor circuit worked smoothly.  
  
Learned: That a flyback diode is almost always necessary when working with a DC motor. Since motors are inductive loads, turning them off cause voltage spikes which may  
damage components like transistors or pins.  
  
## Passive buzzer unresponsive  
  
I was accidentally using a passive buzzer like an active buzzer, which was what I was used to. After observing clicking from the passive buzzer, I realized it was not an active buzzer and I began to use  
PWM to create sound. I had actually meant to use an active buzzer for this project. However, after accidentally using a passive one, I liked it a lot better.  
  
Learned: Active vs. Passive buzzers, ledcAttach and ledcWrite, PWM.  
  
## Ultrasonic sensor without voltage divider  
  
The ultrasonic sensor gave incorrect and stuck readings. Initially I had thought something was wrong with the ultrasonic sensor itself. However, after using a multimeter on the  
Echo pin, I learned that it needed to be reduced to a safer voltage before going into the GPIO pin. So, I created the voltage divider and it worked perfectly afterwards.  
  
Learned: Data transfer works on a regulated voltage logic, voltage dividers, ultrasonic sensor.  
  
## LED shift register not sending inputs

The LEDs connected to the SN74HC595N were not responding correctly. This was my most frustrating issue. I had tried redoing the C/C++ code, getting a new chip, and using a multimeter to learn  
where the problem was. After rewiring the data,clock, and latch wires to different GPIO pins, the system worked smoothly.

Learned: How to debug with a multimeter.
