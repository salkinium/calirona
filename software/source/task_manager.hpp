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
	Manager();

	void
	update();

private:
	bool
	run();

	// 1s timeout
	xpcc::PeriodicTimer<> timer;
	int16_t motorTimeout;
};

} // namespace task

#include "task_manager_impl.hpp"

#endif // TASK_MANAGER_HPP
