#include "LCD1602.h"

sbit Infrad = P3^2;
sbit Infrad1 = P3^3;

int Count = 0;
uchar TxStr[6]={0}; //�������ݻ�����
bit flag1 = 0;
bit flag2 = 0;

void InitUART(uint baud);   //�������ú���
void InitEX();
void ConverData();          //��λ��Ϣ����ת������
void UartTXData(uchar str[]);//���ڷ����ַ�������
void Delay_ms(uint xms);     //������ʱ����

void main()
{   
    PX0 = 1;     //�ⲿ�ж�0�ж����ȼ�����λ
    PX1 = 1;     //�ⲿ�ж�1�ж����ȼ�����λ
 
    InitLcd1602();  //LCD1602��ʼ��
    InitEX();       //�ⲿ�жϳ�ʼ��
    InitUART(9600);    //�����жϳ�ʼ��

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
                UartTXData(TxStr);   //������֡���ͳ�ȥ      
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
                UartTXData(TxStr);   //������֡���ͳ�ȥ
                LcdShowChr(0,0,Count/10+'0');
                LcdShowChr(1,0,Count%10+'0');
//                EX1 = 1; 
            } 
        }    
    }   
}
         
void InitEX()
{
    IT0=1;       //�½��ش���
    EX0=1;       //�ⲿ�ж�0ʹ��

    IT1=1;       //�½��ش���
    EX1=1;       //�ⲿ�ж�0ʹ��
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
    TxStr[2] = Count/10+'0';
    TxStr[3] = Count%10+'0';
    TxStr[4] = 'H';
}

/*******INT0�жϺ���*********/
void counter0(void) interrupt 0  
{
    flag1 = 1;
}

/*******INT1�жϺ���*********/
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


