// coding: utf-8
#ifndef TASK_HEADPHONE_HPP
#	error	"Don't include this file directly, use 'task_headphone.hpp' instead!"
#endif

task::Headphone::Headphone(Leds &leds)
:	leds(leds), compass(compassData), pingTimer(200),
	devicePingable(false),
	deviceId(-1),
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
			if ( (devicePingable = PT_CALL(device.ping(this))) )
			{
				if (deviceId == static_cast<uint16_t>(-1))
				{
					if ( PT_CALL(device.readDeviceId(this, deviceId)) )
					{
						leds.resetHeadphoneError();
						XPCC_LOG_INFO << "DeviceId= " << deviceId << xpcc::endl;
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

		PT_YIELD();
	}

	PT_END();
}
