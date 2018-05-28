#include "Delay.h"

void Delay_us(uchar n)
{
    while(--n);
}

void Delay_ms(uint z)
{
   uint i,j;

   for(i=z;i>0;i--)
      for(j=110;j>0;j--);
}