/**
 * Rotates a servo connected to the maestro in channel 0
 * and rotates in from 0 to 180 degrees in 10 degree increments.
 * Reports back the position that the servo rotated to.
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
}


void loop() { 
  if (!loopOnce) {
    // End of test
  } else { 
	Serial.print("Initial servo degrees: ");
	Serial.println(testServo.getCurrentDegrees());
  Serial.println("Moving to 0");
  testServo.rotateToDegrees(0);

	delay(3000);
	for (int i = 0; i < 181; i += 10) { 
		Serial.print("Expected degrees: ");
		Serial.println(i);
		Serial.print("Actual degrees: ");	
		testServo.rotateToDegrees(i);
		Serial.println(testServo.getCurrentDegrees());
		delay(300);
	}
  loopOnce = false;
  }
}

	

	

	
