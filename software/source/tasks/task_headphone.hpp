// coding: utf-8
#ifndef TASK_HEADPHONE_HPP
#define TASK_HEADPHONE_HPP

#include <xpcc/processing/protothread.hpp>
#include <xpcc/processing/periodic_timer.hpp>
#include <xpcc/driver/inertial/hmc6343.hpp>
#include "../hardware.hpp"
#include "task_leds.hpp"

namespace task
{

class Headphone : private xpcc::pt::Protothread
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

	void ALWAYS_INLINE
	enterCalibrationMode()
	{ isEnterCalibrationMode = true; }

	bool ALWAYS_INLINE
	isEnterCalibrationModeRunning()
	{ return isEnterCalibrationMode; }

	void ALWAYS_INLINE
	exitCalibrationMode()
	{ isExitCalibrationMode = true; }

	bool ALWAYS_INLINE
	isExitCalibrationModeRunning()
	{ return isExitCalibrationMode; }


private:
	bool
	run();

	Leds &leds;

	uint8_t compassData[21];
	xpcc::Hmc6343<Twi> compass;
	xpcc::PeriodicTimer<> pingTimer;
	bool devicePingable;

	uint16_t deviceId;
	bool isEnterCalibrationMode;
	bool isExitCalibrationMode;

	xpcc::Hmc6343<Twi> device;
	uint8_t deviceData[21];
};

} // namespace task

#include "task_headphone_impl.hpp"

#endif // TASK_HEADPHONE_HPP
