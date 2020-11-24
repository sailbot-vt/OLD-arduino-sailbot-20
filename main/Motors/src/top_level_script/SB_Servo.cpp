/**
 * Source file for SB_Servo.hpp, contains function definitions 
 * to implement the SailBot 2021 Servo wrapper 
 * A. Heller-Jones
 */

#include "SB_Servo.hpp"

// Here the maestro is initialized to Serial1 on the Teensy, this is just one of 8 ports 
// pretty neat. I suppose this is how static private variables are initialized
MiniMaestro SB_Servo::maestro(Serial1);

SB_Servo::SB_Servo() { 
	Serial1.begin(9600);
}	


SB_Servo::SB_Servo(int channel) { 
	channelNum = channel;	
	Serial1.begin(9600);
}

SB_Servo::SB_Servo(int minDegree, int maxDegree, int channel) { 
	this->maxDegree = maxDegree;
	this->minDegree = minDegree;
	channelNum = channel;	
	Serial1.begin(9600);
}

bool SB_Servo::setChannel(int channel) { 
	if (channelNum >= 0 && channelNum <= 127) { 
		channelNum = channel;	
		return true;
	} else {
		return false;
	}
}

bool SB_Servo::setMaximumAngle(int maximum) { 
	if (maximum >= 0 && maximum <= 180 && maximum > minDegree) { 
		maxDegree = maximum;
		return true;
	} else { 
		return false;
	}
}

bool SB_Servo::setMinimumAngle(int minimum) { 
	if (minimum >= 0 && minimum <= 180 && minimum < maxDegree) { 
		minDegree = minimum;
		return true;
	} else { 
		return false;
	}
}


bool SB_Servo::rotateToDegrees(float degree) { 
	if (degree > maxDegree) { 
	   degree = maxDegree;	
	} else if (degree < minDegree) { 
		degree = minDegree;
	}

	int msToWrite; // Value to write to the maestro
	if (channelNum == -1) { 
		//throw std::runtime_error("Attempted to move Servo without assigning channel number");
		// figure out something to do, arduino compiler doesn't like to throw errors 
		// I would normally make a std Serial output, but I don't want the code to assume
		// what Serial ports are going to be open. Perhaps we can return a boolean indicating 
		// whether or not the rotate was successful
		return false;
	} else { 
		msToWrite = (int) map(degree, minDegree, maxDegree, 
				MAESTRO_SERVOANGLE_MS_MIN, MAESTRO_SERVOANGLE_MS_MAX);
		maestro.setTarget(channelNum, msToWrite);
		return true;
	}
}
