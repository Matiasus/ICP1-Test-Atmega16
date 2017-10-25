/** 
 * SPI COmmunication
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
#include <stdio.h>
#include <avr/io.h>
#include "spi.h"

/**
 * @description Spi initialisation
 *
 * @param  void
 * @return void
 */
void SpiInit(void)
{
  // Output: 
  //  SCK  - clock 
  //  MOSI - master out, slave in
  //  SS   - slave / chip select
  SPI_DDR |= (1 << SPI_SCK)  | 
             (1 << SPI_MOSI) | 
             (1 << SPI_SS);
  // Input:
  //  MISO - master in, slave out
  SPI_DDR &= ~(1 << SPI_MISO);
  // SPE  - SPI Enale
  // MSTR - Master device
  // with SPI2X - SPI Speed = 8Mhz/2 = 4Mhz
  SPCR  = (1 << SPE) | 
          (1 << MSTR);
  // SPI2X - Double SPI speed bit
  SPSR |= (1 << SPI2X);
}

/**
 * @description 
 *
 * @param  unsigned char
 * @return unsigned int
 */
unsigned int SpiReceive(unsigned char data)
{
  uint16_t receive = 0x00;

  // chip enable - active low
  SPI_PORT &= ~(1 << SPI_SS);

  // transmitting data high byte
  SPDR = data;
  // wait till high byte transmit
  while (!(SPSR & (1 << SPIF)));
  // higher 8 bits
  receive = (SPDR << 8);

  // transmitting data low byte
  SPDR = data;
  // wait till low byte transmit
  while (!(SPSR & (1 << SPIF)));
  // lower 8 bits
  receive |= SPDR;

  // chip disable - idle high
  SPI_PORT |= (1 << SPI_SS);
  // return received data
  return receive;
}

