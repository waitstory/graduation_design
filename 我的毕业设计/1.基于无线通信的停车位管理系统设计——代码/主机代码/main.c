#include "LCD1602.h"
#include "EEPROM.h"
#define Judge() ((RxStr[0] == 'S') && (RxStr[1] == 'Y')&& (RxStr[4] == 'H'))

sbit M0 = P2^7;    //SX1278无线UART模块工作方式选择引脚
sbit M1 = P2^6; 

sbit BEEP = P1^3;
sbit LED = P1^4;
sbit KEY1 = P2^2;
sbit KEY2 = P2^1;
sbit KEY3 = P2^0;

bit flag = 0;       //数据接收完成
bit flagRec = 0;    //开始接收数据标志
bit flag_read = 0;  //将总车位数写入单片机内部EEPROM中标志
uchar dat =0;       //数据保存位
uchar RxStr[5]={0}; //接收数据缓冲区
uchar All = 99;     //总车位数
uchar Count = 0;    //已占用车位
uchar state = 0;    //按键选择模式
bit flag250ms = 0;  //定时250ms标志位

void Write_eeprom();    //将数据写入到单片机内部EEPROM中
void Read_eeprom();     //从单片机内部EEPROM中读取数据函数
void InitEEPROM();     //上电初始化保存总车位数到单片机内部EEPROM中
void InitUART(uint baud);  //串口初始化函数
void ConfigTimer0();    //配置定时器0函数
void Delay_ms(uint x);  //毫秒延时函数
void InitDisplay();     //初始化显示LCD1602函数
void ConvertData();     //将接收的数据转换成整数
void KeyScan();         //按键扫描函数
void Show();            //显示数据到LCD1602上


void main()
{
    M0 = 0; //SX1278无线UART模块配置为模式0
    M1 = 0;
    
    InitLcd1602();   //LCD1602液晶屏初始化
    InitEEPROM();   //初始化单片机内部EEPROM数据
    InitDisplay();   //初始化LCD1602显示内容
    InitUART(9600);  //串口初始化
    ConfigTimer0();  //配置定时器T0
//    P2 = 0xF7;       //P2.3 置 0，即 KeyOut1 输出低电平
        
    while(1)
    {  
        if(flag == 1)
        {
            flag = 0;
            if(Judge())
            {
//                LcdShowStr(10,0,RxStr);   
                ConvertData();       
            }
        } 
        Show();
        if((All-Count) == 5)
        {
            BEEP = 0;
            LED = 0;
//            Delay_ms(30);
//            BEEP = 1;
//           Delay_ms(30);
        }
        else
        {
             BEEP = 1; 
             LED = 1;
        }
        KeyScan();    
    }
}

/* 配置并启动T0,定时50ms*/
void ConfigTimer0()
{
    TMOD &= 0xF0; //清零 T0 的控制位
    TMOD |= 0x01; //配置 T0 为模式 1
    TH0 = 0x4C; //加载T0重载值
    TL0 = 0x00;
    ET0 = 1; //使能 T0 中断
    TR0 = 1; //启动 T0
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

/*把数据保存到单片机内部eeprom中*/
void Write_eeprom()
{
	SectorErase(0x2000);      
	byte_write(0x2000,99);
	byte_write(0x2060, flag_read);	
}

/*把数据从单片机内部eeprom中读出来*/
void Read_eeprom()
{
	All = byte_read(0x2000);
	flag_read  = byte_read(0x2060);
}

/*开机自检eeprom初始化*/
void InitEEPROM() 
{
	Read_eeprom();		    //先读
   
	if(flag_read != 1)		//新的单片机初始单片机内问EEPROM
	{
		flag_read = 1;  
		Write_eeprom();	    //保存数据 
	}	
    
}

void InitDisplay()
{
	
    LcdShowStr(2,0,"Welcome USE!");    
    LcdShowStr(1,1,"SYSTEM Running!");
    Delay_ms(1500);  
    LcdFullClear();     //清除LCD1602全屏显示内容
//	LcdShowStr(0,0,"All:");
//    LcdShowChr(4,0,All/10+'0');   
//    LcdShowChr(5,0,All%10+'0');
//	LcdShowStr(0,1,"Sur:");
//    LcdShowChr(4,1,(All-Count)/10+'0');     //剩余车位
//    LcdShowChr(5,1,(All-Count)%10+'0');
//    LcdShowStr(8,1,"Cur:");
//    LcdShowChr(12,1,Count/10+'0');     //占用车位
//    LcdShowChr(13,1,Count%10+'0');
	
}

void ConvertData()
{    
    Count = (RxStr[2]-'0')*10 + (RxStr[3]-'0');
//    All = byte_read(0x2000);
//    LcdShowStr(0,0,"All:");
//    LcdShowChr(4,0,All/10+'0');   
//    LcdShowChr(5,0,All%10+'0');
//	LcdShowStr(0,1,"Sur:");
//    LcdShowChr(4,1,(All-Count)/10+'0');     //剩余车位
//    LcdShowChr(5,1,(All-Count)%10+'0');
//    LcdShowStr(8,1,"Cur:");
//    LcdShowChr(12,1,Count/10+'0');     //剩余车位
//    LcdShowChr(13,1,Count%10+'0');
}

/*按键扫描函数*/
void KeyScan()
{
    if(KEY1 == 0)
    {
        Delay_ms(5);
    
        if(KEY1 == 0)
        {   
            Show();
            state = (state+1)%2;  
            while(!KEY1);  
        }   
             
    }

    if(KEY2 == 0)        //总车位数加
    {
        Delay_ms(5);     //消抖
    
        if(KEY2 == 0)
        {
            if(state == 1)
            {    
                Show();
                if(All < 99)
                {
                    All = All+ 1;
                    SectorErase(0x2000);	 
                    byte_write(0x2000,All);
                }
            }
            while(!KEY2);
        }      
    }
  
  
    if(KEY3 == 0)     //总车位数减
    {
        Delay_ms(5);  //消抖
    
        if(KEY3 == 0)
        {   
            Show();
            if(state == 1)
            {
                if(All > Count+1) 
                {
                    All = All - 1;
                    SectorErase(0x2000);	 
                    byte_write(0x2000,All);
                }
            }
            while(!KEY3);
        }    
    }
   
}

/*总车位数通过按键调整，显示在LCD1602上*/
    void Show()
{
    if(state == 0)
    {   
        LcdShowStr(4,0,"All:");
        LcdShowChr(8,0,All/10+'0');   
        LcdShowChr(9,0,All%10+'0');
        LcdShowStr(0,1,"Sur:");
        LcdShowChr(4,1,(All-Count)/10+'0');     //剩余车位
        LcdShowChr(5,1,(All-Count)%10+'0');
        LcdShowStr(8,1,"Cur:");
        LcdShowChr(12,1,Count/10+'0');     //已占车位
        LcdShowChr(13,1,Count%10+'0');  
//        LcdShowStr(10,0,RxStr);
    }
    else
    {   
        LcdShowStr(0,0,"    Adjust   ");
        LcdShowStr(0,1,"All:");
        LcdShowStr(6,1,"          ");

        if(flag250ms == 1)
        {
            LcdShowChr(4,1,All/10+'0');   
            LcdShowChr(5,1,All%10+'0');     
        } 
        else
        {
            LcdShowStr(4,1,"  ");
            LcdShowStr(5,1,"  ");
        }        
    }
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
                if(i >= 4)
                {
                    i = 0; 
                    flagRec = 0;   //停止接收数据
                    flag = 1;      //数据接收完成
                }
            }
        }
        SBUF = dat;  //将接收到的数据通过串口发送到电脑打印出来显示，方便调试
    }  
}

void InterruptTimer0() interrupt 1
{
    static unsigned char tmr250ms = 0;

    TH0 = 0x4C; //重新加载T0重载值
    TL0 = 0x00;
    
    tmr250ms++;

    if (tmr250ms%5==0)
    {
        flag250ms = !flag250ms;
    }
}
