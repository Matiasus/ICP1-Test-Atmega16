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
  //  WGM11,  WGM10 = 0; nomal operation 
  TCCR1A = 0x00;
  // COM1A1, COM1A0 = 0; OC1A disconnected
  // COM1B1, COM1B0 = 0; OC1B disconnected
  //  FOC1A,  FOC1B = 0; 
  //  WGM11,  WGM10 = 0; nomal operation 
  TCCR1A = 0;
}
