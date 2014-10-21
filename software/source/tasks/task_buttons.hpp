// coding: utf-8
#ifndef TASK_BUTTONS_HPP
#define TASK_BUTTONS_HPP

#include <xpcc/processing/protothread.hpp>
#include <xpcc/processing/periodic_timer.hpp>
#include <xpcc/ui/button_group.hpp>
#include <xpcc/debug/logger.hpp>
#include "hardware.hpp"

#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::INFO

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
			uint8_t mask = (ButtonStart::read() ? BUTTON_START : 0);
			mask |= (ButtonStop::read() ? BUTTON_STOP : 0);
			buttons.update(mask);
		}
	}

	bool ALWAYS_INLINE
	isX_AxisLimitPressed()
	{
		return (!X_Limit1::read());
		{
			XPCC_LOG_DEBUG << "X" << xpcc::endl;
			return true;
		}
		return false;
	}

	bool ALWAYS_INLINE
	isZ_AxisLimitPressed()
	{
		return (!Z_Limit1::read());
		{
			XPCC_LOG_DEBUG << "Z" << xpcc::endl;
			return true;
		}
		return false;
	}

	bool ALWAYS_INLINE
	isStartPressed()
	{
		if (buttons.isPressedShort(BUTTON_START) ||
			buttons.isPressedLong(BUTTON_START))
		{
			XPCC_LOG_DEBUG << "Start Button Pressed" << xpcc::endl;
			return true;
		}
		return false;
	}

	bool ALWAYS_INLINE
	isStopPressedShort()
	{
		if (buttons.isPressedShort(BUTTON_STOP))
		{
			XPCC_LOG_DEBUG << "Stop Button Pressed Short" << xpcc::endl;
			return true;
		}
		return false;
	}

	bool ALWAYS_INLINE
	isStopPressedLong()
	{
		if (buttons.isPressedLong(BUTTON_STOP))
		{
			XPCC_LOG_DEBUG << "Stop Button Pressed Long" << xpcc::endl;
			return true;
		}
		return false;
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
