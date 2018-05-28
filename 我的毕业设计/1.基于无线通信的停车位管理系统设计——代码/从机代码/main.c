#include "LCD1602.h"

sbit M0 = P2^7;    //SX1278����UARTģ�鹤����ʽѡ������
sbit M1 = P2^6;      

sbit Infrad = P1^0;
sbit Infrad1 = P1^3;

int Count = 0;
uchar TxStr[6]={0}; //�������ݻ�����


void InitUART(uint baud);  //�������ú���
void ConverData();         //��λ��Ϣ����ת������
void UartTXData(uchar str[]);//���ڷ����ַ�������
void Delay_ms(uint x);

void main()
{   
    M0 = 0;
    M1 = 0;

    InitLcd1602();
    InitUART(9600);  

    while(1)
    {   

        if(Infrad == 0)
        {    
            while(!Infrad);
            Count++; 
            if(Count>=99)
            {
                Count = 99;
            }
            ConverData();
            UartTXData(TxStr);   //������֡���ͳ�ȥ 
        }

        if(Infrad1 == 0)
        {   
            while(!Infrad1);
            Count--;    
            if(Count <= 0)
            {
                Count = 0;
            }
            ConverData();
            UartTXData(TxStr);   //������֡���ͳ�ȥ 
        }  
        
    }   
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

/*��ʱ��������ʱ��λΪ����*/
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


