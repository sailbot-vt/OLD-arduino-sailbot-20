/**
 * Source file for SB_Servo.hpp, contains function definitions 
 * to implement the SailBot 2021 Servo wrapper 
 * A. Heller-Jones
 */

#include "SB_Servo.hpp"

// Here the maestro is initialized to Serial1 on the Teensy, this is just one of 8 ports 
// pretty neat. I suppose this is how static private variables are initialized
  MiniMaestro SB_Servo::maestro(Serial1);


/** 
 * uses point slope form to calculate a ms value for the maestro for a given degree
 */ 
ms_t SB_Servo::degToMS(float degree) {
	return ((float) (maxMS - minMS) / MAX_DEGREE) * (degree) + minMS;
}


float SB_Servo::msToDegrees(ms_t ms) { 
	return (MAX_DEGREE / ((float) maxMS - minMS)) * (ms - minMS) + MIN_DEGREE;
}

/* For now I'm going to comment out the default constructor, nothing is supposed to work if you 
 * dont specify a channel number 
SB_Servo::SB_Servo() { 
	Serial1.begin(9600);
}	
*/

SB_Servo::SB_Servo(int channel) { 
	channelNum = channel;	

}

SB_Servo::SB_Servo(ms_t minMS, ms_t maxMS, int channel) { 
	setMinimumUS(minMS);
	setMaximumUS(maxMS);
	channelNum = channel;	
	// For now, I'm going to leave these commented out, I'm not a huge fan of the idea 
	// of keeping the current Degrees in a variable, it should routinely accessed by method
	// currentDegrees = maestro.getPosition(channel); // get the degrees at initialization 
}

bool SB_Servo::setChannel(int channel) { 
	if (channelNum  >= 0 && channelNum <= 127) { 
		channelNum = channel;	
		return true;
	} else {
		return false;
	}
}

/**
 * This method uses the manufacturer's maximum
 * So, for example the HS422 has a rating of 500us - 2500us, thus we pass 2500 here
 * The maestro will use 2000 - 10k ms. Don't ask me why the Maestro uses 4x larger values, it just does.
 * See the .hpp for more info
 */
bool SB_Servo::setMaximumUS(ms_t maximum) { 
	if (maximum >= 0 && maximum > minMS) { 
		maxMS = 4 * maximum;
		return true;
	} else { 
		return false;
	}
}

/** 
 *  READ THE setMaximumUS() comment in the .cpp and .hpp
 */
bool SB_Servo::setMinimumUS(ms_t minimum) { 
	if (minimum >= 0 && minimum < maxMS) { 
		minMS = 4 * minimum;
		return true;
	} else { 
		return false;
	}
}


float SB_Servo::getCurrentDegrees() { 
	ms_t currentMS = maestro.getPosition(channelNum); // TODO: See what these return values are 
													  // and find a way to return -1 when theres comm failure
	return msToDegrees(currentMS); 
}


bool SB_Servo::rotateToDegrees(float degree) { 
	int msToWrite = degToMS(degree);
	if (msToWrite > maxMS) { 
	   msToWrite = maxMS;	
	} else if (msToWrite < minMS) {
    
		msToWrite = minMS;
	}
	if (channelNum == -1) { 
		//throw std::runtime_error("Attempted to move Servo without assigning channel number");
		// figure out something to do, arduino compiler doesn't like to throw errors 
		// I would normally make a std Serial output, but I don't want the code to assume
		// what Serial ports are going to be open. Perhaps we can return a boolean indicating 
		// whether or not the rotate was successful
		return false;
	} else { 
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

	ms_t desiredMS = degToMS(currentDeg + degreesBy);
	if (desiredMS > maxMS || 
		desiredMS < minMS) {
   
		return false;
	} else { 
    // Move the maestro, again this needs a failure mode
		maestro.setTarget(channelNum, desiredMS);
		
		return true;
	}
}
	
