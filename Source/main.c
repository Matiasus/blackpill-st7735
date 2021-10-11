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
 * @depend      spi.h, led.h, libdelay.h
 * --------------------------------------------------------------------------------------------+
 * @descr       
 * --------------------------------------------------------------------------------------------+
 * @inspir      https://github.com/nalepae/stm32_tutorial/blob/master/src/spi.c
 *              https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-master/
 *              http://www.handsonembedded.com/stm32f103-spl-tutorial-5/
 */
 
// libraries
#include <stm32f10x.h>
#include "../Library/led.h"
#include "../Library/spi.h"
#include "../Library/libdelay.h"

/**
 * @desc    Main
 *
 * @param   void
 *
 * @return  void
 */
int main (void)
{
  uint8_t n = 5, i = 0;
  // txbuffer
  uint8_t txbuffer[5] = {1, 2, 3, 4, 5};
  // rxbuffer
  uint8_t rxbuffer[n];

  // DELAY
  // ------------------------------------------------------- 
  DelayInit ();

  // LED
  // ------------------------------------------------------- 
  LedGpioInit ();
  
  // SPI
  // -------------------------------------------------------   
  SPI_Master_Init (SPI1);
  
  while (i < n) {
    // send 
    SPI_TRX_8b (SPI1, txbuffer, rxbuffer, (uint16_t) n);
    // led blink
    LedBlink (rxbuffer[i++]);
    // delay
    DelayMs (1000);
  }
  
  // led blink
  // ------------------------------------------------------- 
  //LedBlink (5);

  // st7735
  // -------------------------------------------------------
//  ST7735_Init (SPI1);

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
