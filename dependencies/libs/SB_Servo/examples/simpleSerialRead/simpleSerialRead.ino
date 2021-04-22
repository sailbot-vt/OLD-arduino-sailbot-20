#include <SB_Servo.hpp>

/**
A very simple program to read off of the serial monitor and then rotate from there

AHJ
*/

SB_Servo servo1(0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
} 

void loop() { 
  if (Serial.available()) {
    int goHere = Serial.parseInt();
    servo1.rotateToDegrees(goHere);
    delay(100); 
    Serial.print("Expected degrees: ");
    Serial.println(goHere);
    Serial.print("Actual degrees: ");
    Serial.println(servo1.getCurrentDegrees());
  }
} 
