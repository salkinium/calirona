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
				XPCC_LOG_INFO << "Calibrating X- and Z-axis" << xpcc::endl;
				if (!PT_CALL(mechanics.calibrateX(this)))
				{
					XPCC_LOG_ERROR << "Unable to calibrate X-axis" << xpcc::endl;
				}
				else if (!PT_CALL(mechanics.calibrateZ(this)))
				{
					XPCC_LOG_ERROR << "Unable to calibrate Z-axis" << xpcc::endl;
				}
			}
			else {
				XPCC_LOG_INFO << "Rotating forward..." << xpcc::endl;
				if(!PT_CALL(mechanics.rotateForward(this)))
				{
					XPCC_LOG_ERROR << "Unable to rotate forward" << xpcc::endl;
				}
				else {
					XPCC_LOG_INFO << "Rotating backward..." << xpcc::endl;
					if(!PT_CALL(mechanics.rotateBackward(this)))
					{
						XPCC_LOG_ERROR << "Unable to rotate backward" << xpcc::endl;
					}
				}
			}
		}

		PT_YIELD();
	}

	// return is included in PT_END();
	PT_END();
}
