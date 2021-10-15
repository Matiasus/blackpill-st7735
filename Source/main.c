/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        MAIN - testing if SPI works
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      11.10.2020
 * @file        main.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      st7735.h, spi.h, led.h, libdelay.h
 * --------------------------------------------------------------------------------------------+
 * @descr       
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */
 
// libraries
#include <stm32f10x.h>
//#include "../Library/led.h"
//#include "../Library/spi.h"
#include "../Library/st7735.h"
//#include "../Library/libdelay.h"

/**
 * @desc    Main
 *
 * @param   void
 *
 * @return  void
 */
int main (void)
{
  // st7735
  // -------------------------------------------------------
  ST7735_Init (SPI1);

  // clear screen
  ST7735_ClearScreen (GREEN);

  for (uint8_t i = MAX_Y; i > 0; i--) {
    // draw line
    ST7735_DrawLine (0, MAX_X, 0, i, RED);
  }

  for (uint8_t i = MAX_Y; i > 0; i--) {
    // draw line
    ST7735_DrawLine (0, MAX_X, i, 0, BLUE);
  }  
  

  // set position
  ST7735_SetPosition (40, 10);
  // draw char
  //ST7735_DrawString ("st7735 <=> stm32f103c8t6", BLACK, X2);
  // draw char
  ST7735_DrawString ("ADRIANKO", BLACK, X3);
  // set position
  ST7735_SetPosition (55, 30);  
  // draw char
  ST7735_DrawString ("MIRKO", BLACK, X3);  

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
