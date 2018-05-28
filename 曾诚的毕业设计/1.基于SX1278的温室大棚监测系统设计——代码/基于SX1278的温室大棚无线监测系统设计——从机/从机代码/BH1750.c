#include "BH1750.h"
#include "I2C.h"
#include "Delay.h"

float lx=0;
int tmp=0;
uint result;
uchar BUF[2]={0};
uchar ge,shi,bai,qian,wan;      

void BH1750WriteCmd(uchar Cmd)
{
    I2CStart();               //��ʼ�ź�
    I2CWrite(BH1750_Addr+0);   //�����豸��ַ+д�ź�
    I2CWrite(Cmd);             //�ڲ��Ĵ�����ַ
    I2CStop();                //����ֹͣ�ź�
	Delay_ms(5);
}

void BH1750Start()
{
	BH1750WriteCmd(BH1750_ON);	  //power on
//	BH1750WriteCmd(BH1750_RSET);	//clear
	BH1750WriteCmd(BH1750_ONE);   //һ��H�ֱ���ģʽ������120ms��֮���Զ��ϵ�ģʽ  
}

void BH1750Read()
{   
    BH1750Start();              //�ϵ��ʼ��BH1750
    Delay_ms(180);
    I2CStart();                 //��ʼ�ź�
    I2CWrite(BH1750_Addr+1);    //�����豸��ַ+���ź�
	BUF[0]=I2CReadACK();        //����ACK
	BUF[1]=I2CReadNAK();        //����NACK
    I2CStop();                  //ֹͣ�ź�
    Delay_ms(5);

    tmp=BUF[0];
	tmp=(tmp<<8)+BUF[1];  //�ϳ����ݣ�����������
	
	lx=(float)tmp/1.2;
    
    result = (uint)lx;
    wan=result/10000;
	qian=result%10000/1000;
    bai=result%1000/100;
    shi=result%100/10;
    ge=result%10; 	
}
