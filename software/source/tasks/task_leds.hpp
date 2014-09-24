// coding: utf-8
#ifndef TASK_LEDS_HPP
#define TASK_LEDS_HPP

#include <xpcc/processing/periodic_timer.hpp>
#include "hardware.hpp"

namespace task
{

class Leds
{
public:
	Leds()
	:	isBusy(false), isHeadphoneError(false), isMechanicalError(false)
	{
	}

	void inline
	initialize()
	{
		LED_Busy1::setOutput(xpcc::Gpio::Low);
		LED_Busy2::setOutput(xpcc::Gpio::Low);
		LED_Headphone::setOutput(xpcc::Gpio::Low);
		LED_MechError::setOutput(xpcc::Gpio::Low);
	}

	void
	update()
	{
		if (isBusy && timerBusy.isExpired())
		{
			LED_Busy1::toggle();
			LED_Busy2::toggle();
		}

		if (isHeadphoneError && timerHeadphone.isExpired())
		{
			LED_Headphone::toggle();
		}

		if (isMechanicalError && timerMechanicalError.isExpired())
		{
			LED_MechError::toggle();
		}
	}

	void inline
	setBusy()
	{
		if (isBusy) return;

		LED_Busy1::set();
		LED_Busy2::reset();
		isBusy = true;
		timerBusy.restart(400);
	}

	void inline
	resetBusy()
	{
		LED_Busy1::reset();
		LED_Busy2::reset();
		isBusy = false;
	}

	void inline
	setHeadphoneError()
	{
		if (isHeadphoneError) return;
		isHeadphoneError = true;
		timerHeadphone.restart(300);
	}

	void inline
	resetHeadphoneError()
	{
		LED_Headphone::reset();
		isHeadphoneError = false;
	}

	void inline
	setMechanicalError()
	{
		if (isMechanicalError) return;
		isMechanicalError = true;
		timerMechanicalError.restart(300);
	}

	void inline
	resetMechanicalError()
	{
		LED_MechError::reset();
		isMechanicalError = false;
	}

private:
	xpcc::PeriodicTimer<> timerBusy;
	xpcc::PeriodicTimer<> timerHeadphone;
	xpcc::PeriodicTimer<> timerMechanicalError;

	bool isBusy;
	bool isHeadphoneError;
	bool isMechanicalError;
};

} // namespace task

#endif // TASK_LEDS_HPP
