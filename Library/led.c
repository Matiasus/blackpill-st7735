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
 * @file        led.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      led.h, libdelay.h
 * --------------------------------------------------------------------------------------------+
 * @descr       LED flashing control of Blackpill's led connected to PB12
 * @note        Before calling function DelayMs() must be called function DelayInit()
 * --------------------------------------------------------------------------------------------+
 * @inspir      
 */
 
#include <stm32f10x.h>
#include "libdelay.h"
#include "led.h"

/**
 * @desc    Init Led
 *
 * @param   GPIO_TypeDef
 *
 * @return  void
 */
void LedGpioInit (void)
{
  // enable clock for corresponding GPIO
  RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;
 
  // ----------------------------
  // CNF[1:0] = 00 -> Push pull  / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 01 -> Open drain / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 10 -> Push pull  / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 01 -> Open drain / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
  // CNF[1:0] = 00 -> Analog     / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 01 -> Floating   / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 10 -> Pull down  / INPUT - MODE[1:0] = 00
  // CNF[1:0] = 11 -> Pull up    / INPUT - MODE[1:0] = 00
  // ----------------------------
  //
  // GPIOB.Pin_12 - GENERAL PURPOSE OUTPUT / Push pull
  GPIOB->CRH &= ~(GPIO_CRH_CNF12);
  
  // MODE[1:0] = 01 // 10 MHz
  // MODE[1:0] = 10 //  2 MHz
  // MODE[1:0] = 11 // 50 MHz
  // ----------------------------
  // 
  // speed 2MHz
  GPIOB->CRH |= GPIO_CRH_MODE12_1;
}

/**
 * @desc    Led on
 *
 * @param   void
 *
 * @return  void
 */
void LedOn (void)
{
  // led ON
  GPIOB->BRR |= PIN12;
}

/**
 * @desc    Led off
 *
 * @param   void
 *
 * @return  void
 */
void LedOff (void)
{
  // led off
  GPIOB->BSRR |= PIN12;
}

/**
 * @desc    Led blink
 *
 * @param   uint8_t number of loops
 *
 * @return  void
 */
void LedBlink (uint8_t loops)
{
  // loops
  while (loops--) {
    // led on
    LedOn ();
    // delay
    DelayMs (LOOPS);
    // led off
    LedOff ();
    // delay
    DelayMs (LOOPS);
  }
}
