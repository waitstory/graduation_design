/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   UART配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#include "drv_uart.h"



/**
  * @brief :串口初始化
  * @param :无
  * @note  :无
  * @retval:无
  */
void drv_uart_init( )
{
	//引脚配置 部分51单片机不需要
	//TX配置为输出 RX配置为输入
	UART_TX_PxM0 |= IO_OUT_PUT_PP_M0 << UART_TX_PIN_BIT;
	UART_TX_PxM1 |= IO_OUT_PUT_PP_M1 << UART_TX_PIN_BIT;
	UART_RX_PxM0 |= IO_IN_PUT_ONLY_M0 << UART_RX_PIN_BIT;
	UART_RX_PxM1 |= IO_IN_PUT_ONLY_M1 << UART_RX_PIN_BIT;
	UART_TX = 1;
	
	//串口配置
	SCON &= (uint8_t)((uint8_t)( ~( UART_MODE | UART_RX )));	//清SM0 SM1 REN
	SCON |= (uint8_t)( UART_8BAUDRATE_VOLATILE | UART_RX );
	
	//TIM1配置
	TMOD &= (uint8_t)((uint8_t)( ~TIM1_MODE ));
	TMOD |= TIM1_MODE_2;	//8位自动重装
	PCON = 0x00;
	TH1 = 0xFD;				//波特率默认配置为9600
	TL1 = 0xFD;
	
	TI = 1;					//清发送标志						
	TR1 = 1;				//使能定时器
}

/**
  * @brief :串口发送数据
  * @param :
  *			@TxBuffer:发送数据首地址
  *			@Length:数据长度
  * @note  :无
  * @retval:无
  */
void drv_uart_tx_bytes( uint8_t* TxBuffer, uint16_t Length )
{
	while( Length-- )
	{
		while( 0 == TI );		//等待上次发送完成
		TI = 0;					//清标志位
		SBUF = *TxBuffer;		//发送数据
		TxBuffer++;
	}
}

/**
  * @brief :串口接收数据
  * @param :
  *			@RxBuffer:发送数据首地址
  * @note  :无
  * @retval:接收到的字节个数
  */
uint8_t drv_uart_rx_bytes( uint8_t* RxBuffer )
{
	uint8_t l_RxLength = 0;
	uint16_t l_UartRxTimOut = 0x7FFF;
	
	while( l_UartRxTimOut-- )		//在超时范围内检测数据
	{
		if( 0 != RI )				//检测是否接收到数据
		{
			RI = 0;					//清标志位		
			*RxBuffer = SBUF;		//读数据
			RxBuffer++;
			l_RxLength++;
			l_UartRxTimOut = 0x7FFF;//重置超时检测
		}
	}
	
	return l_RxLength;			//等待超时，数据接收完成
}

