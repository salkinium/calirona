// coding: utf-8
/* Copyright (c) 2014, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef TASK_A4988_HPP
#	error	"Don't include this file directly, use 'task_motor.hpp' instead!"
#endif

// ----------------------------------------------------------------------------
template< typename Direction, typename Step, uint16_t MotorSteps >
bool
task::Motor<Direction, Step, MotorSteps>::setSpeed(int16_t deciDegreesPerSecond)
{
	// if zero, stop and hold.
	if (deciDegreesPerSecond == 0)
	{
		stop();
		return false;
	}

	// choose the direction
	Direction::set(deciDegreesPerSecond > 0);
	XPCC_LOG_DEBUG << "dDPS = " << deciDegreesPerSecond << xpcc::endl;

	// number of steps in 2*pi
	uint16_t speed = (static_cast<uint32_t>(5400) * 3600 * 18 / (static_cast<uint32_t>(MotorSteps) * 95)) / std::abs(deciDegreesPerSecond);
	if (speed > 255) speed = 255;
	setTimerSpeed(speed);

	mode = MODE_SPEED;
	startTimer();

	return true;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
bool
task::Motor<Direction, Step, MotorSteps>::rotateBy(int16_t deciDegrees, uint16_t milliSeconds)
{
	if (deciDegrees == 0)
		return false;

	// number of steps in 2*pi
	int16_t deciDegreesPerSecond = (static_cast<int32_t>(deciDegrees) * 1000) / milliSeconds;
	XPCC_LOG_DEBUG << "dDPS = " << deciDegreesPerSecond << xpcc::endl;
	setSpeed(deciDegreesPerSecond);
	{
		mode = MODE_ANGLE;
		timer.restart(milliSeconds);
		return true;
	}

	return false;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
void
task::Motor<Direction, Step, MotorSteps>::update()
{
	if (mode > MODE_SPEED && timer.isExpired())
	{
		stop();
	}
}
