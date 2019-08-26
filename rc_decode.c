#include "rc_decode.h"

// Trim struct
interruptStruct trimStruct = malloc(sizeof(interruptStruct));

// Rudder struct
interruptStruct rudderStruct = malloc(sizeof(interruptStruct));

void setup(void) {
    // Start serial connection
    Serial.begin(115200);
    
    // Set trim and rudder pins to input
    pinMode(TRIMPIN, INPUT);
    pinMode(RUDDERPIN, INPUT);

    // Attach interrupt functions to change in pins
    attachInterrupt(TRIMPIN, trimPinInterrupt, CHANGE);
    attachInterrupt(RUDDERPIN, rudderPinInterrupt, CHANGE);

    Serial.println(F("Trim Pin Configured For Input."));
    Serial.println(F("Rudder Pin Configured For Input."));
}

void loop(void) {
    // Get duty cycle for each pin and output over serial
    char trim_duty_cycle_str[24];
    char rudder_duty_cycle_str[24];
    
    sprintf(trim_duty_cycle_str, "Trim Duty Cycle =  %b", trimStruct->dutyCycle);
    sprintf(rudder_duty_cycle_str, "Rudder Duty Cycle =  %b", rudderStruct->dutyCycle);
    Serial.println(trim_duty_cycle_str);
    Serial.println(rudder_duty_cycle_str);

    delay(50);
}

void trimPinInterrupt(void) {
    
    trimStruct->lastRead = micros()         // Get current time
    if (digitalRead(TRIMPIN) == LOW) {
        // Falling edge
        trimStruct->fallTime;       // Store falling edge time
    }
    else {
        unsigned long totalTime = trimStruct->riseTime - trimStruct->lastRead;
        unsigned long onTime = trimStruct->lastRead - trimStruct->fallTime;
        trimStruct->dutyCycle = 100 * (onTime / totalTime);
        trimStruct->riseTime = rudderStruct->lastRead;
    }
}
        
void rudderPinInterrupt(void) {
    
    rudderStruct->lastRead = micros()         // Get current time
    if (digitalRead(RUDDERPIN) == LOW) {
        // Falling edge
        rudderStruct->fallTime = rudderStruct->lastRead;       // Store falling edge time
    }
    else {
        unsigned long totalTime = rudderStruct->riseTime - rudderStruct->lastRead;  // Get total cycle time
        unsigned long onTime = rudderStruct->lastRead - rudderStruct->fallTime;     // Get on time
        rudderStruct->dutyCycle = 100 * (onTime / totalTime);                       // Compute duty cycle
        rudderStruct->riseTime = rudderStruct->lastRead;                            // Store rising edge time
    }
}
