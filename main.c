#include <avr/interrupt.h>
#include <avr/io.h>

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
  if (cycles < 2) {
    // detect rising edge
    if (falling == 0) {
      // write low byte
      times[cycles] = (0x00ff & ICR1L);
      // write high byte
      times[cycles] |= (ICR1H << 8);
      // falling edge trigger
      TCCR1B &= ~(1 << ICES1);     
      // detect falling edge
      falling = 1;
      // decrement cycle
      cycles++;
    } else {
      // write low byte
      values[cycles] = (0x00ff & ICR1L);
      // write high byte
      values[cycles] |= (ICR1H << 8);
      // falling edge trigger
      TCCR1B |= (1 << ICES1);   
      // detect falling edge
      falling = 1;
      // decrement cycle
      cycles++;      
    }
  }
}

/**
 * @description Main function
 *
 * @param  Void
 * @return Void
 */
int main (void)
{
  // COM1A1, COM1A0 = 0; OC1A disconnected
  // COM1B1, COM1B0 = 0; OC1B disconnected
  //  FOC1A,  FOC1B = 0; 
  //  WGM11,  WGM10 = 0; normal operation 
  TCCR1A = 0x00;
  // ICNC1 = 0; deactivate noise canceler 
  // ICES1 = 1; riggered on rising edge
  // WGM13, WGM12 = 0; normal operation
  // CS12, CS11, CS10 = 1; no prescaling clk/1
  TCCR1B = (1 << CS01);
  // null input capture flag
  TIFR = (1 << ICF1);
  // input capture interrupt routine enable
  TIMSK = (1 << TICIE1);
  
  // return value
  return 0;
}
