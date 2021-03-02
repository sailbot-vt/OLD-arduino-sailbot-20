#!/bin/bash

# Call this script from where the script sits, don't do anything silly and call it from 
# a directory up or down, you'll probably mess up $PWD and that makes my life hard

# Makes the Arduino libraries folder where our internal library will reside 

if [[ $1 == "-c" ]]; then 
	rm -rf ~/Arduino/libraries/SB_Servo
	rm -rf ~/Arduino/libraries/PololuMaestro
else 

	mkdir -p ~/Arduino/libraries/SB_Servo

	# Copy the maestro library into the Arduino library 
	# I'm just copying this because I dont know everything thats required from the Maestro,
	# so we're just copying all of it
	cp -r $PWD/dependencies/libs/PololuMaestro ~/Arduino/libraries 

	# Create a soft link from this repository's SB_Servo library, that way 
	# whenever the repo's library is updated, the user's local library is updated as well.
	# This is very important because Arduino demands that library files are in one place
	# Which is very annoying, but it's a fair trade off as the tool chain #justWorks
	ln -fs $PWD/dependencies/libs/SB_Servo/SB_Servo.hpp ~/Arduino/libraries/SB_Servo/SB_Servo.hpp
	ln -fs $PWD/dependencies/libs/SB_Servo/SB_Servo.cpp ~/Arduino/libraries/SB_Servo/SB_Servo.cpp
fi



