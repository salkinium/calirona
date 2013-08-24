# Calibrator for Corona compass

The [Corona][corona] headphones use a HMC6343 compass for heading information which needs to be calibrated from time to time.
To do this, a robot rotates the headphones around two axis [The Media Computing Group][i10].

The main design goals of this project are to:

-	minimize the required technical knowledge to operate the robot,
-	create a mechanically stable and long lasting construction,
-	test the compass functionality to catch the most obvious malfunctions.


## Organisation


This repository is organized as follows:

-	The *hardware* folder contains the EAGLE schematics and layouts of the
	electronic assembly, with parts list and datasheets.
- 	The *software* folder contains the software for the microcontroller.
- 	The *robot* folder contains all OpenSCAD source code and STL files for
	the robot construction.

Robot and software were designed by [Niklas Hauser][hauser].

[i10]: http://hci.rwth-aachen.de
[corona]: http://hci.rwth-aachen.de/corona
[hauser]: http://hci.rwth-aachen.de/hauser
