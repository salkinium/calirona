// coding: utf-8
#ifndef TASK_MANAGER_HPP
#define TASK_MANAGER_HPP

#include <xpcc/processing/protothread.hpp>
#include <xpcc/processing/periodic_timer.hpp>
#include "hardware.hpp"

namespace task
{

class Manager : private xpcc::pt::Protothread
{
public:
	Manager(Buttons &buttons, Headphone &headphone, Mechanics &mechanics);

	void
	initialize();

	void
	update();

	void
	stopMotors()
	{
		Protothread::restart();
		mechanics.stopMotors();
		buttons.isStartPressed();
	}

	void
	releaseMotors()
	{
		Protothread::restart();
		mechanics.releaseMotors();
		buttons.isStartPressed();
	}

private:
	bool
	run();

	Buttons &buttons;
	Headphone &headphone;
	Mechanics &mechanics;

	// 1s timeout
	xpcc::PeriodicTimer<> timer;
	int16_t motorTimeout;
	bool ledsOn;
};

} // namespace task

#include "task_manager_impl.hpp"

#endif // TASK_MANAGER_HPP
