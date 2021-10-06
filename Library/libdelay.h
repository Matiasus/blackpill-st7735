/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        Lib delay
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.04.2021
 * @file        libdelay.h
 * @tested      stm32f103c8t6
 *
 * @depend      
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
   * @desc    Delay ms function
   *
   * @param   uint32_t
   *
   * @return  void
   */
  void delayMs (uint32_t);

#endif
