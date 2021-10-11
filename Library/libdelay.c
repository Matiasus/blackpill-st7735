/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        Delay library
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.04.2021
 * @update      11.10.2020
 * @file        libdelay.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      libdelay.h
 * --------------------------------------------------------------------------------------------+
 * @descr       Delay library using SysTick
 * @note        Before calling function DelayMs() must be called function DelayInit()
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */
 
// libraries
#include <stm32f10x.h>
#include "libdelay.h"

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
 * @desc    Delay init
 *
 * @param   void
 *
 * @return  ErrorStatus defined in stm32f10x.h
 */
ErrorStatus DelayInit (void)
{
  // variables
  // -------------------------------------------------------
  ErrorStatus returnCode;

  // delay
  // -------------------------------------------------------
  // set how many periods need to be between 2 interrupts
  returnCode = SysTick_Config (SystemCoreClock / 1000);
  // enable interrupts
  __enable_irq ();
  // success
  if (returnCode != SUCCESS) {
    // error Handling
    return ERROR;
  }
  
  // success
  return SUCCESS;
}

/**
 * @desc    Delay ms function
 *
 * @param   uint32_t
 *
 * @return  void
 */
void DelayMs (uint32_t cycles)
{
  // set cycles
  delayTime = cycles;
  // loop
  while (delayTime != 0);
}
