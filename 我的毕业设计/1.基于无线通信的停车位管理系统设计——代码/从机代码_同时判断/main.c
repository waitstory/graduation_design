#include "LCD1602.h"

sbit Infrad1 = P2^0;
sbit Infrad2 = P2^1;
sbit BEEP = P1^7;

uchar const *current="Current:";       //��ǰֵ
uchar const *surplus="Surplus:";       //ʣ��ֵ

uchar TxStr[6]={0}; //�������ݻ�����

bit end;          //��ʱ1���־
bit enter=0;      //�����־λ
bit out=0;        //��ȥ��־λ
uchar count=0;    //��ʱ�ۼӱ���
int num=0;    //������������
int num2=99;  //����������
int num3;     //ʣ��ֵ
  
void ConfigTimer0();         //��ʱ��T0����
void Delay_ms(uint xms);     //��ʱ��������ʱ��λΪ����
void Judge();                //�����жϺ���
void Beep();                 //��������������
void Display();              //LCD1602��ʾ����,������������ʾ��LCD1602��
void UartTXData(uchar str[]);
void InitUART(uint baud);
void ConverData();



void main()  
{   
    InitUART(9600);
    ConfigTimer0();     //��ʱ��T0����                               
    InitLcd1602();      //LCD1602��ʼ��
   
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
    TMOD &= 0xF0; //���� T0 �Ŀ���λ
    TMOD |= 0x01; //���� T0 Ϊģʽ 1
    TH0 =  0x4C;          //��ʱ50ms������11.0592M
    TL0 =  0x00;          //ʹ��T0�ж�
    ET0 = 1;        
}

void Display()
{
    LcdShowStr(1,0,surplus);         //��һ����ʾʣ��ֵ
    LcdShowChr(9,0, num3/10+'0');    //��ʾʣ��ֵ���ݵ�ʮλ
    LcdShowChr(10,0, num3%10+'0');   //��ʾʣ��ֵ���ݵĸ�λ

    LcdShowStr(1,1,current);       //�ڶ�����ʾ��ǰֵ
    LcdShowChr(9,1,num/10+'0');    //��ʾ��ǰֵ���ݵ�ʮλ
    LcdShowChr(10,1,num%10+'0');   //��ʾ��ǰֵ���ݵĸ�λ

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
            TR0 = 1;      //������ʱ��0	
        }
        while(out==1)
        {
            if(end==1)         //1s��û�м�⵽�˳�ȥ
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
            TR0 = 1;      //������ʱ��0	   
        }
        while(enter==1) 
        {
            if(end==1)         //1s��û�м�⵽�������������
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
    TR1 = 1;       //����T1
}                                 

/*����ʪ�����ݸ���ͨ��Э�鹹��һ֡���������ݷ��뷢�ͻ�������*/
void ConverData()
{
    TxStr[0] = 'S';
    TxStr[1] = 'Y';
    TxStr[2] = num/10+'0';
    TxStr[3] = num%10+'0';
    TxStr[4] = 'H';
}

/*���ڷ����ַ�������*/	
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

void InterruptTimer0() interrupt 1         //��ʱ��0�ж�,50ms�ж�һ��
{
    TH0 =  0x4C;          //��ʱ50ms������11.0592M
    TL0 =  0x00;                
    count++;

    if (count>=100)            //�������ڵ�20���㣬��ʱ1��
    {
        count = 0;              
        end=1;
    } 
}