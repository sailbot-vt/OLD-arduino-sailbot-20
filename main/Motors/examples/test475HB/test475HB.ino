/**
 * A program to run tests on the rotateBy() method to ensure proper
 * execution by the maestro and teensy.
 * 
 * A simple example for the 475HB, it needs some specific tuning
	Here we use the fancy paramaterized  constructor to provide all 
	of the necessary info for the 475HB servo. 
 * 
 * AHJ
 */

#include <SB_Servo.hpp>

MiniMaestro maestro(Serial1);

SB_Servo servo1(500, 2500, 0, 200, 3, 200, 0); // sepcifications for the 475B in the bay.

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial.print("Initial servo degrees: ");
  Serial.println(servo1.getCurrentDegrees());
  Serial.println("Moving to 0");
  servo1.rotateToDegrees(0);
  delay(3000); 
}


void loop() {
   if (Serial.available()) {
    int goHere = Serial.parseInt();
    servo1.rotateToDegrees(goHere);
    delay(300); // Delay to allow for the servo to actually get to the desired location before reading.
    Serial.print("Expected degrees: ");
    Serial.println(goHere);
    Serial.print("Actual degrees: ");
    Serial.println(servo1.getCurrentDegrees());
  } 

}

  

  

  
