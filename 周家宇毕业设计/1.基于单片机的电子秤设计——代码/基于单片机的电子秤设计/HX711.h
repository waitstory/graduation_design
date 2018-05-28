#ifndef __HX711_H__
#define __HX711_H__


#include <reg52.h>
#include <intrins.h>

//IOÉèÖÃ
sbit HX711_DOUT=P1^4; 
sbit HX711_SCK=P1^5; 

//º¯ÊıÉùÃ÷
unsigned long HX711_Read(void);

#endif