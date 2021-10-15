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

#ifndef __ST7735_H__
#define __ST7735_H__

  #define ST7735_RES            GPIO_BSRR_BS1
  #define ST7735_DC             GPIO_BSRR_BS2
  #define ST7735_BL             GPIO_BSRR_BS3
  #define ST7735_CS             GPIO_BSRR_BS4

  // Success / Error
  // -----------------------------------
  #ifdef SUCCESS
    #define ST7735_SUCCESS      SUCCESS
  #else 
    #define ST7735_SUCCESS      0
  #endif

  #ifdef ERROR
    #define ST7735_ERROR        ERROR
  #else
    #define ST7735_ERROR        1
  #endif

  // Command definition
  // -----------------------------------
  #define DELAY                 0x80
  
  #define NOP                   0x00
  #define SWRESET               0x01
  #define RDDID                 0x04
  #define RDDST                 0x09

  #define SLPIN                 0x10
  #define SLPOUT                0x11
  #define PTLON                 0x12
  #define NORON                 0x13

  #define INVOFF                0x20
  #define INVON                 0x21
  #define DISPOFF               0x28
  #define DISPON                0x29
  #define RAMRD                 0x2E
  #define CASET                 0x2A
  #define RASET                 0x2B
  #define RAMWR                 0x2C

  #define PTLAR                 0x30
  #define MADCTL                0x36
  #define COLMOD                0x3A

  #define FRMCTR1               0xB1
  #define FRMCTR2               0xB2
  #define FRMCTR3               0xB3
  #define INVCTR                0xB4
  #define DISSET5               0xB6

  #define PWCTR1                0xC0
  #define PWCTR2                0xC1
  #define PWCTR3                0xC2
  #define PWCTR4                0xC3
  #define PWCTR5                0xC4
  #define VMCTR1                0xC5

  #define RDID1                 0xDA
  #define RDID2                 0xDB
  #define RDID3                 0xDC
  #define RDID4                 0xDD

  #define GMCTRP1               0xE0
  #define GMCTRN1               0xE1

  #define PWCTR6                0xFC

  // Colors
  // -----------------------------------
  #define YELLOW                0xFFE0
  #define BLACK                 0x0000
  #define WHITE                 0xFFFF
  #define GREEN                 0x07E0
  #define BLUE                  0x00FF  
  #define RED                   0xF000

  // AREA definition
  // -----------------------------------
  #define MAX_X                 161               // max columns / MV = 0 in MADCTL
  #define MAX_Y                 130               // max rows / MV = 0 in MADCTL
  #define SIZE_X                MAX_X - 1         // columns max counter
  #define SIZE_Y                MAX_Y - 1         // rows max counter
  #define CACHE_SIZE_MEM        (MAX_X * MAX_Y)   // whole pixels
  #define CHARS_COLS_LEN        5                 // number of columns for chars
  #define CHARS_ROWS_LEN        8                 // number of rows for chars

  /** @const Command list ST7735B */
  extern const uint8_t INIT_ST7735B[];

  /** @enum Font sizes */
  enum Size {
    // 1x high & 1x wide size
    X1 = 0x00,
    // 2x high & 1x wide size
    X2 = 0x80,
    // 2x high & 2x wider size
    // 0x0A is set because need to offset 5 position to right
    //      when draw the characters of string 
    X3 = 0x81
  };

  /**
   * @desc    Hardware Reset
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_Reset (void);
  
  /**
   * @desc    Set pin High
   *
   * @param   GPIO_TypeDef
   * @param   uint16_t
   *
   * @return  void
   */
  void ST7735_Pin_High (GPIO_TypeDef *, uint16_t);

  /**
   * @desc    Set pin Low
   *
   * @param   GPIO_TypeDef
   * @param   uint16_t
   *
   * @return  void
   */
  void ST7735_Pin_Low (GPIO_TypeDef *, uint16_t);
  
  /**
   * @desc    Init pins
   *
   * @param   GPIO_TypeDef
   *
   * @return  void
   */
  void ST7735_Pins_Init (GPIO_TypeDef *);

  /**
   * @desc    Init SPI communication
   *
   * @param   SPI_TypeDef *SPIx
   *
   * @return  void
   */
  void ST7735_Spi_Init (SPI_TypeDef *);

  /**
   * @desc    Init sequence
   *
   * @param   const uint8_t*
   *
   * @return  void
   */
  void ST7735_Init_Seq (const uint8_t *);

  /**
   * @desc    Init st7735 driver
   *
   * @param   SPI_TypeDef *SPIx
   *
   * @return  void
   */
  void ST7735_Init (SPI_TypeDef *);

  /**
   * @desc    Command send
   *
   * @param   uint8_t
   *
   * @return  void
   */
  void ST7735_Command (uint8_t);

  /**
   * @desc    8bits data send
   *
   * @param   uint8_t
   *
   * @return  void
   */
  void ST7735_Data8b (uint8_t);

  /**
   * @desc    16bits data send
   *
   * @param   uint16_t
   *
   * @return  void
   */
  void ST7735_Data16b (uint16_t);

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
  uint8_t ST7735_SetWindow (uint8_t, uint8_t, uint8_t, uint8_t);

  /**
   * @desc    Write color pixels
   *
   * @param   uint16_t color
   * @param   uint16_t counter
   *
   * @return  void
   */
  void ST7735_SendColor565 (uint16_t, uint16_t);

  /**
   * @desc    Draw pixel
   *
   * @param   uint8_t x position / 0 <= cols <= MAX_X-1
   * @param   uint8_t y position / 0 <= rows <= MAX_Y-1
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_DrawPixel (uint8_t, uint8_t, uint16_t);

  /**
   * @desc    Clear screen
   *
   * @param   uint16_t color
   *
   * @return  void
   */
  void ST7735_ClearScreen (uint16_t);

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
  char ST7735_DrawLine (uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);

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
  void ST7735_DrawLineHorizontal (uint8_t, uint8_t, uint8_t, uint16_t);

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
  void ST7735_DrawLineVertical (uint8_t, uint8_t, uint8_t, uint16_t);

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
  void ST7735_DrawRectangle (uint8_t, uint8_t, uint8_t, uint8_t, uint16_t);
  
  /**
   * @desc    Draw character
   *
   * @param   char character
   * @param   uint16_t color
   * @param   enum Size (X1, X2, X3)
   *
   * @return  uint8_t
   */
  uint8_t ST7735_DrawChar (char, uint16_t, enum Size);
  
  /**
   * @desc    Set text position x, y
   *
   * @param   uint8_t x - position
   * @param   uint8_t y - position
   *
   * @return  uint8_t
   */
  uint8_t ST7735_SetPosition (uint8_t, uint8_t);
  
  /**
   * @desc    Check text position x, y
   *
   * @param   uint8_t x - position
   * @param   uint8_t y - position
   * @param   uint8_t
   *
   * @return  uint8_t
   */
  uint8_t ST7735_CheckPosition (uint8_t, uint8_t, uint8_t, enum Size);
  
  /**
   * @desc    Draw string
   *
   * @param   char * string 
   * @param   uint16_t color
   * @param   enum Size (X1, X2, X3)
   *
   * @return  void
   */
  void ST7735_DrawString (char *, uint16_t, enum Size size);

  /**
   * @desc    RAM Content Show
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_RAM_Content_Show (void);

  /**
   * @desc    RAM Content Hide
   *
   * @param   void
   *
   * @return  void
   */
  void ST7735_RAM_Content_Hide (void);

#endif
