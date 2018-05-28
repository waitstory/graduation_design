#ifndef __DHT11_H
#define __DHT11_H

#include <reg52.h>

sbit Data = P2^7;   //定义数据管脚

typedef unsigned char uchar;
typedef unsigned int uint ;

extern uchar temp[2]; 	  //用于存放温度
extern uchar humi[2];	  //用于存放湿度

void DHT11_delay_ms(uint z);
void DHT11_receive();

#endif