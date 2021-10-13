/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        SPI Library
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      11.10.2020
 * @file        spi.h
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      
 * --------------------------------------------------------------------------------------------+
 * @descr       
 * --------------------------------------------------------------------------------------------+
 * @inspir      https://github.com/nalepae/stm32_tutorial/blob/master/src/spi.c
 *              https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-master/
 *              http://www.handsonembedded.com/stm32f103-spl-tutorial-5/
 */

#ifndef __SPI_H__
#define __SPI_H__

  #define SPI_SS_GPIO       GPIOA
  #define SPI_SS_PIN        GPIO_BRR_BR4

  /**
   * @desc    Init pins for SPI1
   *
   * @param   SPI_TypeDef *
   * @param   void
   *
   * @return  void
   */
  void SPI_Pins_Init (SPI_TypeDef *);

  /**
   * @desc    Init SPI1 - master
   *
   * @param   SPI_TypeDef *
   *
   * @return  void
   */
  void SPI_Master_Init (SPI_TypeDef *);
  
  /**
   * @desc    Send 8 bits
   *
   * @param   SPI_TypeDef *
   * @param   unit8_t
   *
   * @return  void
   */
  void SPI_TX_8b (SPI_TypeDef *, uint8_t);

  /**
   * @desc    Receive 8 bits
   *
   * @param   SPI_TypeDef *
   * @param   void
   *
   * @return  uint8_t
   */
  uint8_t SPI_RX_8b (SPI_TypeDef *);

  /**
   * @desc    Transmission / receive array of data
   *
   * @param   SPI_TypeDef *
   * @param   unit8_t *
   * @param   unit8_t *
   * @param   unit16_t
   *
   * @return  unit16_t
   */
  uint16_t SPI_TRX_8b (SPI_TypeDef *, uint8_t *, uint8_t *, uint16_t);

  /**
   * @desc    Slave select 
   *
   * @param   GPIO_TypeDef *
   * @param   uint16_t
   *
   * @return  void
   */
  uint8_t SPI_SS_High (GPIO_TypeDef *, uint16_t);

  /**
   * @desc    Slave deselect 
   *
   * @param   GPIO_TypeDef *
   * @param   uint16_t
   *
   * @return  void
   */
  uint8_t SPI_SS_Low (GPIO_TypeDef *, uint16_t);

  /**
   * @desc    Stop SPI1
   *
   * @param   SPI_TypeDef *
   *
   * @return  void
   */
  void SPI_Disable (SPI_TypeDef *);

#endif
