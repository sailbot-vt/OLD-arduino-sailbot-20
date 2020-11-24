//#include <PololuMaestro.h>
int msPosition;
char commandArray[1];

//MicroMaestro maestro(Serial1);


// It appears as though it really does not want to include things that are not in the top level directory 
// Yea just tested it, SB_Servo either has to be in this directory or in a sub-directory. Yes it works
// When the #include is in a sub directory from the top level script. Wow that is so annoying
#include "SB_Servo.hpp"

SB_Servo servo1;
void setup() {
  // put your setup code here, to run once:
  
servo1.rotateToDegrees(500);

} 

void loop() {

} 

// /home/andy/Documents/Personal/SailBOT/ArduinoMotorControl2020/arduino-sailbot-20/main/Motors/src/sketch_nov20a/../SB_Servo.hpp
