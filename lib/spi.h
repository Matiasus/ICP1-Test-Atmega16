/** 
 * ADC7476 SPI Communication
 *
 * Copyright (C) 2016 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       01.07.2017
 * @file        spi.h
 * @tested      AVR Atmega16
 * @inspiration 
 *
 */
#ifndef __SPI_H__
#define __SPI_H__

  // SPI Communication
  #ifndef SPI_PORT
    #define SPI_PORT PORTB
  #endif
  #ifndef SPI_DDR
    #define SPI_DDR  DDRB
  #endif
  #ifndef SPI_SCK
    #define SPI_SCK  7 // SCK
  #endif
  #ifndef SPI_MISO
    #define SPI_MISO 6 // MISO
  #endif
  #ifndef SPI_MOSI
    #define SPI_MOSI 5 // MOSI
  #endif
  #ifndef SPI_SS
    #define SPI_SS   4 // SS / CS
  #endif


  /**
   * @description Spi initialisation
   *
   * @param  void
   * @return void
   */
  void SpiInit(void);

  /**
   * @description ADC7476 SPI data receive
   *
   * @param  uint8_t
   * @return uint16_t
   */
  unsigned int Adc7476Data(unsigned char);

#endif
