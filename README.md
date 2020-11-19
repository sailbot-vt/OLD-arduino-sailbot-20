Branch for adding motor controls in, right now we're really only adding Servo functionality, but other motors (primarily servos as of 11/19/2020) can be added as needed


The goal of this branch is to create an API for servos so that:

1) Each Servo can be individually referenced.
2) Allow for simple function calls that provide a natural interface to servo motors that wraps the Maestro interface.
3) Allow for feedback from a particular servo.
4) Allows for various parameters to passed as per servo characteristics.
5) Provides error checking with error feedback mechanisms.
