// coding: utf-8
/* Copyright (c) 2014, Niklas Hauser
* All Rights Reserved.
*/
// ----------------------------------------------------------------------------

#ifndef CALIRONA_HARDWARE
#define CALIRONA_HARDWARE

#include <xpcc/architecture/platform.hpp>

using namespace xpcc::atmega;

// IO #########################################################################
// ATMEL ATMEGA328P
//                     +-v-+
//      (!RESET) PC6  1|   |28  PC5 (SCL)
//         (RXD) PD0  2|   |27  PC4 (SDA)
//         (TXD) PD1  3|   |26  PC3 (!HEADPHONE)
//    (X_LIMIT1) PD2  4|   |25  PC2 (!MECH_ERROR)
//    (X_LIMIT2) PD3  5|   |24  PC1 (!BUSY2)
//    (Z_LIMIT1) PD4  6|   |23  PC0 (!BUSY1)
//               VCC  7|   |22  GND
//               GND  8|   |21  AREF
//       (XTAL1) PB6  9|   |20  AVCC
//       (XTAL2) PB7 10|   |19  PB5 (SCK)
//    (Z_LIMIT2) PD5 11|   |18  PB4 (!START/MISO)
// (Z_STEP/OC0A) PD6 12|   |17  PB3 (!STOP/MOSI)
//     (!ENABLE) PD7 13|   |16  PB2 (Z_DIR)
//       (X_DIR) PB0 14|   |15  PB1 (X_STEP/OC1A)
//                     +---+

typedef xpcc::avr::SystemClock systemClock;

// motor step counters
uint16_t t0_steps;
uint16_t t1_steps;

// Motors
//typedef xpcc::GpioInverted< GpioOpenDrainWithPullUp< GpioD7 > > XZ_Enable;
typedef xpcc::GpioInverted< GpioD7 > XZ_Enable;

// X-Axis
typedef GpioInputD2 X_Limit1;
typedef GpioInputD3 X_Limit2;
typedef GpioOutputB0 X_Dir;
typedef GpioOutputB1 X_Step;

// Z-Axis
typedef GpioInputD4 Z_Limit1;
typedef GpioInputD5 Z_Limit2;
typedef GpioOutputB2 Z_Dir;
typedef GpioOutputD6 Z_Step;

// LEDs
typedef xpcc::GpioInverted< GpioOutputC0 > LED_Busy1;
typedef xpcc::GpioInverted< GpioOutputC1 > LED_Busy2;
typedef xpcc::GpioInverted< GpioOutputC2 > LED_Headphone;
typedef xpcc::GpioInverted< GpioOutputC3 > LED_MechError;

// Buttons
typedef GpioInputB4 ButtonStart;
typedef GpioInputB3 ButtonStop;

// COMMUNICATION ##############################################################
typedef GpioC4 Sda;
typedef GpioC5 Scl;
typedef I2cMaster Twi;

// SERIAL DEBUG ###############################################################
#include <xpcc/io/iodevice_wrapper.hpp>
typedef Uart0 Uart;
xpcc::IODeviceWrapper<Uart> outputDevice;

#include <xpcc/debug/logger/style_wrapper.hpp>
#include <xpcc/debug/logger/style/prefix.hpp>
#include <xpcc/debug/logger.hpp>
xpcc::log::StyleWrapper< xpcc::log::Prefix< char[10] > > loggerDeviceDebug (
		xpcc::log::Prefix< char[10] > ("Debug:   ", outputDevice ) );
xpcc::log::Logger xpcc::log::debug( loggerDeviceDebug );

xpcc::log::StyleWrapper< xpcc::log::Prefix< char[10] > > loggerDeviceInfo (
		xpcc::log::Prefix< char[10] > ("Info:    ", outputDevice ) );
xpcc::log::Logger xpcc::log::info( loggerDeviceInfo );

xpcc::log::StyleWrapper< xpcc::log::Prefix< char[10] > > loggerDeviceWarning (
		xpcc::log::Prefix< char[10] > ("Warning: ", outputDevice ) );
xpcc::log::Logger xpcc::log::warning( loggerDeviceWarning );

xpcc::log::StyleWrapper< xpcc::log::Prefix< char[10] > > loggerDeviceError (
		xpcc::log::Prefix< char[10] > ("Error    ", outputDevice ) );
xpcc::log::Logger xpcc::log::error( loggerDeviceError );

#undef	XPCC_LOG_LEVEL
#define	XPCC_LOG_LEVEL xpcc::log::DEBUG

// TASKS ######################################################################
#include "tasks/task_buttons.hpp"
task::Buttons buttons;
#include "tasks/task_leds.hpp"
task::Leds leds;

#include "tasks/task_headphone.hpp"
task::Headphone headphone(leds);

#include "tasks/task_mechanics.hpp"
task::Mechanics mechanics(leds, buttons);

#include "tasks/task_manager.hpp"
task::Manager manager(buttons, headphone, mechanics);



#endif // CALIRONA_HARDWARE
