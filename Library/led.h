/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        LED Library
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      11.10.2020
 * @file        led.h
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @descr       LED flashing control of Blackpill's led connected to PB12
 * @note        Before calling function DelayMs() must be called function DelayInit()
 * --------------------------------------------------------------------------------------------+
 * @inspir      
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
