/**
 * Runs using the 475 servo connected to channel 0 on the maestro
 * 
 * AHJ
 */

#include <SB_Servo.hpp>


// Here we will use the default values 
// Which are consistent w/ the HS-475
SB_Servo constrainedRange475(500, 2500, 0, 200, 3, 200, 0); 
SB_Servo constrainedAngle475(500, 2500, 0, 200, 50, 180, 0);

bool loopOnce = true;

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  delay(1000);
  constrainedRange475.rotateToDegrees(45);
  delay(3000);
}


void loop() { 
  if (!loopOnce) {
    // End of test
  } else {
     constrainedRange475.rotateToDegrees(0);
     Serial.println("rotating channel0 to 0 degrees, expected degrees: 3");
     Serial.print("Actual degrees: ");
     Serial.println(constrainedRange475.getCurrentDegrees());
     delay(1000);

     constrainedRange475.rotateToDegrees(150);
     Serial.println("rotating channel0 to 150 degrees, expected degrees: 150");
     Serial.print("Actual degrees: ");
     Serial.println(constrainedRange475.getCurrentDegrees());
     delay(1000);

     constrainedRange475.rotateToDegrees(250);
     Serial.println("rotating channel0 to 250 degrees, expected degrees: 200");
     Serial.print("Actual degrees: ");
     Serial.println(constrainedRange475.getCurrentDegrees());
     delay(1000);


     constrainedAngle475.rotateToDegrees(0);
     Serial.println("rotating channel0 w/ constraint to 0 degrees, expected degrees: 50");
     Serial.print("Actual degrees: ");
     Serial.println(constrainedAngle475.getCurrentDegrees());
     delay(1000);

   constrainedAngle475.rotateToDegrees(200);
     Serial.println("rotating channel0 w/ constraint to 200 degrees, expected degrees: 180");
     Serial.print("Actual degrees: ");
     Serial.println(constrainedAngle475.getCurrentDegrees());
     delay(1000);
		Serial.println("hello fart faces");
     
  }
  loopOnce = false;
}
