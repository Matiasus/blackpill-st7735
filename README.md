# STM32F103C8T6 <=> 1.8 LCD with ST7735 driver
Neccessary hardware
- Development board - Blackpill (stm32f103c8t6)
- Debugger/programmer ST-Link v2
- 3.3V to 5V converter module (6 pins) or LCD support 3.3V tolerant
- 1.8 LCD with ST7735 driver
- wires

Neccessary software
- [libusb](https://github.com/Matiasus/stm32f103c8t6#install-libusb) - userspace USB programming library development files
- [stlink](https://github.com/Matiasus/stm32f103c8t6#install-stlink) - open source toolset to program and debug STM32 devices and boards
- [GNU Arm Embedded Toolchain](https://github.com/Matiasus/stm32f103c8t6#install-gnu-toolchain) - is a ready-to-use, open-source suite of tools for C, C++ and assembly programming
- [STM32-Template](https://github.com/geoffreymbrown/STM32-Template) - according to Geoffrey Brown stm32 template
- [Standard Periheral Library](https://www.st.com/content/st_com/en/products/embedded-software/mcu-mpu-embedded-software/stm32-embedded-software/stm32-standard-peripheral-libraries/stsw-stm32054.html) - the Library includes a complete register address mapping with all bits, bitfields and registers declared in C a collection of routines and data structures covering all peripheral functions (drivers with common API)

## ST7735 Library
Example of SPI1 communication with peripheral LCD display. Detailed information how to drive SPI on microcontroller STM32f103C8T6 is described in [RM0008 Reference manual](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf) and information about how to drive LCD with ST7735 driver is described in [ST7735 Manual](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf).

## Library
C library is aimed for driving ST7735 1.8 TFT LCD display 162x132 through SPI1's STM32f103C8T6 in master mode. Library uses SPI1 without remapping pins and software type of slave select management PINA4.

Version 1.0 
- communication through SPI1 (without remapping pins)
- software type of slave select management
- working with only one display

### Usage
Prior defined for microcontroller STM32f103C8T6 (Blackpill, Bluepill). 

### Tested
Library was tested and proved on a ST7735 1.8″ TFT display with Blackpill board used STM32f103C8T6.

### Pinout
SPI1 alternate function remapping according to [RM0008 Reference manual](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf) (9.3.10, page 181).

The GPIO pins configuration is according to [RM0008 Reference manual](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf) (9.1.11, page 167).

| PIN | SPI1 | GPIO Configuration |
| :---: | :---: | :--: |
| SS | PA4 |  Software => General Purpose push-pull |
| SCK | PA5 | Master => Alternate function push-pull |
| MISO | PA6 | Full duplex / master => Input floating / Input pull-up |
| MOSI | PA7 | Full duplex / master => Alternate function push-pull |

## Functions
- [void **ST7735_Init** (SPI_TypeDef * **SPIx**)](#ST7735_Init)
- [void **ST7735_ClearScreen** (uint16_t **color**)](#ST7735_ClearScreen)
- [uint8_t **ST7735_DrawChar** (char **character**, uint16_t **color**, enum Size **size**)](#ST7735_DrawChar)
- [void **ST7735_DrawString** (char * **string**, uint16_t **color**, enum Size **size**)](#ST7735_DrawString)
- [void **ST7735_DrawLine** (uint8_t **x0**, uint8_t **x**, uint8_t **y0**, uint8_t **y1**, uint16_t **color**)](#ST7735_DrawLine)
- [void **ST7735_DrawRectangle** (uint8_t **x0**, uint8_t **x1**, uint8_t **y0**, uint8_t **y1**, uint16_t **color**)](#ST7735_DrawRectangle)

### ST7735_Init
```c
void ST7735_Init (SPI_TypeDef * SPIx)
```
Initialisation process which initialize SPI1 clock, pins, load essential commands and parameters.

### ST7735_ClearScreen
```c
void ST7735_ClearScreen (uint16_t color)
```
Display clear with defined color and set cursor to position 0, 0.

### ST7735_DrawChar
```c
void ST7735_DrawChar (char character, uint16_t color, enum Size size)
```
Draw character on screen with defined color and specific size. Possible sizes are:

- X1 - basic size 5x8 pixels, 
- X2 - higher size 5x16 pixels, 
- X3 - higher and wider size 10x16 pixels.

### ST7735_DrawString
```c
void ST7735_DrawString (char * string, uint16_t color, enum Size size)
```
Draw string on screen with defined color and specific size. It uses function *ST7735_CheckPosition* which check whether the entire text fits at the end of screen. If no, the character is depicted on the new line (row).

### ST7735_DrawLine
```c
void ST7735_DrawLine (uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1, uint16_t color)
```
Draw horizontal, vertical line or sloping line with defined color. Important note - **Function does not check max coordinates**.

### ST7735_DrawRectangle
```c
void ST7735_DrawRectangle (uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color)
```
Draw rectangle with defined color. Important note - **Function does not check max coordinates**.

## Demonstration
<img src="Img/st7735.jpg" />

## Acknowledgement
- [Geoffreymbrown](https://github.com/geoffreymbrown)
- [Erwin Ouyang](http://www.handsonembedded.com/stm32f103-spl-tutorial-5/)
- [Manu NALEPA](https://github.com/nalepae/stm32_tutorial/blob/master/src/spi.c)
- [Learn, build, and share](https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-master/)
- [Adafuit TFT](https://github.com/adafruit/Adafruit-ST7735-Library)
- [AVR TFT](http://w8bh.net/avr/AvrTFT.pdf)

## Links
- [RM0008 Reference manual](https://www.st.com/resource/en/reference_manual/cd00171190-stm32f101xx-stm32f102xx-stm32f103xx-stm32f105xx-and-stm32f107xx-advanced-arm-based-32-bit-mcus-stmicroelectronics.pdf)
- [Datasheet ST7735](http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf)
