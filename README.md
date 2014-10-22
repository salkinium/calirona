# Calirona: A calibration robot for Corona

The [Corona][corona] headphones use a HMC6343 compass for heading information which needs to be calibrated from time to time.
To do this, a robot rotates the headphones around two axis while the compass is in calibration mode.

The main design goals of this project are to:

-	calibrate the compass,
-	minimize the required technical knowledge to operate the robot,
-	test the compass functionality to catch the most obvious malfunctions.

This project was designed and manufactured at [The Media Computing Group][i10].

## Design

The robot can move the headphones around both two axes a full 360°, which are supported using ball bearings. The z-axis uses a dish which holds the headphones.

Since the power and signal lines for the headphones and second axis motor need to be relayed through both axis, the robot cannot rotate infinitely around its axis.  
Therefore two position switches exist, which allow the robot to rotate its axes into known safe positions before attempting a rotation.  
The robot will then rotate 360° around the x-axis, then 360° around the z-axis, and then reverse all rotations until it has reached the starting state again.

The robot is controlled by two buttons Start and Stop, and can display its internal state through leds for power, busy, headphone error and mechanical error.  
The mapping is as follows:

- START: initialize and start the calibration.
- STOP: stop robot movement (short press), release motors and reset robot (long press)
- Busy (blue): blinks, when an operation is in progress, otherwise off
- Headphone (orange): blinks, when no headphone is connected, or it is unresponsive (perhaps broken), otherwise on
- Mechanical Error (red): blinks, when the robot detected an invalid mechanical state (broken or blocked motor), otherwise off

## Usage

The basic usage of the robots is as follows:

1. Plug in the power cord. The green power led should light up and the headphone error led should blink.
2. Place the headphone into the holder and plug in the headphone jack into it. The headphone error led should stop blinking and turn on fully, unless the headphone is broken.
3. Press START. The robot will initialize its axes.
4. Press START again. The robot will do two rotations while calibrating the heaphones.
5. Once the robot stopped moving, unplug the now calibrated headphones and remove them.

If the robot detected an invalid state during any of the actions, it will stop moving and blink an error led.

**Press the STOP button anytime to stop the robot from moving!**  
Pressing the STOP button for a longer time resets the robot.


## Organization


This repository is organized as follows:

- 	The *hardware* folder contains all hardware files for the robot.
	-	*hardware/et* contains the PCB schematics and layouts.
	-	*hardware/mech* contains the vector files of all mechanical parts of the robot (for the lasercutter).
- 	The *software* folder contains the software for the robot.
	- 	*software/source* contains the actual application code for this robot.
	-	*software/xpcc* contains the git submodule for the xpcc.io microcontroller framework.

All hardware and embedded software was designed by [Niklas Hauser][hauser].

[i10]: http://hci.rwth-aachen.de
[corona]: http://hci.rwth-aachen.de/corona
[hauser]: http://hci.rwth-aachen.de/hauser
