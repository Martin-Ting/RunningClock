/*
 * mting005_lab3_part1_ex1.cpp
 *
 * Created: 8/3/2015 12:13:17 PM
 *  Author: user
 */ 


#include <avr/io.h>

#define LEDS PORTB
#define DEBUGP PORTC

const unsigned char CONFIGONE = 0x02;
const unsigned char CONFIGTWO = 0x01;
enum SMStates {SMINIT, STARTONE, WAITONE, STARTTWO, WAITTWO} SMState;

void tick(){
	unsigned char input = ~PINA & 0x01;
	switch(SMState){
		case SMINIT:
			DEBUGP = 0x10;
			LEDS = CONFIGONE;
			SMState = STARTONE;
			break;
		case STARTONE:
			DEBUGP = 0x01;
			if(input == 0){
				SMState = WAITONE;
			}
			break;
		case STARTTWO:
			DEBUGP = 0x02;
			if(input == 0){
				SMState = WAITTWO;
			}
			break;
		case WAITONE:
			DEBUGP = 0x11;
			if(input != 0){
				SMState = STARTTWO;
			}
			break;
		case WAITTWO:
			DEBUGP = 0x12;
			if(input != 0){
				SMState = STARTONE;
			}
			break;
		default:
			PORTC = 0xFF;
	}
	switch(SMState){
		case SMINIT:
		case STARTONE:
		case WAITONE:
			LEDS = CONFIGONE;
			break;
		case STARTTWO:
		case WAITTWO:
			LEDS = CONFIGTWO;
			break;
		default:
			DEBUGP = 0x18;
			break;
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF; // input
	DDRB = 0xFF; PORTB = 0x00; // output
	SMState = SMINIT;
    while(1)
    {
        tick();
    }
}