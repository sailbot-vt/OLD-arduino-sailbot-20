# Pulse Width Modulation (PWM) Channel Branch

This branch contains the implementation of a PWM reader for the Teensy and Arduino platforms. 

Note: *The current code was created and tested on a Teensy 4.0 using the Arduino integrated development environment (IDE) other platforms and/or IDEs may work but have not been tested*

---
## PWM Description:
### ***What Is PWM?***
Pulse width modulation (PWM) is a method of creating a time varying signal by changing the time a direct current (DC) signal is logic level 1 in relation to the time it is at logic level 0.

Varying the time the DC signal is at logic level 1 is done by changing the "on time" or the "duty cycle" of the signal, where the term "duty cycle" refers to the percentage of time the signal spends "on" or at logic level 0 with respect to its period. For example, a signal that is on for half of its period is said to have a 50% duty cycle since it is at logic level 1 for exactly half of its period. The other half the signal spends at logic level 0.

By varying the duty cycle of a DC signal a time varying signal is created that can drive an analog device such as a dimming light, motor, or in this case being used on SailBot, to carry information for a servo.

For more information on the topic please visit the following websites:

> [Wikipedia Link](https://en.wikipedia.org/wiki/Pulse-width_modulation)

> [Spark Fun Link](https://learn.sparkfun.com/tutorials/pulse-width-modulation/all)

> [Youtube Link](https://www.youtube.com/watch?v=5nwNKPs2gco)

### ***How Are We Using It?***
The signals coming from the AR620 remote control receiver (RC receiver) are variations of PWM that contain information regarding how a servo should position itself. The "on time" of these signals is usually in the range of one to two milliseconds. It is this "on time" that the code contained within this branch measures.

More specifically, the positive pulse width of the PWM signal varies according to which direction a servo should be turning. By measuring the pulse width we can use other code to determine which direction we should turn whichever servo is being controlled by the channel being read

### ***How Are We Measuring It?***
The width of the positive pulse can be calculated by subtracting the rising edge time from the falling edge time. That is, allow time to be represented by "t" and let "t = 0" at the rising edge of the PWM signal. When the falling edge is encountered the time "t", is now some time in microseconds, this represents the time the signal is at logic level 1. Of course when "t" does not equal zero the equation become more general as "t1 - t0" where "t0" is the rise time and "t1" is the fall time.

To obtain the rise time and fall time of the PWM signal, external hardware interrupts are being used. An interrupt in it's simplest terms is the halting of main code briefly to execute some other function then continue the main code. This is different from a function call in that it can happen any time at runtime when the code doesn't "expect" it. Since the PWM signals are not "synced" with the Arduino / Teensy, allowing the occurance of a rising edge and a falling edge to interrupt the main code becomes the practical choice.

This means that there are two conditions that will trigger an interrupt, the detection of a rising edge and the detection of a falling edge. On the occurance of each interrupt the main code will stop executing and an interrupt handler is run, since there are two distinct interrupts, there need to be two distinct interrupt handlers, one for rising and the other for falling.

For more information related to interrupts visit the following websites:

>[Adafruit link](https://learn.adafruit.com/multi-tasking-the-arduino-part-2/what-is-an-interrupt)

>[Tutorials Point Link](https://www.tutorialspoint.com/arduino/arduino_interrupts.htm)

## PWM Channel Description:
### ***AR620 Channels***
The AR620 RC receiver is a six channel receiver capable of outputting six distinct PWM signals provided the RC radio transmitter being used uses all six channels.

### ***PWM Channel Struct***
As mentioned earlier, the code contained within this branch targets the "on time" of the PWM signal. In order to read from the RC receiver's six channels independently, a struct containing information about the "on time" is used. Below is the structure of a PWM Channel struct:

 ```C
struct pwmChannel
{
  // rise time of the pwm signal
  volatile int riseTime = 0;

  // pulse time of the pwm signal
  volatile int pwmValue = 0;
};
```

The struct is very succinct only containing two data members however, these members are all we need to obtain a measurement of the signal's "on time".

#### ***Struct Data Members***
> volailte int riseTime
>> Used to record the time in microseconds a rising edge of the PWM signal was encountered. Will later be used in calculating the pulse width.

> volatile int pwmValue
>> Used to record the pulse width of the positive pulse of the pwmSignal. The variable is updated on every falling edge using the following formula: "pwmValue = fallTime - riseTime" where "fallTime" is a numeric literal corresponding to the encounter with the falling edge of a PWM pulse. This value is also in microseconds.

### ***PWM Channel Interrupts***
Earlier the notion of an interrupt handler was introduced, below are such handlers. Two handlers are needed to correctly measure the width, one for rising and one for falling.
#### ***Rising Edge***
```Arduino
void CH2_RISE_ISR()
{
    attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_FALL_ISR, FALLING);
    CH2.riseTime = micros();
}
```
This interrupt service routine / handler (ISR) is triggered when the digital pin connected to channel 2 of the receiver encounters the rising edge of the PWM signal.

This ISR has two jobs:

1. Attach the new interrupt to occur when the falling edge is encountered
2. Record the encounter time in microseconds of the rising edge

#### ***Falling Edge***
```Arduino
void CH2_FALL_ISR()
{
    attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
    CH2.pwmValue = micros() - CH2.riseTime;
}
```
This ISR is triggered when the digital pin connected to channel 2 of the receiver encounters the falling edge of the PWM signal.

This ISR has two jobs:

1. Attach the new interrupt to occur when the rising edge is encountered
2. Compute the width of the positive pulse in microseconds using the encounter time of falling edge in the following equation; "width = fallTime - riseTime".

### ***PWM Channel Initialization***
To set everything up required for the interrupts to function, an initialization function is used. 
```Arduino
void initPWM()
{
  // configure digital IO pins
    pinMode(CH2_PIN, INPUT);  
    pinMode(CH3_PIN, INPUT);

  // attach rising edge interrupts
    attachInterrupt(digitalPinToInterrupt(CH2_PIN), CH2_RISE_ISR, RISING);
    attachInterrupt(digitalPinToInterrupt(CH3_PIN), CH3_RISE_ISR, RISING);
}
```
To use a pwmChannel the rising edge ISR needs to be attached in addition to the pin reading the signal being declared as an input. This function will need to be modified to reflect the number of channels being used, one channel equals one pinMode() call and one rising edge interrupt attached.
## Using the PWM Channel Struct:
### ***Hardware***
To begin measuring the pulse width of a PWM signal with the pwmChannel struct you will need to following pieces of hardware:

1. Computer capable of communicating with an Arduino and/or Teensy board via a USB capable.
2. Arduino or Teensy platform with support for functions found in the "Arduino.h" library (Other platforms may work provided they support Arduino based functions)
3. AR620 six channel RC receiver (Other receivers will likely work however, measurements should be carried out to ensure correct readings prior to use in an actual setting)
4. RC Radio Transmitter paired to the AR620 RC receiver (or the RC reciever being used in place)
5. Voltage supply capable of delivering a minimum of 3.5V DC to supply the AR620 RC receiver (don't put more than 9V DC across it)
6. Jumper wires (male to male, female to male, female to male) The AR620 provides a male pin so it needs a female to female / male jumper wire depending on what you wish to connect it to.

### ***Software***
In addition to the afore mentioned hardware, you will require the following software:

1. An up to date install of the Arduino IDE. If you are using the Teensy you need to install the proper support for the boards which can be found [here](https://www.pjrc.com/teensy/td_download.html). Other modes of software that provide support for Arduino and Teensy may work however as of writing none have been tested.
2. "pwm_channel.h" and "pwm_channel.ino" downloaded to either your Arduino library folder or the local folder for the project using the pwmChannel.

### ***Set Up and Connections***
The AR620 receiver, RC radio transmitter, and Arduino/Teensy will need to be powered on. The AR620 requires a supply voltage of 3.5V DC, Teensy 3.3V DC (Teensy 4.0), and Arduino 5V DC (Arduino UNO).

The AR620 receiver and the Arduino / Teensy being used to read the signals need to share a common ground. This is most easily done by connecting the negative pin of the AR620 directly to the ground (GND) pin of the Arduino / Teensy, or by connecting it to a rail serving as GND for the system.

Once power is supplied to the Arduino / Teensy and the AR620 the number of channels being used needs to be determined, this is done by simply counting the number you are using from the AR620. Each channel will require its own instance of a pwmChannel struct. For example, using channels 2 and 3 of the AR620 will require two instances in your main code, each with their own rising and falling ISRs

```C
pwmChannel CH2;
pwmChannel CH3;
```
To add the rising and falling ISRs you will need to add the function declarations inside "pwm_channel.h" and define the functions within "pwm_channel.ino". Instructions on how to do this are found inside those files. For information on connecting interrupts and writing ISRs visit the following [link](https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/). In short ISRs are void type, take no inputs, and should be quick.

You will also need to edit the initialization function mentioned earlier so the rising edge is attached on the pin you are using to read a channel.

If you followed the above steps you should now be able to power on your RC radio transmitter and obtain PWM readings from the channels you are reading.

To obtain the information about the pulse width you will need to access the "pwmValue" member of a channel's pwmChannel struct like so:
```c
CH2.pwmValue;
```
You shouldn't have to touch the "riseTime" variable as this is only there to log the first occurance of the rising edge.


# Other Useful Links
[AR620 User Manual](https://www.horizonhobby.com/on/demandware.static/-/Sites-horizon-master/default/dw7382b6a5/Manuals/SPMAR620-Manual-EN.pdf)

[Wikipedia Link on Servo Control](https://en.wikipedia.org/wiki/Servo_control)



