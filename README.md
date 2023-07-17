# STM32F103-CMSIS-I2C-LCD
Sample project using a Blue Pill (STM32F103) and a 16x2 LCD display driven by an I2C LCD driver module.
<br><br>
The I2C LCD Driver Module can be controlled via I2C1 (pins B6/B7) or I2C2 (pins B10/B11).
<br>
Uses the following simple include libraries:
- STM32F103-pause-lib.c          (Contains us timing routine)
- STM32F103-CMSIS-I2C-lib.c      (Contiains root I2C routines)
- STM32F103-CMSIS-I2C-LCD-lib.c  (Contains I2C LCD Driver Module routines)

See main.c for details.
