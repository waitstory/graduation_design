#include "LCD1602.h"

#define Judge() ((RxStr[0] == 'S') && (RxStr[1] == 'Y') && (RxStr[2] == 'Y') && (RxStr[12] == 'H'))

sbit M0 = P2^7;     //SX1278无线UART模块工作方式选择引脚
sbit M1 = P2^6; 

bit flag = 0;      //数据接收完成
bit flagRec = 0;   //开始接收数据标志

uchar RxStr[13]={0};
uchar dat =0;        //数据保存位

void InitUART(uint baud);    //串口初始化函数
void Delay_ms(uint x);       //毫秒延时函数
void InitDisplay();          //上电LCD1602初始化显示
void DisplayData();          //显示温湿度和光照强度数据

void main()
{   
    M0 = 0; //SX1278无线UART模块配置为模式0
    M1 = 0;
    InitLcd1602();   //LCD1602液晶屏初始化
    InitDisplay();   //初始化LCD1602显示内容
    InitUART(9600);  //串口初始化

    while(1)
    {
        if(flag == 1)
        {
            flag = 0;
            if(Judge())   //判断接收到的数据是否正确
            {
                DisplayData();      //接收温湿度数据显示  
            }
        } 
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
    TR1 = 1;       //启动T1计数
}

void InitDisplay()
{
    LcdShowStr(2,0,"Welcome USE!");    
    LcdShowStr(1,1,"SYSTEM Running!");
    Delay_ms(1500);  
    LcdFullClear();     //清除LCD1602全屏显示内容
	LcdShowStr(0,0,"Te: 0");  
	LcdShowChr(5,0,0xdf);
	LcdShowChr(6,0,'C');
	LcdShowStr(8,0,"hu: 0");
	LcdShowStr(13,0,"%RH");
    LcdShowStr(0,1,"illu:    0");
	LcdShowStr(11,1,"lx");
}

void DisplayData()
{
	LcdShowChr(3,0,RxStr[3]);    //显示温度的十位
    LcdShowChr(4,0,RxStr[4]);    //显示温度的个位
    LcdShowChr(11,0,RxStr[5]);   //显示湿度的十位
    LcdShowChr(12,0,RxStr[6]);   //显示湿度的个位

    LcdShowChr(5,1,RxStr[7]);    //显示光照强度的万位
    LcdShowChr(6,1,RxStr[8]);    //显示光照强度的千位
    LcdShowChr(7,1,RxStr[9]);    //显示光照强度的百位
    LcdShowChr(8,1,RxStr[10]);    //显示光照强度的十位
    LcdShowChr(9,1,RxStr[11]);    //显示光照强度的个位
}

/*延时函数，延时单位为毫秒*/
void Delay_ms(uint x)
{
   uint i,j;
 
   for(i=x;i>0;i--)
      for(j=110;j>0;j--);
}


/* UART中断服务函数 */
void InterruptUART() interrupt 4
{
    static int i = 0;    

    if(RI)
    {
        RI = 0;
        dat = SBUF;
        if((dat == 'S') && (i == 0))
        {
            RxStr[i] = dat;
            flagRec = 1;   //开始接收数据
        }
        else
        {
            if(flagRec == 1)
            {
                i++;
                RxStr[i]=dat;
                if(i >= 12)
                {
                    i = 0; 
                    flagRec = 0;   //停止接收数据
                    flag = 1;      //数据接收完成
                }
            }
        }
        //SUBF = dat;  //将接收到的数据通过串口发送到电脑打印出来显示，方便调试
    }
}


