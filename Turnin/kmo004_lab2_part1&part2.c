/*	Author: kmo004
 *  Partner(s) Name: Jason Mendoza
 *	Lab Section:
 *	Assignment: Lab # 2 Exercise # 1 & 2
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */

#include <avr/io.h>
#include "usart_ATmega1284.h"
#include "timer.h"

unsigned char temp;
unsigned char light;
unsigned char ltime = 0;
unsigned char stime = 0;
unsigned char B0;


enum lightstates{on, off} lstate;
void mastertick(){
	switch (lstate){
		case on:
			if(ltime == 0x01){
				lstate = off;
				ltime = 0;
			}
			else{
				lstate = on;
				ltime++;
			}
		break;
		case off:
			if(ltime == 0x01){
				lstate = on;
				ltime = 0;
			}
			else{
				lstate = off;
				ltime++;
			}
		break;
	}
	switch(lstate){
		case on:
			light = 0x01;
		break;
		case off:
			light = 0x00;
		break;
	}
}

enum Reciever_states {Recieve, Transmit} states;

void recieve(){
	switch (states){
		case Recieve:
			if(B0 == 0x00){
				states = Recieve;
			}
			else{
				states = Transmit;
			}
			break;
		case Transmit:
			if(B0 == 0x00){
				states = Recieve;
			}
			else{
				states = Transmit;
			}
			break;
	}
	switch (states){
		case Recieve:
			if(USART_HasReceived(0)){
				temp = USART_Receive(0);
			}
			else{
				temp = USART_Receive(0);
			}
			PORTA = temp;
		break;
		case Transmit:
			mastertick();
			if(stime == 0x01){
				if(USART_IsSendReady(1)){
					USART_Send(light,1);
				}
				if(USART_HasReceived(1)){
					PORTA = light;
				}
				PORTA = light;
				stime = 0x00;
			}
			else{
				stime++;
			}
			break;
	}
	
}


int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRD = 0x00; PORTD = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(500);
	TimerOn();
	
    initUSART(0);
	initUSART(1);
	
	temp = 0x00;
	
	states = Recieve;
	lstate = off;
	
    while (1) 
    {
		B0 = ~PINB & 0x01;
		
		recieve();
		
		if(B0 == 0x00){
			PORTC = 0x00;
		}
		else if(B0 == 0x01){
			PORTC = 0x01;
		}
		while(!TimerFlag){}
		TimerFlag = 0;
		
		
		
		
    }
}

