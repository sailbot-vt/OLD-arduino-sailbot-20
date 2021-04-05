#include <SB_Servo.hpp>

/**
	simple test using the 475 w/ some specific bounds
*/


SB_Servo testServo(500, 2500, 0, 200, 3, 200, 0);


bool loopOnce = true;

void setup() { 
	Serial.begin(9600);
	Serial1.begin(9600);
	/**
	Going to instantiate a bunch of 
	objects inorder to throw some errors
	*/
	SB_Servo negMinMS(-2, 200, 0, 200, 3, 200, 0); 
	SB_Servo overMinMS(250, 200, 0, 200, 3, 200, 0); 
	SB_Servo negMaxMS(0, -1, 0, 200, 3, 200, 0);     //                  ^
	SB_Servo overMaxMS(500, 200, 0, 200, 3, 200, 0); // I dont think     I these are actually descernible!!!

	SB_Servo minRangeNegative(500, 2500, -1, 200, 3, 200, 0); 
	SB_Servo minRangeOverMax(500, 2500, 215, 200, 3, 200, 0); 

	SB_Servo maxRangeNegative(500, 2500, 0, -1, 3, 200, 0); 
	SB_Servo maxRangeUnderMin(500, 2500, 215, 200, 3, 200, 0); 

	SB_Servo badAngle(500, 2500, 0, 200, 150, 100, 0);  // we really on need one min / max mixup because they're same thing 
	SB_Servo badChannelNum(500, 2500, 0, 200, 3, 200, 9);  // bad channel number
}

void loop() {

	Serial.println(


