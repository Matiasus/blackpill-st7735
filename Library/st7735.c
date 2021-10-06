/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        ST773 1.8" LCD Driver
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       26.05.2021
 * @file        st7735.c
 * @tested      stm32f103c8t6
 *
 * @depend      font.h
 * --------------------------------------------------------------------------------------------+
 * @inspir      http://www.displayfuture.com/Display/datasheet/controller/ST7735.pdf
 *              https://github.com/adafruit/Adafruit-ST7735-Library
 *              http://w8bh.net/avr/AvrTFT.pdf
 */
#include <stm32f10x.h>
#include "led.h"
#include "spi.h"
#include "font.h"
#include "st7735.h"
#include "libdelay.h"

/** @array Init command */
const uint8_t INIT_ST7735B[] = {
  // number of initializers
  5,
  // ---------------------------------------
  // Software reset - no arguments,  delay
  0, 150, SWRESET,
  // Out of sleep mode, no arguments, delay
  0, 200, SLPOUT,  
  // Set color mode, 1 argument delay
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
  1,   0, MADCTL, 0xA0,
  // Main screen turn on
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
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;
    
  // GPIOB
  // ----------------------------    
  } else if (GPIOx == GPIOB) {
    // enable clock for corresponding GPIOB
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
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
  // speed 50 MHz
  GPIOx->CRL |= (GPIO_CRL_MODE1 | GPIO_CRL_MODE2 | GPIO_CRL_MODE3);
}

/**
 * @desc    Set pin High
 *
 * @param   GPIO_TypeDef
 * @param   uint16_t
 *
 * @return  void
 */
void ST7735_Pin_Set (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // pin high level
  GPIOx->BSRR |= pin;
}

/**
 * @desc    Set pin Low
 *
 * @param   GPIO_TypeDef
 * @param   uint16_t
 *
 * @return  void
 */
void ST7735_Pin_Res (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // pin low level
  GPIOx->BRR |= pin;
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
  ST7735_Pin_Set (GPIOA, ST7735_RES);
  // delay 200 ms
  delayMs (200);
  // set HW low
  ST7735_Pin_Res (GPIOA, ST7735_RES);
  // delay 200 ms
  delayMs (200);
  // set HW high
  ST7735_Pin_Set (GPIOA, ST7735_RES);
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
  // init SPI Pins
  SPI_Pins_Init (SPIx);
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
  // init pins
  ST7735_Pins_Init (GPIOA);
  // set backlight ON
  ST7735_Pin_Set (GPIOA, ST7735_BL);
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

  // loop through whole initializer list
  while (loop--) {
    // 1st arg - number of command arguments
    args = initializers[i++];
    // 2nd arg - delay time
    time = initializers[i++];
    // 3th arg - command
    cmnd = initializers[i++];
    // send command
    ST7735_Cmd_Send (cmnd);
    // send arguments
    while (args--) {
      // send argument
      ST7735_Data8b_Send (initializers[i++]);
    }
    // delay
    delayMs (time);
  }
}

/**
 * @desc    Command send
 *
 * @param   uint8_t
 *
 * @return  void
 */
void ST7735_Cmd_Send (uint8_t data)
{
  // chip enable - active low
  ST7735_Pin_Res (GPIOA, ST7735_CS);
  // command (active low)
  ST7735_Pin_Res (GPIOA, ST7735_DC);
  // transmitting data
  SPI_TX_8b (SPI1, data);
  // chip disable - idle high
  ST7735_Pin_Set (GPIOA, ST7735_CS);
}

/**
 * @desc    8bits data send
 *
 * @param   uint8_t
 *
 * @return  void
 */
void ST7735_Data8b_Send (uint8_t data)
{
  // chip enable - active low
  ST7735_Pin_Res (GPIOA, ST7735_CS);
  // data (active high)
  ST7735_Pin_Set (GPIOA, ST7735_DC);
  // transmitting data
  SPI_TX_8b (SPI1, data);
  // chip disable - idle high
  ST7735_Pin_Set (GPIOA, ST7735_CS);
}

/**
 * @desc    Update screen
 *
 * @param   void
 *
 * @return  void
 */
void ST7735_UpdateScreen (void)
{
  // display on
  ST7735_Cmd_Send (DISPON);
}
