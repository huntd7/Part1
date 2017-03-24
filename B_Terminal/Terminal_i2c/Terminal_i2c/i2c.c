/*
 * i2c.c
 *
 * Created: 3/20/2017 3:57:49 PM
 * Author : David Hunt
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

void i2cInit()		//initialize i2c
{
	TWSR = 0x00;			//no prescaler
	TWBR = 0x0C;			//set SCL to 250kHz
	TWCR = (1<<TWEN);		//enable i2c
}

void i2cStart()		//i2c start bit
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

void TWIStop()		//i2c stop bit
{
	TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void i2cWrite(unsigned char data)	//i2c write byte
{
	TWDR = data;
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));
}

unsigned char i2cRead()		//i2c read byte
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	while (!(TWCR & (1<<TWINT)));
	return TWDR;
}

void printm(char *output)		//print string using i2c
{
	char i = 0;
	while(*(output + i) != '\0'){
		i2cWrite(*(output + i));
		i++;
	}
}

void scanm(unsigned char* buffer)	//read line using i2c
{
	buffer = (unsigned char *) malloc(sizeof(char) * 256);
	int i = 0;
	while(buffer[i] != '\n'){
		buffer[i] = i2cRead();
		i++;
	}
	buffer[i] = '\0';
}