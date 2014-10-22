// coding: utf-8
#ifndef TASK_HEADPHONE_HPP
#define TASK_HEADPHONE_HPP

#include <xpcc/processing.hpp>
#include <xpcc/driver/inertial/hmc6343.hpp>
#include "../hardware.hpp"
#include "task_leds.hpp"
#include <xpcc/debug/logger.hpp>

#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

namespace task
{

class Headphone : private xpcc::pt::Protothread, private xpcc::co::Coroutine
{
public:
	Headphone(Leds &leds);

	void
	update();

	/// @return `true` if device response to pings
	bool ALWAYS_INLINE
	isAvailable()
	{ return devicePingable; }

	/// @return device id or `-1` if device id is not valid (no device, or still reading)
	uint16_t ALWAYS_INLINE
	getDeviceId()
	{ return deviceId; }

	xpcc::co::Result<bool>
	enterCalibrationMode()
	{
		CO_BEGIN(this);

		CO_CALL(device.readRegister(this, device.Register::FilterLsb, filter));
		XPCC_LOG_DEBUG << "\nPrevious IIR Filter= " << filter << xpcc::endl;
		CO_CALL(device.setIIR_Filter(this, 5));

		CO_CALL(device.readRegister(this, device.Register16::X_Offset, xOffset));
		CO_CALL(device.readRegister(this, device.Register16::Y_Offset, yOffset));
		CO_CALL(device.readRegister(this, device.Register16::Z_Offset, zOffset));

		XPCC_LOG_DEBUG << "Offsets before: x=" << xOffset << " y=" << yOffset << " z=" << zOffset << xpcc::endl;

		if ( CO_CALL(device.enterUserCalibrationMode(this)) )
			CO_RETURN(true);

		leds.setHeadphoneError();
		CO_END();
	}

	xpcc::co::Result<bool>
	exitCalibrationMode()
	{
		CO_BEGIN(this);

		if ( CO_CALL(device.exitUserCalibrationMode(this)) )
		{
			CO_CALL(device.readRegister(this, device.Register16::X_Offset, xOffset));
			CO_CALL(device.readRegister(this, device.Register16::Y_Offset, yOffset));
			CO_CALL(device.readRegister(this, device.Register16::Z_Offset, zOffset));

			XPCC_LOG_DEBUG << "Offsets after: x=" << xOffset << " y=" << yOffset << " z=" << zOffset << xpcc::endl;

			CO_RETURN(true);
		}

		leds.setHeadphoneError();
		CO_END();
	}

private:
	bool
	run();

	Leds &leds;

	uint8_t compassData[21];
	xpcc::Hmc6343<Twi> compass;
	xpcc::PeriodicTimer<> pingTimer;
	bool devicePingable;

	uint16_t deviceId;
	uint16_t xOffset;
	uint16_t yOffset;
	uint16_t zOffset;
	uint8_t filter;

	xpcc::Hmc6343<Twi> device;
	uint8_t deviceData[21];
};

} // namespace task

#include "task_headphone_impl.hpp"

#endif // TASK_HEADPHONE_HPP
