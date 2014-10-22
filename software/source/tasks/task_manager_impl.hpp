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
	if (buttons.isStopPressedLong())
	{
		releaseMotors();
		XPCC_LOG_INFO << "RELEASE" << xpcc::endl;
	}
	else if (buttons.isStopPressedShort())
	{
		stopMotors();
		XPCC_LOG_INFO << "STOP" << xpcc::endl;
	}

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
				XPCC_LOG_INFO << "Calibrating X- and Z-axis...";
				if (!PT_CALL(mechanics.calibrateX(this))) {
					XPCC_LOG_ERROR << "Unable to calibrate X-axis!" << xpcc::endl;
				}
				else if (!PT_CALL(mechanics.calibrateZ(this))) {
					XPCC_LOG_ERROR << "Unable to calibrate Z-axis!" << xpcc::endl;
				}
				else XPCC_LOG_INFO << " done." << xpcc::endl;
			}
			else if (headphone.isAvailable())
			{
				XPCC_LOG_INFO << "Entering user calibration mode...";
				if ( !PT_CALL(headphone.enterCalibrationMode()) ) {
					XPCC_LOG_ERROR << "Unable to enter user calibration mode!" << xpcc::endl;
				}
				else
				{
					XPCC_LOG_INFO << " done." << xpcc::endl << "Rotating forward...";
					if (!PT_CALL(mechanics.rotateForward(this))) {
						XPCC_LOG_ERROR << "Unable to rotate forward!" << xpcc::endl;
					}
					else
					{
						XPCC_LOG_INFO << " done." << xpcc::endl << "Exiting user calibration mode...";
						if ( !PT_CALL(headphone.exitCalibrationMode()) ) {
							XPCC_LOG_ERROR << "Unable to exit user calibration mode!" << xpcc::endl;
						}
						else
						{
							XPCC_LOG_INFO << " done." << xpcc::endl << "Rotating backward...";
							if ( !PT_CALL(mechanics.rotateBackward(this)) ) {
								XPCC_LOG_ERROR << "Unable to rotate backward!" << xpcc::endl;
							}
							else  XPCC_LOG_INFO << " done." << xpcc::endl;
						}




					}
				}
			}
			else {
				XPCC_LOG_ERROR << "Headphones unavailable!" << xpcc::endl;
			}
		}

		PT_YIELD();
	}

	// return is included in PT_END();
	PT_END();
}
