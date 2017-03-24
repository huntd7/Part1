/*
 * i2c.h
 *
 * Created: 3/20/2017 3:57:49 PM
 * Author : David Hunt
 */ 

#ifndef	I2C_H
#define	I2C_H

void i2cInit();		//initialize i2c

void i2cStart();		//i2c start bit

void TWIStop();		//i2c stop bit

void i2cWrite(unsigned char data);	//i2c write byte

unsigned char i2cRead();		//i2c read byte

void printm(char *output);		//print string using i2c

void scanm(unsigned char* buffer);	//read line using i2c

#endif