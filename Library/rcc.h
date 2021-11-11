/** 
 * ---------------------------------------------------------------+ 
 * @desc        RCC
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2021 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       21.04.2021
 * @file        rcc.h
 * @update      -
 * @version     1.0
 * @tested      stm32f103c8t6
 *
 * @depend      
 * ---------------------------------------------------------------+
 * @interface   
 * @pins         
 *
 */

#ifndef RCC_H
#define RCC_H

  /**
   * @desc    RCC deinit
   *
   * @param   void
   *
   * @return  void
   */
  void RCC_DeInit (void);

  /**
   * @desc    Set system clock source to HSE
   *
   * @param   void
   *
   * @return  void
   */
  uint32_t SetSysClk_HSE_8MHz (void);

  /**
   * @desc    Set system clock source to PLL 72MHz
   *
   * @param   void
   *
   * @return  void
   */
  uint32_t SetSysClk_PLL_72MHz (void);

  /**
   * @desc    RCC init HSI
   *
   * @param   void
   *
   * @return  void
   */
  void RCC_Init_HSI (void);

  /**
   * @desc    RCC->CFGR Config
   *
   * @param   uint32_t mask
   * @param   uint32_t new value
   *
   * @return  void
   */
  void RCC_CFGR_Config (uint32_t, uint32_t);

#endif
