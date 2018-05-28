#ifndef _EEPROM_H_
#define _EEPROM_H_

#include <intrins.h>
#include <reg52.h>


/********STC89C52�����ֲ�*******
��һ������2000H--21FF
�ڶ�������2200H--23FF
����������2400H--25FF
����������2600H--27FF
����������2800H--29FF
����������2A00H--2BFF
����������2C00H--2DFF
�ڰ�������2E00H--2FFF
*******************************/

#define RdCommand 0x01     //�ֽڱ����������   
#define PrgCommand 0x02    //�ֽڶ���������   
#define EraseCommand 0x03  //����������������

#define Error 1
#define Ok 0
#define WaitTime 0x01 //����CPU�ĵȴ�ʱ��,40M����Ϊ0,20M����Ϊ1,10M����Ϊ2,5M����Ϊ3

/*****ISP/IAP���⹦�ܼĴ�������********/    
sfr ISP_DATA = 0xE2;
sfr ISP_ADDRH = 0xE3;
sfr ISP_ADDRL = 0xE4;
sfr ISP_CMD = 0xE5;
sfr ISP_TRIG = 0xE6;
sfr ISP_CONTR = 0xE7;


unsigned char byte_read(unsigned int byte_addr);
void byte_write(unsigned int byte_addr,unsigned char Orig_data);
void SectorErase(unsigned int sector_addr);

#endif

