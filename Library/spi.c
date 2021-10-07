/** 
 * --------------------------------------------------------------------------------------------+ 
 * @name        SPI Library
 * --------------------------------------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       08.03.2020
 * @update      08.03.2020
 * @file        spi.c
 * @version     1.0
 * @tested      stm32f103c6t8
 *
 * @depend      spi.h
 * --------------------------------------------------------------------------------------------+
 * @descr       Version 1.0 -> basic concept for spi communication with st7735 driver
 * --------------------------------------------------------------------------------------------+
 * @inspir      https://github.com/nalepae/stm32_tutorial/blob/master/src/spi.c
 *              https://learnbuildshare.wordpress.com/about/stm32/using-spi-as-master/
 *              http://www.handsonembedded.com/stm32f103-spl-tutorial-5/
 */

#include <stm32f10x.h>
#include "spi.h"

/**
 * @desc    Init MOSI, MISO, SCK, SS pins for SPIx
 *          Pin NSS is defined as GPIO, because SSM = 1, SSI = 1, SSOE = 0  
 *
 * @param   SPI_TypeDef *SPIx
 *
 * @return  void
 */
void SPI_Pins_Init (SPI_TypeDef *SPIx)
{
  // SPI1
  // --------------------------------------------------------------------------------
  if (SPIx == SPI1) {
    // enable clock for corresponding GPIOA
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN | RCC_APB2ENR_SPI1EN | RCC_APB2ENR_AFIOEN;
    // ----------------------------
    // CNF[1:0] = 00 -> Push pull  / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
    // CNF[1:0] = 01 -> Open drain / GENERAL PURPOSE OUTPUT - MODE[1:0] = 01; 10; 11
    // CNF[1:0] = 10 -> Push pull  / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
    // CNF[1:0] = 11 -> Open drain / ALTERNATE FUNCTION OUTPUT - MODE[1:0] = 01; 10; 11
    // CNF[1:0] = 00 -> Analog     / INPUT - MODE[1:0] = 00
    // CNF[1:0] = 01 -> Floating   / INPUT - MODE[1:0] = 00
    // CNF[1:0] = 10 -> Pull down  / INPUT - MODE[1:0] = 00
    // CNF[1:0] = 11 -> Pull up    / INPUT - MODE[1:0] = 00
    // ----------------------------
    // MODE[1:0] = 01 // 10 MHz
    // MODE[1:0] = 10 //  2 MHz
    // MODE[1:0] = 11 // 50 MHz
    // ----------------------------

    // null corresponding bits
    GPIOA->CRL &= ~(GPIO_CRL_CNF4 | GPIO_CRL_CNF5 | GPIO_CRL_CNF6 | GPIO_CRL_CNF7);
    GPIOA->CRL &= ~(GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE6 | GPIO_CRL_MODE7);

    // SS   - GPIOA.4 - GENERAL PURPOSE OUTPUT / PUSH PULL
    // SCK  - GPIOA.5 - ALTERNATE FUNCTION OUTPUT / PUSH PULL
    // MISO - GPIOA.6 - INPUT / PULL UP
    // MOSI - GPIOA.7 - ALTERNATE FUNCTION OUTPUT / PUSH PULL
    GPIOA->CRL |= GPIO_CRL_CNF5_1 | GPIO_CRL_CNF6_1 | GPIO_CRL_CNF7_1;    
    // SS   - GPIOA.4 - 50 MHz
    // SCK  - GPIOA.5 - 50 MHz
    // MISO - GPIOA.6
    // MOSI - GPIOA.7 - 50 MHz
    GPIOA->CRL |= GPIO_CRL_MODE4 | GPIO_CRL_MODE5 | GPIO_CRL_MODE7;
  // SPI2
  } else if (SPIx == SPI2) {
  
  }
}

/**
 * @desc    Init pins for SPI1
 *
 * @param   SPI_TypeDef *SPIx
 *
 * @return  void
 */
void SPI_Master_Init (SPI_TypeDef *SPIx)
{
  // SPI1_CR1 register
  // ----------------------------------------------------------------------------------
  // [15] BIDIMODE: Bidirectional data mode enable
  //   0: 2-line unidirectional data mode selected
  //   1: 1-line bidirectional data mode selected
  //
  // [14] BIDIOE: Output enable in bidirectional mode
  // This bit combined with the BIDImode bit selects the direction of transfer in bidirectional mode
  //    0: Output disabled (receive-only mode)
  //    1: Output enabled (transmit-only mode)
  // In master mode, the MOSI pin is used while the MISO pin is used in slave mode.
  //
  // [13] CRCEN: Hardware CRC calculation enable
  //    0: CRC calculation disabled
  //    1: CRC calculation enabled
  // Note: This bit should be written only when SPI is disabled (SPE = ‘0’) for correct operation.
  //
  // [12] CRCNEXT: CRC transfer next
  //    0: Data phase (no CRC phase)
  //    1: Next transfer is CRC (CRC phase)
  // Note: When the SPI is configured in full duplex or transmitter only modes, CRCNEXT must be
  // written as soon as the last data is written to the SPI_DR register.
  // When the SPI is configured in receiver only mode, CRCNEXT must be set after the
  // second last data reception.
  // This bit should be kept cleared when the transfers are managed by DMA.
  //
  // [11] DFF: Data frame format
  //    0: 8-bit data frame format is selected for transmission/reception
  //    1: 16-bit data frame format is selected for transmission/reception
  // Note: This bit should be written only when SPI is disabled (SPE = ‘0’) for correct operation.
  //
  // [10] RXONLY: Receive only
  //  This bit combined with the BIDImode bit selects the direction of transfer in 2-line
  //  unidirectional mode. This bit is also useful in a multislave system in which this particular
  //  slave is not accessed, the output from the accessed slave is not corrupted.
  //    0: Full duplex (Transmit and receive)
  //    1: Output disabled (Receive-only mode)
  //
  // [9] SSM: Software slave management
  // When the SSM bit is set, the NSS pin input is replaced with the value from the SSI bit.
  //    0: Software slave management disabled
  //    1: Software slave management enabled
  //
  // [8] SSI: Internal slave select
  // This bit has an effect only when the SSM bit is set. The value of this bit is forced onto the
  // NSS pin and the IO value of the NSS pin is ignored.
  //
  // [7] LSBFIRST: Frame format
  //    0: MSB transmitted first
  //    1: LSB transmitted first
  // Note: This bit should not be changed when communication is ongoing.
  //
  // [6] SPE: SPI enable
  //    0: Peripheral disabled
  //    1: Peripheral enabled
  // When disabling the SPI, follow the procedure described in Section 25.3.8.
  //
  // [5:3] BR[2:0]: Baud rate control
  //    000: f PCLK /2
  //    001: f PCLK /4
  //    010: f PCLK /8
  //    011: f PCLK /16
  //    100: f PCLK /32
  //    101: f PCLK /64
  //    110: f PCLK /128
  //    111: f PCLK /256
  // Note: These bits should not be changed when communication is ongoing.
  //
  // [2] MSTR: Master selection
  //    0: Slave configuration
  //    1: Master configuration
  // Note: This bit should not be changed when communication is ongoing.
  //
  // [1] CPOL: Clock polarity
  //    0: CK to 0 when idle
  //    1: CK to 1 when idle
  // Note: This bit should not be changed when communication is ongoing.
  //
  // [0] CPHA: Clock phase
  //    0: The first clock transition is the first data capture edge
  //    1: The second clock transition is the first data capture edge
  // Note: This bit should not be changed when communication is ongoing.
  // ----------------------------------------------------------------------------------
  // BIDIMODE: 0 - 2-line unidirectional data mode selected
  // BIDIOE:   0 - Output disabled (receive-only mode)
  // CRCEN:    0 - CRC calculation disabled
  // CRCNEXT:  0 - Data phase (no CRC phase)
  // DFF:      0 - 8-bit data frame format is selected for transmission/reception
  // RXONLY:   0 - Full duplex (Transmit and receive)
  // SSM:      1 - Software slave management enabled
  // SSI:      1
  // LSBFIRST: 0 - MSB transmitted first
  // SPE:      0 - Peripheral disabled
  // BR[2:0]:  100 - f PCLK/32
  // MSTR:     1 - Master configuration
  // CPOL:     0 - 0 when idle
  // CPHA:     0 - The first clock transition is the first data capture edge
  // ----------------------------------------------------------------------------------
  // set corresponding bits
  SPIx->CR1 = SPI_CR1_SSM | SPI_CR1_SSI | SPI_CR1_MSTR | SPI_CR1_BR_2;

  // Activate the SPI mode (Reset I2SMOD bit in I2SCFGR register)
  SPIx->I2SCFGR &= ~SPI_I2SCFGR_I2SMOD;

  // pin NSS output disabled
  SPIx->CR2 &= ~SPI_CR2_SSOE;

  // high level
  SPI_SS_Set (SPI_SS_GPIO, SPI_SS_PIN);

  // enable SPI1
  SPIx->CR1 |= SPI_CR1_SPE;
}

/**
 * @desc    Slave select 
 *
 * @param   GPIO_TypeDef *
 * @param   uint16_t
 *
 * @return  void
 */
uint8_t SPI_SS_Set (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // low level
  GPIOx->BRR = pin;
}

/**
 * @desc    Slave deselect 
 *
 * @param   GPIO_TypeDef *
 * @param   uint16_t
 *
 * @return  void
 */
uint8_t SPI_SS_Res (GPIO_TypeDef *GPIOx, uint16_t pin)
{
  // high level
  GPIOx->BSRR = pin;   
}

/**
 * @desc    Send 8 bits
 *
 * @param   SPI_TypeDef *SPIx
 * @param   unit8_t
 *
 * @return  void
 */
void SPI_TX_8b (SPI_TypeDef *SPIx, uint8_t data)
{
  // wait till ready to load next data
  while (!(SPIx->SR & SPI_SR_TXE));
  // fill SPI11 DATA REGISTER with data
  SPIx->DR = data;
  // wait till ready to load next data
  while (SPIx->SR & SPI_SR_BSY);
}

/**
 * @desc    Receive 8 bits
 *
 * @param   SPI_TypeDef *SPIx
 *
 * @return  uint8_t
 */
uint8_t SPI_RX_8b (SPI_TypeDef *SPIx)
{
  // wait till data is received
  while (!(SPIx->SR & SPI_SR_RXNE));
  // return data
  return SPIx->DR;
}

/**
 * @desc    Transmission / receive array of data
 *
 * @param   SPI_TypeDef *SPIx
 * @param   unit8_t *
 * @param   unit8_t *
 * @param   unit16_t
 *
 * @return  unit16_t
 */
uint16_t SPI_TRX_8b (SPI_TypeDef *SPIx, uint8_t *txbuffer, uint8_t *rxbuffer, uint16_t number)
{
  uint16_t i;
  // loop through data array
  for (i = 0; i < number; i++) {
    // send data
    SPI_TX_8b (SPIx, txbuffer[i]);
    // store received data
    rxbuffer[i] = SPI_RX_8b (SPIx);
  }
  // wait for ready to load next data
  while (!(SPIx->SR & SPI_SR_TXE));
  // check BUSY flag
  while (SPIx->SR & SPI_SR_BSY);
  // return received array of data
  return i;
}

/**
 * @desc    Stop SPI
 *
 * @param   SPI_TypeDef *
 *
 * @return  void
 */
void SPI_Disable (SPI_TypeDef *SPIx)
{
/*
  // wait till ready to load next data
  while (!(SPIx->SR & SPI_SR_TXE));
  // wait till data is received
  while (!(SPIx->SR & SPI_SR_RXNE));
  // check BUSY flag
  while (SPIx->SR & SPI_SR_BSY);
*/
  // disable SPI1
  SPIx->CR1 &= ~SPI_CR1_SPE;
  // low level
  SPI_SS_Res (SPI_SS_GPIO, SPI_SS_PIN);

  // SPI1 disable clock
  if (SPIx == SPI1) {
    // disable clock
    RCC->APB2ENR &= ~RCC_APB2ENR_SPI1EN;
  }
}
