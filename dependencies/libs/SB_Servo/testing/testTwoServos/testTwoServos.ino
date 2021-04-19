/**
 * A script for testing two servos running at once
 * Performs the same basic rotateTo test but with two servos instead
 * 
 * AHJ
 */
#include <SB_Servo.hpp>

MiniMaestro maestro(Serial1);

// Here we will use the default values 
// Which are consistent w/ the HS-422 servo 
SB_Servo testServo1(0);
SB_Servo testServo2(1);
bool loopOnce = true;

void setup() {
	Serial.begin(9600);
	Serial1.begin(9600);
	Serial.println("Moving Servo1 to 0");
	testServo1.rotateToDegrees(0);
  delay(500);
	Serial.println("Moving Servo2 to 0");
	testServo2.rotateToDegrees(20);
  delay(3000); 
}


void loop() { 
  if (!loopOnce) {
	// End of test
  } else { 
	delay(3000);
	for (int i = 0; i < 181; i += 10) { 
		Serial.print("Expected degrees: ");
		Serial.println(i);
		testServo1.rotateToDegrees(i);
		testServo2.rotateToDegrees(i);
		Serial.print("Actual degrees Servo1 : ");	
		Serial.println(testServo1.getCurrentDegrees());
    delay(500);
		Serial.print("Actual degrees Servo2 : ");	
		Serial.println(testServo2.getCurrentDegrees());
		delay(300);
	}




  	loopOnce = false;
  }
}

	

	

	
