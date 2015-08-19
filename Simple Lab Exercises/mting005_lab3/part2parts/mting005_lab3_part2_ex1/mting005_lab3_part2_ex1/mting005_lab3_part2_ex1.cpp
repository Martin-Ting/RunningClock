/*
 * mting005_lab3_part2_ex1.cpp
 *
 * Created: 8/2/2015 3:38:07 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include<avr/interrupt.h>
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

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

unsigned long count = 0x00;
int main(void)
{
	DDRB = 0xFF;
	PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
	
	unsigned char tmpB = 0x00;
    while(1)
    {
		// user code (ie syncSM calls)
		// Toggle PORTB; temp bad pro sty
		TimerSet(1000);
		while(!TimerFlag);
			TimerFlag = 0;
			if(count == 0){
				PORTB = 0x01;
				count++;
			} else if(count == 1){
				PORTB = 0x02;
				count++;
			}else if(count == 2){
				PORTB = 0x04;
				count = 0;
			}
			/*if(count == 5){
				tmpB = ~tmpB;
				PORTB = tmpB;	
				PORTC  = 0x01;
			}else if(count == 10){
				tmpB = ~tmpB;
				PORTB = tmpB;
				PORTC = 0x04;
			}else if(count == 15){
				tmpB = ~tmpB;
				PORTB = tmpB;
				PORTC = 0x08;
			}else if(count == 20){
				PORTB = 0x55;
				PORTC = 0x11;
			}else if(count == 25){
				tmpB = ~tmpB;
				PORTB = 0xAA;
				PORTC = 0x12;
			}else if(count == 30){
				PORTB = 0x55;
				PORTC = 0x11;
			}else if(count == 35){
				tmpB = ~tmpB;
				PORTB = 0xAA;
				PORTC = 0x12;
			}else if (count == 40){
				count = 0;
			}*/
			
    }
}