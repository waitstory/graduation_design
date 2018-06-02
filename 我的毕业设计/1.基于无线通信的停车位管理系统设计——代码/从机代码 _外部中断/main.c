#include "LCD1602.h"

sbit Infrad = P3^2;
sbit Infrad1 = P3^3;

int Count = 0;
uchar TxStr[6]={0}; //接收数据缓冲区
bit flag1 = 0;
bit flag2 = 0;

void InitUART(uint baud);   //串口配置函数
void InitEX();
void ConverData();          //车位信息数据转换函数
void UartTXData(uchar str[]);//串口发送字符串函数
void Delay_ms(uint xms);     //毫秒延时函数

void main()
{   
    PX0 = 1;     //外部中断0中断优先级控制位
    PX1 = 1;     //外部中断1中断优先级控制位
 
    InitLcd1602();  //LCD1602初始化
    InitEX();       //外部中断初始化
    InitUART(9600);    //串口中断初始化

    while(1)
    {   
        if(flag1 == 1)
        {
            flag1 = 0;
            Delay_ms(5);
            if(Infrad == 0)
            {
                Count++; 
                if(Count>=99)
                {
                    Count = 99;
                } 
                ConverData();     
                UartTXData(TxStr);   //将数据帧发送出去      
                LcdShowChr(0,0,Count/10+'0');
                LcdShowChr(1,0,Count%10+'0');
            } 
        }

        if(flag2 == 1)
        {
            flag2 = 0;
            Delay_ms(5);
            if(Infrad1 == 0)
            {
                Count--; 
//                EX1 = 0;
                if(Count <=0)
                {
                    Count = 0;
                }
                ConverData(); 
                UartTXData(TxStr);   //将数据帧发送出去
                LcdShowChr(0,0,Count/10+'0');
                LcdShowChr(1,0,Count%10+'0');
//                EX1 = 1; 
            } 
        }    
    }   
}
         
void InitEX()
{
    IT0=1;       //下降沿触发
    EX0=1;       //外部中断0使能

    IT1=1;       //下降沿触发
    EX1=1;       //外部中断0使能
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

/*将温湿度数据根据通信协议构成一帧完整的数据放入发送缓冲区中*/
void ConverData()
{
    TxStr[0] = 'S';
    TxStr[1] = 'Y';
    TxStr[2] = Count/10+'0';
    TxStr[3] = Count%10+'0';
    TxStr[4] = 'H';
}

/*******INT0中断函数*********/
void counter0(void) interrupt 0  
{
    flag1 = 1;
}

/*******INT1中断函数*********/
void counter1(void) interrupt 2 
{
    flag2 = 1;
}

void Delay_ms(uint x)
{
   uint i,j;
 
   for(i=x;i>0;i--)
      for(j=110;j>0;j--);
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


