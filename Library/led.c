/** 
 * --------------------------------------------------------------------------------------------+ 
 * @desc        LED example
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2021
 *              Basic written by Geoffrey Brown
 *              Modified by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Geoffrey Brown / Marian Hrinko
 * @datum       23.03.2021
 * @file        led.c
 * @tested      stm32f103c8t6
 *
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @origin           
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
void ledGpioInit (void)
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
void ledOn (void)
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
void ledOff (void)
{
  // led off
  GPIOB->BSRR |= PIN12;
}

/**
 * @desc    Led off
 *
 * @param   uint8_t
 *
 * @return  void
 */
void ledBlink (uint8_t loops)
{
  // loops
  while (loops--) {
    // led on
    ledOn ();
    // delay
    delayMs (LOOPS);
    // led off
    ledOff ();
    // delay
    delayMs (LOOPS);
  }
}
