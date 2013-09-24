// coding: utf-8
#ifndef TASK_MANAGER_HPP
#	error	"Don't include this file directly, use 'task_manager.hpp' instead!"
#endif

task::Manager::Manager()
:	Protothread(), timer(1000), motorTimeout(0), ledsOn(false)
{
}

void
task::Manager::update()
{
	this->run();
	rotate.update();
	compass.update();
}

bool
task::Manager::run()
{
	PT_BEGIN();

	while(true)
	{
		if ((buttons.isPressedShort(BUTTON_START) ||
			buttons.isPressedLong(BUTTON_START)) &&
			rotate.isFinished())
		{
			rotate.start();
			motorTimeout = 60;
		}

		if (buttons.isPressedShort(BUTTON_STOP))
		{
			rotate.stopMotors();
		}

		if (buttons.isPressedLong(BUTTON_STOP) || motorTimeout == 0)
		{
			motorTimeout = -1;
			rotate.releaseMotors();
		}

		if (timer.isExpired())
		{
			if (!rotate.isSuccessful() && rotate.isFinished())
			{
				Leds::write(ledsOn ? 0b1100 : 0b0000);
				ledsOn = !ledsOn;
			}

			if (motorTimeout >= 0)
			{
				if (motorTimeout % 10 == 0) {
					XPCC_LOG_DEBUG << "timeout = " << motorTimeout << xpcc::endl;
				}
				motorTimeout -= 1;
			}
			else if (motorTimeout == -1) {
				XPCC_LOG_DEBUG << "safety timeout!" << xpcc::endl;
			}
		}

		PT_YIELD();
	}

	// return is included in PT_END();
	PT_END();
}
