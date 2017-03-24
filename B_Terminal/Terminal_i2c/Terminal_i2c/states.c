/*
 * states.c
 *
 * Created: 3/17/2017 3:57:49 PM
 * Author : David Hunt
 */ 

 #define F_CPU 8000000
 #define LEDPort PORTD6		//LED port

 #include <avr/io.h>
 #include <stdlib.h>
 #include <avr/interrupt.h>
 #include <util/delay.h>
 #include <avr/sleep.h>
 #include "i2c.h"
 #include "states.h"

 void delay_ms(int n) {		//dynamic delay function
	 while(n--) {
		 _delay_ms(1);
	 }
 }

void mainMenu()	//main menu
{
	unsigned char mm;	//main menu choice

	printm("\nMain Menu:");	//print main menu
	printm("\n	<a> Hello World!");
	printm("\n	<s> Sleep options");
	printm("\n	<d> LED control");

	mm = i2cRead();		//get main menu choice
	switch (mm){
		case 'a':
			printm("\nHello World!\n");	//print "Hello World!
			break;
		case 's':
			state = sleepOptions;	//goto sleep options menu
			break;
		case 'd':
			state = LEDControl;		//goto LED options menu
			break;
		default:
			printm("\nError, not an available option\n");	//print error
			break;
	}
}

void sleepOptions()	//sleep menu
{
	unsigned char sleep;	//sleep choice

	printm("\nSleep Options:");	//print sleep menu
	printm("\n	<1> Sleep");
	printm("\n	<2> Power down");
	printm("\n	<m> Main menu");

	sleep = i2cRead();		//get sleep choice
	switch (sleep){
		case '1':
			set_sleep_mode(SLEEP_MODE_PWR_SAVE);	//set sleep mode to 'sleep'
			sleep_mode();		//enter 'sleep' mode
			break;
		case '2':
			set_sleep_mode(SLEEP_MODE_PWR_DOWN);	//set sleep mode to 'power down'
			sleep_mode();		//enter 'power down' mode
			break;
		case 'm':
			state = mainMenu;	//goto main menu
			break;
		default:
			printm("\nError, not an available option\n");	//print error
			break;
	}
}

void LEDControl()	//LED menu
{
	unsigned char led;		//LED menu choice

	printm("\nLED CONTROL:");		//print LED menu
	printm("\n	<1> LED on");		//always on option
	printm("\n	<2> More LED options");		//more options
	printm("\n	<3> LED brightness options");	//PWM options
	printm("\n	<m> Main menu");
	
	led = i2cRead();			//get LED choice
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
			printm("\nError, not an available option\n");	//print error
			break;
	}
}

void LEDOn()		//LED always on menu
{
	unsigned char off;		//LED off choice

	DDRD |= (1<<LEDPort);	//LED on

	printm("\nTurn LED Off?:");	//print LED off menu
	printm("\n	<y> yes");
	printm("\n	<n> no");

	off = i2cRead();		//get LED off choice
	switch (off){
		case 'y':
			DDRD &= ~(1<<LEDPort);	//LED off
			state = LEDControl;		//goto LEDControl state
			break;
		case 'n':
			state = LEDControl;		//goto LEDControl state
			break;
		default:
			printm("\nError, not an available option\n");	//print error
			break;
	}
}

void MoreLEDOptions()	//More LED Options menu
{
	unsigned char led;		//LED menu choice
	unsigned char *t;		//LED on time

	printm("\nMore LED Options:");			//print LED menu
	printm("\n	<1> LED always on");		//always on option
	printm("\n	<2> LED blink once");		//blink once option
	printm("\n	<3> LED blink function");	//periodic blink options
	printm("\n	<b> Back");
	printm("\n	<m> Main menu");
	
	led = i2cRead();			//get LED choice
	switch (led){
		case '1':
			state = LEDOn;		//goto LEDOn state
			break;
		case '2':
			DDRD &= ~(1<<LEDPort);	//LED off
			_delay_ms(50);			//make sure LED is off before blinking
			printm("\nHow long would you like the LED to blink in ms?");
			printm("\nPress Enter after input.");
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
			printm("\nError, not an available option\n");	//print error
			break;
	}
}

void LEDBlinkFunc()		//Blink LED function
{
	unsigned char *t;		//LED on time
	unsigned char *r;		//Repeat time

	DDRD &= ~(1<<LEDPort);	//LED off
	printm("\nThis option repeatedly blinks the LED?");
	printm("\nHow long would you like the LED to stay on in ms?");
	printm("\nPress Enter after input.");
	scanm(t);				//get LED on time

	printm("\nHow frequently would you like the LED to blink in ms?");
	printm("\nPress Enter after input.");
	scanm(r);				//get repeat time

	printm("\nPress any key to stop");
	while (!(UCSR0A & (1<<RXC0))){
		DDRB |= (1<<LEDPort);	//LED on
		delay_ms(atoi((const char*)t));		//delay based on user input
		DDRD &= ~(1<<LEDPort);	//LED off
		delay_ms(atoi((const char*)r));		//delay based on user input
	}
	state = MoreLEDOptions;		//goto MoreLEDOptions state
}

void LEDBrightness()	//Brightness adjustment menu
{
	unsigned char led;		//brightness choice
	unsigned char *t;		//custom brightness choice

	DDRD |= (1<<LEDPort);	//turn on LED

	printm("\nLED brightness Options:");	//print brightness menu
	printm("\n	<1> Low");
	printm("\n	<2> Medium");
	printm("\n	<3> High");
	printm("\n	<4> Custom");
	printm("\n	<b> Back");
	printm("\n	<m> Main menu");
	
	led = i2cRead();			//get LED choice
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
			printm("\nHow bright would you like the LED? (0 - 100)");
			printm("\nPress Enter after input.");
			scanm(t);		//get LED on time
			OCR0A = ((atoi((const char*)t)/10)*255);	//set duty cycle to custom choice
			break;
		case 'b':
			state = MoreLEDOptions;		//goto MoreLEDOptions state
			break;
		case 'm':
			state = mainMenu;		//goto Main menu state
			break;
		default:
			printm("\nError, not an available option\n");	//print error
			break;
	}
}