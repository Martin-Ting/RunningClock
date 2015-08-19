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
enum SMStates {INIT, LEDON, CHECKINPUT, CHECKINPUTFALL, GAMEWINNER, WINLED1, WINLED2} SMState;

void displayScore(unsigned char score){
	LCD_DisplayString(1, "Score: ");
	LCD_WriteData('0' + score);
}
// constants
const unsigned char MAXPOINTS = 9;
const unsigned char LEDTIMER = 3;
// variable data
unsigned char gameScore = 0;
unsigned char elapsedDuration = 0;
unsigned char currLED = 0x00;
#define GAMELIGHTS PORTB
void tick(){
	unsigned char bInput = ~PINA & 0x01;
	// state actions
	switch(SMState){
		case INIT:
			gameScore = 0;
			currLED = 0x01;
			elapsedDuration = 0;
			displayScore(gameScore);
			break;
		case LEDON:
			if(!(elapsedDuration < LEDTIMER)){ // ran out of time for current LED
				// change LED
				currLED = currLED * 2;
				if(currLED > 4){
					currLED = 0x01;
				}
				// reset "timer"
				elapsedDuration = 0;
			}
			// increment "timer"
			elapsedDuration++;
			break;
		case CHECKINPUT:
			if(currLED == 2){ // goal achieved
				if(gameScore < MAXPOINTS) // increase points
					gameScore++;
			}else{
				if(gameScore > 0) // decrease points
					gameScore--;
			}
			displayScore(gameScore);
			
			break;
		case CHECKINPUTFALL:
			break;
		case GAMEWINNER:
			LCD_DisplayString(1, "Winner!!!");
			break;
		case WINLED1:
			currLED = 0x55;
			break;
		case WINLED2:
			currLED = 0xAA;
			break;
		default:
			break;
	}
	switch(SMState){
		case INIT:
			SMState = LEDON;
			break;
		case LEDON:
			if(gameScore == MAXPOINTS){
				SMState = GAMEWINNER;
			}
			if(bInput != 0){
				SMState = CHECKINPUT;
			}
			break;
		case CHECKINPUT:
			SMState = CHECKINPUTFALL;
			
			break;
		case CHECKINPUTFALL:
			/*if(gameScore == MAXPOINTS){
				SMState = GAMEWINNER;
			}*/
			if(!bInput){
				SMState = LEDON;
			}
			break;
		case GAMEWINNER:
			SMState = WINLED1;
			break;
		case WINLED1:
			if(bInput != 0){
				SMState = INIT;
			}else{
				SMState = WINLED2;
			}
			break;
		case WINLED2:
			if(bInput != 0){
				SMState = INIT;
			}else {
				SMState = WINLED1;
			}
			break;
		default:
			break;
	}
	PORTB = currLED;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // button input
	DDRB = 0xFF; PORTB = 0x00; // LED output
	DDRC = 0xFF; PORTC = 0x00; // LCD Data Lines
	DDRD = 0xFF; PORTD = 0x00; // LCD Control Lines [ PD6 | PD7 ]
	LCD_init();		
	
	LCD_DisplayString(1, "Welcome to             the GAME.");
	
	TimerSet(100);
	TimerOn();
	while(1)
	{
		while(!TimerFlag);
		TimerFlag = 0;
		
		tick();
	}
}