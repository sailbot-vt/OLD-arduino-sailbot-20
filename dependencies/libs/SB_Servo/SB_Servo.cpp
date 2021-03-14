/**
 * Source file for SB_Servo.hpp, contains function definitions 
 * to implement the SailBot 2021 Servo wrapper 
 *
 * AHJ
 */

//#include "SB_Servo.hpp"

// Here the maestro is initialized to Serial1 on the Teensy, this is just one of 8 ports 
// pretty neat. I suppose this is how static private variables are initialized
  //MiniMaestro SB_Servo::maestro(Serial1);


/** 
 * uses point slope form to calculate a ms value for the maestro for a given degree
 */ 
int SB_Servo::degToUS(float degree) {
	return ((float) (maxUS - minUS) / degreeRangeTop) * (degree) + minUS;
}


float SB_Servo::usToDegrees(int ms) { 
	return (degreeRangeTop / ((float) maxUS - minUS)) * (ms - minUS) + degreeRangeBottom;
}

SB_Servo::SB_Servo(int channel) : SB_Servo(DEFAULT_MIN_US, DEFAULT_MAX_US, channel) {}

SB_Servo::SB_Servo(int minUS, int maxUS, int channel) : 
	SB_Servo(
		minUS, maxUS, 
		DEFAULT_MIN_ANGLE, DEFAULT_MAX_ANGLE, DEFAULT_MIN_ANGLE, DEFAULT_MAX_ANGLE) {}


SB_Servo::SB_Servo(int minimumUS, int maximumUS, float minimumRange, float maximumRange, 
	float minimumAngle, float maximumAngle, int channel) : 
		minUS{4 * minimumUS},
		maxUS{4 * maximumUS}, 
		minDegreeRange{minimumRange},
		maxDegreeRange{maximumRange},
		minAngle{minimumAngle},
		maxAngle{maximumangle},
		channelNum{channel} 
		servoNumber{servoCount++} 
		{ 
	
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
void SB_Servo::checkMinMS() { 
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
	}
#ifdef DEBUG
	Serial.println("checkMinMS() error");
#endif
}


/**
 * This method uses the manufacturer's maximum
 * So, for example the HS422 has a rating of 500us - 2500us, thus we pass 2500 here
 * The maestro will use 2000 - 10k ms. Don't ask me why the Maestro uses 4x larger values, it just does.
 * See the .hpp for more info
 */
void SB_Servo::checkMaxMS() { 
	if (maxUS < 0 || maxUS <= minUS) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x2
	}
#ifdef DEBUG
	Serial.println("checkMaxMS() error");
#endif
}

void SB_Servo::checkMinDegreeRange() { 
	if (minDegreeRange < 0 || minDegreeRange > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x4;
	}
#ifdef DEBUG
	Serial.println("checkMinDegreeRange() error");
#endif
}

void SB_Servo::checkMaxDegreeRange() { 
	if (maxDegreeRange > 360 || maxDegreeRange < minDegreeRange ) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x8;
	}
#ifdef DEBUG
	Serial.println("checkMaxDegreeRange() error");
#endif
}


void SB_Servo::checkMinAngle() { 
	if (minAngle < 0 || minAngle > maxAngle || 
			minAngle < minDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x10
	}
#ifdef DEBUG
	Serial.println("checkMinAngle() error");
#endif
}

void  SB_Servo::checkMaxAngle() { 
	if (maxAngle < 0 || maxAngle < minAngle || 
			maxAngle > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_BIT;
		errorCode |= 0x20
	}
#ifdef DEBUG
	Serial.println("checkMaxAngle() error");
#endif
}

bool SB_Servo::checkChannel() { 
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
	}
#ifdef DEBUG
	Serial.println("checkChannel() error");
#endif
}

float SB_Servo::getCurrentDegrees() { 
	int currentUS = maestro.getPosition(channelNum); // TODO: See what these return values are 
													  // and find a way to return -1 when theres comm failure
	return msToDegrees(currentUS); 
}


bool SB_Servo::rotateToDegrees(float degree) { 
	if (degree > maximumAngle) { 
		degree = maximumAngle;
		errorCode |= ROTATE_TO_ERROR_BIT;
		errorCode |= ANGLE_OVER_WARNING
#ifdef DEBUG
	Serial.println("Requested RotateTo() angle exceeds rating warning");
#endif
	} 
	if (degree < minimumAngle) { 
		degree = minimumAngle;
		errorCode |= ROTATE_TO_ERROR_BIT;
		errorCode |= ANGLE_UNDER_WARNING
#ifdef DEBUG
	Serial.println("Requested RotateTo() angle under rating warning");
#endif
	}
	int msToWrite = degToMS(degree);
	if (msToWrite > maxUS) { 
	   msToWrite = maxUS; // I dont think this should ever be happening 	
#ifdef DEBUG
	Serial.println("Calculated us to write to servo exceeds rated maximum us"); 
#endif
	} else if (msToWrite < minUS) { 
		msToWrite = minUS;
#ifdef DEBUG
	Serial.println("Calculated us to write to servo under rated minimum us"); 
#endif
	}
	checkChannel();
	maestro.setTarget(channelNum, msToWrite); 
	return true;
}


void SB_Servo::rotateBy(float degreesBy) { 
	float currentDeg;
	checkChannel();
	if (errorCode & CHANNEL_ERROR_BIT) { // Mask wit the channel error bit to see if we fucked up the channel num
		return; // Servo not connected yet 
#ifdef DEBUG
	Serial.println("Bad channel num, aborting rotateBy()"); 
#endif
	} else { 
		// Here we will call getCurrentDegrees() only once because it involves a two way
		// transmission w/ the maestro, which we'll want to decrease the amount of times performed
		// to decrease latency. It could be argued that getCurrentDegrees() should be called 
		// each time we wish to compare, but this is how it will be implemented to begin with. 
		// We also add .5 and cast to an integer in order to round to the nearest int 
		// this stops cumulative error build up when performing multiple rotateBy's -- 
		// do enough of them and the system can get off by entire degrees without this fix 
		currentDeg = (int) (getCurrentDegrees() + .5);  
	}
	int desiredMS = degToMS(currentDeg + degreesBy);
	if (desiredMS > maxUS || 
		desiredMS < minUS) {
		return false;
	} 
	// Move the maestro, again this needs a failure mode
	maestro.setTarget(channelNum, desiredMS);
	return true;
}
	
