/*
 * states.h
 *
 * Created: 3/17/2017 3:57:49 PM
 * Author : David Hunt
 */ 

#ifndef states_H 
#define states_H

void delay_ms(int n);	//dynamic delay function

void mainMenu();		//main menu

void sleepOptions();	//sleep menu

void LEDControl();		//LED menu

void LEDOn();			//LED always on menu

void MoreLEDOptions();	//More LED Options menu

void LEDBlinkFunc();	//Blink LED function

void LEDBrightness();	//Brightness adjustment menu

void (*state)();		//current state, initialized to main Menu

#endif
