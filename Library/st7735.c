/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        LCD Library
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      11.10.2020
 * @file        st7735.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      led.h, libdelay.h
 * --------------------------------------------------------------------------------------------+
 * @descr       C library for driving LCD 1.8" with st7735 driver
 * @note        Before calling function DelayMs() must be called function DelayInit()
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 */

#include <stm32f10x.h>
#include "spi.h"
#include "font.h"
#include "st7735.h"
#include "libdelay.h"

/** @array Init command */
const uint8_t INIT_ST7735B[] = {
  // number of initializers
  5,
  // ---------------------------------------
  // 0x01 Software reset - no arguments,  delay
  0, 150, SWRESET,
  // 0x11 Out of sleep mode, no arguments, delay
  0, 200, SLPOUT,  
  // 0x3A Set color mode, 1 argument delay
  1,  10, COLMOD, 0x05,
  // D7  D6  D5  D4  D3  D2  D1  D0
  // MY  MX  MV  ML RGB  MH   -   -
  // ------------------------------
  // ------------------------------
  // MV  MX  MY -> {MV (row / column exchange) MX (column address order), MY (row address order)}
  // ------------------------------
  //  0   0   0 -> begin left-up corner, end right-down corner 
  //               left-right (normal view) 
  //  0   0   1 -> begin left-down corner, end right-up corner 
  //               left-right (Y-mirror)
  //  0   1   0 -> begin right-up corner, end left-down corner 
  //               right-left (X-mirror)
  //  0   1   1 -> begin right-down corner, end left-up corner
  //               right-left (X-mirror, Y-mirror)
  //  1   0   0 -> begin left-up corner, end right-down corner
  //               up-down (X-Y exchange)  
  //  1   0   1 -> begin left-down corner, end right-up corner
  //               down-up (X-Y exchange, Y-mirror)
  //  1   1   0 -> begin right-up corner, end left-down corner 
  //               up-down (X-Y exchange, X-mirror)  
  //  1   1   1 -> begin right-down corner, end left-up corner
  //               down-up (X-Y exchange, X-mirror, Y-mirror)
  // ------------------------------
  //  ML: vertical refresh order 
  //      0 -> refresh top to bottom 
  //      1 -> refresh bottom to top
  // ------------------------------
  // RGB: filter panel
  //      0 -> RGB 
  //      1 -> BGR        
  // ------------------------------ 
  //  MH: horizontal refresh order 
  //      0 -> refresh left to right 
  //      1 -> refresh right to left
  // 0xA0 = 1010 0000
  // 0x36
  1,   0, MADCTL, 0xA0,
  // 0x29 Main screen turn on
  0, 200, DISPON 
  // ---------------------------------------
};

/** @var array Chache memory char index row */
unsigned short int cacheMemIndexRow = 0;
/** @var array Chache memory char index column */
unsigned short int cacheMemIndexCol = 0;

/**
 * @desc    Init pins
 *
 * @param   GPIO_TypeDef
 *
 * @return  void
 */
void ST7735_Pins_Init (GPIO_TypeDef *GPIOx)
{
  // GPIOA
  // ----------------------------
  if (GPIOx == GPIOA) {
    // enable clock for corresponding GPIOA
    SET_BIT (RCC->APB2ENR, RCC_APB2ENR_IOPAEN);
    
  // GPIOB
  // ----------------------------    
  } else if (GPIOx == GPIOB) {
    // enable clock for corresponding GPIOB
    SET_BIT (RCC->APB2ENR, RCC_APB2ENR_IOPBEN);
  }
 
  // ----------------------------
  // CNF[1:0] = 00 -> Push pull  / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 01 -> Open drain / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 10 -> Push pull  / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 11 -> Open drain / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 00 -> Analog     / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 01 -> Floating   / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 10 -> Pull down  / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 11 -> Pull up    / INPUT - MODE[1:0] = 00
  // ----------------------------
  //
  // null corresponding bits
  GPIOx->CRL &= ~(GPIO_CRL_MODE1 | GPIO_CRL_CNF1 | GPIO_CRL_MODE2 | GPIO_CRL_CNF2 | GPIO_CRL_MODE3 | GPIO_CRL_CNF3);
  
  // RES - GPIOA.Pin_1 - GENERAL PURPOSE OUTPUT / Push pull
  // DC -  GPIOA.Pin_2 - GENERAL PURPOSE OUTPUT / Push pull
  // BL  - GPIOA.Pin_3 - GENERAL PURPOSE OUTPUT / Push pull
  // ----------------------------
  // 
  // GENERAL PURPOSE OUTPUT / Push pull
  // GPIOx->CRL &= ~(GPIO_CRL_CNF1 | GPIO_CRL_CNF2 | GPIO_CRL_CNF3);

  // MODE[1:0] = 01 // 10 MHz
  // MODE[1:0] = 10 //  2 MHz
  // MODE[1:0] = 11 // 50 MHz
  // ----------------------------
  // 
  // speed 2 MHz
  GPIOx->CRL |= (GPIO_CRL_MODE1_1 | GPIO_CRL_MODE2_1 | GPIO_CRL_MODE3_1);
}

/**
 * @desc    Set pin High
 *
 * @param   GPIO_TypeDef
 * @param   uint16_t
 *
 * @return  void
 */
void ST7735_Pin_High (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // pin high level
  SET_BIT (GPIOx->BSRR, pin);
}

/**
 * @desc    Set pin Low
 *
 * @param   GPIO_TypeDef
 * @param   uint16_t
 *
 * @return  void
 */
void ST7735_Pin_Low (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // pin low level
  SET_BIT (GPIOx->BRR, pin);
}

/**
 * @desc    Hardware Reset
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_Reset (void)
{
  // Rseset Impulse shape with times
  // _____________              _____
  //              |            |
  //              |____________|
  //
  // |<- 200 ms ->|<- 200 ms ->|
  // -------------------------------
  // set HW high
  ST7735_Pin_High (GPIOA, ST7735_RES);
  // delay 200 ms
  DelayMs (200);
  // set HW low
  ST7735_Pin_Low (GPIOA, ST7735_RES);
  // delay 200 ms
  DelayMs (200);
  // set HW high
  ST7735_Pin_High (GPIOA, ST7735_RES);
}

/**
 * @desc    Init SPI communication
 *
 * @param   SPI_TypeDef *SPIx
 *
 * @return  void
 */
void ST7735_Spi_Init (SPI_TypeDef *SPIx)
{
  // init SPI Master
  SPI_Master_Init (SPIx);
}

/**
 * @desc    Init st7735 driver
 *
 * @param   SPI_TypeDef *SPIx
 *
 * @return  void
 */
void ST7735_Init (SPI_TypeDef *SPIx)
{
  // init delay
  DelayInit (); 
  // init pins
  ST7735_Pins_Init (GPIOA);
  // set backlight ON
  ST7735_Pin_High (GPIOA, ST7735_BL);
  // init spi
  ST7735_Spi_Init (SPIx);
  // hardware reset
  ST7735_Reset ();
  // initial seqeunce list
  ST7735_Init_Seq (INIT_ST7735B);
}

/**
 * @desc    Init sequence
 *
 * @param   const uint8_t*
 *
 * @return  void
 */
void ST7735_Init_Seq (const uint8_t *initializers)
{
  uint8_t i = 0;
  uint8_t args;
  uint8_t cmnd;
  uint8_t time;
  uint8_t loop = initializers[i++];

  // loop through whole initializer's list
  while (loop--) {

    // 1st arg - number of command arguments
    args = initializers[i++];
    // 2nd arg - delay time
    time = initializers[i++];
    // 3th arg - command
    cmnd = initializers[i++];
  
    // send command
    ST7735_Command (cmnd);
    // send arguments
    while (args--) {
      // send argument
      ST7735_Data8b (initializers[i++]);
    }
    // delay
    DelayMs (time);
  }

}

/**
 * @desc    Command send
 *
 * @param   uint8_t
 *
 * @return  void
 */
void ST7735_Command (uint8_t data)
{
  // chip enable - active low
  ST7735_Pin_Low (GPIOA, ST7735_CS);
  // command (active low)
  ST7735_Pin_Low (GPIOA, ST7735_DC);
  // transmitting data
  SPI_TRX_8b (SPI1, data);
  // chip disable - idle high
  ST7735_Pin_High (GPIOA, ST7735_CS);
}

/**
 * @desc    8bits data send
 *
 * @param   uint8_t
 *
 * @return  void
 */
void ST7735_Data8b (uint8_t data)
{
  // chip enable - active low
  ST7735_Pin_Low (GPIOA, ST7735_CS);
  // data (active high)
  ST7735_Pin_High (GPIOA, ST7735_DC);
  // transmitting data
  SPI_TRX_8b (SPI1, data);
  // chip disable - idle high
  ST7735_Pin_High (GPIOA, ST7735_CS);
}

/**
 * @desc    16bits data send
 *
 * @param   uint16_t
 *
 * @return  void
 */
void ST7735_Data16b (uint16_t data)
{
  // chip enable - active low
  ST7735_Pin_Low (GPIOA, ST7735_CS);
  // data (active high)
  ST7735_Pin_High (GPIOA, ST7735_DC);
  // transmitting data
  SPI_TRX_16b (SPI1, data);
  // chip disable - idle high
  ST7735_Pin_High (GPIOA, ST7735_CS);
}

/**
 * @desc    Set window
 *
 * @param   uint8_t x - start position
 * @param   uint8_t x - end position
 * @param   uint8_t y - start position
 * @param   uint8_t y - end position
 *
 * @return  uint8_t
 */
uint8_t ST7735_SetWindow (uint8_t x0, uint8_t x1, uint8_t y0, uint8_t y1)
{
  // check if coordinates is out of range
  if ((x0 > x1)     ||
      (x1 > SIZE_X) ||
      (y0 > y1)     ||
      (y1 > SIZE_Y)) { 
    // out of range
    return ST7735_ERROR;
  }  
  // column address set
  ST7735_Command (CASET);
  // send start x position
  ST7735_Data16b (0x0000 | x0);
  // send end x position
  ST7735_Data16b (0x0000 | x1);

  // row address set
  ST7735_Command (RASET);
  // send start y position
  ST7735_Data16b (0x0000 | y0);
  // send end y position
  ST7735_Data16b (0x0000 | y1);

  // success
  return ST7735_SUCCESS;
}

/**
 * @desc    Write color pixels
 *
 * @param   uint16_t color
 * @param   uint16_t counter
 *
 * @return  void
 */
void ST7735_SendColor565 (uint16_t color, uint16_t count)
{
  // access to RAM
  ST7735_Command (RAMWR);
  // counter
  while (count--) {
    // write color
    ST7735_Data16b (color);
  }
}

/**
 * @desc    Draw pixel
 *
 * @param   uint8_t x position / 0 <= cols <= MAX_X-1
 * @param   uint8_t y position / 0 <= rows <= MAX_Y-1
 * @param   uint16_t color
 *
 * @return  void
 */
void ST7735_DrawPixel (uint8_t x, uint8_t y, uint16_t color)
{
  // set window
  ST7735_SetWindow (x, x, y, y);
  // draw pixel by 565 mode
  ST7735_SendColor565 (color, 1);
}

/**
 * @desc    Clear screen
 *
 * @param   uint16_t color
 *
 * @return  void
 */
void ST7735_ClearScreen (uint16_t color)
{
  // set whole window
  ST7735_SetWindow (0, SIZE_X, 0, SIZE_Y);
  // draw individual pixels
  ST7735_SendColor565 (color, CACHE_SIZE_MEM);
}

/**
 * @desc    Draw line by Bresenham algoritm
 * @surce   https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
 *  
 * @param   uint8_t x start position / 0 <= cols <= MAX_X-1
 * @param   uint8_t x end position   / 0 <= cols <= MAX_X-1
 * @param   uint8_t y start position / 0 <= rows <= MAX_Y-1 
 * @param   uint8_t y end position   / 0 <= rows <= MAX_Y-1
 * @param   uint16_t color
 *
 * @return  char
 */
char ST7735_DrawLine (uint8_t x1, uint8_t x2, uint8_t y1, uint8_t y2, uint16_t color)
{
  // determinant
  int16_t D;
  // deltas
  int16_t delta_x, delta_y;
  // steps
  int16_t trace_x = 1, trace_y = 1;

  // delta x
  delta_x = x2 - x1;
  // delta y
  delta_y = y2 - y1;

  // check if x2 > x1
  if (delta_x < 0) {
    // negate delta x
    delta_x = -delta_x;
    // negate step x
    trace_x = -trace_x;
  }

  // check if y2 > y1
  if (delta_y < 0) {
    // negate detla y
    delta_y = -delta_y;
    // negate step y
    trace_y = -trace_y;
  }

  // Bresenham condition for m < 1 (dy < dx)
  if (delta_y < delta_x) {
    // calculate determinant
    D = (delta_y << 1) - delta_x;
    // draw first pixel
    ST7735_DrawPixel (x1, y1, color);
    // check if x1 equal x2
    while (x1 != x2) {
      // update x1
      x1 += trace_x;
      // check if determinant is positive
      if (D >= 0) {
        // update y1
        y1 += trace_y;
        // update determinant
        D -= 2*delta_x;    
      }
      // update deteminant
      D += 2*delta_y;
      // draw next pixel
      ST7735_DrawPixel (x1, y1, color);
    }
  // for m > 1 (dy > dx)    
  } else {
    // calculate determinant
    D = delta_y - (delta_x << 1);
    // draw first pixel
    ST7735_DrawPixel (x1, y1, color);
    // check if y2 equal y1
    while (y1 != y2) {
      // update y1
      y1 += trace_y;
      // check if determinant is positive
      if (D <= 0) {
        // update y1
        x1 += trace_x;
        // update determinant
        D += 2*delta_y;    
      }
      // update deteminant
      D -= 2*delta_x;
      // draw next pixel
      ST7735_DrawPixel (x1, y1, color);
    }
  }
  // success return
  return ST7735_SUCCESS;
}

/**
 * @desc    Fast draw line horizontal
 *
 * @param   uint8_t xs - start position
 * @param   uint8_t xe - end position
 * @param   uint8_t y - position
 * @param   uint16_t color
 *
 * @return void
 */
void ST7735_DrawLineHorizontal (uint8_t xs, uint8_t xe, uint8_t y, uint16_t color)
{
  uint8_t temp;
  // check if start is > as end  
  if (xs > xe) {
    // temporary safe
    temp = xs;
    // start change for end
    xe = xs;
    // end change for start
    xs = temp;
  }
  // set window
  ST7735_SetWindow (xs, xe, y, y);
  // draw pixel by 565 mode
  ST7735_SendColor565 (color, xe - xs);
}

/**
 * @desc    Fast draw line vertical
 *
 * @param   uint8_t x - position
 * @param   uint8_t ys - start position
 * @param   uint8_t ye - end position
 * @param   uint16_t color
 *
 * @return  void
 */
void ST7735_DrawLineVertical (uint8_t x, uint8_t ys, uint8_t ye, uint16_t color)
{
  uint8_t temp;
  // check if start is > as end
  if (ys > ye) {
    // temporary safe
    temp = ys;
    // start change for end
    ye = ys;
    // end change for start
    ys = temp;
  }
  // set window
  ST7735_SetWindow (x, x, ys, ye);
  // draw pixel by 565 mode
  ST7735_SendColor565 (color, ye - ys);
}

/**
 * @desc    Draw rectangle
 *
 * @param   uint8_t x start position
 * @param   uint8_t x end position
 * @param   uint8_t y start position
 * @param   uint8_t y end position
 * @param   uint16_t color
 *
 * @return  void
 */
void ST7735_DrawRectangle (uint8_t xs, uint8_t xe, uint8_t ys, uint8_t ye, uint16_t color)
{
  uint8_t temp;
  // check if start is > as end  
  if (xs > xe) {
    // temporary safe
    temp = xe;
    // start change for end
    xe = xs;
    // end change for start
    xs = temp;
  }
  // check if start is > as end
  if (ys > ye) {
    // temporary safe
    temp = ye;
    // start change for end
    ye = ys;
    // end change for start
    ys = temp;
  }
  // set window
  ST7735_SetWindow (xs, xe, ys, ye);
  // send color
  ST7735_SendColor565 (color, (xe-xs+1)*(ye-ys+1));  
}

/**
 * @desc    RAM Content Show
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_RAM_Content_Show (void)
{
  // display content on
  ST7735_Command (DISPON);
}

/**
 * @desc    RAM Content Hide
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_RAM_Content_Hide (void)
{
  // display content off
  ST7735_Command (DISPOFF);
}
