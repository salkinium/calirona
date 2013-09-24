#ifndef TASK_ROTATE_HPP
#define TASK_ROTATE_HPP

#include <xpcc/processing/protothread.hpp>
#include "motor.hpp"
#include "hardware.hpp"

namespace task
{

class Rotate : private xpcc::pt::Protothread
{
public:
	Rotate();

	void
	start();

	bool
	isFinished();

	bool
	isSuccessful();

	void
	stopMotors();

	void
	releaseMotors();

	void
	update();

private:
	bool
	run();

	xpcc::Timeout<> i2cTimeout;
	bool success;
};

} // namespace task

#include "task_rotate_impl.hpp"

#endif // TASK_ROTATE_HPP
