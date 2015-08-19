/*
 * mting005_lab2_ex3.cpp
 *
 * Created: 7/31/2015 3:46:08 PM
 *  Author: user
 */ 


#include <avr/io.h>
enum SMStates { INIT, WAIT, WAIT_TRANS, SEQ1_1, SEQ1_2, SEQ1_TRANS, SEQ2_1, SEQ2_2, SEQ2_TRANS, SEQ3_1, SEQ3_2, SEQ3_3, SEQ3_TRANS} SMstate;
void softpause(){
	unsigned long pauseTime = 0;
	while(pauseTime++ <= 1000){
		// wait
	}
}
void tick(){
	unsigned char sequence = 0x00;
	unsigned char input = ~PINA & 0x01;
	switch(SMstate){
		case INIT:
			SMstate = WAIT;
			break;
		case WAIT:
			PORTC = 0x01;
			if(input != 0){
				SMstate = WAIT_TRANS;
			}
			break;
		case WAIT_TRANS:
			PORTC = 0x11;
			softpause();
			if(!input){
				SMstate = SEQ1_1;
			}
		case SEQ1_1:
			PORTC = 0x02;
			if(input){
				SMstate = SEQ1_TRANS;
			}
			SMstate = SEQ1_2;
			break;
		case SEQ1_2:
			if(input){
				SMstate = SEQ1_TRANS;
			}
			SMstate = SEQ1_1;
			break;
		case SEQ2_1:
			PORTC = 0x04;
			if(input){
				SMstate = SEQ2_TRANS;
			}
			SMstate = SEQ2_2;
			break;
		case SEQ2_2:
			if(input){
				SMstate = SEQ2_TRANS;
			}
			SMstate = SEQ2_1;
			break;
		case SEQ3_1:
			PORTC = 0x08;
			if(input){
				SMstate = SEQ3_TRANS;
			}
			SMstate = SEQ3_2;
			break;
		case SEQ3_2:
			if(input){
				SMstate = SEQ3_TRANS;
			}
			SMstate = SEQ3_3;
			break;
		case SEQ3_3:
			if(input){
				SMstate = SEQ3_TRANS;
			}
			SMstate = SEQ3_1;
			break;
		case SEQ1_TRANS:
			PORTC = 0x12;
			if(!input){
				SMstate = SEQ2_1;
			}
			break;
		case SEQ2_TRANS:
			PORTC = 0x14;
			if(!input){
				SMstate = SEQ3_1;
			}
			break;
		case SEQ3_TRANS:
			PORTC = 0x18;
			if(!input){
				SMstate = SEQ1_1;
			}
			break;
	}
	switch(SMstate){
		case INIT:
			break;
		case WAIT:
			sequence = 0x00;
			break;
		case WAIT_TRANS:
			break;
		case SEQ1_1:
			sequence = 0x2A;
			break;
		case SEQ1_2:
			//sequence = 0x15;
			break;
		case SEQ2_1:
			sequence = 0x33;
			break;
		case SEQ2_2:
			sequence = 0x0C;
			break;
		case SEQ3_1:
			sequence = 0x24;
			break;
		case SEQ3_2:
			sequence = 0x12;
			break;
		case SEQ3_3:
			sequence = 0x09;
			break;
		case SEQ1_TRANS:
		case SEQ2_TRANS:
		case SEQ3_TRANS:
			sequence = 0x00;
			break;
		default:
			break;
	}
	softpause();
	PORTB = sequence;
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