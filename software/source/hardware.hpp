
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
//     (Z-DIR) PD4 18|     |23  PC1 (SDA)
//    (Z-STEP) PD5 19|     |22  PC0 (SCL)
//  (Z-ENABLE) PD6 20|     |21  PD7
//                   +-----+

// LEDs
typedef xpcc::GpioInverted< GpioOutputA0 > Led7;
typedef xpcc::GpioInverted< GpioOutputA2 > Led5;
typedef xpcc::GpioInverted< GpioOutputA5 > Led2;
typedef xpcc::GpioInverted< GpioOutputA7 > Led0;

typedef xpcc::SoftwareGpioPort< Led7, Led5, Led2, Led0 > Leds;

// Stepper motor driver
typedef GpioOutputD0 Y_Dir;
typedef GpioOutputD1 Y_Step;
typedef GpioOpenDrain< GpioD2 > Y_Enable;

typedef GpioOutputD4 Z_Dir;
typedef GpioOutputD5 Z_Step;
typedef GpioOpenDrain< GpioD6 > Z_Enable;

#include "motor.hpp"
xpcc::A4988< Y_Dir, Y_Step, 400*4 > yMotor;
xpcc::A4988< Z_Dir, Z_Step, 400*4 > zMotor;

// I2C compass driver
#include <xpcc/driver/inertial/hmc6343.hpp>
typedef I2cMaster Twi;
uint8_t hmcData[20];
xpcc::Hmc6343<Twi> compass(hmcData);

#endif // HARDWARE_HPP
