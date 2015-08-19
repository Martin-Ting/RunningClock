/*
 * mting005_lab3_part1_ex3.cpp
 *
 * Created: 8/3/2015 12:55:21 PM
 *  Author: user
 */ 


#include <avr/io.h>

unsigned char gKey[10];

const unsigned char gKeySize = 4;
const unsigned char gUnlocked = 0x01;
const unsigned char gLocked = 0x00;

enum SMStates { SMINIT, SMUNLOCKED, SMLOCKED, SMCHECK, SMCHECKFALL, SMRESET} SMState;
unsigned char index = 0;
unsigned char lockStatus = 0;
unsigned char flagKeyIncorrect = 0;
void tick(){
	// collect inputs
	unsigned char bLock = ~PINA & 0x08;
	unsigned char bInput = ~PINA & 0x07;
	// SM Transitions
	switch(SMState){
		case SMINIT:
			SMState = SMUNLOCKED;
			break;
		case SMUNLOCKED:
			//PORTC=0x10;
			if(bLock != 0){
				SMState = SMLOCKED;
			}
			break;
		case SMLOCKED:
			//PORTC = 0x01;
			//TODO: check if we need to unlock
			if(index == gKeySize && flagKeyIncorrect == 0){
				SMState = SMUNLOCKED;
			} else if( index == gKeySize && flagKeyIncorrect != 0){
			//TODO: check if we need to reset
				SMState = SMRESET;
			} else if(bInput){
				SMState = SMCHECK;
			}

			break;
		case SMCHECK:
			//PORTC = 0x02;
			SMState = SMCHECKFALL;
			break;
		case SMCHECKFALL:
			//PORTC = 0x12;
			if(bInput == 0x00){
				SMState = SMLOCKED;
			}
			break;
		case SMRESET:
			//PORTC = PORTC | 0x10;
			// TEMPORARY TRANSITION
			if(bLock){
				SMState = SMLOCKED;
			}
			break;
		default:
			PORTC = 0xFF;
			break;
	}
	// SM Actions
	switch(SMState){
		case SMINIT:
			break;
		case SMUNLOCKED:
			lockStatus = gUnlocked;
			index = 0;
			///PORTC = 0;
			flagKeyIncorrect = 0;
			break;
		case SMLOCKED:
			lockStatus = gLocked;
			break;
		case SMCHECK:
			PORTC = index+1;
			if( ((bInput & gKey[index]) != 0) && flagKeyIncorrect == 0){
				// correct 
			} else {
				// incorrect, set key
				flagKeyIncorrect = 1;
			}
			index++;
			break;
		case SMCHECKFALL:
			break;
		case SMRESET:
			index = 0;
			flagKeyIncorrect = 0;
			PORTC = 0;
			break;
		default: 
			PORTC = 0xFF;
			break;
	}
	PORTB = lockStatus;
}

int main(void)
{
	gKey[0] = 0x04; // #
	gKey[1] = 0x02; // Y
	gKey[2] = 0x01;
	gKey[3] = 0x02;
	//				   X would be 0x01
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	PORTC = 0xFF; PORTC = 0x00;
    while(1)
    {
		tick();
    }
}