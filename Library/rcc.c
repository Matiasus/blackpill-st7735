/** 
 * --------------------------------------------------------------------------------------------+
 * @desc        RCC Library for Blackpill (stm32f103c8t6)
 * --------------------------------------------------------------------------------------------+
 *              Copyright (C) 2021 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       21.04.2021
 * @file        rcc.c
 * @update      11.11.2021
 * @version     1.0
 * @tested      stm32f103c8t6
 *
 * @depend      rcc.h     
 * --------------------------------------------------------------------------------------------+
 * @descr       Select clock source - HSE (8MHz), PLL (72MHz)
 * @note        
 * --------------------------------------------------------------------------------------------+
 * @inspir      Standard Peripheral Library 
 */

// libraries
#include "rcc.h"

/**
 * @desc    RCC deinit according to SPL stm32f10x.h
 *
 * @param   void
 *
 * @return  void
 */
void RCC_DeInit (void)
{
  // set enable Internal high-speed clock
  SET_BIT (RCC->CR, RCC_CR_HSION);
  // Reset SW, SWS, HPRE, PPRE1, PPRE2, ADCPRE 
  CLEAR_BIT (RCC->CFGR, (RCC_CFGR_SW | RCC_CFGR_SWS | RCC_CFGR_HPRE | RCC_CFGR_PPRE1 | RCC_CFGR_PPRE2 | RCC_CFGR_ADCPRE));
  // Reset MCO bits, NOTE: for stm32f103c8t6 only MCO bits [0:2]
  CLEAR_BIT (RCC->CFGR, (RCC_CFGR_MCO_0 | RCC_CFGR_MCO_1 | RCC_CFGR_MCO_2));
  // Reset HSEON, CSSON and PLLON bits
  CLEAR_BIT (RCC->CR, (RCC_CR_HSEON | RCC_CR_CSSON | RCC_CR_PLLON));
  // Reset HSEBYP bit
  CLEAR_BIT (RCC->CR, RCC_CR_HSEBYP);
  // Reset PLLSRC, PLLXTPRE, PLLMUL and USBPRE/OTGFSPRE bits
  CLEAR_BIT (RCC->CFGR, (RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE | RCC_CFGR_PLLMULL | RCC_CFGR_USBPRE));
  // Disable all interrupts and clear pending bits
  WRITE_REG (RCC->CIR, (RCC_CIR_LSIRDYC | RCC_CIR_LSERDYC | RCC_CIR_HSIRDYC | RCC_CIR_HSERDYC | RCC_CIR_PLLRDYC | RCC_CIR_CSSC));
}

/**
 * @desc    Set system clock source to HSE
 *
 * @param   void
 *
 * @return  uint32_t
 */
uint32_t SetSysClk_HSE_8MHz (void)
{
  // counter for startup counts
  __IO uint32_t StartUpCounter = 0;
  // flag status
  uint32_t status;

  // Deinit process
  // ----------------------------------------------------------------------
  RCC_DeInit ();

  // Reset HSEON bit
  CLEAR_BIT (RCC->CR, RCC_CR_HSEON);
  // Reset HSEBYP bit
  CLEAR_BIT (RCC->CR, RCC_CR_HSEBYP);
  // set HSE ON
  SET_BIT (RCC->CR, RCC_CR_HSEON);

  // wait till startup timer count or HSE is ready
  do {
    // get HSE Ready status
    status = READ_BIT (RCC->CR, RCC_CR_HSERDY);
    // increment counter
    StartUpCounter++;
  // counter < 0x0500 or status is not ready
  } while ((!status) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  // if HSE Ready status still not ready
  if (status != RCC_CR_HSERDY) {
    // error, HSE not ready
    return ERROR;
  }

  // PREFETCH BUFFER
  // ----------------------------------------------------------------------
  // disable prefetch buffer
  CLEAR_BIT (FLASH->ACR, FLASH_ACR_PRFTBE);
  // enable prefetch buffer
  SET_BIT (FLASH->ACR, FLASH_ACR_PRFTBE);

  // LATENCY
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_0);

  // AHB / 1 Clock
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

  // APB1 / 1 Clock
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV1);

  // APB2 / 1 Clock
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

  // SYSCLK Clock
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_SW, RCC_CFGR_SW_HSE);

  // check if HSE is selected as SYSCLK 
  while (!READ_BIT(RCC->CFGR, RCC_CFGR_SWS_HSE));

  // succes, HSE ready
  return SUCCESS;
}

/**
 * @desc    Set system clock source to PLL 72MHz
 *
 * @param   void
 *
 * @return  uint32_t
 */
uint32_t SetSysClk_PLL_72MHz (void)
{
  // counter for startup counts
  __IO uint32_t StartUpCounter = 0;
  // flag status
  uint32_t status;

  // Deinit process
  // ----------------------------------------------------------------------
  RCC_DeInit ();

  // Reset HSEON bit
  CLEAR_BIT (RCC->CR, RCC_CR_HSEON);
  // Reset HSEBYP bit
  CLEAR_BIT (RCC->CR, RCC_CR_HSEBYP);
  // set HSE ON
  SET_BIT (RCC->CR, RCC_CR_HSEON);

  // wait till startup timer count or HSE is ready
  do {
    // get HSE Ready status
    status = READ_BIT (RCC->CR, RCC_CR_HSERDY);
    // increment counter
    StartUpCounter++;
  // counter < 0x0500 or status is not ready
  } while ((!status) && (StartUpCounter != HSE_STARTUP_TIMEOUT));

  // if HSE Ready status still not ready
  if (status != RCC_CR_HSERDY) {
    // error, HSE not ready
    return ERROR;
  }

  // PREFETCH BUFFER
  // ----------------------------------------------------------------------
  // disable prefetch buffer
  CLEAR_BIT (FLASH->ACR, FLASH_ACR_PRFTBE);
  // enable prefetch buffer
  SET_BIT (FLASH->ACR, FLASH_ACR_PRFTBE);

  // LATENCY
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (FLASH_ACR_LATENCY, FLASH_ACR_LATENCY_2);

  // AHB Clock / 1 = 72MHz
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_HPRE, RCC_CFGR_HPRE_DIV1);

  // APB1 Clock / 2 = 36MHz
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_PPRE1, RCC_CFGR_PPRE1_DIV2);

  // APB2 Clock / 1 = 72MHz
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_PPRE2, RCC_CFGR_PPRE2_DIV1);

  // SYSCLK Clock
  // ----------------------------------------------------------------------
  RCC_CFGR_Config (RCC_CFGR_SW, RCC_CFGR_SW_PLL);

  // PLL Source HSE not divided & Multiplication factor = 9
  RCC_CFGR_Config ((RCC_CFGR_PLLMULL | RCC_CFGR_PLLSRC | RCC_CFGR_PLLXTPRE), (RCC_CFGR_PLLSRC_HSE | RCC_CFGR_PLLXTPRE_HSE | RCC_CFGR_PLLMULL9));

  // enable PLL
  SET_BIT (RCC->CR, RCC_CR_PLLON);

  // check if PLL is selected as SYSCLK 
  while (!READ_BIT(RCC->CFGR, RCC_CFGR_SWS_PLL));

  // succes, HSE ready
  return SUCCESS;
}

/**
 * @desc    RCC->CFGR Config
 *
 * @param   uint32_t mask
 * @param   uint32_t new value
 *
 * @return  void
 */
void RCC_CFGR_Config (uint32_t mask, uint32_t value)
{
  // temp register
  uint32_t temp;
  // save current value
  temp = READ_REG (RCC->CFGR);
  // apply mask 
  CLEAR_BIT (temp, mask);
  // set correstonding bits
  SET_BIT (temp, value);
  // write into register
  WRITE_REG (RCC->CFGR, temp);
}
