
#ifndef TASK_ROTATE_HPP
#	error	"Don't include this file directly, use 'task_rotate.hpp' instead!"
#endif

void
task::Rotate::start()
{
	this->restart();
}

bool
task::Rotate::isFinished()
{
	return (!this->isRunning() );
}

void
task::Rotate::update()
{
	yMotor.run();
	zMotor.run();
	this->run();
}

bool
task::Rotate::run()
{
	PT_BEGIN();

	Y_Enable::reset();
	Z_Enable::reset();

	Leds::write(0b1000);

//	PT_WAIT_UNTIL(compass.enterUserCalibrationMode());

	yMotor.rotateBy(360, 7500);

	PT_WAIT_UNTIL(!yMotor.isRunning());

	Leds::write(0b1100);
	zMotor.rotateBy(360, 7500);

	PT_WAIT_UNTIL(!zMotor.isRunning());

	Leds::write(0b0010);

//	PT_WAIT_UNTIL(compass.exitUserCalibrationMode());

	zMotor.rotateBy(-360, 2000);

	PT_WAIT_UNTIL(!zMotor.isRunning());

	Leds::write(0b0011);
	yMotor.rotateBy(-360, 2000);

	PT_WAIT_UNTIL(!yMotor.isRunning());

	Leds::write(0b1111);

	Y_Enable::set();
	Z_Enable::set();

	PT_END();	// return in included in PT_END();
}
