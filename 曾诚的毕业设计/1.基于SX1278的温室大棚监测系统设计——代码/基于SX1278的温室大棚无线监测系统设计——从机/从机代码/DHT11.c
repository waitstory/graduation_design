#include "DHT11.h"

uchar temp[2]={0}; 	  //���ڴ���¶�
uchar humi[2]={0};	  //���ڴ��ʪ��

//uchar i = 0;
//uchar j = 0;
//uchar memo[5];
uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,check;

void DHT11_start()
{
   Data=1;
   Delay_us(20);
   Data=0;
   Delay_ms(20);   //��ʱ18ms����
   Data=1;
   Delay_us(20);
}

uchar DHT11_rec_byte()      //����һ���ֽ�
{
   uchar i,dat=0;

   for(i=0;i<8;i++)    //�Ӹߵ������ν���8λ����
   {          
      while(!Data);    //�ȴ�50us�͵�ƽ��ȥ
      Delay_us(20);    //��ʱ60us�������Ϊ��������Ϊ1������Ϊ0 
      dat<<=1;       //��λʹ��ȷ����8λ���ݣ�����Ϊ0ʱֱ����λ
      if(Data==1)    //����Ϊ1ʱ��ʹdat��1����������1
	  {
         dat+=1;
	  }
      while(Data);  //�ȴ�����������    
   }  
   return dat;
}

void DHT11Receive()      //����40λ������
{
    uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,check; 

    Delay_ms(1000);  
    DHT11_start();
     
//    if(i<= 4)
//    {   
//        memo[i] = Data;
//        i++; 
//    }
//    else
//    {
//        i=0;
//        for(j=0;j<5;j++)  //�ж��Ƿ��Ĵν��յ����ݶ�Ϊ1����������û����Ӧ
//        {
//            if(memo[j] != 1)
//            {
//                break;
//            }
//            else
//            {
//                humi[0]='-'-'0';	
//                humi[1]='-'-'0';
//                temp[0]='-'-'0';
//                temp[1]='-'-'0';  
//            }
//        }
//    } 
   
    if(Data==0)
    {
        while(Data==0);   //�ȴ�����     
        Delay_us(40);  //���ߺ���ʱ40us
//      while(Data);
        R_H=DHT11_rec_byte();    //����ʪ�ȸ߰�λ  
        R_L=DHT11_rec_byte();    //����ʪ�ȵͰ�λ  
        T_H=DHT11_rec_byte();    //�����¶ȸ߰�λ  
        T_L=DHT11_rec_byte();    //�����¶ȵͰ�λ
        check=DHT11_rec_byte();  //����У��λ

        Delay_us(20);    //����

        if((R_H+R_L+T_H+T_L)==check)   //У��
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }   
        humi[0]=(RH/10);	 //�������ݴ�������������ݴ���
        humi[1]=(RH%10);
        temp[0]=(TH/10);
        temp[1]=(TH%10);  
    } 
} 


   

  
