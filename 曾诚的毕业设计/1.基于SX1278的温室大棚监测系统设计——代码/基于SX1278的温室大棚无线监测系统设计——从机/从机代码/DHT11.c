#include "DHT11.h"

uchar temp[2]={0}; 	  //用于存放温度
uchar humi[2]={0};	  //用于存放湿度

//uchar i = 0;
//uchar j = 0;
//uchar memo[5];
uchar R_H,R_L,T_H,T_L,RH,RL,TH,TL,check;

void DHT11_start()
{
   Data=1;
   Delay_us(20);
   Data=0;
   Delay_ms(20);   //延时18ms以上
   Data=1;
   Delay_us(20);
}

uchar DHT11_rec_byte()      //接收一个字节
{
   uchar i,dat=0;

   for(i=0;i<8;i++)    //从高到低依次接收8位数据
   {          
      while(!Data);    //等待50us低电平过去
      Delay_us(20);    //延时60us，如果还为高则数据为1，否则为0 
      dat<<=1;       //移位使正确接收8位数据，数据为0时直接移位
      if(Data==1)    //数据为1时，使dat加1来接收数据1
	  {
         dat+=1;
	  }
      while(Data);  //等待数据线拉低    
   }  
   return dat;
}

void DHT11Receive()      //接收40位的数据
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
//        for(j=0;j<5;j++)  //判断是否四次接收的数据都为1，即传感器没有响应
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
        while(Data==0);   //等待拉高     
        Delay_us(40);  //拉高后延时40us
//      while(Data);
        R_H=DHT11_rec_byte();    //接收湿度高八位  
        R_L=DHT11_rec_byte();    //接收湿度低八位  
        T_H=DHT11_rec_byte();    //接收温度高八位  
        T_L=DHT11_rec_byte();    //接收温度低八位
        check=DHT11_rec_byte();  //接收校正位

        Delay_us(20);    //结束

        if((R_H+R_L+T_H+T_L)==check)   //校正
        {
            RH=R_H;
            RL=R_L;
            TH=T_H;
            TL=T_L;
        }   
        humi[0]=(RH/10);	 //进行数据处理，方便后续数据传输
        humi[1]=(RH%10);
        temp[0]=(TH/10);
        temp[1]=(TH%10);  
    } 
} 


   

  
