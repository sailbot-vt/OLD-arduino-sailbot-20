#include "pwm_channel.h"

// pwm channel instantiations for testing
pwmChannel CH2;
pwmChannel CH3;

void setup(void) 
{
  // initialize the interrupts for the pwm channels 
  initPWM();

  // used for serial monitor
  Serial.begin(9600);
}

void loop(void) 
{
  // Every 50 milliseconds print the values of the two channels
  static int refTime = micros();
  int curTime = micros();

  if(curTime - refTime > 50000)
  {
    // update ref time
    refTime = curTime;
    Serial.println(CH2.pwmValue);
    Serial.println(CH3.pwmValue); 
  }
}
