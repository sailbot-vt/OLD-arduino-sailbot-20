#include <PololuMaestro.h>
#include <Servo.h>
int msPosition;
char commandArray[1];

MiniMaestro maestro(Serial1);


// It appears as though it really does not want to include things that are not in the top level directory 
// Yea just tested it, SB_Servo either has to be in this directory or in a sub-directory. Yes it works
// When the #include is in a sub directory from the top level script. Wow that is so annoying
#include "SB_Servo.hpp"
Servo myServo;
SB_Servo servo1((ms_t) 2000, (ms_t) 10000, 0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
} 

void loop() { 
  
  if (Serial.available()) {
    /** 
    int val = Serial.parseInt();
    maestro.setTarget(0, val);
    int pos = maestro.getPosition(0);
    Serial.print("Position: ");
    Serial.println(pos);
    */
    //servo1.rotateToDegrees(Serial.parseInt());
    servo1.rotateBy(Serial.parseInt());
    
  }

  
  
} 
