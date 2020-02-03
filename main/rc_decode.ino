void setupRCDecode() {

    // Set trim and rudder inputs to input
    pinMode(TRIMPIN, INPUT);
    pinMode(RUDDERPIN, INPUT);

}

float getScaledTrimOutput(float dutyCycle) {

    float trimmedOutput = ((dutyCycle - TRIMMIN) / TRIMRANGE) * (1.8/5) * 255;
    if (trimmedOutput < 0) {
        trimmedOutput = 0;
    }
    else if (trimmedOutput > 91.8) {
       trimmedOutput = 91.8;
    }
    else if (trimmedOutput >= 43.5 & trimmedOutput <= 43.7) {
        trimmedOutput = 90.0;     // Crappy fix to hardware tendency to get stuck at 0.86 V in middle of knob swing
    }
    return trimmedOutput;        // Scales voltage to scale from 0 - 1.8V (scale factor determined by testing)
}

float getScaledRudderOutput(float dutyCycle) {

    float trimmedOutput = ((dutyCycle - RUDDERMIN) / RUDDERRANGE) * (1.8/5) * 255;

    if (trimmedOutput < 0) {
      trimmedOutput = 0;
    }
    else if (trimmedOutput > 91.8) {
       trimmedOutput = 91.8;
    }
    return trimmedOutput;
}

static void trimPinInterrupt() {
    
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
    }
}
        
static void rudderPinInterrupt() {

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
    }
}
