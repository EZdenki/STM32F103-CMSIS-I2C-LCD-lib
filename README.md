# STM32F103-CMSIS-I2C-LCD-lib
Library and sample project using a Blue Pill (STM32F103) and a 16x2 LCD display driven by an I2C LCD driver module.
<br><br>
The I2C LCD Driver Module can be controlled via I2C1 (pins B6/B7) or I2C2 (pins B10/B11).
<br>
### Uses the following simple include libraries:
- STM32F103-Pause-lib.c          (Contains us timing routine)
- STM32F103-CMSIS-I2C-lib.c      (Contains root I2C routines)
- STM32F103-CMSIS-I2C-LCD-lib.c  (Contains I2C LCD Driver Module routines)

### STM32F103-CMSIS-I2C-LCD-lib.c Functions:
+ I2C_LCD_CMD( uint8_t data )<br>
	Associate and initialize I2C interface (I2C1 or I2C2) with the I2C LCD module.
+ I2C_LCD_putc( char data )<br>
	Display a character on the LCD.
+ I2C_LCD_puts( char *data )<br>
	Display a string on the LCD.
+ i100toa( int16_t realV, char *thisString )<br>
	Convert a 16-bit integer to a pre-allocated string
	
See library files and main.c for further details.
