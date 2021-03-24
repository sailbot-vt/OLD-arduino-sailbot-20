#include "rc_decode.h"

// Initialization function
void initPWM()
{
  // configure GPIO pins as inputs
  pinMode(CH2_PIN, INPUT);  
  pinMode(CH3_PIN, INPUT);

  // initial attach, rising edge only
  attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_RISE_ISR, RISING);
}

// ISR Definitions
void CH2_RISE_ISR()
{
  // attach new interrupt for falling
  attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_FALL_ISR, FALLING);
  CH2.riseTime = micros();
}

void CH3_RISE_ISR()
{
  attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_FALL_ISR, FALLING);
  CH3.riseTime = micros();
}

void CH2_FALL_ISR()
{
  // attach new interrupt for rising
  attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
  CH2.pwmValue = micros() - CH2.riseTime;
}

void CH3_FALL_ISR()
{
  attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_RISE_ISR, RISING);
  CH3.pwmValue= micros() - CH3.riseTime;
}
