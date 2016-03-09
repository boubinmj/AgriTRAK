*ECE 387 Project 
*by: Adam Benjamin

*This program interfaces the DS18B20 waterproof temperature sensor and the eTape Continuous Fluid Level Sensor 
PN-12110215TC-8 with an Arduino Zero microcontroller.

*The code serially prints the temperature in celcius measured by the temperature sensor and the resistance of the eTape,
which is based on the water level. It also lights up LEDs based on if the temperature measured is above or below a certain
threshold.