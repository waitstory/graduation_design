#ifndef __DHT11_H
#define __DHT11_H

#include <reg52.h>
#include "Delay.h"

sbit Data = P1^0;   //�������ݹܽ�

extern uchar temp[2]; 	  //���ڴ���¶�
extern uchar humi[2];	  //���ڴ��ʪ��

void DHT11Receive();

#endif