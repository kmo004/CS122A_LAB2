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

unsigned char leader = 0x00;
unsigned char time1 = 0x00;

int main(void) {
    /* Insert DDR and PORT initializations */
    DDRA = 0xFF; PORTA = 0x00;
    DDRD = 0xFF; PORTD = 0x00;
    DDRB = 0x00; PORTB = 0xFF;
    DDRC = 0xFF; PORTC = 0x00;
    
    TimerSet(500);
    TimerOn();
    state = off;

	initUSART(0);
	initUSART(1);
    /* Insert your solution below */
    while (1) {
		
		if(PINB == 0xFE){
			PORTC = 0x01;
			leader = 0x01;
		}
		else{
			PORTC = 0x00;
			leader = 0x00;
		}
		
		if(leader == 0x01){
			if(time1 == 0x01){
				masterTick();
				if(USART_IsSendReady(1)){
					USART_Send(Light,1);
				}
			
				if(USART_HasTransmitted(1)){
					PORTA = Light;
				}
				time1 = 0x00;
			}
			else{
				time1 = 0x01;
			}
		}
		else{
			
			
			if(USART_HasReceived(0)){
				PORTA = USART_Receive(0);
			}
			else if(!USART_HasReceived(0)){
				PORTA = USART_Receive(0); // gets stuck here if it doesnt recieve
			}
			
			
			
		}
			
			
			while (!TimerFlag){}
			TimerFlag = 0;
			
    }
    return 1;
}
