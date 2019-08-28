#include "PinChangeInt.h"

#define TRIMPIN A0
#define RUDDERPIN A1

#define TRIMOUTPUT D2
#define RUDDEROUTPUT D3

#define TRIMMIN 5.49
#define TRIMMAX 9.14

#define RUDDERMIN 5.49
#define RUDDERMAX 8.93

// Interrupt struct

struct interruptStruct {
  volatile unsigned long fallTime = 0;       // Microsecond time of last falling edge
  volatile unsigned long riseTime = 0;       // Microsecond time of last rising edge
  volatile float dutyCycle = 0;               // Duty Cycle %
  volatile unsigned long lastRead = 0;        // Last interrupt time
};

/**
   pinout !
   trim -- PC0
   rudder -- PC1
*/
