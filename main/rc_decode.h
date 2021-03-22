#define TRIMPIN 18
#define RUDDERPIN 19
#define CTRLPIN A8

#define TRIMOUTPUT 2
#define RUDDEROUTPUT 3

#define TRIMMIN 4.64
#define TRIMMAX 8.77
#define TRIMRANGE 4.13

#define RUDDERMIN 5.29
#define RUDDERMAX 8.93
#define RUDDERRANGE 3.64


// Channel Struct
/*  
 * Struct for individual channels on the receiver
 * 
 * place more of these in the class as needed to account for each channel
 * 
 * Each channel is going to need it's own interrupt service routine (ISR)
 * 
 * Each channel is also going to need a function to scale it's values
 * 
 */
struct channelStruct 
{
  volatile int pwmValue = 0;  // pos duty width in micro seconds
  volatile int prevRead = 0;  // read on interrupt
};

class RCDecode 
{
    public:

        RCDecode();

        int getRudderPWM();
        int getTrimPWM();

        static void trimRising();
        static void trimFalling();
        static void rudderRising();
        static void rudderFalling();

    private:
        // trim channel (channel 3)
        channelStruct* trimStruct;

        // rudder channel (channel 2)
        channelStruct* rudderStruct;

        // ctrl channel (channel 6) [NOT IMPLEMENTED]
        channelStruct* ctrlStruct;
};

/**
   pinout !
   trim -- PC0
   rudder -- PC1
*/
