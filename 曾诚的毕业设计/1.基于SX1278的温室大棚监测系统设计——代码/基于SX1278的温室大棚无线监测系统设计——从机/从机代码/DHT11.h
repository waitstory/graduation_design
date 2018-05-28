#ifndef __DHT11_H
#define __DHT11_H

#include <reg52.h>
#include "Delay.h"

sbit Data = P1^0;   //定义数据管脚

extern uchar temp[2]; 	  //用于存放温度
extern uchar humi[2];	  //用于存放湿度

void DHT11Receive();

#endif