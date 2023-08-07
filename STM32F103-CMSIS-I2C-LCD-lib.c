// =====================================================================================
//  STM32F103-CMSIS-I2C-LCD-lib.c
//      Version 1.1   7 Aug 2023    Updated I2C-lib, changed pause-lib to Delay-lib,
//                                  removed unnecessary delays, added delay to clear/home
//                                  commands.
//      Version 1.0   07/17/2023    Updated core files and comments
//      Started       May 2023      Mike Shegedin
//
//  Target Microcontroller: STM32F103 (Blue Pill)
//  Target I2C device: I2C 16x2 LCD Driver Module based on the PCF8574.
//
//  Mike Shegedin
//
//
//  HARDWARE SETUP
//  ==============
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
//     Blue Pill  I2C LCD Driver Module
//     =========  =====================
//        GND ----------- GND
//         5V ----------- VDD
//         B6 or B10 ---- SCL
//         B7 or B11 ---- SDA
//        ====  ====
//        I2C1  I2C2
//                        LED Jumper -- [1k ohm] --,
//                                                 |
//                        LED Jumper --------------'
//
//

#ifndef __STM32F103_CMSIS_I2C_LCD_LIB_C
#define __STM32F103_CMSIS_I2C_LCD_LIB_C


#include <stdlib.h>
#include <string.h>
#include "stm32f103xb.h"              // Primary CMSIS header file
#include "STM32F103-Delay-lib.c"      // Delay function
#include "STM32F103-CMSIS-I2C-lib.c"  // I2C library

I2C_TypeDef *LCD_I2C;                 // Global variable to point to the I2C interface used for
                                      // the LCD driver.

#define I2C_LCD_ADD 0x3F              // I2C address of I2C LCD driver module

// Pin-Bit definitions between the LCD module pins and the I2C LCD driver module data-byte bits.
#define I2C_LCD_RS  0b00000001
#define I2C_LCD_RW  0b00000010
#define I2C_LCD_EN  0b00000100
#define I2C_LCD_BL  0b00001000
#define I2C_LCD_D4  0b00010000
#define I2C_LCD_D5  0b00100000
#define I2C_LCD_D6  0b01000000
#define I2C_LCD_D7  0b10000000


//  LCD Command Codes
//  These command codes are used to put the LCD module into various modes.
#define LCD_CLEAR                0x01 // Clear display
#define LCD_HOME                 0x02 // Move to home position
#define LCD_OFF                  0x08 // Turn off LCD
#define LCD_ON_NO_CURSOR         0x0C // Turn on LCD, no cursor
#define LCD_ON_BLINK_CURSOR      0x0D // Turn on LCD, blinking block cursor
#define LCD_ON_LINE_CURSOR       0x0E // Turn on LCD, underline cursor
#define LCD_ON_LINE_BLINK_CURSOR 0x0F // Turn on LCD, blinking underline cursor
#define I2C_LCD_4B               0x02 // 4-bit mode (for initialization)
#define LCD_4B_58F_2L            0x2B // 4-bit, 5x8 character, 2 lines
#define LCD_8B_58F_2L            0x3B // 8-bit, 5x8 character, 2 lines
#define LCD_1ST_LINE             0x80 // Position cursor at beginning of 1st line
#define LCD_2ND_LINE             0xC0 // Position cursor at beginning of 2nd line




// I2C_LCD_init
// Initialize the LCD display module. The first step is to initialize the associated I2C port.
// Then there is a 20 ms wait time to give the display module time to fully power up. Then the
// command is sent to set the LCD module into the 4-bit mode.
void
I2C_LCD_init( I2C_TypeDef *thisI2C, uint32_t I2CSpeed )
{
  LCD_I2C = thisI2C;    // Set Global LCD_I2C interface to I2C1 or I2C2

  uint8_t LCD_data;


  I2C_init( LCD_I2C, I2CSpeed );

  // Send initial 4-bit command
  LCD_data = I2C_LCD_EN | I2C_LCD_BL | (I2C_LCD_4B << 4 );
  I2C_writeByte ( LCD_I2C, LCD_data, I2C_LCD_ADD );
  
  // Turn off EN after 1 ms
  LCD_data = I2C_LCD_BL;
  I2C_writeByte ( LCD_I2C, LCD_data, I2C_LCD_ADD );
}


// I2C_LCD_cmd
// Send command (not character) to LCD display. Commands are sent like data but with the RS pin
// set LOW. Will tack on extra delay when given clear or home commands as outlined in the
// datasheet.
void
I2C_LCD_cmd( uint8_t data )
{
  uint8_t I2C_data;

  // Place upper nibble and EN and BL and WR
  I2C_data = (data & 0b11110000) | I2C_LCD_EN | I2C_LCD_BL;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Clear EN bit
  I2C_data =   I2C_LCD_BL  ;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Place lower nibble and EN and BL and WR
  I2C_data = (data << 4) | I2C_LCD_EN | I2C_LCD_BL;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Clear EN bit
  I2C_data =  I2C_LCD_BL  ;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Standard 43 us pause
  delay_us( 43 );
  // If LCD Clear or Home commands, then give extra pause
  if(( data == LCD_CLEAR ) || ( data == LCD_HOME ))
    delay_us( 1487 );

}


// I2C_LCD_putc
// Writes a single character to the current position on the LCD display
void
I2C_LCD_putc( char data )
{
  char I2C_data;

  // Place upper nibble and set EN, RS, and BL bits 
  I2C_data = (data & 0b11110000) | I2C_LCD_EN | I2C_LCD_RS | I2C_LCD_BL;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Clear EN bit
  I2C_data =  I2C_LCD_BL | I2C_LCD_RS ;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Place lower nibble and set EN, RS, and BL bits
  I2C_data = (data << 4) | I2C_LCD_EN | I2C_LCD_RS | I2C_LCD_BL;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  // Clear EN bit
  I2C_data =  I2C_LCD_BL | I2C_LCD_RS ;
  I2C_writeByte( LCD_I2C, I2C_data, I2C_LCD_ADD );
  delay_us( 43 );
}


// LCD_puts
// Takes a pointer to a null-terminated string and displays that string from the current LCD 
// cursor position. Does not check for LCD line/string overflow.
void
I2C_LCD_puts( char *data )
{
  uint8_t j=0;

    while( data[j] != 0 )
      {
        I2C_LCD_putc( data[j] );
        j++;
      }
}

//  void
//  i100toa( int16_t realV, char *thisString )
//  i100toa takes a number with 2 decimal places multiplied by 100, and returns a string
//  of the original decimal number rounded to 1 decimal place. For example, if the number
//  in question is 12.36, then 1236 is passed via realV. The resulting string is 12.4,
//  because 12.36 rounds up to 12.4. Negative numbers and more complex rounding work as
//  expected. For example, -2.35, passed as -235, returns "-2.4", and 19.96, passed as 1996,
//  "w" = -2 and "d" = 4. Also, 19.96 would return "w" = 20 and "d" = 0. Note that "w" and "d"
//  are passed by reference.
void
i100toa( int16_t realV, char *thisString )
{
  char tmpString[6];              // Used to build output string. Might need to hold "-100\0"

  int16_t  x = abs( realV );      // realV = 2596 (target: 26.0C) -> x = 2596
  int16_t  w = x / 100;           // w (whole part) = 25
  int16_t  f = x - ( w * 100 );   // f (fraction)   = 2596 - 2500 = 96
  int16_t  d = f / 10;            // d (decimal)    = 9
  int16_t  r = f - ( d * 10 );    // r (remainder)  = 96 - 90 = 6

  if( r>=5 )                      // Round up if needed
  {
    d = d + 1;                    // d gets rounded up to 10
    if( d >= 10 )                 // If d gets rounded up,
    {
      d = 0;                      // d = 0    zero out d and
      w = w + 1;                  // w = 26   bump up w. 
    }
  }   
  if( realV < 0 )                 // Start with negative sign if original value was negative
    strcpy( thisString, "-" );
  else                            // otherwise start with blank string.
    strcpy( thisString, "" );

  itoa( w, tmpString, 10 );         // Make whole part into string
  strcat( thisString, tmpString );  // Tack onto thisString
  strcat( thisString, "." );        // Add the decimal point
  itoa( d, tmpString, 10 );         // Make a string for the single decimal place
  strcat( thisString, tmpString );  // Tack on the decimal digit to the rest of the string
}  

#endif /* __STM32F103_CMSIS_I2C_LCD_LIB_C */
