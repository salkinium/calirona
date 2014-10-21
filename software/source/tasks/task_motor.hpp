// coding: utf-8
/* Copyright (c) 2013, Roboterclub Aachen e.V.
 * All Rights Reserved.
 *
 * The file is part of the xpcc library and is released under the 3-clause BSD
 * license. See the file `LICENSE` for the full license governing this code.
 */
// ----------------------------------------------------------------------------

#ifndef TASK_A4988_HPP
#define TASK_A4988_HPP

#include <xpcc/architecture/utils.hpp>
#include <xpcc/processing/timeout.hpp>
#include <xpcc/debug/logger.hpp>

#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::INFO

namespace task
{

/**
 * A4988 stepper motor driver.
 *
 * @author	Niklas Hauser
 */
template< typename Direction, typename Step, uint16_t MotorSteps >
class Motor
{
public:
	Motor(volatile uint8_t *compare, volatile uint8_t *prescalar, uint16_t &steps)
	:	mode(MODE_OFF), compare(compare), prescalar(prescalar), steps(steps)
	{
		stopTimer();
		steps = 0;
	}

	void
	initialize()
	{
		Direction::setOutput(xpcc::Gpio::Low);
		Step::setOutput(xpcc::Gpio::Low);
	}

	bool
	setSpeed(int16_t deciDegreesPerSecond);

	bool
	rotateBy(int16_t deciDegrees, uint16_t milliSeconds);

	void
	stop()
	{
		stopTimer();
		mode = MODE_OFF;
		Step::reset();
		steps = 0;
	}

	bool inline
	isRunning()
	{
		return (mode > MODE_OFF);
	}

	void
	update();

private:
	void inline
	stopTimer()
	{
		*prescalar &= ~0b111;
	}

	void inline
	startTimer()
	{
		// Prescaler 1024
		// horrible hack, just horrible.
		if (prescalar == &TCCR0B) TCNT0 = 0;
		else TCNT1 = 0;
		*prescalar |= 0b101;
	}

	void inline
	setTimerSpeed(uint8_t speed)
	{
		if (speed == 0) speed = 1;
		*compare = speed;
		XPCC_LOG_DEBUG << "speed = " << speed << xpcc::endl;
	}

	enum
	{
		MODE_OFF = 0,
		MODE_SPEED = 1,
		MODE_ANGLE = 2
	};
	volatile uint8_t mode;
	uint16_t required_steps;

	volatile uint8_t *compare;
	volatile uint8_t *prescalar;
	uint16_t &steps;
};

}	// namespace task

#include "task_motor_impl.hpp"

#endif // TASK_A4988_HPP
