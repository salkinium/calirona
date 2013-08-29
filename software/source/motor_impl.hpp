// coding: utf-8
/* Copyright (c) 2009, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_A4988_HPP
#	error	"Don't include this file directly, use 'a4988.hpp' instead!"
#endif

template< typename Direction, typename Step, uint16_t MotorSteps >
int16_t xpcc::A4988<Direction, Step, MotorSteps>::stepTime(0);

template< typename Direction, typename Step, uint16_t MotorSteps >
int16_t xpcc::A4988<Direction, Step, MotorSteps>::numberOfSteps(0);

template< typename Direction, typename Step, uint16_t MotorSteps >
xpcc::PeriodicTimer<> xpcc::A4988<Direction, Step, MotorSteps>::toggleTimer(0);

template< typename Direction, typename Step, uint16_t MotorSteps >
bool xpcc::A4988<Direction, Step, MotorSteps>::running(false);

// ----------------------------------------------------------------------------
template< typename Direction, typename Step, uint16_t MotorSteps >
void
xpcc::A4988<Direction, Step, MotorSteps>::initialize()
{
	Direction::reset();
	Step::reset();
	running = false;
	toggleTimer.stop();
	stepTime = -1;
	numberOfSteps = -1;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
bool
xpcc::A4988<Direction, Step, MotorSteps>::setSpeed(int16_t degreesPerSecond)
{
	// if zero, stop and hold.
	if (degreesPerSecond == 0) {
		stepTime = -1;
		stop();
		return true;
	}

	// choose the direction
	if (degreesPerSecond < 0)
	{
		Direction::reset();
		degreesPerSecond = -degreesPerSecond;
	}
	else Direction::set();

	// number of steps in 2*pi
	float steps = MotorSteps * static_cast<float>(degreesPerSecond) / 360.f;
	stepTime = 2000.f / steps;

	if (stepTime == 0) stepTime = 1;
	toggleTimer.restart(stepTime);
	numberOfSteps = -1;

	return true;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
bool
xpcc::A4988<Direction, Step, MotorSteps>::rotateBy(int16_t degrees, uint16_t milliSeconds)
{
	if (!degrees)
		return false;

	// number of steps in 2*pi
	float steps = MotorSteps * static_cast<float>(degrees) / 360.f;
	float degreesPerSecond = degrees * 1000.f / static_cast<float>(milliSeconds);

	if (steps < 0) steps = -steps;

	setSpeed(degreesPerSecond);
	numberOfSteps = steps;

	return true;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
void
xpcc::A4988<Direction, Step, MotorSteps>::stop()
{
	stepTime = -1;
	numberOfSteps = -1;
}

template< typename Direction, typename Step, uint16_t MotorSteps >
bool
xpcc::A4988<Direction, Step, MotorSteps>::isRunning()
{
	return (stepTime != -1);
}

template< typename Direction, typename Step, uint16_t MotorSteps >
void
xpcc::A4988<Direction, Step, MotorSteps>::run()
{
	if (stepTime > 0)
	{
		if (toggleTimer.isExpired())
		{
			Step::toggle();

			if (numberOfSteps == 0) stop();
			if (numberOfSteps >= 0) numberOfSteps--;
		}
	}
}
