// coding: utf-8
#include "hardware.hpp"
// INTERRUPTS #################################################################
ISR(TIMER2_COMPA_vect)
{
	xpcc::Clock::increment();
}

ISR(TIMER0_COMPA_vect)
{
	t0_steps++;
}

ISR(TIMER1_COMPA_vect)
{
	t1_steps++;
}

MAIN_FUNCTION
{
	// Initiate 1kHz interrupt for clock using Timer2
	// Clear Timer on Compare Match (CTC) Mode
	TCCR2A = (1 << WGM21);
	// 1kHz (= 11059.2kHz / 64 / 173)
	OCR2A = 173;
	// Prescaler 64, enable Timer2
	TCCR2B = (1 << CS22);
	// Enable Overflow Interrupt
	TIMSK2 = (1 << OCIE2A);

	leds.initialize();
	buttons.initialize();
	mechanics.initialize();
	manager.initialize();

	Scl::connect(Twi::Scl);
	Sda::connect(Twi::Sda);
	Twi::initialize<systemClock, Twi::Baudrate::Standard>();

	GpioD0::connect(Uart::Rx);
	GpioD1::connect(Uart::Tx);
	Uart::initialize<systemClock, Uart::B115200>();

	xpcc::atmega::enableInterrupts();

	XPCC_LOG_INFO << "RESTART" << xpcc::endl;

	while (1)
	{
		buttons.update();
		leds.update();
		headphone.update();
		mechanics.update();
		manager.update();

		if (buttons.isStopPressedLong())
		{
			manager.releaseMotors();
			XPCC_LOG_INFO << "RELEASE" << xpcc::endl;
		}
		else if (buttons.isStopPressedShort())
		{
			manager.stopMotors();
			XPCC_LOG_INFO << "STOP" << xpcc::endl;
		}
	}

//	return 0;
}
