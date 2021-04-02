// Initialization function
/* Function Structure / Implementation
 *  
 *  The basic structure this function should follow is as follows:
 *    1. digital IO configures using pinMode(#pin,INPUT)
 *    2. rising edge interrupt attaches using attachInterrupt(digitalPinToInterrupt(#pin), #RISING_ISR, RISING)
 *    
 *  The rising edge ISR NEEDS to be the only one attached in this function, this is due to 
 *  a digital IO pin only being able to have one interrupt attached at a time. Due to the calculation
 *  taking place within the falling edge ISR, the rising edge must be attached first.
 *  
 *  Every instance of a pwmChannel struct is required to have its pin configured as an input and have its
 *  associated rising ISR attached within this function.
 */
void initPWM()
{
  // configure digital IO pins
    pinMode(CH2_PIN, INPUT);  
    pinMode(CH3_PIN, INPUT);

  // attach rising edge interrupts
    attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_RISE_ISR, RISING);
}

// ISR DEFINITIONS
  // RISING EDGE
  /*  EVERY rising edge interrupt must first attach the corresponding falling edge
   * interrupt for its associated pin using attachInterrupt(digitalPinToInterrupt(#pin), #FALLING_ISR, FALLING)
   * This is due to digital IO pin limitations of having only one interrupt attached at a time. To measure the
   * width of the pwm pulse for this associated channel, the next interrupt MUST be a falling edge.
   * 
   *  The rising edge ISR is used to log the rise time of the positive edge of the pwm signal pulse.
   * This is accomplished using the micros() function. DO NOT use millis(), whilst in an ISR, millis() does not function
   * however, micros() does.
   * 
   *  The rising edge ISR should do nothing else and should exit right after the micros() call.
   */

    // channel 2 ISR (rising)
      void CH2_RISE_ISR()
      {
        attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_FALL_ISR, FALLING);
        CH2.riseTime = micros();
      }

    // channel 3 ISR (rising)
      void CH3_RISE_ISR()
      {
        attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_FALL_ISR, FALLING);
        CH3.riseTime = micros();
      }

  // FALLING EDGE
  /*  EVERY falling edge interrupt must first attach the corresponding rising edge
   * interrupt for its associated pin using attachInterrrupt(digitalPintToInterrupt(#pin), #RISING_ISR, RISING)
   * This is due to digital IO pin limitations of having only one interrupt attached at a time. To begin capture
   * of the next pwm signal pulse for this associated channel, the next interrupt MUST be a falling edge.
   * 
   *  The falling edge ISR is used to calculate the width of the pwm pulse in microseconds.
   * This is accomplished using the micros() function. DO NOT use millis(), whilst in an ISR, millis() does not function
   * however, micros() does.
   * 
   *  The calculation taking place is as follows TIMEfalling - TIMErising = pwm signal pusle width (microseconds)
   */

    // channel 2 ISR (falling)
      void CH2_FALL_ISR()
      {
        attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
        CH2.pwmValue = micros() - CH2.riseTime;
      }

      // channel 3 ISR (falling)
      void CH3_FALL_ISR()
      {
        attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_RISE_ISR, RISING);
        CH3.pwmValue= micros() - CH3.riseTime;
      }
