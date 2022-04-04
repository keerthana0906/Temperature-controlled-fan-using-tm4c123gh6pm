# Temperature-controlled-fan-using-tm4c123gh6pm
Embedded Systems course project
We have made a temperature controlled fan using tm4c123gh6pm board ,NTC temperature sensor, DC motor and fan.

Circuit connections :

tm4c123gh6pm board  
+3.3V :  Vcc source for temperature sensor  
GND   : Common ground  
PB4   : Motor driver  
PE3   : Connected to NTC sensor and 10k resistor  

EN1 of motor driver : output from PB4  
EN2 of motor driver : common ground   
Vcc of motor driver : 9V from battery source  
GND of motor driver : Common Ground  

The temperature sensor will get the surrounding temperature and based on the temperature it will turn on the fan and control its speed

Further the LED present on tmc board will give an idea of the changing temperature. If the temperature is above 35 degree it will glow in red , if the temperature is between 30 to 35 degrees it will glow blue ,if the temperature is between 25 to 30 degrees it will glow green , for temperatures below 25 it glows in magenta.
