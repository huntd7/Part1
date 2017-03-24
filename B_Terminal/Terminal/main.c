/*
 * Terminal.c
 *
 * Created: 3/9/2017 3:57:49 PM
 * Author : David Hunt
 */ 

#define F_CPU 8000000
#define onButtINT	PCINT1		//on interrupt from PINB1

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "states.h"
#include "ioe.h"

//On and Off button ISR
ISR (PCINT0_vect)
{
}

int main(void)
{
	DDRD = 0x00;				//PORTD6 as output
	DDRB = 0b0000100;			//PORTB2 as output, others input

	PCMSK0 |= (1<<onButtINT);	//set off button interrupt mask
	PCICR |= (1<<PCIE0);		//enable pin interrupts

	//fast PWM with 0xFF as top, non-inverting mode, no prescaler
	TCCR0A |= (1<<COM0A1) | (1<<WGM01) | (1<<WGM00);
	TCCR0B |= (1<<CS00);
	OCR0A = 230;	//set 90% duty cycle

	uartInit(9600, 0, 0);		//initialize uart with 9600 baud, normal mode, recieve interrupts on.
	
	sei();

	state= mainMenu;			//set to main Menu
    while (1){
		(*state)();				//begin state machine
    }
}