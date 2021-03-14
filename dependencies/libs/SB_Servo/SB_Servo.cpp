/**
 * Source file for SB_Servo.hpp, contains function definitions 
 * to implement the SailBot 2021 Servo wrapper 
 *
 * AHJ
 */

#include "SB_Servo.hpp"

// Here the maestro is initialized to Serial1 on the Teensy, this is just one of 8 ports 
// pretty neat. I suppose this is how static private variables are initialized
MiniMaestro SB_Servo::maestro(Serial1);


int SB_Servo::servoCount{0};
/** 
 * uses point slope form to calculate a ms value for the maestro for a given degree
 */ 

int SB_Servo::degToUS(float degree) {
	return ((float) (maxUS - minUS) / maxDegreeRange) * (degree) + minUS;
}


float SB_Servo::usToDegrees(int ms) { 
	return (maxDegreeRange / ((float) maxUS - minUS)) * (ms - minUS) + minDegreeRange;
}

SB_Servo::SB_Servo(int channel) : SB_Servo(DEFAULT_MIN_US, DEFAULT_MAX_US, channel) {}

SB_Servo::SB_Servo(int minUS, int maxUS, int channel) : 
	SB_Servo(
		minUS, maxUS, 
		(float) DEFAULT_MIN_ANGLE, (float) DEFAULT_MAX_ANGLE, (float) DEFAULT_MIN_ANGLE, (float) DEFAULT_MAX_ANGLE, channel) {}


SB_Servo::SB_Servo(int minimumUS, int maximumUS, float minimumRange, float maximumRange, 
	float minimumAngle, float maximumAngle, int channel) : 
		minUS(4 * minimumUS),
		maxUS(4 * maximumUS), 
		minDegreeRange(minimumRange),
		maxDegreeRange(maximumRange),
		minAngle(minimumAngle),
		maxAngle(maximumAngle),
		channelNum(channel), 
		servoNumber(servoCount++) { 
	
	checkMinUS();	
	checkMaxUS();	
	checkMinDegreeRange();
	checkMaxDegreeRange();
	checkMinAngle();
	checkMaxAngle();
	checkChannel();

}

/** 
 *  READ THE setMaximumUS() comment in the .cpp and .hpp
 */
void SB_Servo::checkMinUS() { 
	/**
	if (minimum >= 0 && 4 * minimum < maxUS) { 
		minUS = 4 * minimum;
	} else { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x1
	}
	*/
	if (minUS < 0 || minUS < maxUS) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x1;
		printDebug("checkMinUS() error");
	}
}


/**
 * This method uses the manufacturer's maximum
 * So, for example the HS422 has a rating of 500us - 2500us, thus we pass 2500 here
 * The maestro will use 2000 - 10k ms. Don't ask me why the Maestro uses 4x larger values, it just does.
 * See the .hpp for more info
 */
void SB_Servo::checkMaxUS() { 
	if (maxUS < 0 || maxUS <= minUS) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x2;
		printDebug("checkMaxMS() error");
	}
}

void SB_Servo::checkMinDegreeRange() { 
	if (minDegreeRange < 0 || minDegreeRange > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x4;
		printDebug("checkMinDegreeRange() error");
	}
}

void SB_Servo::checkMaxDegreeRange() { 
	if (maxDegreeRange > 360 || maxDegreeRange < minDegreeRange ) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x8;
		printDebug("checkMaxDegreeRange() error");
	}
}


void SB_Servo::checkMinAngle() { 
	if (minAngle < 0 || minAngle > maxAngle || 
			minAngle < minDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x10;
		printDebug("checkMinAngle() error");
	}
}

void  SB_Servo::checkMaxAngle() { 
	if (maxAngle < 0 || maxAngle < minAngle || 
			maxAngle > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x20;
		printDebug("checkMaxAngle() error");
	}
}

void SB_Servo::checkChannel() { 
	/**
	if (channelNum  >= 0 && channelNum <= 127) { 
		channelNum = channel;	
		return true;
	} else {
		return false;
	}
	*/
	if (channelNum < 0 || channelNum > NUM_MAESTRO_CHANNELS) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= CHANNEL_ERROR_BIT;
		printDebug("checkChannel() error");
	}
}

float SB_Servo::getCurrentDegrees() { 
	if (errorCode & CHANNEL_ERROR_BIT) { // Mask wit the channel error bit to see if we fucked up the channel num
		printDebug("Bad channel num, aborting getCurrentDegrees()"); 
		return -1; // Servo not connected properly 
	} else { 
		int currentUS = maestro.getPosition(channelNum); 
		return usToDegrees(currentUS); 
	}
}


void SB_Servo::rotateToDegrees(float degree) { 
	if (degree > maxAngle) { 
		degree = maxAngle;
		errorCode |= ROTATE_TO_ERROR_BIT;
		errorCode |= ANGLE_OVER_WARNING;
		printDebug("Requested RotateTo() angle exceeds rating warning");
	} 
	if (degree < minAngle) { 
		degree = minAngle;
		errorCode |= ROTATE_TO_ERROR_BIT;
		errorCode |= ANGLE_UNDER_WARNING;
		printDebug("Requested RotateTo() angle under rating warning");
	}
	int msToWrite = degToUS(degree);
	if (msToWrite > maxUS) { 
	   msToWrite = maxUS; // I dont think this should ever be happening 	
		printDebug("Calculated us to write to servo exceeds rated maximum us"); 
	} else if (msToWrite < minUS) { 
		msToWrite = minUS;
		printDebug("Calculated us to write to servo under rated minimum us"); 
	}
	checkChannel();
	if (errorCode & CHANNEL_ERROR_BIT) { // Mask wit the channel error bit to see if we fucked up the channel num
		printDebug("Bad channel num, aborting rotateTo()"); 
		return; // Servo not connected yet 
	} else { 
		maestro.setTarget(channelNum, msToWrite); 
		return;
	}
}


void SB_Servo::rotateBy(float degreesBy) { 
	float currentDeg;
	checkChannel();
	if (errorCode & CHANNEL_ERROR_BIT) { // Mask wit the channel error bit to see if we fucked up the channel num
		printDebug("Bad channel num, aborting rotateBy()"); 
		return; // Servo not connected yet 
	} else { 
		currentDeg = (int) (getCurrentDegrees() + .5);  
	}
	int desiredUS = degToUS(currentDeg + degreesBy);
	if (desiredUS > maxUS) {
		errorCode |= ROTATE_BY_ERROR_BIT;
		errorCode |= ANGLE_OVER_WARNING;
		printDebug("Requested rotateBy() angle over rating warning");
		desiredUS = maxUS;
	} else if (desiredUS < minUS) { 
		errorCode |= ROTATE_BY_ERROR_BIT;
		errorCode |= ANGLE_UNDER_WARNING;
		printDebug("Requested rotateBy() angle under rating warning");
		desiredUS = minUS;
	}
	// Move the maestro, again this needs a failure mode
	maestro.setTarget(channelNum, desiredUS);
}

void SB_Servo::printDebug(String printMe) { 
#ifdef DEBUG
	Serial.print("Servo #");
	Serial.print(servoNumber);
	Serial.print(": ");
	Serial.println(String(printMe));
#endif
}

	
