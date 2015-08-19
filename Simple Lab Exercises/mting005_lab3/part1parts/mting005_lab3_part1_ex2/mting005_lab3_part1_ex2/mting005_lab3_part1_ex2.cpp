/*
 * mting005_lab3_part1_ex2.cpp
 *
 * Created: 8/3/2015 12:28:34 PM
 *  Author: user
 */ 


#include <avr/io.h>

unsigned char gCount = 0;

enum SMStates{ SMINIT, SMWAIT, SMINC, SMDEC, RISEWAIT, SMRESET } SMState;

#define OUTPUTPORT PORTB
#define DEBUGPORT PORTC

void tick(){
	unsigned char bIncrement = ~PINA & 0x01;
	unsigned char bDecrement = ~PINA & 0x02;
	unsigned char bReset = (bIncrement != 0 && bDecrement != 0) ? 1 : 0;
	switch(SMState){
		case SMINIT:
			DEBUGPORT = 0x10;
			gCount = 7;
			SMState = SMWAIT;
			break;
		case SMWAIT:
			DEBUGPORT = 0x01;
			if(bReset != 0){
				SMState = SMRESET;
			}else if(gCount < 9 && (bIncrement != 0 && bDecrement == 0) ){
				// Only Increment button pressed
				SMState = SMINC;
			}else if(gCount > 0 && (bIncrement == 0 && bDecrement != 0) ){
				// Only Decrement button pressed
				SMState = SMDEC;
			}
			break;
		case RISEWAIT:
			DEBUGPORT = 0x11;
			if(bReset != 0){
				SMState = SMRESET;
			}else if(bIncrement == 0 && bDecrement == 0){
				SMState = SMWAIT;
			}
			break;
		case SMINC:
			DEBUGPORT = 0x02;
			SMState = RISEWAIT;
			break;
		case SMDEC:
			DEBUGPORT = 0x04;
			SMState = RISEWAIT;
			break;
		case SMRESET:
			DEBUGPORT = 0x1F;
			SMState = SMWAIT;
		default:
			break;
	}
	switch(SMState){
		case SMINIT:
			break;
		case SMWAIT:
			break;
		case SMINC:
			gCount++;
			break;
		case SMDEC:
			gCount--;
			break;
		case RISEWAIT:
			break;
		case SMRESET:
			gCount = 0;
			break;
		default:
			break;
	}
	OUTPUTPORT = gCount;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
    while(1)
    {
		tick();
    }
}