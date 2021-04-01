/**
 * A program to run tests on the rotateBy() method to ensure proper
 * execution by the maestro and teensy.
 * 
 * The program increments the servo in the 0th channel
 * from 0 to around 180, then decrements back to 0. 
 * The test results can be read on the serial monitor.
nckjdscnskjcnh
 * 
 * AHJ
 */

#include <SB_Servo.hpp>

MiniMaestro maestro(Serial1);

// Here we will use the default values 
// Which are consistent w/ the HS-422 servo 
SB_Servo testServo(0);
bool loopOnce = true;

void setup() {
	Serial.begin(9600);
	Serial1.begin(9600);
  randomSeed(analogRead(0));
  Serial.print("Initial servo degrees: ");
  Serial.println(testServo.getCurrentDegrees());
  Serial.println("Moving to 0");
  testServo.rotateToDegrees(0);
  delay(3000); 
}


void loop() { 
  if (!loopOnce) {
    // End of test
  } else { 
	double angle = 0;
	int rotateBy = 0;
	while (angle <= 180) { 
		rotateBy = random(30);	
		Serial.print("Rotating by: ");
		Serial.println(rotateBy);
		angle += rotateBy;
		Serial.print("Expected degrees: ");
		Serial.println(angle);
		Serial.print("Actual degrees (not to go over 180): ");	
		testServo.rotateBy(rotateBy);
		Serial.println(testServo.getCurrentDegrees());
		delay(700);
	}
  angle = testServo.getCurrentDegrees();
  Serial.println("Begin negative rotateBy() test!");
  while (angle > 0) { 
      rotateBy = -1*random(30);  
      Serial.print("Rotating by: ");
      Serial.println(rotateBy);
      angle += rotateBy;
      Serial.print("Expected degrees: ");
      Serial.println(angle);
      Serial.print("Actual degrees (not to go under 0): ");  
      testServo.rotateBy(rotateBy);
      Serial.println(testServo.getCurrentDegrees());
      delay(700);
    }
  	loopOnce = false;
  }
}

	

	

	
