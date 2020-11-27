/**
 * Source file for SB_Servo.hpp, contains function definitions 
 * to implement the SailBot 2021 Servo wrapper 
 * A. Heller-Jones
 */

#include "SB_Servo.hpp"

// Here the maestro is initialized to Serial1 on the Teensy, this is just one of 8 ports 
// pretty neat. I suppose this is how static private variables are initialized
MiniMaestro SB_Servo::maestro(Serial);


/** 
 * uses point slope form to calculate a ms value for the maestro for a given degree
 */ 
ms_t SB_Servo::degToMS(float degree) { 
	return (ms_t) ((maxMS - minMs) / MAX_DEGREE) * (degree - MIN_DEGREE) 
}


float SB_Servo::msToDegrees(ms_t ms) { 
	return (MAX_DEGREE / (maxMS - minMS) * (ms - minMs)) + MIN_DEGREE;
	
}

/* For now I'm going to comment out the default constructor, nothing is supposed to work if you 
 * dont specify a channel number 
SB_Servo::SB_Servo() { 
	Serial1.begin(9600);
}	
*/

SB_Servo::SB_Servo(int channel) { 
	channelNum = channel;	
	Serial.begin(9600);
	//currentDegrees = maestro.getPosition(channel); // get the degrees at initialization 
}

SB_Servo::SB_Servo(ms_t minMS, ms_t maxMS, int channel) { 
	this->maxMS = maxMS;
	this->minMS= minMS;
	channelNum = channel;	
	Serial.begin(9600);
	// For now, I'm going to leave these commented out, I'm not a huge fan of the idea 
	// of keeping the current Degrees in a variable, it should routinely accessed by method
	// currentDegrees = maestro.getPosition(channel); // get the degrees at initialization 
}

bool SB_Servo::setChannel(int channel) { 
	if (channelNum >= 0 && channelNum <= 127) { 
		channelNum = channel;	
		return true;
	} else {
		return false;
	}
}

bool SB_Servo::setMaximumMS(int maximum) { 
	if (maximum >= 0 && maximum <= 180 && maximum > minMS) { 
		maxMS = maximum;
		return true;
	} else { 
		return false;
	}
}

bool SB_Servo::setMinimumAngle(int minimum) { 
	if (minimum >= 0 && minimum <= 180 && minimum < maxMS) { 
		minDegree = minimum;
		return true;
	} else { 
		return false;
	}
}


float SB_Servo::getCurrentDegrees() { 
	ms_t currentMS = maestro.getPosition(channelNum); // TODO: See what these return values are 
													  // and find a way to return -1 when theres comm failure
	return	currentMS; 
}


bool SB_Servo::rotateToDegrees(float degree) { 
	if (degree > maxMS) { 
	   degree = maxMS;	
	} else if (degree < minDegree) {
    
		degree = minDegree;
	}
 Serial.println("Attempting to move"); 

	int msToWrite; // Value to write to the maestro
	if (channelNum == -1) { 
		//throw std::runtime_error("Attempted to move Servo without assigning channel number");
		// figure out something to do, arduino compiler doesn't like to throw errors 
		// I would normally make a std Serial output, but I don't want the code to assume
		// what Serial ports are going to be open. Perhaps we can return a boolean indicating 
		// whether or not the rotate was successful
		return false;
	} else { 
		msToWrite = degToMS(degree);
		maestro.setTarget(channelNum, msToWrite); // TODO: implement some sort of comm failure mode
		return true;
	}
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
		currentDeg = getCurrentDegrees(); 
	}
	if (currentDeg + degreesBy > maxMS) { 
		return false;
	} else if (currentDeg + degreesBy < minDegree) { 
		return false;
	} else { 
		ms_t desiredMS = degToMS(currentDeg + degreesBy);
		maestro.setTarget(channelNum, desiredMS); // Move the maestro, again this needs a failure mode
	}
}
	
