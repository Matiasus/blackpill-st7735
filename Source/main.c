/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        MAIN - testing if LCD works
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      15.10.2020
 * @file        main.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      st7735.h
 * --------------------------------------------------------------------------------------------+
 * @descr       FUnction test of lcd
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */
 
// libraries
#include "../Library/st7735.h"

/**
 * @desc    Main
 *
 * @param   void
 *
 * @return  void
 */
int main (void)
{
  // start
  uint8_t start = 30;
  // end
  uint8_t end = MAX_X - start;

  // st7735
  // -------------------------------------------------------
  ST7735_Init (SPI1);

  // clear screen
  ST7735_ClearScreen (WHITE);
  // set position X, Y
  ST7735_SetPosition (start + 12, 10);  
  // draw string
  ST7735_DrawString ("STM32F103C8T6", BLACK, X2);

  // draw Loading
  for (uint8_t i = start; i < end; i++) {
    // draw rectangle
    ST7735_DrawRectangle (start, i, 30, 40, RED);
    // delay
    Delay_Ms (10);
  }
  // set position
  ST7735_SetPosition (10, 50);
  // draw char
  ST7735_DrawString ("BLACKPILL <=> LCD ST7735", BLACK, X2);

  // return
  // -------------------------------------------------------
  return SUCCESS; 
}
 
#ifdef USE_FULL_ASSERT
  void assert_failed(uint8_t* file, uint32_t line)
  {
    // Use GDB to find out why we're here
    while (1);
  }
#endif
