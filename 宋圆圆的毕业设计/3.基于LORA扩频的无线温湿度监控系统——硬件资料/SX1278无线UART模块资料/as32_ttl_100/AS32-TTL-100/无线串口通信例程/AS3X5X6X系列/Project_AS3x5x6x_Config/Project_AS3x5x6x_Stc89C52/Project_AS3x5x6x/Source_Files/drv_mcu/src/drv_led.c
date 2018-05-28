/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   LED配置C文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */



#include "drv_led.h"



/**
  * @brief :LED初始化
  * @param :无
  * @note  :无
  * @retval:无
  */ 
void drv_led_init( void )
{
	//引脚配置 部分51单片机不需要
	//LED引脚配置为推挽输出
	LED_RED_PxM0 |= IO_OUT_PUT_PP_M0 << LED_RED_PIN_BIT;
	LED_RED_PxM1 |= IO_OUT_PUT_PP_M1 << LED_RED_PIN_BIT;
	
	LED_RED_PIN = 1;			//IO初始化状态置高，LED灭
	LED_GREEN_PIN = 1;
}

/**
  * @brief :LED亮
  * @param :
  *			@LedPort:LED选择
  * @note  :无
  * @retval:无
  */
void drv_led_on( LedPortType LedPort )
{
	//IO电平置低，LED亮
	if( LED_RED == LedPort )
	{
		LED_RED_PIN = 0;		
	}
	else
	{
		LED_GREEN_PIN = 0;
	}
}

/**
  * @brief :LED灭
  * @param :
  *			@LedPort:LED选择
  * @note  :无
  * @retval:无
  */
void drv_led_off( LedPortType LedPort )
{
	//IO电平置高，LED灭
	if( LED_RED == LedPort )
	{
		LED_RED_PIN = 1;		
	}
	else
	{
		LED_GREEN_PIN = 1;
	}
}

/**
  * @brief :LED闪烁
  * @param :
  *			@LedPort:LED选择
  * @note  :无
  * @retval:无
  */
void drv_led_flashing( LedPortType LedPort )
{
	if( LED_RED == LedPort )
	{
		if( 1 == LED_RED_PIN )	
		{
			LED_RED_PIN = 0;
		}
		else
		{
			LED_RED_PIN = 1;
		}
	}
	else
	{
		if( 1 == LED_GREEN_PIN )	
		{
			LED_GREEN_PIN = 0;
		}
		else
		{
			LED_GREEN_PIN = 1;
		}
	}
}


