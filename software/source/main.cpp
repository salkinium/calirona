// coding: utf-8
#include <xpcc/architecture/platform.hpp>
#include <xpcc/architecture/driver.hpp>
#include <xpcc/processing.hpp>
#include "hardware.hpp"

using namespace xpcc::atmega;

#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

// INTERRUPTS #################################################################
ISR(TIMER0_COMPA_vect)
{
	xpcc::Clock::increment();

	static uint8_t count(10);
	if (!count--)
	{
		count = 10;
		uint8_t mask = (StartButton::read() ? BUTTON_START : 0);
		mask |= (StopButton::read() ? BUTTON_STOP : 0);
		buttons.update(mask);
	}
}

int
main(void)
{
	// Initiate 1kHz interrupt for clock using timer0
	// Clear Timer on Compare Match (CTC) Mode
	TCCR0A = (1 << WGM01);
	// 1kHz (= 20000kHz / 256 / 78)
	OCR0A = 78;
	// Prescaler 256, enable Timer0
	TCCR0B = (1 << CS02);
	// Enable Overflow Interrupt
	TIMSK0 = (1 << OCIE0A);

	Leds::setOutput();
	Leds::write(0);

	StartButton::setInput(Gpio::Configuration::PullUp);
	StopButton::setInput(Gpio::Configuration::PullUp);

	Y_Dir::setOutput(xpcc::Gpio::LOW);
	Y_Step::setOutput(xpcc::Gpio::LOW);
	Y_Enable::set();

	Z_Dir::setOutput(xpcc::Gpio::LOW);
	Z_Step::setOutput(xpcc::Gpio::LOW);
	Z_Enable::set();

	Scl::connect(Twi::Scl);
	Sda::connect(Twi::Sda);
	Scl::configure(Gpio::Configuration::PullUp);
	Sda::configure(Gpio::Configuration::PullUp);
	Twi::initialize<Twi::DataRate::Standard>();

	GpioD0::connect(Uart::Rx);
	GpioD1::connect(Uart::Tx);
	Uart::initialize<19200>();

	xpcc::atmega::enableInterrupts();

	XPCC_LOG_INFO << "\nRESTART\n" << xpcc::endl;

	while (1)
	{
		manager.update();
	}

	return 0;
}
