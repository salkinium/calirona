// coding: utf-8

#ifndef TASK_MECHANICS_HPP
#	error	"Don't include this file directly, use 'task_mechanics.hpp' instead!"
#endif

task::Mechanics::Mechanics(Leds &leds, Buttons &buttons)
:	leds(leds), buttons(buttons), correctionX(-15), correctionZ(0),
	xMotor(&OCR1AL, &TCCR1B), zMotor(&OCR0A, &TCCR0B)
{
}

void
task::Mechanics::initialize()
{
	// setup CTC mode for Z axis
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B = 0;
	OCR0A = 0;

	// setup CTC mode for X axis
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12);
	OCR1A = 0;

	releaseMotors();
	xMotor.initialize();
	zMotor.initialize();
}

xpcc::co::Result<bool>
task::Mechanics::calibrateX(void *ctx)
{
	CO_BEGIN(ctx);
	stopMotors();
	XZ_Enable::set();
	leds.setBusy();

	if (buttons.isX_AxisLimitPressed())
	{
		xMotor.rotateBy(100, 1500);
		CO_WAIT_WHILE(xMotor.isRunning());
	}

	xMotor.setSpeed(-75);
	timeout.restart(18000);
	CO_WAIT_UNTIL(buttons.isX_AxisLimitPressed() || timeout.isExpired());
	xMotor.stop();

	if ( (isCalibratedX = !timeout.isExpired()) )
	{
		xMotor.rotateBy(correctionX, 300);
		CO_WAIT_WHILE(xMotor.isRunning());
		leds.resetMechanicalError();
		leds.resetBusy();
		CO_RETURN(true);
	}

	leds.setMechanicalError();
	leds.resetBusy();
	CO_RETURN(false);

	CO_END();
}

xpcc::co::Result<bool>
task::Mechanics::calibrateZ(void *ctx)
{
	CO_BEGIN(ctx);
	stopMotors();
	XZ_Enable::set();
	leds.setBusy();

	zMotor.stop();
	if (buttons.isZ_AxisLimitPressed())
	{
		zMotor.rotateBy(100, 1000);
		CO_WAIT_WHILE(zMotor.isRunning());
		zMotor.stop();
	}

	zMotor.setSpeed(-75);
	timeout.restart(18000);
	CO_WAIT_UNTIL(buttons.isZ_AxisLimitPressed() || timeout.isExpired());

	zMotor.stop();
	if ( (isCalibratedZ = !timeout.isExpired()) )
	{
		zMotor.rotateBy(correctionZ, 1000);
		CO_WAIT_WHILE(zMotor.isRunning());
		leds.resetMechanicalError();
		leds.resetBusy();
		CO_RETURN(true);
	}

	leds.setMechanicalError();
	leds.resetBusy();
	CO_RETURN(false);

	CO_END();
}

xpcc::co::Result<bool>
task::Mechanics::rotateForward(void *ctx)
{
	CO_BEGIN(ctx);

	if (!isCalibrated())
		CO_RETURN(false);
	leds.setBusy();

	xMotor.rotateBy(3600, 10000);
	CO_WAIT_WHILE(xMotor.isRunning());

	zMotor.rotateBy(3600, 10000);
	CO_WAIT_WHILE(zMotor.isRunning());

	leds.resetBusy();
	CO_RETURN(true);

	CO_END();
}

xpcc::co::Result<bool>
task::Mechanics::rotateBackward(void *ctx)
{
	CO_BEGIN(ctx);

	leds.setBusy();

	zMotor.rotateBy(-3600, 10000);
	CO_WAIT_WHILE(zMotor.isRunning());

	xMotor.rotateBy(-3600, 10000);
	CO_WAIT_WHILE(xMotor.isRunning());

	leds.resetBusy();
	CO_RETURN(true);

	CO_END();
}

void
task::Mechanics::stopMotors()
{
	xMotor.stop();
	zMotor.stop();
	stopCoroutine();
	isCalibratedX = false;
	isCalibratedZ = false;
	leds.resetBusy();
}

void
task::Mechanics::releaseMotors()
{
	stopMotors();
	XZ_Enable::reset();
	leds.resetMechanicalError();
}

void
task::Mechanics::update()
{
	xMotor.update();
	zMotor.update();
	this->run();
}

bool
task::Mechanics::run()
{
	PT_BEGIN();

	while(true)
	{




		PT_YIELD();
	}

	PT_END();
}
