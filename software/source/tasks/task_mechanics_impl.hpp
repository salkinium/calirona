// coding: utf-8

#ifndef TASK_MECHANICS_HPP
#	error	"Don't include this file directly, use 'task_mechanics.hpp' instead!"
#endif

task::Mechanics::Mechanics(Leds &leds, Buttons &buttons)
:	leds(leds), buttons(buttons),
	isCalibratedX(false), isCalibratedZ(false),
	correctionX(-40), correctionZ(15),
	xMotor(&OCR1AL, &TCCR1B, t1_steps), zMotor(&OCR0A, &TCCR0B, t0_steps),
	motorTimeout(30000)
{
}

void
task::Mechanics::initialize()
{
	// setup CTC mode for Z axis
	TCCR0A = (1 << COM0A0) | (1 << WGM01);
	TCCR0B = 0;
	OCR0A = 0;
	TIMSK0 = (1 << OCIE0A);

	// setup CTC mode for X axis
	TCCR1A = (1 << COM1A0);
	TCCR1B = (1 << WGM12);
	OCR1A = 0;
	TIMSK1 = (1 << OCIE1A);

	XZ_Enable::setOutput(xpcc::Gpio::Low);

	releaseMotors();
	xMotor.initialize();
	zMotor.initialize();
}

xpcc::co::Result<bool>
task::Mechanics::calibrateX(void *ctx)
{
	CO_BEGIN(ctx);
	startMotors();

	if (buttons.isX_AxisLimitPressed())
	{
		xMotor.rotateBy(100, 500);
		CO_WAIT_WHILE(xMotor.isRunning());
	}

	xMotor.setSpeed(-200);
	timeout.restart(2000);
	CO_WAIT_UNTIL(buttons.isX_AxisLimitPressed() || timeout.isExpired());
	xMotor.stop();

	if ( (isCalibratedX = !timeout.isExpired()) )
	{
		xMotor.rotateBy(correctionX, 200);
		CO_WAIT_WHILE(xMotor.isRunning());
		leds.resetMechanicalError();
		leds.resetBusy();
		CO_RETURN(true);
	}

	leds.setMechanicalError();
	leds.resetBusy();
	CO_END_RETURN(false);
}

xpcc::co::Result<bool>
task::Mechanics::calibrateZ(void *ctx)
{
	CO_BEGIN(ctx);
	startMotors();

	zMotor.stop();
	if (buttons.isZ_AxisLimitPressed())
	{
		zMotor.rotateBy(100, 500);
		CO_WAIT_WHILE(zMotor.isRunning());
		zMotor.stop();
	}

	zMotor.setSpeed(-200);
	timeout.restart(2000);
	CO_WAIT_UNTIL(buttons.isZ_AxisLimitPressed() || timeout.isExpired());

	zMotor.stop();
	if ( (isCalibratedZ = !timeout.isExpired()) )
	{
		zMotor.rotateBy(correctionZ, 300);
		CO_WAIT_WHILE(zMotor.isRunning());
		leds.resetMechanicalError();
		leds.resetBusy();
		CO_RETURN(true);
	}

	leds.setMechanicalError();
	leds.resetBusy();
	CO_END_RETURN(false);
}

xpcc::co::Result<bool>
task::Mechanics::rotateForward(void *ctx)
{
	CO_BEGIN(ctx);

	if (!isCalibrated())
		CO_RETURN(false);

	startMotors();

	xMotor.rotateBy(3600, 10000);
	CO_WAIT_WHILE(xMotor.isRunning());

	zMotor.rotateBy(3600, 10000);
	CO_WAIT_WHILE(zMotor.isRunning());

	leds.resetBusy();
	isCalibratedX = false;
	isCalibratedZ = false;
	CO_END_RETURN(true);
}

xpcc::co::Result<bool>
task::Mechanics::rotateBackward(void *ctx)
{
	CO_BEGIN(ctx);
	startMotors();

	zMotor.rotateBy(-3600, 5000);
	CO_WAIT_WHILE(zMotor.isRunning());

	xMotor.rotateBy(-3600, 5000);
	CO_WAIT_WHILE(xMotor.isRunning());

	releaseMotors();
	CO_END_RETURN(true);
}

void
task::Mechanics::startMotors()
{
	leds.setBusy();
	XZ_Enable::set();
	motorTimeout.restart(30000);
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
		if (!xMotor.isRunning() && !zMotor.isRunning() && motorTimeout.isExpired())
		{
			stopMotors();
			XZ_Enable::reset();
		}

		PT_YIELD();
	}

	PT_END();
}
