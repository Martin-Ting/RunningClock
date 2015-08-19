/*
 * mting005_lab4_ex1.cpp
 *
 * Created: 8/4/2015 12:31:34 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include<avr/interrupt.h>
#include "io.h"

// Timer =============================================================================
//TimerISR() sets this to 1. C Programmer should clear to 0
volatile unsigned char TimerFlag = 0;
//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;		  // start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn(){
	// AVR timer/counter controller register TCCR1
	// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8MHz clock or 8,000,000/64 = 125,000 ticks/s
	// Thus, TCNT1register will count at 125,000 tick/s
	TCCR1B = 0x0B;
	
	// AVR output compare register OCR1A
	// Timer interrupt will be gnereated when TCNT1==OCR1A
	// We want a 1ms tick. 0.01 s * 125,000 tick/s = 125
	// So when TCNT1 register equals 125,
	// 1ms has passed. Thus, we compare to 125.
	OCR1A=125;
	
	// AVR timer interrupt mask register
	// bit1: OC1E1A -- enables compare match intterupt
	TIMSK1 = 0x02;
	// Initialize avr counter
	TCNT1=0;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;
	// Enable global interrupts: 0x80: 10000000
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B=0x00;
}
void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	// CPU automatically calls when TCNT1 == OCR1
	// (every 1ms per TimerOn settings)
	
	// Count down to 0 rather than up to TOP (results in a more efficient comparison)
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		// call the isr that the user uses
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
//
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
// =============================================================================
enum SMStates {INIT, WAIT, INC, DEC, RESET} SMState;
#define DEBUGPORT PORTB

unsigned char DisplayCount = 0;
void displayScore(unsigned char score){
	LCD_DisplayString(1, "Score: ");
	LCD_WriteData('0' + score);
}

void tick(){
	unsigned char bIncrement = ~PINA & 0x01;
	unsigned char bDecrement = ~PINA & 0x02;
	unsigned char bReset = (bIncrement != 0 && bDecrement != 0) ? 1 : 0;
	// state actions
	switch(SMState){
		case INIT:
			PORTB = 0x00;
			DisplayCount = 0;
			displayScore(DisplayCount);
			break;
		case WAIT:
			// waiting for input
			// Do nothing
			break;
		case INC:
			if(DisplayCount < 9)
				DisplayCount += 1;
			displayScore(DisplayCount);
			break;
		case DEC:
			if(DisplayCount > 0)
				DisplayCount -= 1;
			displayScore(DisplayCount);
			break;
		case RESET:
			DisplayCount = 0;
			displayScore(DisplayCount);
			break;
		default:
			break;
	}
	// SM transitions
	switch(SMState){
		case INIT:
			SMState = WAIT;
			break;
		case WAIT:
			if(bReset != 0){
				SMState = RESET;
			}else if(bIncrement != 0){
				SMState = INC;
			}else if(bDecrement != 0){
				SMState = DEC;
			}
			break;
		case INC:
			if(bReset != 0){
				SMState = RESET;
			}else if(bReset == 0){
				SMState = WAIT;
			}else if(bDecrement != 0){
				SMState = DEC;
			}
			break;
		case DEC:
			if(bReset != 0){
				SMState = RESET;
			}else if(bReset == 0){
				SMState = WAIT;
			}else if(bIncrement != 0){
				SMState = INC;
			}
			break;
		case RESET:
			if(bReset == 0){
				SMState = WAIT;
			}
			break;
		default:
			break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // button input
	DDRB = 0xFF; PORTB = 0x00; // LED output
	DDRC = 0xFF; PORTC = 0x00; // LCD Data Lines
	DDRD = 0xFF; PORTD = 0x00; // LCD Control Lines [ PD6 | PD7 ]
	LCD_init();		
	
	LCD_DisplayString(1, "Welcome to             the GAME.");
	
	TimerSet(1000);
	TimerOn();
	unsigned char count = 0x00;
	unsigned char tmpB = 0x00;
	while(1)
	{
		while(!TimerFlag);
		TimerFlag = 0;
		
		tick();
	}
}