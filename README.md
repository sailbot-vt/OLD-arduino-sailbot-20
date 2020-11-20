Branch for adding motor controls in, right now we're really only adding Servo functionality, but other motors (primarily servos as of 11/19/2020) can be added as needed


The goal of this branch is to create an API for servos so that:

1) Each Servo can be individually referenced.
2) Allow for simple function calls that provide a natural interface to servo motors that wraps the Maestro interface.
3) Allow for feedback from a particular servo.
4) Allows for various parameters to passed as per servo characteristics.
5) Provides error checking with error feedback mechanisms.

Motor source code shall reside in `./main/Motors/src` while .hpp's shall be in `./main/Motors/inc`

External dependencies: 


`PololuMaestro` -- this can be installed by using `crtl+shift+I` from within the Arduino IDE and then searching for the library.
	If for some ungodly reason this isn't working you can get these files from within this repo, however you'll have to put them into a specific directory yourself
	
The Arduino one (AND ONLY) `ld` path is along the lines of `yourOSprefix/User/Documents/Arduino/libraries`. It then searches for library directories from within there. (You may be able to get away with just modifying `$LD_LIBRARY_PATH` and that may work but I haven't tried it since the changes won't persist) 	

To use the PololuMaestro library, the library directory must be copied into the above directory. These files are included in the 
	`./dependencies/PololuMaestro` directory and need to be moved to the above directories. The final path once pasted should be along the lines of 
	`/home/yourName/Documents/Arduino/PololuMaestro/` for a unix system, or `C:\Users\yourName\Documents\Arduino\PololuMaestro` for windows





notes: 
	Our self wrapper Library (SB_Servo) is going to most likely have to use relational `#includes`. This is because it's compiling Arduino stuff by hand is difficult, thus altering the library path 
	is also difficult. For now we'll use relational include statements, such as `#include ../../Servo.h`
