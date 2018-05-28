/**
  ******************************************************************************
  * @file    main.c
  * @author  waitstory
  * @version V1.0
  * @date    2017-12-21
  * @brief   ������ʪ�ȼ�شӻ����ݷ���
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:STC89C52RCоƬ������11.0592MHz
  *
  ******************************************************************************
**/

#include "DHT11.h"

sbit M0 = P2^7;      //SX1278����UARTģ�鹤����ʽѡ������
sbit M1 = P2^6;                                                                                                  

uchar count = 0;
uchar TxStr[8]={0};    //������ʪ�����ݻ�����
void InitUART(uint baud);   //���ڳ�ʼ������
void UartTXData(uchar str[]);   //��ʪ�����ݷ��ͺ���
void ConverData();    //��ʪ������ת������

void main()
{

    M0 = 0; //SX1278����UARTģ������Ϊģʽ0
    M1 = 0;
    InitUART(9600);   //���ڳ�ʼ��

    while(1)
    {  
        DHT11_delay_ms(1000);  
        DHT11_receive();     //��ȡ��ʪ������
        ConverData();   //����ʪ�����ݸ���ͨ��Э�鹹��һ֡����������֡���뷢�ͻ�������
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

//void ConfigTimer0()
//{
//    TMOD &= 0xF0; //���� T0 �Ŀ���λ
//    TMOD |= 0x01; //���� T0 Ϊģʽ 1
//    TH0 =  0x4C;  //��ʱ50ms������11.0592M
//    TL0 =  0x00;  //ʹ��T0�ж�
//    ET0 = 1;        
//}                               

/*����ʪ�����ݸ���ͨ��Э�鹹��һ֡���������ݷ��뷢�ͻ�������*/
void ConverData()
{
    TxStr[0] = 'S';
    TxStr[1] = 'Y';
    TxStr[2] = 'Y';
    TxStr[3] = temp[0]+'0';
    TxStr[4] = temp[1]+'0';
    TxStr[5] = humi[0]+'0';
    TxStr[6] = humi[1]+'0';
    TxStr[7] = 'H';
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

//void InterruptTimer0() interrupt 1      //��ʱ��0�ж�,50ms�ж�һ��
//{
//    TH0 =  0x4C;          //��ʱ50ms������11.0592M
//    TL0 =  0x00;                
//    count++;


//    if (count >= 20)            //�������ڵ�20���㣬��ʱ1��
//    {
//        count = 0;              
//        flag1s = 1;
//    } 
//    

//}
