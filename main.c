#include <avr/interrupt.h>
#include <avr/io.h>

/**
 * @description Main function
 *
 * @param  Void
 * @return Void
 */
int main (void)
{
  // need 3 times for determining duty cycle
  //    ______          ______
  // __|@@@@@@|________|@@@@@@|_______
  //   |      |        |
  //   t1     t2      t3
  uint8_t cycles = 3;
  // start with falling edge
  uint8_t falling = 1;
  // storage values
  uint16_t values[3];
  
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
  // 3 cycles loop to grab duty cycle
  while (cycles) {
    // check if input capture flag is set
    if (TIFR & (1 << ICF1)) {
      // detect rising edge
      if (falling == 0) {
        // write low byte
        values[cycles-1] = (0x00ff & ICR1L);
        // write high byte
        values[cycles-1] |= (ICR1H << 8);
        // falling edge trigger
        TCCR1B &= ~(1 << ICES1);
        // null input capture flag
        TIFR = (1 << ICF1);        
        // detect falling edge
        falling = 1;
        // decrement cycle
        cycles--;
      } else {
        // write low byte
        values[cycles-1] = (0x00ff & ICR1L);
        // write high byte
        values[cycles-1] |= (ICR1H << 8);
        // falling edge trigger
        TCCR1B |= (1 << ICES1);
        // null input capture flag
        TIFR = (1 << ICF1);        
        // detect falling edge
        falling = 1;
        // decrement cycle
        cycles--;      
      }
    }
  }
  // calc duty cycle
  uint16_t duty = (values[1]-values[0])/(values[2]-values[0])
  
  // return value
  return 0;
}
