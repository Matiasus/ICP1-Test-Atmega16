#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/io.h>
#include "lib/spi.h"
#include "lib/st7735.h"

#define PRESCALER_STOP   0
#define PRESCALER_1      1
#define PRESCALER_8      2
#define PRESCALER_64     3
#define PRESCALER_256    4
#define PRESCALER_1024   5

// Spustenie casovaca 1A
// - nulovanie bitov preddelicky
// - nastavenie
#define TIMER1A_START(PRESCALER) { TCCR1B &= ~((1 << CS12) | (1 << CS11) | (1 << CS10)); TCCR1B |= PRESCALER & 0x07; }

// gathred times
volatile uint16_t times[3];
// need 3 times for determining duty cycle
//    ______          ______
// __|@@@@@@|________|@@@@@@|_______
//   |      |        |
//   t1     t2      t3
volatile uint8_t cycles = 0;
// start with falling edge
volatile uint8_t falling = 1;

/**
 * @description Input capture interrupt routine
 *
 * @param  TIMER1_CAPT_vect
 * @return Void
 */
ISR(TIMER1_CAPT_vect)
{
  if (cycles < 3) {
    // detect rising edge
    if (falling == 0) {
      // write low byte
      times[cycles] = ICR1L;
      // write high byte
      times[cycles] |= (ICR1H << 8);
      // falling edge trigger
      TCCR1B |= (1 << ICES1);     
      // detect falling edge
      falling = 1;
      // decrement cycle
      cycles++;
    } else {
      // write low byte
      times[cycles] = ICR1L;
      // write high byte
      times[cycles] |= (ICR1H << 8);
      // falling edge trigger
      TCCR1B &= ~(1 << ICES1);  
      // detect falling edge
      falling = 0;
      // decrement cycle
      cycles++;
    }
  }
}

/***
 * Inicializacia casovaca Timer1A
 * nastavenie frekvencie snimaneho impulzu
 *
 * @param  Void
 * @return Void
 */
void Timer1AInit(void)
{
  // OC1A as output
  DDRD |= (1 << PD5);
  // nuluje pocitadlo
  TCNT1  = 0;
  // COM1A1, COM1A0 = 0; OC1A disconnected
  // COM1B1, COM1B0 = 0; OC1B disconnected
  //  FOC1A,  FOC1B = 0; 
  //  WGM11,  WGM10 = 0; normal operation 
  // Waveform generation - toggle
  TCCR1A |= (1 << COM1A0);
  // ICNC1 = 0; deactivate noise canceler 
  // ICES1 = 1; riggered on rising edge
  // WGM13, WGM12 = 0; normal operation
  // CS12, CS11, CS10 = 1; no prescaling clk/1
  // Mod CTC -> TOP = OCR1A
  TCCR1B = (1 << ICES1) | (1 << WGM12);  
  // null input capture flag
  TIFR = (1 << ICF1);
  // input capture interrupt routine enable
  TIMSK = (1 << TICIE1);
  // foc1A = 1kHz:
  // ---------------------------------------------
  //   OCR1A = [fclk/(2.N.foc1A)] - 1
  //    fclk = 16 Mhz
  //       N = 1
  //   foc1A = 1/Toc1A
  OCR1A = 10299;
  // timer start
  TIMER1A_START(PRESCALER_1024); 
}

/**
 * @description Main function
 *
 * @param  Void
 * @return Void
 */
int main (void)
{
  // duty cycle
  //uint16_t duty;
  // string
  char str[20];

  // spi init
  SpiInit();
  // display init
  St7735Init();
  // init timer 1A
  Timer1AInit();
  
  // clear screen
  ClearScreen(0x0000);
  // set text on position
  SetPosition(40, 20);
  // draw string
  DrawString("TIMER/COUNTER", 0xffff, X2);
  // update screen
  UpdateScreen();
  // delay
  _delay_ms(1000);
  // enable global interrupts
  sei();
 
  while (1) {
    // calc duty cycle
    //duty = (times[1]-times[0])/(times[2]-times[0]);

    if (cycles > 2) {
      // disable global interrupts
      cli();
      // set text on position
      SetPosition(10, 40);
      // to string
      itoa(times[0], str, 10);
      // draw string
      DrawString(str, 0xffff, X1);
      // draw string
      DrawString(" ", 0xffff, X1);
      // to string
      itoa(times[1], str, 10);
      // draw string
      DrawString(str, 0xffff, X1);
      // draw string
      DrawString(" ", 0xffff, X1);
      // to string
      itoa(times[2], str, 10);
      // draw string
      DrawString(str, 0xffff, X1);
      // set text on position
      SetPosition(10, 60);
      // to string
      itoa(times[2]-times[1], str, 10);
      // draw string
      DrawString(str, 0xffff, X1);
      // draw string
      DrawString(" ", 0xffff, X1);
      // to string
      itoa(times[1]-times[0], str, 10);
      // draw string
      DrawString(str, 0xffff, X1);
      // break
      break;
    }
  }
  // update screen
  UpdateScreen();

  // return value
  return 0;
}
