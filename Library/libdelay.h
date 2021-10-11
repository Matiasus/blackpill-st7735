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
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @descr       Delay library using SysTick
 * @note        Before calling function DelayMs() must be called function DelayInit()
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */

#ifndef __LIBDELAY_H__
#define __LIBDELAY_H__

  /**
   * @desc    SysTick interrupt Handler
   *
   * @param   void
   *
   * @return  void
   */
  void SysTick_Handler (void);
  
  /**
   * @desc    Delay init
   *
   * @param   void
   *
   * @return  ErrorStatus defined in stm32f10x.h
   */
  ErrorStatus DelayInit (void);

  /**
   * @desc    Delay us function
   *
   * @param   uint32_t
   *
   * @return  void
   */
  void delayUs (uint32_t);

  /**
   * @desc    Delay ms function
   *
   * @param   uint32_t
   *
   * @return  void
   */
  void DelayMs (uint32_t);

#endif
