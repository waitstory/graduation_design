/**
  ******************************************************************************
  * @file    main.c
  * @author  waitstory
  * @version V1.0
  * @date    2017-12-21
  * @brief   无线温湿度监控主机数据接收，LCD1602的显示方式进行了改变，由静态显示改变成了字符移动显示
  ******************************************************************************
  * @attention
  *
  * 实验平台:STC89C52RC芯片，晶振11.0592MHz
  *
  ******************************************************************************
**/

#include "LCD1602.h"

#define Thre 20 //温度阈值
 
sbit M0 = P2^7;    //SX1278无线UART模块工作方式选择引脚
sbit M1 = P2^6; 
sbit BEEP = P1^3;   //蜂鸣器控制引脚
 
bit flag = 96;   //数据接收完成
bit flagRec = 0;   //开始接收数据标志
bit flag800ms = 0; //500ms 定时标志

uchar code str[] = "SYSTEM Running!";
uchar RxStr[8]={0};
uchar dat =0;   //数据保存位
uchar TemptSum = 0;  //温度整数值
void ConfigUART(uint baud);
void ConfigTimer0();
void Delay_ms(uint x);
void InitDisplay();
void DisplayData();

void main()
{
    uchar i;
    uchar index = 0; //移动索引
    uchar pdata bufMove[16+sizeof(str)+16]; //移动显示缓冲区 

    for (i=0; i<16; i++)
    {
        bufMove[i] = ' ';
    }
    //待显示字符串拷贝到缓冲区中间位置
    for (i=0; i<(sizeof(str)-1); i++)
    {
        bufMove[16+i] = str[i];
    }
    //缓冲区结尾一段也填充为空格
    for (i=(16+sizeof(str)-1); i<sizeof(bufMove); i++)
    {
        bufMove[i] = ' ';
    }
         
    M0 = 0;      //SX1278无线UART模块配置为模式0
    M1 = 0;
    IP = 0x10;       //设置串口中断优先级最高
    EA = 1;        //使能中断总开关
    InitLcd1602();   //LCD1602液晶屏初始化
    InitDisplay();   //初始化LCD1602显示内容
    ConfigUART(9600);  //串口初始化
    ConfigTimer0();    //定时器0初始化

    while(1)
    {
        if (flag800ms) //每500ms移动一次屏幕
        {
            flag800ms = 0;
            //从缓冲区抽出需显示的一段字符显示到液晶上
            LcdShowStrLen(0, 0, bufMove+index, 16);
            
            //移动索引递增，实现左移
            index++;
            if (index >= (16+sizeof(str)-1))
            { //起始位置达到字符串尾部后即返回从头开始
                index = 0;
            }
        }

        if(flag == 1)
        {
			flag = 0;
            if((RxStr[0] == 'S') && (RxStr[1] == 'Y') && (RxStr[2] == 'Y') && (RxStr[7] == 'H'))
            {
                DisplayData();      //接收温湿度数据显示        
                TemptSum = (RxStr[3]-'0')*10+(RxStr[4]-'0');  //温度整数值
                if(TemptSum >= Thre)      //接收的温度数据超过阈值
                {
                    TemptSum = 0;
                    BEEP = 0;
                    Delay_ms(200);
                    BEEP = 1;
                }
            }
        } 
    }
}

/* 串口配置函数，baud-通信波特率 */
void ConfigUART(uint baud)
{
    SCON  = 0x50;  //配置串口为模式1
    TMOD &= 0x0F;  //清零T1的控制位
    TMOD |= 0x20;  //配置T1为模式2
    TH1 = 256 - (11059200/12/32)/baud;  //计算T1重载值
    TL1 = TH1;     //初值等于重载值
    ET1 = 0;       //禁止T1中断
    ES  = 1;       //使能串口中断
    TR1 = 1;       //启动T1
}

/* 配置并启动T0*/
void ConfigTimer0()
{
    TMOD &= 0xF0; //清零 T0 的控制位
    TMOD |= 0x01; //配置 T0 为模式 1
    TH0 = 0xDC; //加载 T0 重载值
    TL0 = 0x00;
    ET0 = 1; //使能 T0 中断
    TR0 = 1; //启动 T0
}


void InitDisplay()
{
	
    LcdShowStr(2,0,"Welcome USE!");    
    LcdShowStr(1,1,"SYSTEM Running!");
    Delay_ms(1500);  
    LcdFullClear();     //清除LCD1602全屏显示内容
//    LcdShowStr(1,0,"SYSTEM Running");
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
	LcdShowChr(3,1,RxStr[3]);    //显示温度数据
    LcdShowChr(4,1,RxStr[4]);
    LcdShowChr(11,1,RxStr[5]);   //显示湿度数据
    LcdShowChr(12,1,RxStr[6]);   
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
            flagRec = 1;
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
                    flagRec = 0;
                    flag = 1;
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


void InterruptTimer0() interrupt 1
{
    static unsigned char tmr800ms = 0;

    TH0 = 0xDC; //重新加载T0重载值
    TL0 = 0x00;
    
    tmr800ms++;

    if (tmr800ms >= 80)
    {
        tmr800ms = 0;
        flag800ms = 1;
    }
}

