// coding: utf-8
#ifndef TASK_MANAGER_HPP
#	error	"Don't include this file directly, use 'task_manager.hpp' instead!"
#endif

task::Manager::Manager(Buttons &buttons, Headphone &headphone, Mechanics &mechanics)
:	buttons(buttons), headphone(headphone), mechanics(mechanics)
{
}

void
task::Manager::initialize()
{

}

void
task::Manager::update()
{
	this->run();
}

bool
task::Manager::run()
{
	PT_BEGIN();

	while(true)
	{
		if (buttons.isStartPressed())
		{
			if (!mechanics.isCalibrated())
			{
				if (PT_CALL(mechanics.calibrateX(this)) &&
					PT_CALL(mechanics.calibrateZ(this)))
				{
					PT_CALL(mechanics.rotateForward(this));
				}
			}
		}

		PT_YIELD();
	}

	// return is included in PT_END();
	PT_END();
}
