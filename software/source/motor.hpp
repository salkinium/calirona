// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef XPCC_A4988_HPP
#define XPCC_A4988_HPP

#include <xpcc/architecture/utils.hpp>
#include <xpcc/processing.hpp>

namespace xpcc
{

/**
 * A4988 stepper motor driver.
 *
 * @author	Niklas Hauser
 */
template< typename Direction, typename Step, uint16_t MotorSteps >
class A4988
{
public:
	static void
	initialize();

	static bool
	setSpeed(int16_t degreesPerSecond);

	static bool
	rotateBy(int16_t degrees, uint16_t milliSeconds);

	static void
	stop();

	static bool
	isRunning();

	static void
	run();

private:
	static int16_t stepTime;
	static int16_t numberOfSteps;
	static xpcc::PeriodicTimer<> toggleTimer;
	static bool running;

};

}	// namespace xpcc

#include "motor_impl.hpp"

#endif // XPCC_A4988_HPP
