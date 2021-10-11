/** 
 * ---------------------------------------------------------------+ 
 * @desc        LED
 * ---------------------------------------------------------------+ 
 *              Copyright (C) 2021 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       24.03.2021
 * @file        led.h
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

#ifndef LED_H
#define LED_H

  #define PIN12   GPIO_BRR_BR12
  #define LOOPS   250

  /**
   * @desc    Init Led
   *
   * @param   GPIO_TypeDef
   *
   * @return  void
   */
  void LedGpioInit (void);

  /**
   * @desc    Led on
   *
   * @param   void
   *
   * @return  void
   */
  void LedOn (void);

  /**
   * @desc    Led off
   *
   * @param   void
   *
   * @return  void
   */
  void LedOff (void);

  /**
   * @desc    Led blink
   *
   * @param   uint8_t
   *
   * @return  void
   */
  void LedBlink (uint8_t);

#endif
