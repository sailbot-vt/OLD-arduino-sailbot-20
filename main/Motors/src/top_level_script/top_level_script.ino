#include "SB_Servo.hpp"
MiniMaestro maestro(Serial1);


// It appears as though it really does not want to include things that are not in the top level directory 
// Yea just tested it, SB_Servo either has to be in this directory or in a sub-directory. Yes it works
// When the #include is in a sub directory from the top level script. Wow that is so annoying
#include "SB_Servo.hpp"
//SB_Servo servo1((ms_t) 2000, (ms_t) 10000, 0); // It appears that this doesn't work apparently
SB_Servo servo1(0);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
} 

void loop() { 
  
  if (Serial.available()) {
    int goHere = Serial.parseInt();
    Serial.println(goHere);
    servo1.rotateToDegrees(goHere);
    Serial.println(servo1.getCurrentDegrees());
  }

  
  
} 
