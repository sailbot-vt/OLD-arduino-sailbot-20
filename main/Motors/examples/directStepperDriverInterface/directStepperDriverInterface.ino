/** 
A simple example for interfacing via SPI 
*/



#include <HighPowerStepperDriver> // Stepper driver lib
#include <SPI> // Stepper driver lib

HighPowerStepperDriver sd;

/** 
Connections to make: 
(from pololu site)
Teensy (3.2) 	Driver		notes 
Digital pin 2	DIR			Just regular PWM pins 
Digital pin 3	STEP
Digital pin 4	SCS
SCK				SCLK		teensy pin13
MOSI			SDATI 		11	
MISO			SDATO		12
GND				GND
*/

// This is mostly copy pasted from pololu example
const uint8_t DirPin = 2;
const uint8_t StepPin = 3;
const uint8_t CSPin = 4;

// This period is the length of the delay between steps, which controls the
// stepper motor's speed.  You can increase the delay to make the stepper motor
// go slower.  If you decrease the delay, the stepper motor will go faster, but
// there is a limit to how fast it can go before it starts missing steps.
const uint16_t StepPeriodUs = 2000;

HighPowerStepperDriver sd;

void setup()
{
  SPI.begin();
  sd.setChipSelectPin(CSPin);

  // Drive the STEP and DIR pins low initially.
  pinMode(StepPin, OUTPUT);
  digitalWrite(StepPin, LOW);
  pinMode(DirPin, OUTPUT);
  digitalWrite(DirPin, LOW);

  // Give the driver some time to power up.
  delay(1);

  // Reset the driver to its default settings and clear latched status
  // conditions.
  sd.resetSettings();
  sd.clearStatus();

  // Select auto mixed decay.  TI's DRV8711 documentation recommends this mode
  // for most applications, and we find that it usually works well.
  sd.setDecayMode(HPSDDecayMode::AutoMixed);

  // Set the current limit. You should change the number here to an appropriate
  // value for your particular system.
  // pretty sure this is the correct driver that we purchased
  sd.setCurrentMilliamps36v4(1000);

  // Set the number of microsteps that correspond to one full step.
  sd.setStepMode(HPSDStepMode::MicroStep32);

  // Enable the motor outputs.
  sd.enableDriver();
}


void loop() { 
	setDirection(0); // run in the default direction
	if (Serial.available()) { 
		int numberOfSteps = Serial.parseInt();
		for (int i = 0; i < numberOfSteps; i++) { 
			step();
			delayMicroseconds(StepPeriodUs);
			// have to make sure to delay I guess
			// because it'll burn if you go too fast 
		}
	}
}
