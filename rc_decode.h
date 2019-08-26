// Interrupt structs

#define TRIMPIN 0
#define RUDDERPIN 1

struct interruptStruct {
    volatile unsigned long fallTime = 0;       // Microsecond time of last falling edge
    volatile unsigned long riseTime = 0;       // Microsecond time of last rising edge
    volatile byte dutyCycle = 0;               // Duty Cycle %
    volatile unsigned long lastRead = 0;        // Last interrupt time
};

void trimPinInterrupt(void);
void rudderPinInterrupt(void);



/**
 * pinout !
 * trim -- PC0
 * rudder -- PC1
 */

