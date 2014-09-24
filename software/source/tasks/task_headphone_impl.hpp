// coding: utf-8
#ifndef TASK_HEADPHONE_HPP
#	error	"Don't include this file directly, use 'task_headphone.hpp' instead!"
#endif

task::Headphone::Headphone(Leds &leds)
:	leds(leds), compass(compassData), pingTimer(200),
	devicePingable(false),
	deviceId(-1),
	isEnterCalibrationMode(false), isExitCalibrationMode(false),
	device(deviceData)
{
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
			devicePingable = PT_CALL(device.ping(this));
			if (devicePingable)
			{
				if (deviceId == static_cast<uint16_t>(-1))
				{
					if (PT_CALL(device.readDeviceId(this, deviceId)))
					{
						leds.resetHeadphoneError();
					}
					else {
						deviceId = -1;
						leds.setHeadphoneError();
					}
				}
			}
			else {
				deviceId = -1;
				leds.setHeadphoneError();
			}
		}

		if (isEnterCalibrationMode)
		{
			isEnterCalibrationMode = !PT_CALL(device.enterUserCalibrationMode(this));
		}

		if (isExitCalibrationMode)
		{
			isExitCalibrationMode = !PT_CALL(device.exitUserCalibrationMode(this));
		}

		PT_YIELD();
	}

	PT_END();
}
