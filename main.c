//  main.c for STM32F103-CMSIS-I2C-LCD-lib
//      Version 1.1   7 Aug 2023    Updated I2C and delay libraries. Udpdated I2C-LCD library.
//      Version 1.0   07/17/2023    Confirmed Working with New Core
//      Started       May 2023, Mike Shegedin
//
//  Target Microcontroller: STM32F103 (Blue Pill)
//  Target I2C device: I2C LCD Module driving a 2x16 LCD display
//
//  Program to demostrate LCD output routines. Displays the following:
//      I2C-LCD 123 xxx
//        Hello World!
//  where xxx is a group of 3 characters that rapidly changes at increassing speeds.
//
//  HARDWARE SETUP
//  ==============
//
//  While a 5V 16x2 LCD module can be driven mostly with 3.3 logic levels,
//  and while the I2C display driver module can operate in a 3.3V system,
//  the combination probably requires that, if using a 3.3V microcontroller,
//  the I2C LCD driver module itself must be powered by 5 V in order to
//  properly drive a 5V LCD module.
//
//  The 16 pins of the I2C LCD driver module are connected to the corresponding
//  pins on the 16x2 LCD module. The I2C data lines (SCA and SCL) lines can be
//  directly connected to the I2C lines on the microcontroller. Pullup resistors
//  are built into the I2C LCD driver module, so additional pullup resistors on
//  the I2C lines are not needed.
//
//  If the display seems too bright with the backlight jumper installed, then the
//  jumper can be replaced with a resistor of hundreds of ohms or higher to dim
//  the display. Technically the backlight on the LCD could be driven via PWM, but
//  as it is being controlled via I2C on the I2C LCD driver module, this is not
//  practical to implement.
//  
//
//             I2C LCD Driver Module
//     ======================================
//     1 2 3 4 5 6 7 8 9 10 11 12 13 14 15 16
//     ======================================
//                16x2 LCD Module
//
//
//              Blue Pill  I2C LCD Driver Module
//              =========  =====================
//                 GND ----------- GND
//                  5V ----------- VDD
//           B6 or B10 ----------- SCL
//           B7 or B11 ----------- SDA
//          ====  ====
//          I2C1  I2C2
//  
//                                LED Jumper -- [1k ohm] --,
//                                                         |
//                                LED Jumper --------------`
//
//

#include "stm32f103xb.h"                  // Primary CMSIS header file

I2C_TypeDef *LCD_I2C;                     // Needed for below I2C LCD driver library
#include "STM32F103-CMSIS-I2C-LCD-lib.c"  // I2C LCD driver library


// ============================================================================
// main
// ============================================================================
int
main()
{
  char myString[] = "Hello World!";   // Define "string"

  I2C_LCD_init( I2C2, 100e3);         // Set the LCD interface to I2C2 at 100 kHz
  I2C_LCD_cmd( LCD_4B_58F_2L );       // Set LCD to 4-bit, 5x8 character set, 2 lines
  I2C_LCD_cmd( LCD_CLEAR );           // Clear LCD
  I2C_LCD_cmd( LCD_HOME );            // Move LCD to home position
  I2C_LCD_cmd( LCD_ON_NO_CURSOR );    // Turn on LCD

  I2C_LCD_puts( myString );           // Display string
  I2C_LCD_cmd( LCD_1ST_LINE + 13 );   // Adjust display position
  I2C_LCD_putc( '1' );                // Display individual characters
  I2C_LCD_putc( '2' );
  I2C_LCD_putc( '3' );

  I2C_LCD_cmd( LCD_2ND_LINE + 1 );    // Move to 2nd postion on 2nd line
  I2C_LCD_puts( "I2C-LCD-lib.c" );    // Display string constant
  I2C_LCD_cmd( LCD_8B_58F_2L );       // Return to 8-bit mode
  return 1;
}  
