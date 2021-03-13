  

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


#include <PololuMaestro.h>
// #include <stdexcept> arduino compiler doesnt like to throw errors

#ifndef SB_servo
#define SB_servo

#define CONFIG_ERROR_MASK 0x80
#define ROTATE_TO_ERROR_MASK 0x400
#define ROTATE_BY_ERROR_MASK 0x800

#define DEFAULT_MIN_US 500
#define DEFAULT_MAX_US 2500 

#define DEFAULT_MIN_ANGLE 0  
#define DEFAULT_MAX_ANGLE 180 

class SB_Servo { 
	private: 
		// We make the maestro static so that it's shared across all instances 
		// of Servos
		static MiniMaestro maestro;
		int errorCode = 0;
		const int channelNum; // default value for the channel, 
						     // indicates the channel has not been assigned

		/** 
		 * These values are found from servo specific data sheets					 
		 * The default values are referenced from the HS-422 0-180* digital servo 
		 * Which has specific values of 500-2500 us. The maestro's specifications use 
		 * 4x pulse widths, just how it works, thus we use 2k and 10k as
		 * the default minimum and maximum pulses, these are modified in the parameterized constructor
		 */
		const int minUS; // default 500 
		const int maxUS; //default 2500 
		// The RANGES are manufacturer define ranges that the servo can move to 
		// For example, most servos move between 0 and 180. However, some go from 0 to 200
		const int minDegreeRange; //default: 180 
		const int maxDegreeRange; // default: 0 

		// The ANGLES are maximum and minimum angles that a servo can go to, found experimentally
		// For example, one of the HS475's goes from 3* to 200* 
		const float maximumAngle; // default 180
		const float minimumAngle; //default 0 
		

		/** Convert a ms value into a degrees value
		 * @param ms -- the ms to convert
		 * @return the degrees in float form from 0 to 180
		 */
		float msToDegrees(ms_t ms);

		/**
		 * Convert traditional 0-180 degrees into ms for the maestro to use
		 * the maestro uses ms for calculating servo angles, thus this method
		 * acts as an interface from human readable degrees to ms, which the machine uses
		 * @param degrees -- the degrees in 0-180
		 * @return the ms representation of the degrees
		 */
		ms_t degToMS(float degrees);  

		/** 
		 * Sets the channel number for a particular system. Pretty straight forward
		 *
		 * Requires that the channel setting is within the  bounds for the maestro, 
		 * 0 and 127.  
		 * TODO: change 127 to the number of channels available in the MicroMaestro, 
		 * obviously we don't have 127 channels, but there is a max num
		 * @param channel -- the channel to assign this servo to 
		 * @return whether or not the channel was set properly, the method only returns flase
		 * if your provide a bad channel number
		 */
		void checkChannel();

		/**
		 * Sets the maximum possible us that this given servo can rotate to
		 * Each servo has it's own possible maximum. These values are probably 
		 * going to need to be stored in some sort of database or something 
		 * This, along with min us or something like that 
		 * 
		 * THESE METHODS USE MICRO SECONDS, NOT MAESTRO MILLISECONDS *******
		 * The maestro writes micro seconds to servos with a 4x scalar multiplier. Don't ask me why. 
		 * Thus, when we use the manufacturer's rating of 500-2500us, the internal maximums for the class
		 * are thus 2000-10k. WRITE TO THESE METHODS USING 500-2500us. If this doesnt make sense to you, 
		 * Look up the HS422 - it has a maximum range of 500-2500us. 
		 * Thus you would call this method, myHS422Servo.setMaximumUS(2500);
		 * @param maximum -- the max us that the servo can rotate to 
		 * @return whether or not the maximum was set properly 
		 * 		   The method should really only return false if the max is less than the min
		 * 		   Or if the value passed is less than 0
		 */
		void checkMinUS();
		void checkMaxUS();

		/**
		 * The range of a servo is not the same as its min/max angle! The range is the tolerances
		 * of a particular servo. Some servos do not like to go all the way down to 0 
		 * or all the way up to 180. This portion of the constructor will have to come
		 * from experimental data and can not be specified by the manufacturer
		 */ 
		bool checkMinDegreeRange();
		bool checkMaxDegreeRange();

		/** 
		 * Sets the max angle
		 * @param angle -- the value to set to maxDegrees
		 * 						which dictates the max value the servo can rotate to 
		 * For now, you can't set a max angle over 180 or under 0 
		 * It also cannot be set less than or equal to the minimum
		 * @return error code 11 -- bad min angle
		 * @return error code 12 -- bad max angle
		 */
		void checkMinAngle();
		void checkMaxAngle();

   	public: 
		/** 
		 * Default constructor for the servo, I don't think 
		 * this guy really needs to do anything
		 * 
		 * Begins the Serial monitor on Serial1. This could be changed in the future
		 * to be able to dictated by .xml or .yml file
		 */
   		SB_Servo();

		/**
		 * Sets the channelNumber of a particular servo 
		 * @param channel the channel number to use, which is the pin on
		 * the maestro that the servo is connected to 
		 * Begins the Serial monitor on Serial1. This could be changed in the future
		 * to be able to dictated by .xml or .yml file
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
		SB_Servo(ms_t minDegr, ms_t maxDegr, int channel);



		SB_Servo(ms_t minMS, ms_t maxMS, float minRange, float maxRange, int channel);


		
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
		bool rotateToDegrees(float degrees);


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
		bool rotateBy(float degreesBy);
};
#endif
