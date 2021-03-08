#define TRIMPIN A0
#define RUDDERPIN A1

#define TRIMOUTPUT 2
#define RUDDEROUTPUT 3

#define TRIMMIN 4.64
#define TRIMMAX 8.77
#define TRIMRANGE 4.13

#define RUDDERMIN 5.29
#define RUDDERMAX 8.93
#define RUDDERRANGE 3.64


// Interrupt struct
struct interruptStruct {
  volatile unsigned long fallTime = 0;       // Microsecond time of last falling edge
  volatile unsigned long riseTime = 0;       // Microsecond time of last rising edge
  volatile float dutyCycle = 0;               // Duty Cycle %
  volatile unsigned long lastRead = 0;        // Last interrupt time
};

class RCDecode {
    public:

        RCDecode();

        float getScaledTrimOutput();
        float getScaledRudderOutput();

        static void trimPinInterrupt();
        static void rudderPinInterrupt();

    private:
        interruptStruct *trimStruct;
        interruptStruct *rudderStruct;
};

// quick change
        

/**
   pinout !
   trim -- PC0
   rudder -- PC1
*/
