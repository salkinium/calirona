// coding: utf-8
#include <xpcc/architecture/platform.hpp>
#include <xpcc/architecture/driver.hpp>
#include <xpcc/processing.hpp>

using namespace xpcc::atmega;

// IO #########################################################################
// ATMEL ATMEGA644
//                   +--v--+
//             PB0  1|     |40  PA0 (LED7)
//             PB1  2|     |39  PA1
//             PB2  3|     |38  PA2 (LED5)
//             PB3  4|     |37  PA3
//             PB4  5|     |36  PA4
//    (P-MOSI) PB5  6|     |35  PA5 (LED2)
//    (P-MISO) PB6  7|     |34  PA6
//     (P-SCK) PB8  8|     |33  PA7 (LED0)
//          !RESET  9|     |32  AREF
//             VCC 10|     |31  GND
//             GND 11|     |30  AVCC
//           XTAL2 12|     |29  PC7
//           XTAL1 13|     |28  PC6
// (RXD,Y-DIR) PD0 14|     |27  PC5
//(TXD,Y-STEP) PD1 15|     |26  PC4
//  (Y-ENABLE) PD2 16|     |25  PC3
//             PD3 17|     |24  PC2
//     (X-DIR) PD4 18|     |23  PC1 (SDA)
//    (X-STEP) PD5 19|     |22  PC0 (SCL)
//  (X-ENABLE) PD6 20|     |21  PD7
//                   +-----+

// Stepper motor driver
typedef GpioOutputD0 Y_Dir;
typedef GpioOutputD1 Y_Step;
typedef GpioOutputD2 Y_Enable;

typedef GpioOutputD4 X_Dir;
typedef GpioOutputD5 X_Step;
typedef GpioOutputD6 X_Enable;

// I2C compass driver
#include <xpcc/driver/inertial/hmc6343.hpp>
typedef I2cMaster Twi;
uint8_t hmcData[20];
xpcc::Hmc6343<Twi> compass(hmcData);

// Serial debug
#include <xpcc/io/iodevice_wrapper.hpp>
typedef Uart0 Uart;
xpcc::IODeviceWrapper<Uart> logger;

#include <xpcc/debug/logger.hpp>
xpcc::log::Logger xpcc::log::debug(logger);
xpcc::log::Logger xpcc::log::info(logger);
xpcc::log::Logger xpcc::log::warning(logger);
xpcc::log::Logger xpcc::log::error(logger);

#undef	XPCC_LOG_LEVEL
//#define	XPCC_LOG_LEVEL xpcc::log::DEBUG
#define	XPCC_LOG_LEVEL xpcc::log::DISABLED


// INTERRUPTS #################################################################
ISR(TIMER0_COMPA_vect)
{
	xpcc::Clock::increment();
}


MAIN_FUNCTION // ##############################################################
{
	// Initiate 10kHz interrupt for clock using timer0
	// Clear Timer on Compare Match (CTC) Mode
	TCCR0A = (1 << WGM01);
	// 1kHz (= 20000kHz / 64 / 250)
	OCR0A = 156;
	// Prescaler 8, enable Timer0
	TCCR0B = (1 << CS01) | (1 << CS00);
	// Enable Overflow Interrupt
	TIMSK0 = (1 << OCIE0A);

	GpioC0::connect(Twi::Scl);
	GpioC1::connect(Twi::Sda);
	Twi::initialize<>();

	GpioD0::connect(Uart::Rx);
	GpioD1::connect(Uart::Tx);
	Uart::initialize<38400>();

	XPCC_LOG_INFO << "\n\nRESTART\n\n";
	xpcc::atmega::enableInterrupts();

	while (1)
	{

	}

	return 0;
}
