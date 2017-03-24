/*
 * avrioe.c
 *
 * Created: 2/23/2017 3:12:42 PM
 * Author : David Hunt
 */ 

#ifndef ioe_H
#define ioe_H

void uartInit(unsigned int, unsigned char, unsigned char);

unsigned char GetChar();

void PutChar(unsigned char data);

void printm(char* output);

void scanm(unsigned char* buffer);

#endif