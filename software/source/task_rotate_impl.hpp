// coding: utf-8
#ifndef TASK_ROTATE_HPP
#	error	"Don't include this file directly, use 'task_rotate.hpp' instead!"
#endif

task::Rotate::Rotate()
:	Protothread(), i2cTimeout(200), success(true)
{
	this->stop();
}

void
task::Rotate::start()
{
	XPCC_LOG_DEBUG << "Rotate::start()" << xpcc::endl;
	this->restart();
}

bool
task::Rotate::isFinished()
{
	return (!this->isRunning());
}

bool
task::Rotate::isSuccessful()
{
	return success;
}

void
task::Rotate::stopMotors()
{
	this->stop();
	Leds::write(0b1100);
	XPCC_LOG_DEBUG << "Rotate::stopMotors()" << xpcc::endl;
}

void
task::Rotate::releaseMotors()
{
	this->stop();
	Y_Enable::set();
	Z_Enable::set();
	Leds::write(0);
	XPCC_LOG_DEBUG << "Rotate::releaseMotors()" << xpcc::endl;
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
	XPCC_LOG_DEBUG << "PT_BEGIN()" << xpcc::endl;

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(compass.sendPing());" << xpcc::endl;
	success = false;
	i2cTimeout.restart(500);
	PT_WAIT_UNTIL((success = compass.sendPing()) || i2cTimeout.isExpired());

	if (!success) {
		XPCC_LOG_ERROR << "compass.sendPing() timed out! " << (int)Twi::getErrorState() << xpcc::endl;
		XPCC_LOG_DEBUG << "PT_EXIT()" << xpcc::endl;
		PT_EXIT();
	}

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(compass.checkCommunication());" << xpcc::endl;
	success = false;
	i2cTimeout.restart(500);
	PT_WAIT_UNTIL((success = compass.checkCommunication()) || i2cTimeout.isExpired());

	if (!success) {
		XPCC_LOG_ERROR << "compass.checkCommunication() failed! " << (int)Twi::getErrorState() << xpcc::endl;
		XPCC_LOG_DEBUG << "PT_EXIT()" << xpcc::endl;
		PT_EXIT();
	}

	Y_Enable::reset();
	Z_Enable::reset();

	Leds::write(0b1000);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(compass.enterUserCalibrationMode());" << xpcc::endl;
	success = false;
	i2cTimeout.restart(500);
	PT_WAIT_UNTIL((success = compass.enterUserCalibrationMode()) || i2cTimeout.isExpired());

	if (!success) {
		XPCC_LOG_ERROR << "compass.enterUserCalibrationMode() timed out! " << (int)Twi::getErrorState() << xpcc::endl;
		XPCC_LOG_DEBUG << "PT_EXIT()" << xpcc::endl;
		PT_EXIT();
	}

	yMotor.rotateBy(-360, 7500);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(!yMotor.isRunning());" << xpcc::endl;
	PT_WAIT_UNTIL(!yMotor.isRunning());

	Leds::write(0b1100);
	zMotor.rotateBy(360, 7500);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(!zMotor.isRunning());" << xpcc::endl;
	PT_WAIT_UNTIL(!zMotor.isRunning());

	Leds::write(0b0010);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(compass.exitUserCalibrationMode());" << xpcc::endl;
	success = false;
	i2cTimeout.restart(1000);
	PT_WAIT_UNTIL((success = compass.exitUserCalibrationMode()) || i2cTimeout.isExpired());

	if (!success) {
		XPCC_LOG_ERROR << "compass.exitUserCalibrationMode() timed out! " << (int)Twi::getErrorState() << xpcc::endl;
	}

	zMotor.rotateBy(-360, 2000);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(!zMotor.isRunning());" << xpcc::endl;
	PT_WAIT_UNTIL(!zMotor.isRunning());

	Leds::write(0b0011);
	yMotor.rotateBy(360, 2000);

	XPCC_LOG_DEBUG << "PT_WAIT_UNTIL(!yMotor.isRunning());" << xpcc::endl;
	PT_WAIT_UNTIL(!yMotor.isRunning());

	Leds::write(0b1111);

	Y_Enable::set();
	Z_Enable::set();

	XPCC_LOG_DEBUG << "PT_END();" << xpcc::endl;
	// return is included in PT_END();
	PT_END();
}
