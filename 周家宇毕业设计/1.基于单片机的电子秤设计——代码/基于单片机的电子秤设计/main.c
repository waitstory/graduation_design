#include "LCD1602.h"
#include "EEPROM.h"
#include "HX711.h"

sbit LED = P2^7;
sbit Buzzer = P2^6; 
sbit KEY1 = P2^2;
sbit KEY2 = P2^1;
sbit KEY3 = P2^0;

//uint count = 0;

volatile bit flag = 0;		    //��ʱ0.5s��־λ

uchar flag_read;       //������������д�뵥Ƭ���ڲ�EEPROM�б�־
uint GapValue;     //����������ֵ
uint GapValue1;    //��������������ֵ
unsigned long Weight_Maopi_0 = 0;  //ëƤ��������
unsigned long Weight_Maopi = 0;    //�ȶ���ëƤ��������
long Weight_Shiwu = 0;     //������������
unsigned int qupi=0;       //ȥƤ��������
unsigned char key_press_num=0;

void ConfigTimer0();    //��ʱ��T0���ú���
void Write_eeprom();    //������д�뵽��Ƭ���ڲ�EEPROM��
void Read_eeprom();     //�ӵ�Ƭ���ڲ�EEPROM�ж�ȡ���ݺ���
void Init_eeprom();     //�ϵ��ʼ������ѹ����������������Ƭ���ڲ�EEPROM��
void Get_Maopi();       //��ȡëƤ��������
void Get_Weight();      //��ȡ��������
void Delay_ms(uint x);  //��ʱ���뺯��
void KeyPress();        //����ɨ�躯��

void main()
{
    Init_eeprom();  //�ϵ��ʼ������ѹ����������������Ƭ���ڲ�EEPROM��
    InitLcd1602();  //��ʼ��LCD1602����
    ConfigTimer0(); //���ö�ʱ��T0
   
    LcdShowStr(0,0," Welcome To Use ");
    LcdShowStr(0,1,"Electronic Scale");

    Get_Maopi(); 

    LcdShowStr(1,0,"SYSTEM Running");
    LcdShowStr(0,1," Weight: 0.000kg");

    while(1)
    {   
        if (flag == 1)      //ÿ0.5�����һ��
        {
            flag = 0;
            Get_Weight();   
        }	 
        KeyPress();
    }
}


/* ���ò�����T0,��12M�����¶�ʱ50ms*/
void ConfigTimer0()
{
    EA = 1;       //ʹ���ж��ܿ���
    TMOD &= 0x3C; //����T0�Ŀ���λ
    TMOD |= 0xB0; //����T0Ϊģʽ1
    TH0 = 0x3C;   //����T0����ֵ
    TL0 = 0xB0;
    ET0 = 1; //ʹ��T0�ж�
    TR0 = 1; //����T0
    
}

/*�����ݱ��浽��Ƭ���ڲ�eeprom��*/
void Write_eeprom()
{
	SectorErase(0x2000);      
	GapValue1=GapValue&0x00ff;
	byte_write(0x2000, GapValue1);
	GapValue1=(GapValue&0xff00)>>8;
	byte_write(0x2001, GapValue1);
	byte_write(0x2060, flag_read);	
}

/*�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*/
void Read_eeprom()
{
	GapValue   = byte_read(0x2001);
	GapValue   = (GapValue<<8)|byte_read(0x2000);
	flag_read  = byte_read(0x2060);
}

/*�����Լ�eeprom��ʼ��*/
void Init_eeprom() 
{
	Read_eeprom();		    //�ȶ�
	if(flag_read != 1)		//�µĵ�Ƭ����ʼ��Ƭ������EEPROM
	{
		GapValue  = 3500;
		flag_read = 1;
		Write_eeprom();	    //��������
	}	
}

/*��ʾ��������λkg����λ��������λС��*/
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

/*��ȡëƤ����*/
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

/*��ȡ��������*/
void Get_Weight()
{
	Weight_Shiwu = HX711_Read();
	Weight_Shiwu = Weight_Shiwu - Weight_Maopi;		//��ȡ����
	
	Weight_Shiwu = (unsigned int)((float)(Weight_Shiwu*10)/GapValue)-qupi; 	//����ʵ���ʵ������																
	if(Weight_Shiwu >= 10000)		//���ر���
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
	if(KEY1==0)         //ȥƤ��
	{
		Delay_ms(5);   //��������

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
	if(KEY2==0)	   //У׼������������
	{
		Delay_ms(5);    //��������

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
			Write_eeprom();		//�������󴫸�������д�뵥Ƭ���ڲ�EEPROM
		} 
	}
	if(KEY3==0)     //У׼������������
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
			Write_eeprom();	      //�������󴫸�������д�뵥Ƭ���ڲ�EEPROM
		}
	}
}
/*������ʱ����(12M������)*/
void Delay_ms(uint x)
{
	uint i,j;
	for(i=0;i<x;i++)
		for(j=0;j<121;j++);
}

/*��ʱ��0�жϷ�����*/
void InterruptTimer0 () interrupt 1 
{   
    static uchar counter = 0;
    
    TH0 = 0x3C; //��ʱ������ֵ    
    TL0 = 0xB0;   

    counter++;	
    if(counter >= 10)   //ÿ0.5����ˢ������
    {  
        counter = 0;
        flag = 1;
    }
}
