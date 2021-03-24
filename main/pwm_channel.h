#ifndef PWM_CHANNEL_H
#define PWM_CHANNEL_H

// Pin define macros
#define CH2_PIN 19
#define CH3_PIN 18

// PWM Struct
struct pwmChannel
{
  // rise time of the pwm signal
  volatile int riseTime = 0;

  // pulse time of the pwm signal
  volatile int pwmValue = 0;
};

// Channel ISRs
void CH2_RISE_ISR();  // called on rising edge interrupt
void CH2_FALL_ISR();  // called on falling edge interrupt

void CH3_RISE_ISR();
void CH3_FALL_ISR();

// Initialization function
void initPWM();

#endif
