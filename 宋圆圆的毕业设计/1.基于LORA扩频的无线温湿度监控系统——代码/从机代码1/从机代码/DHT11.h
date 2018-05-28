#ifndef __DHT11_H
#define __DHT11_H

#include <reg52.h>

sbit Data = P2^7;   //�������ݹܽ�

typedef unsigned char uchar;
typedef unsigned int uint ;

extern uchar temp[2]; 	  //���ڴ���¶�
extern uchar humi[2];	  //���ڴ��ʪ��

void DHT11_delay_ms(uint z);
void DHT11_receive();

#endif