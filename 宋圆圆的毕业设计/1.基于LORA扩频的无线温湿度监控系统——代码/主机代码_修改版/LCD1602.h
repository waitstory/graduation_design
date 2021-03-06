#ifndef __LCD1602_H
#define __LCD1602_H

#include <reg52.h>

typedef unsigned char uchar;
typedef unsigned int uint ;

#define LCD1602_DB  P0
sbit LCD1602_RS = P1^0;
sbit LCD1602_RW = P1^1;
sbit LCD1602_E  = P1^5;

void InitLcd1602();
void LcdFullClear();
void LcdShowChr(unsigned char x, unsigned char y, unsigned char chr);
void LcdShowStr(unsigned char x, unsigned char y, unsigned char *str);
void LcdShowStrLen(unsigned char x, unsigned char y,unsigned char *str, unsigned char len);
#endif