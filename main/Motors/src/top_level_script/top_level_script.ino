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
//SB_Servo servo1(1);
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  while (!Serial1) { 
      
  }
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  //servo1.rotateToDegrees(500);
  digitalWrite(13, HIGH);
  myServo.attach(4);
  
} 

void loop() {
//  servo1.rotateToDegrees(100);
  
  //delay(400);
  //digitalWrite(13, LOW);
  //maestro.setTarget(0, 6000);
  if (Serial.available()) { 
    int val = Serial.parseInt();
    //Serial.println(val);
    maestro.setTarget(1, val);
    int pos = maestro.getPosition(1);
    Serial.print("Position: ");
    Serial.println(pos);
    myServo.write(val);
  }

  
  
} 

// /home/andy/Documents/Personal/SailBOT/ArduinoMotorControl2020/arduino-sailbot-20/main/Motors/src/sketch_nov20a/../SB_Servo.hpp
