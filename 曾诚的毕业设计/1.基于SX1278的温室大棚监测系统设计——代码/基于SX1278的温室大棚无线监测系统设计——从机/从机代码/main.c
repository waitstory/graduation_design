#include "DHT11.h"
#include "BH1750.h"

sbit M0 = P2^7;      //SX1278无线UART模块工作方式选择引脚
sbit M1 = P2^6;                                                                                                 

uchar count = 0;
uchar TxStr[13]={0};    //发送温湿度和光照强度数据缓冲区

void InitUART(uint baud);   //串口初始化函数
void UartTXData(uchar str[]);   //温湿度和光照强度数据发送函数
void ConverData();    //温湿度和光照强度数据转换函数

void main()
{

    M0 = 0; //SX1278无线UART模块配置为模式0
    M1 = 0;
    InitUART(9600);   //串口初始化

    while(1)
    {  
        DHT11Receive();    //获取温湿度数据
        BH1750Read();      //获取光照数据
        ConverData();      //将温湿度和光照强度数据根据通信协议构成一帧完整的数据帧放入发送缓冲区中
        UartTXData(TxStr);   //将数据帧发送出去
    }
}              
    
/* 串口配置函数，baud-通信波特率 */
void InitUART(uint baud)
{
    EA = 1;        //使能中断总开关
    SCON  = 0x50;  //配置串口为模式1
    TMOD &= 0x0F;  //清零T1的控制位
    TMOD |= 0x20;  //配置T1为模式2
    TH1 = 256 - (11059200/12/32)/baud;  //计算T1重载值
    TL1 = TH1;     //初值等于重载值
    ET1 = 0;       //禁止T1中断
    ES  = 1;       //使能串口中断
    TR1 = 1;       //启动T1
}  
                    

/*将温湿度和光照强度数据根据通信协议构成一帧完整的数据放入发送缓冲区中*/
void ConverData()
{
    TxStr[0] = 'S';
    TxStr[1] = 'Y';
    TxStr[2] = 'Y';
    TxStr[3] = temp[0]+'0';
    TxStr[4] = temp[1]+'0';
    TxStr[5] = humi[0]+'0';
    TxStr[6] = humi[1]+'0';
    TxStr[7] = wan+'0';
    TxStr[8] = qian+'0';
    TxStr[9] = bai+'0';
    TxStr[10] = shi+'0';
    TxStr[11] = ge+'0';
    TxStr[12] = 'H';
}

/*串口发送字符串函数*/	
void UartTXData(uchar str[])
{   
    uchar i = 0;
    
    while(str[i] != '\0')
    {    
        SBUF= str[i];
        i++;
        while(!TI);
        TI=0;
    }
}


