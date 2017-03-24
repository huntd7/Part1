/*
 * Terminal_i2c.c
 *
 * Created: 3/20/2017 3:57:49 PM
 * Author : David Hunt
 */ 

#define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "i2c.h"
#include "states.h"

int main(void)
{
	DDRD = 0x00;				//PORTD6 as output

	//fast PWM with 0xFF as top, non-inverting mode, no prescaler
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<CS00);
	OCR0A = 230;	//set 90% duty cycle



	PCMSK0 |= (1<<PCINT16);		//set RX interrupt mask
	PCICR |= (1<<PCIE2);		//enable pin interrupts

	sei();						//enable interrupts

	state= mainMenu;			//set to main Menu
    while (1){
		(*state)();				//begin state machine
    }
}