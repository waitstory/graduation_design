#ifndef __I2C_H
#define __I2C_H

#include <reg52.h>
#include <intrins.h>

#define I2CDelay()  {_nop_();_nop_();_nop_();_nop_();}

sbit I2C_SCL = P1^2;
sbit I2C_SDA = P1^1;

void I2CStart();
void I2CStop();
bit I2CWrite(unsigned char dat);
unsigned char I2CReadNAK();
unsigned char I2CReadACK();


#endif



