/**
 * SPI example
 *
 * Copyright (C) 2020 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @file        main.c
 * @tested      stm32f103c8t6
 * @descript    Example spi communication for stm32f103c8t6
 * @usage       Education
 * --------------------------------------------------------
 */
 
// libraries
#include <stm32f10x.h>
#include "../Library/led.h"
#include "../Library/st7735.h"
#include "../Library/libdelay.h"

/**
 * @desc    Main
 *
 * @param   void
 * @return  void
 */
int main (void)
{
  // variables
  // -------------------------------------------------------
  uint32_t returnCode;
  uint32_t t = 2000;

  // delay
  // -------------------------------------------------------
  // set how many periods need to be between 2 interrupts
  returnCode = SysTick_Config (SystemCoreClock / 1000);
  // enable interrupts
  __enable_irq ();
  // success
  if (returnCode != 0) {
    // error Handling
    return 1;
  }

  // st7735
  // -------------------------------------------------------
  ST7735_Init (SPI1);

  // return
  // -------------------------------------------------------
  return 0; 
}
 
#ifdef USE_FULL_ASSERT
  void assert_failed(uint8_t* file, uint32_t line)
  {
    // Use GDB to find out why we're here
    while (1);
  }
#endif
