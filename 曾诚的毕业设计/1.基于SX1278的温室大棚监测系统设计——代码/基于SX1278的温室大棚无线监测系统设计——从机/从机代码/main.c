#include "DHT11.h"
#include "BH1750.h"

sbit M0 = P2^7;      //SX1278����UARTģ�鹤����ʽѡ������
sbit M1 = P2^6;                                                                                                 

uchar count = 0;
uchar TxStr[13]={0};    //������ʪ�Ⱥ͹���ǿ�����ݻ�����

void InitUART(uint baud);   //���ڳ�ʼ������
void UartTXData(uchar str[]);   //��ʪ�Ⱥ͹���ǿ�����ݷ��ͺ���
void ConverData();    //��ʪ�Ⱥ͹���ǿ������ת������

void main()
{

    M0 = 0; //SX1278����UARTģ������Ϊģʽ0
    M1 = 0;
    InitUART(9600);   //���ڳ�ʼ��

    while(1)
    {  
        DHT11Receive();    //��ȡ��ʪ������
        BH1750Read();      //��ȡ��������
        ConverData();      //����ʪ�Ⱥ͹���ǿ�����ݸ���ͨ��Э�鹹��һ֡����������֡���뷢�ͻ�������
        UartTXData(TxStr);   //������֡���ͳ�ȥ
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
                    

/*����ʪ�Ⱥ͹���ǿ�����ݸ���ͨ��Э�鹹��һ֡���������ݷ��뷢�ͻ�������*/
void ConverData()
{
    TxStr[0] = 'S';
    TxStr[1] = 'Y';
    TxStr[2] = 'Y';
    TxStr[3] = temp[0]+'0';
    TxStr[4] = temp[1]+'0';
    TxStr[5] = humi[0]+'0';
    TxStr[6] = humi[1]+'0';
    TxStr[7] = wan+'0';
    TxStr[8] = qian+'0';
    TxStr[9] = bai+'0';
    TxStr[10] = shi+'0';
    TxStr[11] = ge+'0';
    TxStr[12] = 'H';
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


