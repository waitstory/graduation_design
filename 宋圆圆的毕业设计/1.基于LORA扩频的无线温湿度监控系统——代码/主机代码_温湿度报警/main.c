/**
  ******************************************************************************
  * @file    main.c
  * @author  waitstory
  * @version V1.0
  * @date    2017-12-21
  * @brief   无线温湿度监控主机数据接收
  ******************************************************************************
  * @attention
  *
  * 实验平台:STC89C52RC芯片，晶振11.0592MHz
  *
  ******************************************************************************
**/

#include "LCD1602.h"

#define Thre 20  //温度阈值
#define Humi 75  //湿度阈值
 
sbit M0 = P2^7;    //SX1278无线UART模块工作方式选择引脚
sbit M1 = P2^6; 
sbit BEEP = P1^3;   //蜂鸣器控制引脚
sbit LED1 = P1^4;
sbit LED2 = P1^5;

bit flag = 0;   //数据接收完成
bit flagRec = 0;   //开始接收数据标志

uchar RxStr[8]={0};
uchar dat =0;   //数据保存位
uchar TemptSum = 0;  //温度值
uchar HumiSum = 0;  //湿度值

void InitUART(uint baud);
void Delay_ms(uint x);
void InitDisplay();
void DisplayData();

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
            if((RxStr[0] == 'S') && (RxStr[1] == 'Y') && (RxStr[2] == 'Y') && (RxStr[7] == 'H'))
            {
                DisplayData();      //接收温湿度数据显示        
                TemptSum = (RxStr[3]-'0')*10+(RxStr[4]-'0');  //温度值
                HumiSum =  (RxStr[5]-'0')*10+(RxStr[6]-'0');  //湿度值
                if((TemptSum >= Thre) || (Humi >= HumiSum))      //判断接收的温度或湿度数据是否超过阈值
                {
                    TemptSum = 0;
                    HumiSum = 0;
                    BEEP = 0;
                    LED1 = 0;
                    LED2 = 1;
                    Delay_ms(200);
                    LED1 = 1;
                    LED2 = 0;
                    BEEP = 1;
                }
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
    LcdShowStr(1,0,"SYSTEM Running");
	LcdShowStr(0,1,"Te: 0");
	LcdShowChr(5,1,0xdf);
	LcdShowChr(6,1,'C');
	LcdShowStr(8,1,"hu: 0");
	LcdShowChr(13,1,'%');
	LcdShowChr(14,1,'R');
	LcdShowChr(15,1,'H');
}

void DisplayData()
{
	LcdShowChr(3,1,RxStr[3]);    //显示温度的十位
    LcdShowChr(4,1,RxStr[4]);    //显示温度的个位
    LcdShowChr(11,1,RxStr[5]);   //显示湿度的十位
    LcdShowChr(12,1,RxStr[6]);   //显示湿度的个位
    
/*    //如果温度的数据十位为0，十位就不显示
    if(RxStr[3] == '0')
    {
        LcdShowChr(3,1,' ');    //显示温度的十位
    }
    else
    {
        LcdShowChr(3,1,RxStr[3]);   //显示温度的十位
    }
    LcdShowChr(4,1,RxStr[4]);
    
    //如果湿度的数据十位为0，十位就不显示
    if(RxStr[5] == '0')
    {
        LcdShowChr(11,1,' ');    //显示湿度的十位
    }
    else
    {
        LcdShowChr(11,1,RxStr[5]);   //显示湿度的十位
    }
    LcdShowChr(12,1,RxStr[6]);   //显示湿度的个位
*/   
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
                if(i >= 7)
                {
                    i = 0; 
                    flagRec = 0;   //停止接收数据
                    flag = 1;      //数据接收完成
                }
            }
        }
        //SUBF = dat;  //将接收到的数据通过串口发送到电脑打印出来显示，方便调试
    }

     /*if (RI)  //接收到字节
    {
        RI = 0;  //清零接收中断标志位
        //*buf++ = SBUF;  //接收到的数据保存buf缓冲区中
        RxStr[i] = SBUF;
        i++;
        if(i >=4 )
        {
            i = 0;
            flag = 1;
			//*buf = RxStr;
//            ES = 0;
        }
    }*/
    
}

