#include "rc_decode.h"
//#include "lidar.h"
//#include "PinChangeInt.h"
#include <stdio.h>

// initalize trimStruct, ruddeStruct, and ctrlStruct
channelStruct *trimStruct = (channelStruct *)malloc(sizeof(channelStruct));
channelStruct *rudderStruct = (channelStruct *)malloc(sizeof(channelStruct));
//channelStruct *ctrlStruct = (interruptStruct *)malloc(sizeOf(interruptStruct));


// initialize LiDAR struct
//LiDARData *lidar_data = (LiDARData *)malloc(sizeof(LiDARData));

// set up LiDAR connection
//SoftwareSerial LiDARSerial = setupLiDAR();

void setup(void) {

    // set up RC decode pins
    setupRCDecode();
  
    // Start serial connection to beaglebone
    //Serial.begin(115200); 

    // Start serial connection to the Teensy 4.0
    Serial.begin(9600);
}

void loop(void) 
{
  //Serial.print("Rudder: "); //Serial.print(getRudderPWM());
  //Serial.println("Trim: "); //Serial.print(getTrimPWM());
}
