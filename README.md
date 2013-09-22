# Calibrator for Corona compass

The [Corona][corona] headphones use a HMC6343 compass for heading information which needs to be calibrated from time to time.
To do this, a robot rotates the headphones around two axis while the compass is in user calibration mode.
Designed by [The Media Computing Group][i10].

The main design goals of this project are to:

-	calibrate the compass,
-	minimize the required technical knowledge to operate the robot,
-	test the compass functionality to catch the most obvious malfunctions.


## Organisation


This repository is organized as follows:

- 	The *software* folder contains the software for the microcontroller.

The embedded software was designed by [Niklas Hauser][hauser].

[i10]: http://hci.rwth-aachen.de
[corona]: http://hci.rwth-aachen.de/corona
[hauser]: http://hci.rwth-aachen.de/hauser
