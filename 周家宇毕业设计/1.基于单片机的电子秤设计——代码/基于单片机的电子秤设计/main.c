#include "LCD1602.h"
#include "EEPROM.h"
#include "HX711.h"

sbit LED = P2^7;
sbit Buzzer = P2^6; 
sbit KEY1 = P2^2;
sbit KEY2 = P2^1;
sbit KEY3 = P2^0;

//uint count = 0;

volatile bit flag = 0;		    //定时0.5s标志位

uchar flag_read;       //将传感器参数写入单片机内部EEPROM中标志
uint GapValue;     //传感器参数值
uint GapValue1;    //传感器参数缓存值
unsigned long Weight_Maopi_0 = 0;  //毛皮重量缓存
unsigned long Weight_Maopi = 0;    //稳定后毛皮重量缓存
long Weight_Shiwu = 0;     //物体重量缓存
unsigned int qupi=0;       //去皮重量缓存
unsigned char key_press_num=0;

void ConfigTimer0();    //定时器T0配置函数
void Write_eeprom();    //将数据写入到单片机内部EEPROM中
void Read_eeprom();     //从单片机内部EEPROM中读取数据函数
void Init_eeprom();     //上电初始化保存压力传感器参数到单片机内部EEPROM中
void Get_Maopi();       //获取毛皮重量函数
void Get_Weight();      //获取重量函数
void Delay_ms(uint x);  //延时毫秒函数
void KeyPress();        //按键扫描函数

void main()
{
    Init_eeprom();  //上电初始化保存压力传感器参数到单片机内部EEPROM中
    InitLcd1602();  //初始化LCD1602设置
    ConfigTimer0(); //配置定时器T0
   
    LcdShowStr(0,0," Welcome To Use ");
    LcdShowStr(0,1,"Electronic Scale");

    Get_Maopi(); 

    LcdShowStr(1,0,"SYSTEM Running");
    LcdShowStr(0,1," Weight: 0.000kg");

    while(1)
    {   
        if (flag == 1)      //每0.5秒称重一次
        {
            flag = 0;
            Get_Weight();   
        }	 
        KeyPress();
    }
}


/* 配置并启动T0,在12M晶振下定时50ms*/
void ConfigTimer0()
{
    EA = 1;       //使能中断总开关
    TMOD &= 0x3C; //清零T0的控制位
    TMOD |= 0xB0; //配置T0为模式1
    TH0 = 0x3C;   //加载T0重载值
    TL0 = 0xB0;
    ET0 = 1; //使能T0中断
    TR0 = 1; //启动T0
    
}

/*把数据保存到单片机内部eeprom中*/
void Write_eeprom()
{
	SectorErase(0x2000);      
	GapValue1=GapValue&0x00ff;
	byte_write(0x2000, GapValue1);
	GapValue1=(GapValue&0xff00)>>8;
	byte_write(0x2001, GapValue1);
	byte_write(0x2060, flag_read);	
}

/*把数据从单片机内部eeprom中读出来*/
void Read_eeprom()
{
	GapValue   = byte_read(0x2001);
	GapValue   = (GapValue<<8)|byte_read(0x2000);
	flag_read  = byte_read(0x2060);
}

/*开机自检eeprom初始化*/
void Init_eeprom() 
{
	Read_eeprom();		    //先读
	if(flag_read != 1)		//新的单片机初始单片机内问EEPROM
	{
		GapValue  = 3500;
		flag_read = 1;
		Write_eeprom();	    //保存数据
	}	
}

/*显示重量，单位kg，两位整数，三位小数*/
void Display_Weight()
{
	
	if(Weight_Shiwu/10000==0)
	{
        LcdShowChr(8,1,' ');
    }
	else
    {
        LcdShowChr(8,1,Weight_Shiwu/10000 + 0x30);
    }
	LcdShowChr(9,1,Weight_Shiwu%10000/1000 + 0x30);
	LcdShowChr(10,1,'.');
	LcdShowChr(11,1,Weight_Shiwu%1000/100 + 0x30);
	LcdShowChr(12,1,Weight_Shiwu%100/10 + 0x30);
	LcdShowChr(13,1,Weight_Shiwu%10 + 0x30);
}

/*获取毛皮重量*/
void Get_Maopi()
{
	uchar i;

mm:	Weight_Maopi_0 = HX711_Read();
	for(i=0;i<10;i++)
	{
		Buzzer=1;
		LED=0;
		Delay_ms(100);
		LED=1;
		Delay_ms(100);	
	}
	Weight_Maopi = HX711_Read();
	if(Weight_Maopi/GapValue!=Weight_Maopi_0/GapValue)
	goto mm;
	Buzzer=0;
	Delay_ms(500);
	Buzzer=1;
} 

/*获取重量函数*/
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//获取净重
	
	Weight_Shiwu = (unsigned int)((float)(Weight_Shiwu*10)/GapValue)-qupi; 	//计算实物的实际重量																
	if(Weight_Shiwu >= 10000)		//超重报警
	{
		Buzzer = !Buzzer;	
		LED=!LED;
		LcdShowStr(8,1,"--.---");
	}
	else
	{
		if(Weight_Shiwu==0)
		{
            LED=0;
        }
		else if(Weight_Shiwu>0)
		{
            LED=1;
        }
		Buzzer = 1;
		Display_Weight();
	}
}
void KeyPress()
{
	if(KEY1==0)         //去皮键
	{
		Delay_ms(5);   //按键消抖

		if(KEY1==0)
		{
			if(qupi==0)
			{
                qupi=Weight_Shiwu;
            }
			else
			{
                qupi=0;
            }
			Buzzer=0;
			Delay_ms(50);
			Buzzer=1;	
			while(KEY1==0);
		}
	}
	if(KEY2==0)	   //校准传感器参数加
	{
		Delay_ms(5);    //按键消抖

		if(KEY2==0)
		{
			while(!KEY2)
			{
				key_press_num++;
				if(key_press_num>=100)
				{
					key_press_num=0;
					
                    while(!KEY2)
					{
						if(GapValue<10000)
						{
                            GapValue+=10;
                        }

						Buzzer=0;
						Delay_ms(10);
						Buzzer=1;
						Delay_ms(10);
						Get_Weight();
					}
				}
				Delay_ms(10);
			}

			if(key_press_num != 0)
			{
				key_press_num = 0;
				if(GapValue<10000)
				{
                    GapValue++;
                }
				Buzzer=0;
				Delay_ms(50);
				Buzzer=1;
			}
			Write_eeprom();		//将调整后传感器参数写入单片机内部EEPROM
		} 
	}
	if(KEY3==0)     //校准传感器参数减
	{
		Delay_ms(5);

		if(KEY3==0)
		{
			while(!KEY3)
			{
				key_press_num++;
				if(key_press_num>=100)
				{
					key_press_num=0;
					while(!KEY3)
					{
						if(GapValue>1)
						{
                            GapValue-=10;
                        }
						Buzzer=0;
						Delay_ms(10);
						Buzzer=1;
						Delay_ms(10);
						Get_Weight();
					}
				}
				Delay_ms(10);
			}

			if(key_press_num!=0)
			{
				key_press_num=0;
				if(GapValue>1)
				{   
                    GapValue--;
				}
                Buzzer=0;
				Delay_ms(50);
				Buzzer=1;
			}
			Write_eeprom();	      //将调整后传感器参数写入单片机内部EEPROM
		}
	}
}
/*毫秒延时函数(12M晶振下)*/
void Delay_ms(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<121;j++);
}

/*定时器0中断服务函数*/
void InterruptTimer0 () interrupt 1 
{   
    static uchar counter = 0;
    
    TH0 = 0x3C; //定时器赋初值    
    TL0 = 0xB0;   

    counter++;	
    if(counter >= 10)   //每0.5秒钟刷新重量
    {  
        counter = 0;
        flag = 1;
    }
}
