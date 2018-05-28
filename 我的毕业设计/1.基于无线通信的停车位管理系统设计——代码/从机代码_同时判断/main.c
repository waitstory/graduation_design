#include "LCD1602.h"

sbit Infrad1 = P2^0;
sbit Infrad2 = P2^1;
sbit BEEP = P1^7;

uchar const *current="Current:";       //当前值
uchar const *surplus="Surplus:";       //剩余值

uchar TxStr[6]={0}; //接收数据缓冲区

bit end;          //定时1秒标志
bit enter=0;      //进入标志位
bit out=0;        //出去标志位
uchar count=0;    //定时累加变量
int num=0;    //进出计数变量
int num2=99;  //计数器容量
int num3;     //剩余值
  
void ConfigTimer0();         //定时器T0配置
void Delay_ms(uint xms);     //延时函数，延时单位为毫秒
void Judge();                //计数判断函数
void Beep();                 //蜂鸣器报警函数
void Display();              //LCD1602显示函数,将进出数据显示在LCD1602上
void UartTXData(uchar str[]);
void InitUART(uint baud);
void ConverData();



void main()  
{   
    InitUART(9600);
    ConfigTimer0();     //定时器T0配置                               
    InitLcd1602();      //LCD1602初始化
   
    while(1)
    {   
        Judge();  
        ConverData();
        UartTXData(TxStr);
        Display();
    }
}

void ConfigTimer0()
{
    TMOD &= 0xF0; //清零 T0 的控制位
    TMOD |= 0x01; //配置 T0 为模式 1
    TH0 =  0x4C;          //定时50ms，晶振11.0592M
    TL0 =  0x00;          //使能T0中断
    ET0 = 1;        
}

void Display()
{
    LcdShowStr(1,0,surplus);         //第一行显示剩余值
    LcdShowChr(9,0, num3/10+'0');    //显示剩余值数据的十位
    LcdShowChr(10,0, num3%10+'0');   //显示剩余值数据的个位

    LcdShowStr(1,1,current);       //第二行显示当前值
    LcdShowChr(9,1,num/10+'0');    //显示当前值数据的十位
    LcdShowChr(10,1,num%10+'0');   //显示当前值数据的个位

}

void Beep()  
{
    BEEP = 0;
    Delay_ms(20);
    BEEP = 1;
}

void Judge()           
{
	 if(!Infrad2)
	 {		
        if(!Infrad2)
        {
            out=1;
            TR0 = 1;      //开启定时器0	
        }
        while(out==1)
        {
            if(end==1)         //1s内没有检测到人出去
			{ 
                end=0;
				TR0=0;
				count=0;
				out=0;
				break;
            } 
            if(!Infrad1)
            {
                 while(!Infrad1);
                 num--;  
                 Beep();
                 end=0;
                 TR0=0;
                 count=0;
                 enter=0;
                 out=0;
                 break;
            } 
        }
    }
    if(!Infrad1)
	{
		
        if(!Infrad1)
        {
            enter=1;	 
            TR0 = 1;      //开启定时器0	   
        }
        while(enter==1) 
        {
            if(end==1)         //1s内没有检测到人真正进入教室
            { 
                end=0;
                TR0=0;
                count=0;
                enter=0;
                break;
            }
            if(!Infrad2)
            {
                while(!Infrad2);
                num++;
                Beep();
                end=0;
                TR0=0;
                count=0;
                enter=0;
                break;
            } 
        }
    }
	if(num<=0)
	{
		num=0;
	}
	if(num>num2)
	{
        num=num2;
    }
	num3=num2-num;
 
}


void Delay_ms(uint xms)
{
    uint i,j;
 
    for(i=xms;i>0;i--)
       for(j=110;j>0;j--);
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
    TxStr[2] = num/10+'0';
    TxStr[3] = num%10+'0';
    TxStr[4] = 'H';
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

void InterruptTimer0() interrupt 1         //定时器0中断,50ms中断一次
{
    TH0 =  0x4C;          //定时50ms，晶振11.0592M
    TL0 =  0x00;                
    count++;

    if (count>=100)            //计数大于等20清零，定时1秒
    {
        count = 0;              
        end=1;
    } 
}