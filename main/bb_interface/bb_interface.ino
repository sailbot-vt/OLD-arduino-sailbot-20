/**
 * bb_interface.ino
 * 
 * Test program to interface with the Beaglebone.
 * Uses Serial1 (Teensy 4.0 pins 0 [RX] and 1 [TX]).
 * 
 * Code adapted from example here:
 *  https://www.pjrc.com/teensy/td_uart.html
 * 
 * Evan Allen
 * SailBOT ECE 2020-2021
 */

// Macro for the interface we're using to talk to the 
// Beaglebone [BB].
//
// This lets us switch which pins we want to use later.
// The Teensy 4.0 has multiple hardware Serial ports
// (Serial1 .. Serial8): https://www.pjrc.com/teensy/td_uart.html
//
// Serial 1 uses pins 0 [RX] and 1 [TX].
#define BBSERIAL          Serial1

// UART format.
// 8 data bits, no parity, and 1 stop bit.
// This is the default on the Beaglebone [BB].
#define BBSERIAL_FORMAT   SERIAL_8N1

void setup() {
  // Just "Serial" represents the Serial connection to
  // the computer, not any of the external ones on
  // the side.
  Serial.begin(9600);
  BBSERIAL.begin(9600, BBSERIAL_FORMAT);
}

void loop() {
  char incomingByte;

  // Write to BB from computer.
  if (Serial.available() > 0)
  {
    incomingByte = Serial.read();
    Serial.print("Received via USB:\t");
    Serial.println(incomingByte);
    BBSERIAL.print("Received via USB:\t");
    BBSERIAL.println(incomingByte);
  }

  // Write to computer from BB.
  if (BBSERIAL.available() > 0)
  {
    incomingByte = BBSERIAL.read();
    Serial.print("Received via UART:\t");
    Serial.println(incomingByte);
    BBSERIAL.print("Received via UART:\t");
    BBSERIAL.println(incomingByte);
  }
}
