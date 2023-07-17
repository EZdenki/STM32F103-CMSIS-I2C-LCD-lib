//  main.c for STM32F103-CMSIS-I2C-LCD-lib
//      Version 1.0   07/17/2023    Confirmed Working with New Core
//
//  Target Microcontroller: STM32F103 (Blue Pill)
//  Mike Shegedin, 05/2023
//
//  Target I2C device: I2C LCD Module driving a 2x16 LCD display
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
#include "STM32F103-Pause-lib.c"          // pause and delay_us library

I2C_TypeDef *LCD_I2C;                     // Needed for below I2C LCD driver library
#include "STM32F103-CMSIS-I2C-LCD-lib.c"  // I2C LCD driver library


// ============================================================================
// main
// ============================================================================
int
main()
{
  char myString[] = "Hello World!";

  I2C_LCD_init( I2C1);                // Set the LCD interface to I2C1 and initalize it
  I2C_LCD_cmd( LCD_4B_58F_2L );
  I2C_LCD_cmd( LCD_CLEAR );
  delay_us( 2e3 );                    // Min. 1.5 ms delay needed after LCD_CLEAR command
  I2C_LCD_cmd( LCD_HOME );
  delay_us( 2e3 );                    // Min. 1.5 ms delay needed after LCD_HOME command
  I2C_LCD_cmd( LCD_ON_BLINK_CURSOR );

  I2C_LCD_puts( "Mike" );
  I2C_LCD_cmd( LCD_1ST_LINE + 8 );
  I2C_LCD_putc( '1' );
  I2C_LCD_putc( '2' );
  I2C_LCD_putc( '3' );
  I2C_LCD_putc( '4' );
  delay_us( 1E6 ); 
  I2C_LCD_cmd( LCD_2ND_LINE + 2 );
  I2C_LCD_puts( myString );

  I2C_LCD_cmd( 0x38 );


  return 1;
}  
