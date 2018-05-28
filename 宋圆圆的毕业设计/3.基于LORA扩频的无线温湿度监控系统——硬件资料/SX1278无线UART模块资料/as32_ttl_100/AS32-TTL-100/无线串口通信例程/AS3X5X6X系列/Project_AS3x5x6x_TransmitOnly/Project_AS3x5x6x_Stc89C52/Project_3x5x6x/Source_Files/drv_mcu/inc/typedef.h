/**
  ******************************************************************************
  * @author  泽耀科技 ASHINING
  * @version V3.0
  * @date    2016-10-08
  * @brief   类型定义H文件
  ******************************************************************************
  * @attention
  *
  * 官网	:	http://www.ashining.com
  * 淘宝	:	https://shop105912646.taobao.com
  * 阿里巴巴:	https://cdzeyao.1688.com
  ******************************************************************************
  */


#ifndef __TYPEDEF_H__
#define __TYPEDEF_H__

typedef   signed          char int8_t;
typedef   signed short     int int16_t;
typedef   signed           int int32_t;
//typedef   signed       		 int64_t;

    /* exact-width unsigned integer types */
typedef unsigned          char uint8_t;
typedef unsigned short     int uint16_t;
typedef unsigned           int uint32_t;
//typedef unsigned            uint64_t;


//===================ADD========================ADD===========================//


//添加部分标准头文件没有的寄存器定义
//IO引脚属性配置寄存器 --> PxMx,部分51单片机没有这几个寄存器
sfr P0M0			= 0x93;
sfr P0M1			= 0x94;
sfr P1M0			= 0x91;
sfr P1M1			= 0x92;
sfr P2M0			= 0x95;
sfr P2M1			= 0x96;
sfr P3M0			= 0xB1;
sfr P3M1			= 0xB2;

/** IO属性定义 */
//准双向口
#define IO_IN_OUT_PUT_M0			((uint8_t)0x00)
#define IO_IN_OUT_PUT_M1			((uint8_t)0x00)
//推挽输出
#define IO_OUT_PUT_PP_M0			((uint8_t)0x00)
#define IO_OUT_PUT_PP_M1			((uint8_t)0x01)
//仅为输入(高阻)
#define IO_IN_PUT_ONLY_M0			((uint8_t)0x01)
#define IO_IN_PUT_ONLY_M1			((uint8_t)0x00)
//开漏
#define IO_OPEN_DRAIN_M0			((uint8_t)0x01)
#define IO_OPEN_DRAIN_M1			((uint8_t)0x01)


//===================ADD========================ADD===========================//

#endif
