/*
 * mting005_lab2_ex2.cpp
 *
 * Created: 7/31/2015 2:36:30 PM
 *  Author: user
 */ 


#include <avr/io.h>
enum SMStates { SM_INIT, SM_WAIT, SM_RESET, SM_INC, SM_DEC, SM_INC_FALL, SM_DEC_FALL } SMstate;
void debounce(){
	unsigned long delay = 0;
	while (delay++ <= 100000);
}
unsigned char count = 0x00;
void tick(){
	unsigned char bIncrement = ~PINA & 0x01;
	unsigned char bDecrement = ~PINA & 0x02;
	unsigned char bReset = (bIncrement != 0 && bDecrement != 0) ? 1 : 0; 

	switch(SMstate){
		case SM_INIT:
			SMstate = SM_WAIT;
			PORTC = 0x00;
			PORTB = 0x00;
			break;
		case SM_WAIT:
			debounce();
			if(bReset != 0){
				SMstate = SM_RESET;
			} else if(bIncrement != 0){
				SMstate = SM_INC;
			} else if(bDecrement != 0){
				SMstate = SM_DEC;
			}
			break;
		case SM_RESET:
			if(bReset == 0){
				SMstate = SM_WAIT;
			}
			break;
		case SM_INC:
			if(bReset != 0){
				SMstate = SM_RESET;
			}else if(bIncrement != 0 && bDecrement == 0){
				SMstate = SM_INC_FALL;
			}
			break;
		case SM_INC_FALL:
			if(bReset != 0){
				//SMstate = SM_RESET;
			} else if(bIncrement == 0 && bDecrement == 0){
				SMstate = SM_WAIT;
			}
			break;
		case SM_DEC:
			if(bReset != 0){
				SMstate = SM_RESET;
			} else if(bIncrement == 0 && bDecrement != 0){
				SMstate = SM_DEC_FALL;
			}
			break;
		case SM_DEC_FALL:
			if(bReset != 0){
				//SMstate = SM_RESET;
			} else if(bIncrement == 0 && bDecrement == 0){
				SMstate = SM_WAIT;
			}
			break;
		default:
			PORTC = 0x05;
			break;
	}
	switch (SMstate){
		case SM_WAIT:
			PORTC = 0x01;
			break;
		case SM_INC:
			PORTC = 0x02;
			if(count < 9){
				count= count + 1;
			}
			break;
		case SM_DEC:
			PORTC = 0x04;
			if(count > 0){
				count = count - 1;
			}
			break;
		case SM_DEC_FALL:
		case SM_INC_FALL:
			debounce();
			PORTC = 0x08;
			break;
		case SM_RESET:
			PORTC = 0x03;
			count = 0x00;
			break;
		default:
			PORTC = 0x05;
			break;
	}
	PORTB = count;
}

int main(void)
{
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB=0x00;
	SMstate = SM_INIT; 
    while(1)
    {
		tick();
    }
}