#include <stdint.h>
#include <pic32mx.h>
#include "mipslab.h"

int getsw(void)
{
  //the thing in () looks at bits 11 through 8. den we take them back to the LSB with the >> 8 at the end
  return (PORTD & (0xf << 8)) >> 8;
}

int getbtns(void)
{
  //uses similar logic to method getsw
  return (PORTD & (0x7 << 5)) >> 5;
}
