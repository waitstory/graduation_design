#include "BH1750.h"
#include "I2C.h"
#include "Delay.h"

float lx=0;
int tmp=0;
uint result;
uchar BUF[2]={0};
uchar ge,shi,bai,qian,wan;      

void BH1750WriteCmd(uchar Cmd)
{
    I2CStart();               //起始信号
    I2CWrite(BH1750_Addr+0);   //发送设备地址+写信号
    I2CWrite(Cmd);             //内部寄存器地址
    I2CStop();                //发送停止信号
	Delay_ms(5);
}

void BH1750Start()
{
	BH1750WriteCmd(BH1750_ON);	  //power on
//	BH1750WriteCmd(BH1750_RSET);	//clear
	BH1750WriteCmd(BH1750_ONE);   //一次H分辨率模式，至少120ms，之后自动断电模式  
}

void BH1750Read()
{   
    BH1750Start();              //上电初始化BH1750
    Delay_ms(180);
    I2CStart();                 //起始信号
    I2CWrite(BH1750_Addr+1);    //发送设备地址+读信号
	BUF[0]=I2CReadACK();        //发送ACK
	BUF[1]=I2CReadNAK();        //发送NACK
    I2CStop();                  //停止信号
    Delay_ms(5);

    tmp=BUF[0];
	tmp=(tmp<<8)+BUF[1];  //合成数据，即光照数据
	
	lx=(float)tmp/1.2;
    
    result = (uint)lx;
    wan=result/10000;
	qian=result%10000/1000;
    bai=result%1000/100;
    shi=result%100/10;
    ge=result%10; 	
}
