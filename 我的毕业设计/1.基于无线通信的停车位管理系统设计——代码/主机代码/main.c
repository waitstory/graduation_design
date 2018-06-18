#include "LCD1602.h"
#include "EEPROM.h"
#define Judge() ((RxStr[0] == 'S') && (RxStr[1] == 'Y')&& (RxStr[4] == 'H'))

sbit M0 = P2^7;    //SX1278����UARTģ�鹤����ʽѡ������
sbit M1 = P2^6; 

sbit BEEP = P1^3;
sbit LED = P1^4;
sbit KEY1 = P2^2;
sbit KEY2 = P2^1;
sbit KEY3 = P2^0;

bit flag = 0;       //���ݽ������
bit flagRec = 0;    //��ʼ�������ݱ�־
bit flag_read = 0;  //���ܳ�λ��д�뵥Ƭ���ڲ�EEPROM�б�־
uchar dat =0;       //���ݱ���λ
uchar RxStr[5]={0}; //�������ݻ�����
uchar All = 99;     //�ܳ�λ��
uchar Count = 0;    //��ռ�ó�λ
uchar state = 0;    //����ѡ��ģʽ
bit flag250ms = 0;  //��ʱ250ms��־λ

void Write_eeprom();    //������д�뵽��Ƭ���ڲ�EEPROM��
void Read_eeprom();     //�ӵ�Ƭ���ڲ�EEPROM�ж�ȡ���ݺ���
void InitEEPROM();     //�ϵ��ʼ�������ܳ�λ������Ƭ���ڲ�EEPROM��
void InitUART(uint baud);  //���ڳ�ʼ������
void ConfigTimer0();    //���ö�ʱ��0����
void Delay_ms(uint x);  //������ʱ����
void InitDisplay();     //��ʼ����ʾLCD1602����
void ConvertData();     //�����յ�����ת��������
void KeyScan();         //����ɨ�躯��
void Show();            //��ʾ���ݵ�LCD1602��


void main()
{
    M0 = 0; //SX1278����UARTģ������Ϊģʽ0
    M1 = 0;
    
    InitLcd1602();   //LCD1602Һ������ʼ��
    InitEEPROM();   //��ʼ����Ƭ���ڲ�EEPROM����
    InitDisplay();   //��ʼ��LCD1602��ʾ����
    InitUART(9600);  //���ڳ�ʼ��
    ConfigTimer0();  //���ö�ʱ��T0
//    P2 = 0xF7;       //P2.3 �� 0���� KeyOut1 ����͵�ƽ
        
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

/* ���ò�����T0,��ʱ50ms*/
void ConfigTimer0()
{
    TMOD &= 0xF0; //���� T0 �Ŀ���λ
    TMOD |= 0x01; //���� T0 Ϊģʽ 1
    TH0 = 0x4C; //����T0����ֵ
    TL0 = 0x00;
    ET0 = 1; //ʹ�� T0 �ж�
    TR0 = 1; //���� T0
}

/* �������ú�����baud-ͨ�Ų����� */
void InitUART(uint baud)
{
    EA = 1;        //ʹ���ж��ܿ���
    SCON  = 0x50;  //���ô���Ϊģʽ1
    TMOD &= 0x0F;  //����T1�Ŀ���λ
    TMOD |= 0x20;  //����T1Ϊģʽ2
    TH1 = 256 - (11059200/12/32)/baud;  //����T1����ֵ
    TL1 = TH1;     //��ֵ��������ֵ
    ET1 = 0;       //��ֹT1�ж�
    ES  = 1;       //ʹ�ܴ����ж�
    TR1 = 1;       //����T1����
}

/*�����ݱ��浽��Ƭ���ڲ�eeprom��*/
void Write_eeprom()
{
	SectorErase(0x2000);      
	byte_write(0x2000,99);
	byte_write(0x2060, flag_read);	
}

/*�����ݴӵ�Ƭ���ڲ�eeprom�ж�����*/
void Read_eeprom()
{
	All = byte_read(0x2000);
	flag_read  = byte_read(0x2060);
}

/*�����Լ�eeprom��ʼ��*/
void InitEEPROM() 
{
	Read_eeprom();		    //�ȶ�
   
	if(flag_read != 1)		//�µĵ�Ƭ����ʼ��Ƭ������EEPROM
	{
		flag_read = 1;  
		Write_eeprom();	    //�������� 
	}	
    
}

void InitDisplay()
{
	
    LcdShowStr(2,0,"Welcome USE!");    
    LcdShowStr(1,1,"SYSTEM Running!");
    Delay_ms(1500);  
    LcdFullClear();     //���LCD1602ȫ����ʾ����
//	LcdShowStr(0,0,"All:");
//    LcdShowChr(4,0,All/10+'0');   
//    LcdShowChr(5,0,All%10+'0');
//	LcdShowStr(0,1,"Sur:");
//    LcdShowChr(4,1,(All-Count)/10+'0');     //ʣ�೵λ
//    LcdShowChr(5,1,(All-Count)%10+'0');
//    LcdShowStr(8,1,"Cur:");
//    LcdShowChr(12,1,Count/10+'0');     //ռ�ó�λ
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
//    LcdShowChr(4,1,(All-Count)/10+'0');     //ʣ�೵λ
//    LcdShowChr(5,1,(All-Count)%10+'0');
//    LcdShowStr(8,1,"Cur:");
//    LcdShowChr(12,1,Count/10+'0');     //ʣ�೵λ
//    LcdShowChr(13,1,Count%10+'0');
}

/*����ɨ�躯��*/
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

    if(KEY2 == 0)        //�ܳ�λ����
    {
        Delay_ms(5);     //����
    
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
  
  
    if(KEY3 == 0)     //�ܳ�λ����
    {
        Delay_ms(5);  //����
    
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

/*�ܳ�λ��ͨ��������������ʾ��LCD1602��*/
    void Show()
{
    if(state == 0)
    {   
        LcdShowStr(4,0,"All:");
        LcdShowChr(8,0,All/10+'0');   
        LcdShowChr(9,0,All%10+'0');
        LcdShowStr(0,1,"Sur:");
        LcdShowChr(4,1,(All-Count)/10+'0');     //ʣ�೵λ
        LcdShowChr(5,1,(All-Count)%10+'0');
        LcdShowStr(8,1,"Cur:");
        LcdShowChr(12,1,Count/10+'0');     //��ռ��λ
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

/*��ʱ��������ʱ��λΪ����*/
void Delay_ms(uint x)
{
   uint i,j;
 
   for(i=x;i>0;i--)
      for(j=110;j>0;j--);
}

/* UART�жϷ����� */
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
            flagRec = 1;   //��ʼ��������
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
                    flagRec = 0;   //ֹͣ��������
                    flag = 1;      //���ݽ������
                }
            }
        }
        SBUF = dat;  //�����յ�������ͨ�����ڷ��͵����Դ�ӡ������ʾ���������
    }  
}

void InterruptTimer0() interrupt 1
{
    static unsigned char tmr250ms = 0;

    TH0 = 0x4C; //���¼���T0����ֵ
    TL0 = 0x00;
    
    tmr250ms++;

    if (tmr250ms%5==0)
    {
        flag250ms = !flag250ms;
    }
}
