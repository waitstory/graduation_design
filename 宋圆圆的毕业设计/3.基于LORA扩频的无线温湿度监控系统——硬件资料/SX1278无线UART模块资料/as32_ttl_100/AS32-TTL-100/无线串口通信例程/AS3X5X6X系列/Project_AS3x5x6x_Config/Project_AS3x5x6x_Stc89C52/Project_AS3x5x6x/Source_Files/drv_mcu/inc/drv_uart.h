/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __DRV_UART_H__
#define __DRV_UART_H__



#include <reg52.h>
#include "typedef.h"


#define SYSTEM_CLK_HZ				((uint32_t)11059200)	//系统时钟11.0592M


//串口接口定义
sbit	UART_TX						=P3^1;
sbit	UART_RX						=P3^0;

//IO在端口对应的Bit位置
#define UART_TX_PIN_BIT				1
#define UART_RX_PIN_BIT				0

//IO配置寄存器
#define UART_TX_PxM0				P3M0
#define UART_TX_PxM1				P3M1

#define UART_RX_PxM0				P3M0
#define UART_RX_PxM1				P3M1

//串口工作模式
#define UART_MODE					((uint8_t)0xC0)
#define UART_SHITT_REGSISTER		((uint8_t)0x00)		//移位寄存器
#define UART_8BAUDRATE_VOLATILE		((uint8_t)0x40)		//8位波特率可变	
#define UART_9BAUDRATE				((uint8_t)0x80)		//9位波特率	
#define UART_9BAUDRATE_VOLATILE		((uint8_t)0xC0)		//9位波特率可变	
#define UART_RX						((uint8_t)0x10)		//接收功能

//定时器1控制定义
#define TIM1_MODE					((uint8_t)0x30)
#define TIM1_MODE_0					((uint8_t)0x00)		//TIM1模式0	13位计数器/定时器
#define TIM1_MODE_1					((uint8_t)0x10)		//TIM1模式1	16位计数器/定时器
#define TIM1_MODE_2					((uint8_t)0x20)		//TIM1模式2 8位自动重装
#define TIM1_MODE_3					((uint8_t)0x30)		//TIM1模式3 2个8位定时器







void drv_uart_init( );
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint16_t Length );
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer );



#endif



