#include <SB_Servo.hpp>

/**
	simple test using the 475 w/ some specific bounds
*/





bool loopOnce;

void setup() { 
	Serial.begin(9600);
	Serial1.begin(9600);
  loopOnce = true;
  delay(1000);
	/**
	Going to instantiate a bunch of 
	objects inorder to throw some errors
	*/
	
}

void loop() {

  if (!loopOnce)  { 
//    //SSerial.println("Here");
  } 
  else { 
    SB_Servo controlServo(500, 2500, 0, 200, 3, 200, 0);

  SB_Servo negMinMS(-2, 200, 0, 200, 3, 200, 0); 
  SB_Servo negMaxMS(0, -1, 0, 200, 3, 200, 0); 
  SB_Servo badMS(250, 200, 0, 200, 3, 200, 0); 

  SB_Servo minRangeNegative(500, 2500, -1, 200, 3, 200, 0); 
  SB_Servo maxRangeNegative(500, 2500, 0, -1, 3, 200, 0); 
  SB_Servo badRange(500, 2500, 215, 200, 3, 200, 0); 

  SB_Servo minAngleNegative(500, 2500, 0, 200, -1, 200, 0); 
  SB_Servo maxAngleNegative(500, 2500, 0, 200, 0, -3, 0); 
  SB_Servo badAngle(500, 2500, 0, 200, 150, 50, 0); 

  SB_Servo angleUnderRange(500, 2500, 50, 200, 40, 200, 0); 
  SB_Servo angleOverRange(500, 2500, 0, 200, 3, 250, 0); 
  SB_Servo angleOutOfBounds(500, 2500, 50, 150, 0, 200, 0); 
  SB_Servo badChannelNum(500, 2500, 0, 200, 3, 200, 9);  // bad channel number
    
    
  	Serial.print("Expected error code for negative minimum us: ");
  	Serial.println(US_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(negMinMS.getErrorCode());
  	Serial.print("Expected error code for negative max us: ");
  	Serial.println(US_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(negMaxMS.getErrorCode());
  	Serial.print("Expected error code for max us under minimum us: ");
  	Serial.println(US_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(badMS.getErrorCode());
  
  	Serial.print("Expected error code for minimum range negative: ");
  	Serial.println(RANGE_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(minRangeNegative.getErrorCode());
  	Serial.print("Expected error code for maximum range negative: ");
  	Serial.println(RANGE_ERROR_BIT | ANGLE_ERROR_BIT); // max range is neg, thus is less than the angle too
  	Serial.print("Actual: ");
  	Serial.println(maxRangeNegative.getErrorCode());
  	Serial.print("Expected error code for max range under min range: ");
  	Serial.println(RANGE_ERROR_BIT | ANGLE_ERROR_BIT); // max range is less than angle
  	Serial.print("Actual: ");
  	Serial.println(badRange.getErrorCode());
  
  	Serial.print("Expected error code for min angle negative: ");
  	Serial.println(ANGLE_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(minAngleNegative.getErrorCode());
  	Serial.print("Expected error code for max angle negative: ");
  	Serial.println(ANGLE_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(maxAngleNegative.getErrorCode());
  	Serial.print("Expected error code for max angle under min angle: ");
  	Serial.println(ANGLE_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(badAngle.getErrorCode());


    Serial.print("Expected error code for min angle under min range: ");
    Serial.println(ANGLE_ERROR_BIT); 
    Serial.print("Actual: ");
    Serial.println(angleUnderRange.getErrorCode());
    Serial.print("Expected error code for max angle over max range: ");
    Serial.println(ANGLE_ERROR_BIT); 
    Serial.print("Actual: ");
    Serial.println(angleOverRange.getErrorCode());

    Serial.print("Expected error code for alotted angles out of the spec'd range: ");
    Serial.println(ANGLE_ERROR_BIT); 
    Serial.print("Actual: ");
    Serial.println(angleOutOfBounds.getErrorCode());
  
  
  	Serial.print("Expected error code for bad channel number: ");
  	Serial.println(CHANNEL_ERROR_BIT); 
  	Serial.print("Actual: ");
  	Serial.println(badChannelNum.getErrorCode());

    delay(500);

    Serial.print("Expected error code for rotateTo() below bounds: ");
    Serial.println(ROTATE_TO_UNDER_ERROR_BIT); 
    Serial.print("Actual: ");
    controlServo.rotateToDegrees(0);
    Serial.println(controlServo.getErrorCode());
    controlServo.clearErrorCode();

    delay(500);

    Serial.print("Expected error code for rotateTo() above bounds: ");
    Serial.println(ROTATE_TO_OVER_ERROR_BIT); 
    Serial.print("Actual: ");
    controlServo.rotateToDegrees(250);
    Serial.println(controlServo.getErrorCode());
    controlServo.clearErrorCode();

    delay(500);

    Serial.print("Expected error code for rotateBy() below bounds: ");
    Serial.println(ROTATE_BY_UNDER_ERROR_BIT); 
    Serial.print("Actual: ");
    controlServo.rotateToDegrees(40);
    controlServo.rotateBy(-40);
    Serial.println(controlServo.getErrorCode());
    controlServo.clearErrorCode();

    delay(500);

    Serial.print("Expected error code for rotateBy() above bounds: ");
    Serial.println(ROTATE_BY_OVER_ERROR_BIT); 
    Serial.print("Actual: ");
    controlServo.rotateToDegrees(140);
    controlServo.rotateBy(100);
    Serial.println(controlServo.getErrorCode());
    controlServo.clearErrorCode();

    delay(500);


   
    loopOnce = false;
  }
}
