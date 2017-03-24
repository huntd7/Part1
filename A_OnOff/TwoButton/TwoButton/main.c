/*
 * TwoButton.c
 *
 * Created: 3/9/2017 3:57:49 PM
 * Author : David Hunt
 */ 

 #define F_CPU 8000000

#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

#define offButtINT	PCINT0		//off interrupt from PINB0
#define onButtINT	PCINT1		//on interrupt from PINB1
#define LEDPort		PORTB2		//LED port

//On and Off button ISR
ISR (PCINT0_vect)
{	

	cli();						//disable interrupts
	if (PCMSK0 == (1<<offButtINT)){	//if powered on
		PCMSK0 |= (1<<onButtINT);	//enable 'on' interrupt
		PCMSK0 &= ~(1<<offButtINT);	//disable 'off' interrupt
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//set sleep mode to 'power down'
		sei();						//enable interrupts
	PORTB = (1<<PORTB3);		//set PORTB2 high (remains on for LED)

		sleep_mode();				//enter 'power down' mode
	}
	else{	//if powered down
		PCMSK0 &= ~(1<<onButtINT);	//disable 'on' interrupt
		PCMSK0 |= (1<<offButtINT);	//enable 'off' interrupt
	}
	sei();
}


int main(void)
{
	DDRB = 0b0000100;			//PORTB2 as output, others input

	PCMSK0 |= (1<<offButtINT);	//set off button interrupt mask
	PCICR |= (1<<PCIE0);		//enable pin interrupts
	sei();

    while (1) 
    {
		PORTB = (1<<LEDPort);		//set PORTB2 high (remains on for LED)
    }
}

