/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        Lib delay
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.04.2021
 * @file        libdelay.c
 * @tested      stm32f103c8t6
 *
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */
 
// libraries
#include <stm32f10x.h>
#include "../Library/libdelay.h"

// delay
volatile uint32_t delayTime;

/**
 * @desc    SysTick interrupt Handler
 *
 * @param   void
 *
 * @return  void
 */
void SysTick_Handler (void)
{
  // decrement
  delayTime--;
}

/**
 * @desc    Delay ms function
 *
 * @param   uint32_t
 *
 * @return  void
 */
void delayMs (uint32_t cycles)
{
  delayTime = cycles;
  while (delayTime != 0);
}
