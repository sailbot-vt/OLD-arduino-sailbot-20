#include "rc_decode.h"
//#include "lidar.h"
#include "PinChangeInt.h"
#include <stdio.h>

// initalize trimStruct and ruddeStruct
interruptStruct *trimStruct = (interruptStruct *)malloc(sizeof(interruptStruct));
interruptStruct *rudderStruct = (interruptStruct *)malloc(sizeof(interruptStruct));

// initialize LiDAR struct
//LiDARData *lidar_data = (LiDARData *)malloc(sizeof(LiDARData));

// set up LiDAR connection
//SoftwareSerial LiDARSerial = setupLiDAR();

void setup(void) {

    // set up RC decode pins
    setupRCDecode();
  
    // Start serial connection to beaglebone
    Serial.begin(115200); 

    // Attach interrupt functions to change in pins
    PCintPort::attachInterrupt(TRIMPIN, trimPinInterrupt, CHANGE);
    PCintPort::attachInterrupt(RUDDERPIN, rudderPinInterrupt, CHANGE);

}

char trim_duty_cycle_str[24];
char rudder_duty_cycle_str[24];
char lidar_str[24];

void loop(void) {
  
    // Get duty cycle for each pin and output over serial
    sprintf(trim_duty_cycle_str, "Trim Val =  %f", getScaledTrimOutput(trimStruct->dutyCycle));
    sprintf(rudder_duty_cycle_str, "Rudder Val =  %f", getScaledRudderOutput(rudderStruct->dutyCycle));
    Serial.println(trim_duty_cycle_str);
    Serial.println(rudder_duty_cycle_str);

/*
    // read LiDAR reading and print it over serial
    readLiDARStruct(lidar_data);
    if (lidar_data->data != -1) {
      sprintf(lidar_str, "LiDAR Val =  %x", lidar_data->data);
      Serial.println(lidar_str);
    }
    else Serial.println("BAD LiDAR VAL");
*/
    delay(50);
}
