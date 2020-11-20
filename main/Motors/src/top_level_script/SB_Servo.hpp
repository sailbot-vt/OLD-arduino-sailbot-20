

/** 
 * A class representing a Servo for SailBot 2021 @ Virginia Tech.
 * A. Heller-Jones 
 * 11.20.2020
 *
 * The class utilizes the Servo controller, the Pololu Mini Maestro 
 * and is written for the Teensy 4.0, 3.2 platforms, which can be compiled and 
 * uploaded using the Arduino IDE 
 * directory referencing:
/home/andy/Documents/Personal/SailBOT/ArduinoMotorControl2020/arduino-sailbot-20/main/Motors/src

/home/andy/Documents/Personal/SailBOT/ArduinoMotorControl2020/arduino-sailbot-20/dependencies/libs
 */

#include <PololuMaestro.h>

class SB_Servo { 
	private: 
		static MiniMaestro maestro;

   public: 
    SB_Servo();
};
