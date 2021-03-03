#include <SB_Servo.hpp>

MiniMaestro maestro(Serial1);

// Here we will use the default values 
// Which are consistent w/ the HS-422 servo 
SB_Servo testServo(0);
SB_Servo testServo1(2);
bool loopOnce = true;

void setup() { 
	Serial.begin(9600);
	Serial1.begin(9600);
}


void loop() { 
  if (!loopOnce) {
    
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

	

	

	
