#include "rc_decode.h"

// Trim struct
interruptStruct *trimStruct = (interruptStruct *)malloc(sizeof(interruptStruct));

// Rudder struct
interruptStruct *rudderStruct = (interruptStruct *)malloc(sizeof(interruptStruct));

float scaleTrimOutput(float dutyCycle) {

    float trimmedOutput = ((dutyCycle - TRIMMIN) / TRIMRANGE) * (1.8/5) * 255;
    if (trimmedOutput < 0) {
        trimmedOutput = 0;
    }
    else if (trimmedOutput > 91.8) {
       trimmedOutput = 91.8;
    }
    else if (trimmedOutput >= 43.5 & trimmedOutput <= 43.7) {
        trimmedOutput = 0.90;     // Crappy fix to hardware tendency to get stuck at 0.86 V in middle of knob swing
    }
    return trimmedOutput;        // Scales voltage to scale from 0 - 1.8V (scale factor determined by testing)
}

float scaleRudderOutput(float dutyCycle) {
  
    float trimmedOutput = ((dutyCycle - RUDDERMIN) / RUDDERRANGE) * (1.8/5) * 255;

    if (trimmedOutput < 0) {
      trimmedOutput = 0;
    }
    else if (trimmedOutput > 91.8) {
       trimmedOutput = 91.8;
    }
    return trimmedOutput;
}

void trimPinInterrupt(void) {
    
    trimStruct->lastRead = micros();        // Get current time
    if (digitalRead(TRIMPIN) == LOW) {
        // Falling edge
        trimStruct->fallTime = trimStruct->lastRead;       // Store falling edge time
    }
    else {
        unsigned long totalTime = trimStruct->lastRead - trimStruct->riseTime;
        unsigned long onTime = trimStruct->fallTime - trimStruct->riseTime;
        trimStruct->dutyCycle = 100.0 * (float)onTime / (float)totalTime;
        trimStruct->riseTime = trimStruct->lastRead;

        Serial.println(scaleTrimOutput(trimStruct->dutyCycle));
    }
}
        
void rudderPinInterrupt(void) {

    rudderStruct->lastRead = micros();         // Get current time
    if (digitalRead(RUDDERPIN) == LOW) {
        // Falling edge
        rudderStruct->fallTime = rudderStruct->lastRead;       // Store falling edge time
    }
    else {
        unsigned long totalTime = rudderStruct->lastRead - rudderStruct->riseTime;  // Get total cycle time
        unsigned long onTime = rudderStruct->fallTime - rudderStruct->riseTime;     // Get on time
        rudderStruct->dutyCycle = 100.0 * (float)onTime / (float)totalTime;                       // Compute duty cycle
        rudderStruct->riseTime = rudderStruct->lastRead;                            // Store rising edge time

        Serial.println(scaleRudderOutput(rudderStruct->dutyCycle));
    }
}

void setup(void) {
    // Start serial connection
    Serial.begin(115200); 
   
    // Set trim and rudder inputs to input
    pinMode(TRIMPIN, INPUT);
    pinMode(RUDDERPIN, INPUT);

    // Attach interrupt functions to change in pins
    PCintPort::attachInterrupt(TRIMPIN, trimPinInterrupt, CHANGE);
    PCintPort::attachInterrupt(RUDDERPIN, rudderPinInterrupt, CHANGE);

    Serial.println("Trim Pin Configured For Input.");
    Serial.println("Rudder Pin Configured For Input.");
}

void loop(void) {}
  /*
    // Get duty cycle for each pin and output over serial
    char trim_duty_cycle_str[24];
    char rudder_duty_cycle_str[24];
    
    sprintf(trim_duty_cycle_str, "Trim Duty Cycle =  %f", trimStruct->dutyCycle);
    sprintf(rudder_duty_cycle_str, "Rudder Duty Cycle =  %f", rudderStruct->dutyCycle);
    Serial.println(trim_duty_cycle_str);
    Serial.println(rudder_duty_cycle_str);

    delay(50);
}*/
