// coding: utf-8
#ifndef HARDWARE_HPP
#define HARDWARE_HPP

#include <xpcc/architecture/platform.hpp>
using namespace xpcc::atmega;

// IO #########################################################################
// ATMEL ATMEGA644
//                   +--v--+
//             PB0  1|     |40  PA0 (LED7)
//             PB1  2|     |39  PA1
//             PB2  3|     |38  PA2 (LED5)
//             PB3  4|     |37  PA3 (START)
//             PB4  5|     |36  PA4 (STOP)
//    (P-MOSI) PB5  6|     |35  PA5 (LED2)
//    (P-MISO) PB6  7|     |34  PA6
//     (P-SCK) PB8  8|     |33  PA7 (LED0)
//          !RESET  9|     |32  AREF
//             VCC 10|     |31  GND
//             GND 11|     |30  AVCC
//           XTAL2 12|     |29  PC7
//           XTAL1 13|     |28  PC6
//       (RXD) PD0 14|     |27  PC5
//       (TXD) PD1 15|     |26  PC4
//  (Z-ENABLE) PD2 16|     |25  PC3 (Z-DIR)
//             PD3 17|     |24  PC2 (Z-STEP)
//     (Y-DIR) PD4 18|     |23  PC1 (SDA)
//    (Y-STEP) PD5 19|     |22  PC0 (SCL)
//  (Y-ENABLE) PD6 20|     |21  PD7
//                   +-----+

// LEDs
typedef xpcc::GpioInverted< GpioOutputA0 > Led7;
typedef xpcc::GpioInverted< GpioOutputA2 > Led5;
typedef xpcc::GpioInverted< GpioOutputA5 > Led2;
typedef xpcc::GpioInverted< GpioOutputA7 > Led0;

typedef xpcc::SoftwareGpioPort< Led7, Led5, Led2, Led0 > Leds;

// Buttons
#include <xpcc/ui/button_group.hpp>
typedef GpioInputA3 StartButton;
typedef GpioInputA4 StopButton;
enum
{
	BUTTON_START = 0x01,
	BUTTON_STOP = 0x02,
};
xpcc::ButtonGroup<> buttons(BUTTON_START | BUTTON_STOP);

// Stepper motor driver
typedef GpioOutputC3 Y_Dir;
typedef GpioOutputC2 Y_Step;
typedef GpioOpenDrain< GpioD2 > Y_Enable;

typedef GpioOutputD4 Z_Dir;
typedef GpioOutputD5 Z_Step;
typedef GpioOpenDrain< GpioD6 > Z_Enable;

#include "motor.hpp"
xpcc::A4988< Y_Dir, Y_Step, 400*4 > yMotor;
xpcc::A4988< Z_Dir, Z_Step, 400*4 > zMotor;

// I2C compass driver
#include <xpcc/driver/inertial/hmc6343.hpp>
//*
typedef I2cMaster Twi;
typedef GpioC0 Scl;
typedef GpioC1 Sda;
/*/
typedef GpioOpenDrain< GpioC0 > Scl;
typedef GpioOpenDrain< GpioC1 > Sda;
typedef xpcc::SoftwareI2cMaster<Scl, Sda> Twi;//*/
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
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

// Tasks
#include "task_rotate.hpp"
task::Rotate rotate;

#include "task_manager.hpp"
task::Manager manager;

#endif // HARDWARE_HPP
