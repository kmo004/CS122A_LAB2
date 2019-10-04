/*	Author: kmo004
 *  Partner(s) Name: Jason Mendoza
 *	Lab Section:
 *	Assignment: Lab #  Exercise #
 *	Exercise Description: [optional - include for your own benefit]
 *
 *	I acknowledge all content contained herein, excluding template or example
 *	code, is my own original work.
 */
#include <avr/io.h>
#ifdef _SIMULATE_
#include "simAVRHeader.h"
#include "timer.h"
#include "usart_ATmega1284.h"
#endif

unsigned char Light = 0x00;
enum states{on,off} state;

void masterTick(){
	switch(state){
		case on:
			if(state == off){
				state = on;
			}
			else{
				state = off;
			}
			break;
		case off:
			if(state == on){
				state = off;
			}
			else{
				state = on;
			}
			break;
		default:
			state = off;
			break;
	}
	switch(state){
		case on:
			Light = 0x01;
			break;
		case off:
			Light = 0x00;
			break;
		default:
			Light = 0x00;
			break;
	}
}

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    
    TimerSet(1000);
    TimerOn();
    state = off;

	initUSART(0);
    /* Insert your solution below */
    while (1) {
	
			
			masterTick();
			if(USART_IsSendReady(0)){
				USART_Send(Light,0);
			}
			
			if(USART_HasTransmitted(0)){
				PORTA = Light;
			}
			
			
			while (!TimerFlag){}
			TimerFlag = 0;
			
    }
    return 1;
}
