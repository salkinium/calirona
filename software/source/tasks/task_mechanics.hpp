// coding: utf-8
#ifndef TASK_MECHANICS_HPP
#define TASK_MECHANICS_HPP

#include <xpcc/processing/protothread.hpp>
#include <xpcc/processing/coroutine.hpp>
#include <xpcc/processing/timeout.hpp>
#include "../hardware.hpp"
#include "task_motor.hpp"
#include "task_leds.hpp"
#include "task_buttons.hpp"

namespace task
{

class Mechanics : private xpcc::pt::Protothread, public xpcc::co::Coroutine
{
public:
	Mechanics(Leds &leds, Buttons &buttons);

	void
	initialize();

	void
	update();

	xpcc::co::Result<bool>
	calibrateX(void *ctx);

	xpcc::co::Result<bool>
	calibrateZ(void *ctx);

	bool ALWAYS_INLINE
	isCalibrated()
	{
		return isCalibratedX && isCalibratedZ;
	}

	xpcc::co::Result<bool>
	rotateForward(void *ctx);

	xpcc::co::Result<bool>
	rotateBackward(void *ctx);

	void
	stopMotors();

	void
	releaseMotors();

private:
	void
	startMotors();

	bool
	run();

	Leds &leds;
	Buttons &buttons;
	xpcc::Timeout<> timeout;

	bool isCalibratedX;
	bool isCalibratedZ;

	int16_t correctionX;
	int16_t correctionZ;

	Motor< X_Dir, X_Step, 400 * 8 > xMotor;
	Motor< Z_Dir, Z_Step, 400 * 8 > zMotor;
	xpcc::Timeout<> motorTimeout;
};

} // namespace task

#include "task_mechanics_impl.hpp"

#endif // TASK_MECHANICS_HPP
