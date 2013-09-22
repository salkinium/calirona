// coding: utf-8
#ifndef TASK_MANAGER_HPP
#	error	"Don't include this file directly, use 'task_manager.hpp' instead!"
#endif

task::Manager::Manager()
:	Protothread(), timer(1000), motorTimeout(60)
{
}

void
task::Manager::update()
{
	rotate.update();
	this->run();
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
		}

		if (buttons.isPressedShort(BUTTON_STOP))
		{
			rotate.stopMotors();
		}

		if (buttons.isPressedLong(BUTTON_STOP) || motorTimeout <= 0)
		{
			rotate.releaseMotors();
		}

		if (timer.isExpired())
		{
			if (motorTimeout > 0)
			{
				motorTimeout -= 1;
			}
		}
	}

	// return is included in PT_END();
	PT_END();
}
