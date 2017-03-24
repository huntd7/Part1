/*
 * ioe.c
 *
 * Created: 2/23/2017 2:40:39 PM
 * Author : David Hunt
 */ 

#define F_CPU 8000000
#include <avr/io.h>
#include <stdlib.h>


/*8. Function that initializes UART. This includes setting the baud rate, appropriate registers, Sync/Async mode, polling/interrupt and all the rest.
You can write additionally helper functions if you need.*/

//set a common baud rate of 1200, 2400, 4800, 9600, 19200, 38400, or 57600, if not specified correctly, default is 9600.
//set mode based on: 1 sets double speed synchronous, 2 sets asynchronous, default is regular speed asynchronous
//set interrupt to 1 if interrupt needed
void uartInit(unsigned int baudrate, unsigned char Mode, unsigned char interrupted)
{	
	DDRD |= (1<<PORTD1);		//set RXD pin as input
	DDRD &= ~(1<<PORTD0);		//set TXD pin as output

	switch (baudrate){
		case 1200:
			#define BAUD 1200
		break;

		case 2400:
			#define BAUD 2400
		break;

		case 4800:
			#define BAUD 4800
		break;

		case 19200:
			#define BAUD 19200
		break;

		case 38400:
			#define BAUD 38400
		break;

		case 57600:
			#define BAUD 57600
		break;

		default:
			#define BAUD 9600
		break;
	}
	#include <util/setbaud.h>

	UBRR0H = UBRRH_VALUE;
	UBRR0L = UBRRL_VALUE;

	switch (Mode){
		case 1:			//double speed Asynchronous
			UCSR0A |= (1<<U2X0);
			break;

		case 2:			//synchronous
			UCSR0C |= (1<<UMSEL00);
			break;

		default:		//regular speed Asynchronous
			UCSR0A &= ~(1<<U2X0);
			break;
	}

	if (interrupted)
		UCSR0B |= (1<<RXCIE0);	//set receive interrupts

	UCSR0B |= (1<<RXEN0) | (1<<TXEN0);		//setup pins for transmit and receive
	UCSR0C |= (1<<UCSZ01) | (1<<UCSZ00);	//8 bit character size
}

/*9. Function similar to Getchar
(essentially pulling the UDR0 register's data during receive mode).*/

unsigned char GetChar()
{
	while (!(UCSR0A & (1<<RXC0)));
    return UDR0;
}

/*10. Function similar to putchar.*/

void PutChar(unsigned char data)
{
    while (!(UCSR0A & (1<<UDRE0)));
    UDR0 = data;
}

/*11. A string print called printm that uses a char array and your putchar clone to transmit strings.*/

void printm(char *output)
{
    char i = 0;
    while(*(output + i) != '\0'){
        PutChar(*(output + i));
        i++;
    }
}


/*12. A function called scanm that uses an uninitialized char array and your getchar clone to construct a string for your atmega.
Note you may have to use some kind of end of line or end of file token.*/
//must have declared size of buffer is calling routine
void scanm(unsigned char* buffer)
	{
		int i = 0;
		while(buffer[i] != '\n'){
			buffer[i] = GetChar();
			if (buffer[i]=='\r')
				break;
			PutChar(buffer[i]);
			i++;
		}
		buffer[i] = '\0';
	}