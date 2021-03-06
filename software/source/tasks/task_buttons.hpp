// coding: utf-8
#ifndef TASK_BUTTONS_HPP
#define TASK_BUTTONS_HPP

#include <xpcc/processing/periodic_timer.hpp>
#include <xpcc/ui/button_group.hpp>
#include "hardware.hpp"

namespace task
{

class Buttons
{
public:
	Buttons()
	:	timerInput(10), buttons(BUTTON_START | BUTTON_STOP)
	{
	}

	void inline
	initialize()
	{
		ButtonStart::setInput(Gpio::InputType::PullUp);
		ButtonStop::setInput(Gpio::InputType::PullUp);
		X_Limit1::setInput(Gpio::InputType::PullUp);
		Z_Limit1::setInput(Gpio::InputType::PullUp);
	}

	void
	update()
	{
		if (timerInput.isExpired())
		{
			uint8_t mask = 0;
			if (!ButtonStart::read()) mask |= BUTTON_START;
			if (!ButtonStop::read())  mask |= BUTTON_STOP;
			buttons.update(mask);
		}
	}

	bool ALWAYS_INLINE
	isX_AxisLimitPressed()
	{
		return (X_Limit1::read());
	}

	bool ALWAYS_INLINE
	isZ_AxisLimitPressed()
	{
		return (Z_Limit1::read());
	}

	bool ALWAYS_INLINE
	isStartPressed()
	{
		return (buttons.isPressedShort(BUTTON_START) ||
				buttons.isPressedLong(BUTTON_START));
	}

	bool ALWAYS_INLINE
	isStopPressedShort()
	{
		return buttons.isPressedShort(BUTTON_STOP);
	}

	bool ALWAYS_INLINE
	isStopPressedLong()
	{
		return buttons.isPressedLong(BUTTON_STOP);
	}

private:
	xpcc::PeriodicTimer<> timerInput;

	enum
	{
		BUTTON_START = (1 << 0),
		BUTTON_STOP = (1 << 1),
	};
	xpcc::ButtonGroup<> buttons;
	xpcc::Timeout<> timeout;
};

} // namespace task

#endif // TASK_LEDS_HPP
