Branch for adding motor controls in, right now we're really only adding Servo functionality, but other motors (primarily servos as of 11/19/2020) can be added as needed


The goal of this branch is to create an API for servos so that:

1) Each Servo can be individually referenced.
2) Allow for simple function calls that provide a natural interface to servo motors that wraps the Maestro interface.
3) Allow for feedback from a particular servo.
4) Allows for various parameters to passed as per servo characteristics.
5) Provides error checking with error feedback mechanisms.


## External dependencies: 
Since the Arduino IDE is by the far the easiest way to compile the repository's programs, we'll have to work around a few of its quirks, which mainly just involves moving some files around
once you've cloned this repo 

The two directories that have to be moved are the PololuMaestro library, and our own SB_Servo library. The Maestro can easily be installed with the Arduino IDE, but our personal library will have to be moved
by hand 

On the Linux, the following should accomplish everything listed below.
1) Install the Arduino IDE, you should have a directory at `/home/$USER/Arduino`
2) Move into the directory where you cloned the repo to, a `pwd` should show your path as: `/home/$USER/Blah/Blah/Blah/arduino-sailbot-20`
3) Execute the following command which copies the library dependencies into your Arduino library directory: `cp -r dependencies/libs/* ~/Arduino/libraries/`


On Windows, find the two directories, `PololuMaestro` and `SB_Servo` in `arduino-sailbot-20\dependencies\libs\`, and copy them into `C:\Users\yourName\Documents\Arduino\Libraries\`

## Testing:
Since traditional testing in C++ using the Catch framework is a little awkward, the current approach is to simply run various Arduino and ensure the correct outputs are presented on the Serial monitor. The current scripts directories for testing are located in `arduino-sailbot-20/main/Motors/testing`

> `testConversions` -- test the methods `degToMS(float)` and `msToDegrees(ms_t)`
> `testRotations` -- test the methods `rotateToDegrees(float)`, `getCurrentDegrees()` and `rotateByDegrees(float)`