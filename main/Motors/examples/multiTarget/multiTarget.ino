#include <SB_Servo.hpp>
#include <vector>

/**
A quick program to try out the set multitarget function  

AHJ
*/

SB_Servo servo1(0);
SB_Servo servo2(1);
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
} 

void loop() { 
  	delay(6000);
    int goHere = Serial.parseInt();
    servo1.rotateToDegrees(10);
    delay(1000); // Delay to allow for the servo to actually get to the desired location before reading.
    servo2.rotateToDegrees(30);
    delay(1000);
    std::vector<SB_Servo> servos {servo1, servo2};
    std::vector<float> targets{ 90, 180};
    SB_Servo::setMultipleTargets(servos, targets);
    Serial.print("Expected degrees: ");
    Serial.println(goHere);
    Serial.print("Servo1: ");
    Serial.println(servo1.getCurrentDegrees());
    Serial.print("Servo2: ");
    Serial.println(servo2.getCurrentDegrees());
  //}
} 
