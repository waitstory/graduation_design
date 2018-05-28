#ifndef __HX711_H__
#define __HX711_H__


#include <reg52.h>
#include <intrins.h>

//IO����
sbit HX711_DOUT=P1^4; 
sbit HX711_SCK=P1^5; 

//��������
unsigned long HX711_Read(void);

#endif