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
	degreeRangeTop





{ 

	setMinimumUS(minUS);
	setMaximumUS(maxUS);
	degreeRangeBottom = minRange;
	degreeRangeTop = maxRange;
	channelNum = channel;
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
	if (channelNum < 0 || channelNum > 127) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x40;
	}
}


/** 
 *  READ THE setMaximumUS() comment in the .cpp and .hpp
 */
void SB_Servo::checkMinMS(int minimum) { 
	/**
	if (minimum >= 0 && 4 * minimum < maxUS) { 
		minUS = 4 * minimum;
	} else { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x1
	}
	*/
	if (minUS < 0 || minUS < maxUS) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x1;
	}
}


/**
 * This method uses the manufacturer's maximum
 * So, for example the HS422 has a rating of 500us - 2500us, thus we pass 2500 here
 * The maestro will use 2000 - 10k ms. Don't ask me why the Maestro uses 4x larger values, it just does.
 * See the .hpp for more info
 */
void SB_Servo::checkMaxMS() { 
	if (maxUS < 0 || maxUS <= minUS) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x2
	}
}

void SB_Servo::checkMinDegreeRange() { 
	if (minDegreeRange < 0 || minDegreeRange > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x4;
	}
}

void SB_Servo::checkMaxDegreeRange() { 
	if (maxDegreeRange > 360 || maxDegreeRange < minDegreeRange ) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x8;
	}
}


void SB_Servo::checkMinAngle() { 
	if (minAngle < 0 || minAngle > maxAngle || 
			minAngle < minDegreeRange) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x10
	}
}

void  SB_Servo::checkMaxAngle() { 
	if (maxAngle < 0 || maxAngle < minAngle || 
			maxAngle > maxDegreeRange) { 
		errorCode |= CONFIG_ERROR_MASK;
		errorCode |= 0x20
	}
}

float SB_Servo::getCurrentDegrees() { 
	int currentMS = maestro.getPosition(channelNum); // TODO: See what these return values are 
													  // and find a way to return -1 when theres comm failure
	return msToDegrees(currentMS); 
}


bool SB_Servo::rotateToDegrees(float degree) { 
	if (degree > maximumAngle) { 
		degree = maximumAngle;
	} 
	if (degree < minimumAngle) { 
		degree = minimumAngle;
	}
	int msToWrite = degToMS(degree);
	if (msToWrite > maxUS) { 
	   msToWrite = maxUS;	
	} else if (msToWrite < minUS) { 
		msToWrite = minUS;
	}
	if (channelNum == -1) { 
		//throw std::runtime_error("Attempted to move Servo without assigning channel number");
		// figure out something to do, arduino compiler doesn't like to throw errors 
		// I would normally make a std Serial output, but I don't want the code to assume
		// what Serial ports are going to be open. Perhaps we can return a boolean indicating 
		// whether or not the rotate was successful
		return false;
	} 
	Serial.println(msToWrite);
	maestro.setTarget(channelNum, msToWrite); // TODO: implement some sort of comm failure mode
	return true;
}


bool SB_Servo::rotateBy(float degreesBy) { 
	float currentDeg;
	if (channelNum == -1) { 
		return false; // Servo not connected yet 
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
	
