#ifndef PWM_CHANNEL_H
#define PWM_CHANNEL_H

// MACROS (digital IO)
/*    Any pins being used to read a pwm signal
 * from a channel of the AR620 should be defined here. This is not
 * crucial but helps with readability.
 */
#define CH1_PIN NULL    // define if being used
#define CH2_PIN 19      // right stick (horizontal movement)
#define CH3_PIN 18      // right stick (vertical movement)
#define CH4_PIN NULL    // define if being used
#define CH5_PIN NULL    // define if being used
#define CH6_PIN NULL    // define if being used

// PWM Struct
/* Struct Description:
 *  pwmChannel provides an interface between a AR620 PWM Channel and the teensy 4.0
 * Each instance of the struct represents a single channel from the AR620, so to use all
 * six channels of the AR620, six pwmChannel structs will need to be instantiated in the 
 * main code. For example, to use channels 2 and 3 the code using them will need to have
 * two pwmChannel instances like so:
 * 
 *          ...
 *          pwmChannel Channel2;  // channel two of the AR620 RC Receiver
 *          pwmChannel Channel3;  // channel three of the AR620 RC Receiver
 *          ...
 *          
 * Struct Members:
 *  riseTime:
 *    type: volatile int
 *    purpose: stores the time of a rising edge in microseconds
 *    
 *  fallTime:
 *    type: volatile int
 *    purpose: stores the width of a pwm signal pulse in microseconds
 */
struct pwmChannel
{
  // rise time of the pwm signal
  volatile int riseTime = 0;  // typed as volatile due to assignment in an ISR

  // pulse time of the pwm signal
  volatile int pwmValue = 0;
};

// FUNCTIONS

// Initialization function
/* Function Description:
 *  initPWM's main purpose is to configure the digital IO pins
 * being used for a pwmChannel struct as inputs, and to attach the interrupts
 * for said pins. 
 *   
 *  This function NEEDS to be called prior to using any of the pwmChannel 
 * structs to read a pwm from the AR620. 
 */
void initPWM();


// INTERRUPT SERVICE ROUTINES (ISR(s))
/*    Every instance of pwmChannel struct REQUIRES both a rising and falling edge
 * interrupt service routine (ISR). 
 * 
 *  RISING ISR
 *    trigger: RISING EDGE
 *    purpose: This ISR is used to obtain the time of the rising edge of the pwm signal
 *             in microseconds.
 *             
 *  FALLING ISR
 *    trigger: FALLING EDGE
 *    purpose: This ISR is used to obtain the width of the pwm signal pulse in microseconds. 
 */
  // channel 1 ISR(s)
    /* DECLARE ISR(s) HERE IF CH1 used */
    
  // channel 2 ISR(s)
    void CH2_RISE_ISR();
    void CH2_FALL_ISR();

  // channel 3 ISR(s)
    void CH3_RISE_ISR();
    void CH3_FALL_ISR();

  // channel 4 ISR(s)
    /* DECLARE ISR(s) HERE IF CH4 used */

  // channel 5 ISR(s)
    /* DECLARE ISR(s) HERE IF CH5 used */

  // channel 6 ISR(s)
    /* DECLARE ISR(s) HERE IF CH6 used */
#endif
