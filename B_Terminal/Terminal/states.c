/*
 * states.c
 *
 * Created: 3/17/2017 3:57:49 PM
 * Author : David Hunt
 */ 

 #define F_CPU 8000000
 #define LEDPort PORTD6			//LED port

 #include <avr/io.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/sleep.h>
 #include "ioe.h"
 #include "states.h"

 void delay_ms(int n) {		//dynamic delay function
	 while(n--) {
		 _delay_ms(1);
	 }
 }

void mainMenu()	//main menu
{
	unsigned char mm;	//main menu choice

	printm("\n\rMain Menu:");	//print main menu
	printm("\n\r\t<a> Hello World!");
	printm("\n\r\t<s> Sleep options");
	printm("\n\r\t<d> LED control");

	mm = GetChar();		//get main menu choice
	switch (mm){
		case 'a':
			printm("\n\rHello World!\n");	//print "Hello World!
			break;
		case 's':
			state = sleepOptions;	//goto sleep options menu
			break;
		case 'd':
			state = LEDControl;		//goto LED options menu
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
}

void sleepOptions()	//sleep menu
{
	unsigned char sleep;	//sleep choice

	printm("\n\rSleep Options:");	//print sleep menu
	printm("\n\r\t<1> Sleep");
	printm("\n\r\t<2> Power down");
	printm("\n\r\t<m> Main menu");

	sleep = GetChar();		//get sleep choice
	switch (sleep){
		case '1':
			printm("\n\rPress hardware button to wake");
			_delay_ms(10);
			set_sleep_mode(SLEEP_MODE_PWR_SAVE);	//set sleep mode to 'sleep'
			sleep_mode();		//enter 'sleep' mode
			_delay_ms(1);
			break;
		case '2':
			printm("\n\rPress hardware button to wake");
			_delay_ms(10);
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//set sleep mode to 'power down'
			sleep_mode();		//enter 'power down' mode
			_delay_ms(1);
			break;
		case 'm':
			state = mainMenu;	//goto main menu
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
}

void LEDControl()	//LED menu
{
	unsigned char led;		//LED menu choice

	printm("\n\rLED CONTROL:");		//print LED menu
	printm("\n\r\t<1> LED on");		//always on option
	printm("\n\r\t<2> More LED options");		//more options
	printm("\n\r\t<3> LED brightness options");	//PWM options
	printm("\n\r\t<m> Main menu");
	
	led = GetChar();			//get LED choice
	switch (led){
		case '1':
			state = LEDOn;		//goto LEDOn state
			break;
		case '2':
			state = MoreLEDOptions;	//goto MoreLEDOptions state
			break;
		case '3':
			state = LEDBrightness;	//goto LEDBrightness state
			break;
		case 'm':
			state = mainMenu;	//goto main menu
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
}

void LEDOn()		//LED always on menu
{
	unsigned char off;		//LED off choice

	DDRD |= (1<<LEDPort);	//LED on

	printm("\n\rTurn LED Off?:");	//print LED off menu
	printm("\n\r\t<y> yes");
	printm("\n\r\t<n> no");

	off = GetChar();		//get LED off choice
	switch (off){
		case 'y':
			DDRD &= ~(1<<LEDPort);	//LED off
			state = LEDControl;		//goto LEDControl state
			break;
		case 'n':
			state = LEDControl;		//goto LEDControl state
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
}

void MoreLEDOptions()	//More LED Options menu
{
	unsigned char led;		//LED menu choice
	unsigned char *t = (unsigned char *) malloc(sizeof(char)*8);		//custom brightness choice

	printm("\n\rMore LED Options:");		//print LED menu
	printm("\n\r\t<1> LED always on");		//always on option
	printm("\n\r\t<2> LED blink once");		//blink once option
	printm("\n\r\t<3> LED blink function");	//periodic blink options
	printm("\n\r\t<b> Back");
	printm("\n\r\t<m> Main menu");
	
	led = GetChar();			//get LED choice
	switch (led){
		case '1':
			state = LEDOn;		//goto LEDOn state
			break;
		case '2':
			DDRD &= ~(1<<LEDPort);	//LED off
			printm("\n\rHow long would you like the LED to blink in ms?");
			printm("\n\rPress Enter after input: ");
			scanm(t);				//get LED on time
			DDRD |= (1<<LEDPort);	//LED on
			delay_ms(atoi((const char*)t));		//delay based on user input
			DDRD &= ~(1<<LEDPort);	//LED off
			break;
		case '3':
			state = LEDBlinkFunc;	//goto LEDBlinkFunc state
			break;
		case 'b':
			state = LEDControl;		//goto LEDControl state
			break;
		case 'm':
			state = mainMenu;		//goto Main menu state
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
	free(t);	//free memory of input buffer
}

void LEDBlinkFunc()		//Blink LED function
{
	unsigned char *t = (unsigned char *) malloc(sizeof(char)*8);		//custom brightness choice
	unsigned char *r = (unsigned char *) malloc(sizeof(char)*8);		//custom brightness choice

	DDRD &= ~(1<<LEDPort);	//LED off
	printm("\n\rThis option repeatedly blinks the LED?");
	printm("\n\rHow long would you like the LED to blink in ms?");
	printm("\n\rPress Enter after input: ");
	scanm(t);				//get LED on time
	printm("\n\rHow frequently would you like the LED to blink in ms?");
	printm("\n\rPress Enter after input: ");
	scanm(r);				//get repeat time
	printm("\n\rPress any key to stop");
	while (!(UCSR0A & (1<<RXC0))){		//wait for input
		DDRD |= (1<<LEDPort);	//LED on
		delay_ms(atoi((const char*)t));		//delay based on user input
		DDRD &= ~(1<<LEDPort);	//LED off
		delay_ms(atoi((const char*)r));		//delay based on user input
	}
	UCSR0B &= ~(1<<RXEN0);		//flush receive flag
	UCSR0B |= (1<<RXEN0);		//reset receive interrupt
	state = MoreLEDOptions;		//goto MoreLEDOptions state
	free(t);	//free memory of input buffer
	free(r);	//free memory of input buffer
}

void LEDBrightness()	//Brightness adjustment menu
{
	unsigned char led;		//brightness choice
	unsigned char *t = (unsigned char*) malloc(sizeof(char)*8);//custom brightness choice

	DDRD |= (1<<LEDPort);	//turn on LED

	printm("\n\rLED brightness Options:");	//print brightness menu
	printm("\n\r\t<1> Low");
	printm("\n\r\t<2> Medium");
	printm("\n\r\t<3> High");
	printm("\n\r\t<4> Custom");
	printm("\n\r\t<b> Back");
	printm("\n\r\t<m> Main menu");
	
	led = GetChar();		//get LED choice
	switch (led){
		case '1':
			OCR0A = 26;		//set 10% duty cycle
			break;
		case '2':
			OCR0A = 128;	//set 50% duty cycle
			break;
		case '3':
			OCR0A = 230;	//set 90% duty cycle
			break;
		case '4':
			printm("\n\rHow bright would you like the LED? (0 - 99)");
			printm("\n\rPress Enter after input: ");
			scanm(t);		//get LED on time
			OCR0A = (atoi((const char*)t)*255)/100;	//set duty cycle to custom choice
			break;
		case 'b':
			state = MoreLEDOptions;		//goto MoreLEDOptions state
			break;
		case 'm':
			state = mainMenu;		//goto Main menu state
			break;
		default:
			printm("\n\rError, not an available option\n");	//print error
			break;
	}
	free(t);	//free memory of input buffer
}