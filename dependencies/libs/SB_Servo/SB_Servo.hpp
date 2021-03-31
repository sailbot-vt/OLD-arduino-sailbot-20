  

/** 
 * A class representing a Servo for SailBot 2021 @ Virginia Tech.
 *
 * The class utilizes the Servo controller, the Pololu Mini Maestro 
 * and is written for the Teensy 4.0, 3.2 platforms, which can be compiled and 
 * uploaded using the Arduino IDE 
 * directory referencing:
 *
 *
 * By convention this program uses: 
 * `floats` for floating point numbers as the double is just not needed 
 *
 * AHJ
 */



// #include <stdexcept> arduino compiler doesnt like to throw errors
#define DEBUG

#ifndef SB_servo
#define SB_servo
#include <PololuMaestro.h>
#include <vector>

/** 
 * These are the error codes for our system, 
 * See the documentation that I haven't written yet for 
 * a full list detailing the error codes
 * Im sure theres a better way to do than using binary but 
 */
#define CHANNEL_ERROR_BIT 0x40
#define CONFIG_ERROR_BIT 0x80
#define ROTATE_TO_ERROR_BIT 0x800
#define ROTATE_BY_ERROR_BIT 0x1000
#define ANGLE_UNDER_WARNING  0x100
#define ANGLE_OVER_WARNING  0x200

/**
 * These are default values for instantiated servos
 * the min/max us are the general guidelines for minimum and maximum
 * microsecond pulse times of servos. 
 * The default min angle and max angle are used as the default range of degrees of a servo
 * most servos go from 0-180 believe it or not
 */
#define DEFAULT_MIN_US 500
#define DEFAULT_MAX_US 2500 

#define DEFAULT_MIN_ANGLE 0  
#define DEFAULT_MAX_ANGLE 180 

/**
 * The number of servos that the maestro can handle
 * This could change in the future if a different maestro were to be used
 */
#define NUM_MAESTRO_CHANNELS 8
using maestro_units = uint16_t;

class SB_Servo { 
	private: 
		// We make the maestro static so that it's shared across all instances 
		// of Servos
		static MiniMaestro maestro;
		// This is the number of servos we're using, the count increments for 
		// each servo added. The servo count is used in debugging print values 
		// as it provides a unique identifier for each servo 
		static int servoCount; 
		int errorCode = 0;

		/** 
		 * These values are found from servo specific data sheets					 
		 * The default values are referenced from the HS-422 0-180* digital servo 
		 * Which has specific values of 500-2500 us. The maestro's specifications use 
		 * 4x pulse widths, just how it works, thus we use 2k and 10k as
		 * the default minimum and maximum pulses, these are modified in the parameterized constructor
		 */
		const int minUS; 		// default 500 
		const int maxUS; 		//default 2500 

		// The RANGES are manufacturer define ranges that the servo can move to 
		// For example, most servos move between 0 and 180. However, some go from 0 to 200
		const int minDegreeRange; // default: 0 
		const int maxDegreeRange; // default: 180  

		// The ANGLES are maximum and minimum angles that a servo can go to, found experimentally
		// For example, one of the HS475's goes from 3* to 200* 
		// You can find out the min and max angles of a servo by using the 
		// simpleSerialRead.ino program and finding where the servo start jittering.
		// Some servos work fine for their full operational range and thus these min and max 
		// Angles won't need to be modified in the constructor 
		const float minAngle; 	//default 0 
		const float maxAngle; 	// default 180

		const int channelNum; 	// default value for the channel, 
						     	// indicates the channel has not been assigned
		const int servoNumber;  // The identifier for this servo taken from servoCount 
		

		/** Convert a ms value into a degrees value
		 * @param ms -- the ms to convert
		 * @return the degrees in float form from 0 to 180
		 */
		float usToDegrees(int ms);

		/**
		 * Convert traditional 0-180 degrees into ms for the maestro to use
		 * the maestro uses ms for calculating servo angles, thus this method
		 * acts as an interface from human readable degrees to ms, which the machine uses
		 * @param degrees -- the degrees in 0-180
		 * @return the ms representation of the degrees
		 */
		int degToUS(float degrees);  

		/**
		 * Checks that the channel was set to a correct value 
		 * TODO: fill in error code 
		 */
		void checkChannel();

		/**
		 * Check that the minimum and maximum values provided by the constructor 
		 * make sense. Else throw error codes. TODO: fill in the error code values
		 */
		void checkMinUS();
		void checkMaxUS();

		/**
		 * The range of a servo is not the same as its min/max angle! The range is the tolerances
		 * of a particular servo. Some servos do not like to go all the way down to 0 
		 * or all the way up to 180. This portion of the constructor will have to come
		 * from experimental data and can not be specified by the manufacturer
		 *
		 *
		 * Simply checks that the min and max range specified makes sense
		 * TODO: error 
		 */ 
		void checkMinDegreeRange();
		void checkMaxDegreeRange();

		/** 
		 * checks the max/min angle
		 * For now, you can't set a max angle over 360 or under 0 
		 * It also cannot be set less than or equal to the minimum
		 * @return error code 11 -- bad min angle TODO: see if these are correct
		 * @return error code 12 -- bad max angle
		 */
		void checkMinAngle();
		void checkMaxAngle();

		/**
		 * Simply used to print debug lines if debug mode has been toggled on,
		 * which can easily be done from line #20
		 * @param String the string to print
		 */
		void printDebug(String);

   	public: 
		/**
		 * Sets the channelNumber of a particular servo 
		 * Calls the next few constructors using hte default values
		 * as specified in the preprocessor directives 
		 * @param channel the channel number to use, which is the pin on
		 * the maestro that the servo is connected to 
		 */
		SB_Servo(int);

		/**
		 * Sets the channelNumber of a particular servo 
		 * Also sets the minimum and maximum degrees 
		 * @param channel -- the channel number to use, which is the pin on the maestro to use
		 * @param minDegr -- the minimum degree the servo can rotate to 
		 * @param maxDegr -- the maximum degree the servo can rotate to 
		 *
		 * Begins the Serial monitor on Serial1. This could be changed in the future
		 * to be able to dictated by .xml or .yml file
		 */
		SB_Servo(int minDegr, int maxDegr, int channel);


		/**
		 * Big fat ass constructor and the final constructor to be called
		 * All SB_Servo objects have to eventually call this method upon construction
		 * It also performs a lot of configuration checks to set the error codes if 
		 * a parameter in this is not set properly 
		 * @param minUS -- the bottom of the range of the us of the servo 
		 * @param maxUS -- the top of the range of the us of the servo 
		 * @param  minRange -- the bottom degree of the servo
		 * @param  maxRange -- the top degree of the servo
		 * @param minAngle -- the experimentally found minimum angle the servo can move to 
		 * @param maxAngle -- the experimentally found maximum angle the servo can move to 
		 * @param channelNum -- the channel number this servo uses on the maestro
		 */ 
		SB_Servo(int minUS, int maxUS, float minRange, float maxRange, float minAngle, float maxAngle, int channel);


		
		/** 
		 * Gets the current degrees of this servo 
		 * by sending asking the maestro for the current degrees of the servo
		 *
		 * @return the current degrees.
		 * @return -1 if there is a communication failure
		 */
		float getCurrentDegrees();

		/** Rotates this servo to a specific degrees, 0-180, or within the minimum
		 * and maximum values as dictated by the member values
		 *
		 * @param degrees -- the degrees to rotate to 
		 * @return whether or not the Servo is properly connected to the Maestro, for example
		 * if a channel number was never asserted then the Servo will do nothing and return false; 
		 */
		void rotateToDegrees(float degrees);


		/** 
		 * Rotates the servo by a set amount   
		 * Checks to make sure that the requested amount is within the servos
		 * tolerated limits
		 * This method also directly modifies the Servos, and thus 
		 * needs to be able to raise some kind of error if communication with the Maestro
		 * fails 
		 * @param degreesBy -- the amount of degrees to turn by 
		 * @return whether or not the requested operation was doable
		 */
		void rotateBy(float degreesBy);



		/**
		 * Moves servos at the exact same time  
		 * @param servos -- a list of servos to move 
		 *  THE SERVOS HAVE TO HAVE CONTIGUOUS CHANNEL NUMBERS	
		 * For example, you have to use servos on channels 0, 1, 2, 3
		 *
		 * @param degrees -- the degrees to send to the servos. They match up respectively. For example 
		 * degrees[0] is where to turn servos[0]  
		 *
		 */
		static void setMultipleTargets(std::vector<SB_Servo> servos, std::vector<float> degrees);
};

#endif
