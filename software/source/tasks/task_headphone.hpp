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

class Headphone : private xpcc::pt::Protothread, private xpcc::co::NestedCoroutine<>
{
public:
	Headphone(Leds &leds);

	/// @return `true` if device response to pings
	bool ALWAYS_INLINE
	isAvailable()
	{ return compassPingable; }

	/// @return device id or `0xffff` if device id is not valid (no device, or still reading)
	uint16_t ALWAYS_INLINE
	getDeviceId()
	{ return compassId; }

	xpcc::co::Result<bool>
	enterCalibrationMode();

	xpcc::co::Result<bool>
	exitCalibrationMode();

	void
	update();

private:
	bool
	run();

private:
	Leds &leds;

	xpcc::hmc6343::Data compassData;
	xpcc::Hmc6343<Twi> compass;

	xpcc::PeriodicTimer<> pingTimer;
	bool compassPingable;

	uint16_t compassId;
	uint16_t xOffset;
	uint16_t yOffset;
	uint16_t zOffset;
	uint8_t filter;
};

} // namespace task

#include "task_headphone_impl.hpp"

#endif // TASK_HEADPHONE_HPP
