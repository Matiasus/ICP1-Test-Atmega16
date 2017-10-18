#include <avr/interrupt.h>
#include <avr/io.h>

/**
 * @description Main function
 *
 * @param  Void
 * @return Void
 */
void main (void)
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
}
