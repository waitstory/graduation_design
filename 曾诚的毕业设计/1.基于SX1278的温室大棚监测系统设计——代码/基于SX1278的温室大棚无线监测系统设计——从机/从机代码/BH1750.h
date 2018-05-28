#ifndef __BH1750_H
#define __BH1750_H

#include <reg52.h>
#include "Delay.h"

#define BH1750_Addr 0x46
#define BH1750_ON   0x01
#define BH1750_CON  0x10
#define BH1750_ONE  0x20
#define BH1750_RSET 0x07    

extern uchar ge,shi,bai,qian,wan;   
   
void BH1750Read();   //连续的读取内部寄存器数据

#endif


