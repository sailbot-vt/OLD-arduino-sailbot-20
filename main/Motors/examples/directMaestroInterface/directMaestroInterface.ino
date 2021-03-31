/** 
	A very minimalist setup for checking to make sure the maestro is working properly
	Needs: minimaestro, teensy 3.2, 5V power supply for maestro

	Connections are as follows: Maestro Rx -> teensy Tx
	Maestro Tx -> teensy Rx
	Teensy and Maestro both connected to power supply's ground. 
	Maestro Vin -> powersupply 5V
	Teensy connected via usb
	1 servo attached to the maestro's first slot
*/
#include <PololuMaestro.h>
#include <vector> 
MiniMaestro maestro(Serial1);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
} 

void loop() {
 if (Serial.available()) {
  
	/** Some stuff for testing the set multi target method on the maestro
    maestro.setTarget(0, 10000);
    delay(1000);
    maestro.setTarget(1, 9000);
    delay(2000);
    uint16_t targets[2] =  {4000, 2000};
    //uint16_t *target = &targets[0];
    // *targetPtr = &targets;
    maestro.setMultiTarget(2, 0, &targets[0]);
    */
    int val = Serial.parseInt();
    maestro.setTarget(0, val);
    int pos = maestro.getPosition(0);
    Serial.print("Position: ");
    Serial.println(pos);
  }
} 
