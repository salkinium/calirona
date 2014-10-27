// coding: utf-8
#ifndef TASK_HEADPHONE_HPP
#	error	"Don't include this file directly, use 'task_headphone.hpp' instead!"
#endif

task::Headphone::Headphone(Leds &leds)
:	leds(leds), compass(compassData),
	pingTimer(200), compassPingable(false),
	compassId(-1)
{
}

xpcc::co::Result<bool>
task::Headphone::enterCalibrationMode()
{
	CO_BEGIN(this);

	CO_CALL(compass.readRegister(this, compass.Register::FilterLsb, filter));
	XPCC_LOG_DEBUG << "\nPrevious IIR Filter= " << filter << xpcc::endl;
	CO_CALL(compass.setIIR_Filter(this, 5));

	CO_CALL(compass.readRegister(this, compass.Register16::X_Offset, xOffset));
	CO_CALL(compass.readRegister(this, compass.Register16::Y_Offset, yOffset));
	CO_CALL(compass.readRegister(this, compass.Register16::Z_Offset, zOffset));

	XPCC_LOG_DEBUG << "Offsets before: x=" << xOffset << " y=" << yOffset << " z=" << zOffset << xpcc::endl;

	if ( CO_CALL(compass.enterUserCalibrationMode(this)) )
		CO_RETURN(true);

	leds.setHeadphoneError();
	CO_END_RETURN(false);
}

xpcc::co::Result<bool>
task::Headphone::exitCalibrationMode()
{
	CO_BEGIN(this);

	if ( CO_CALL(compass.exitUserCalibrationMode(this)) )
	{
		xOffset = yOffset = zOffset = -1;
		CO_CALL(compass.readRegister(this, compass.Register16::X_Offset, xOffset));
		CO_CALL(compass.readRegister(this, compass.Register16::Y_Offset, yOffset));
		CO_CALL(compass.readRegister(this, compass.Register16::Z_Offset, zOffset));

		XPCC_LOG_DEBUG << "Offsets after: x=" << xOffset << " y=" << yOffset << " z=" << zOffset << xpcc::endl;

		CO_RETURN(true);
	}

	leds.setHeadphoneError();
	CO_END_RETURN(false);
}

void
task::Headphone::update()
{
	this->run();
}

bool
task::Headphone::run()
{
	PT_BEGIN();

	while(true)
	{
		if (pingTimer.isExpired())
		{
			if ( (compassPingable = PT_CALL(compass.ping(this))) )
			{
				if (compassId == 0xFFFF)
				{
					if ( PT_CALL(compass.readDeviceId(this, compassId)) )
					{
						leds.resetHeadphoneError();
						XPCC_LOG_INFO << "DeviceId= " << compassId << xpcc::endl;
					}
					else {
						compassId = 0xFFFF;
						leds.setHeadphoneError();
					}
				}
			}
			else {
				compassId = 0xFFFF;
				leds.setHeadphoneError();
			}
		}

		PT_YIELD();
	}

	PT_END();
}
