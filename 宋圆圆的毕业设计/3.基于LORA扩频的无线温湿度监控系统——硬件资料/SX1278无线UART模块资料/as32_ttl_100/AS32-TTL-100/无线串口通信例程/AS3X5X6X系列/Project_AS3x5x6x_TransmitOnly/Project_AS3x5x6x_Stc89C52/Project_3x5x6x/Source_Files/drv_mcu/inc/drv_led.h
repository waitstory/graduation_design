/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   LED配置H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef	__DRV_LED_H__
#define __DRV_LED_H__


#include <reg52.h>
#include "typedef.h"


//LED接口定义
sbit 		LED_RED_PIN				=P1^0;		
sbit 		LED_GREEN_PIN			=P1^1;

//LED引脚在端口的Bit位置
#define		LED_RED_PIN_BIT			0				
#define		LED_GREEN_PIN_BIT		1			

#define		LED_RED_PxM0			P1M0
#define		LED_RED_PxM1			P1M1
#define		LED_GREEN_PxM0			P1M0
#define		LED_GREEN_PxM1			P1M1



//LED定义
typedef enum
{
	LED_RED = 0,
	LED_GREEN
}LedPortType;


void drv_led_init( void );
void drv_led_on( LedPortType LedPort );
void drv_led_off( LedPortType LedPort );
void drv_led_flashing( LedPortType LedPort );



#define led_red_on( )				drv_led_on( LED_RED )
#define led_red_off( )				drv_led_off( LED_RED )
#define led_red_flashing( )			drv_led_flashing( LED_RED )

#define led_green_on( )				drv_led_on( LED_GREEN )
#define led_green_off( )			drv_led_off( LED_GREEN )
#define led_green_flashing( )		drv_led_flashing( LED_GREEN )



#endif

