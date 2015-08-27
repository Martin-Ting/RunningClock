/*
 * mting005_lab1_part1.cpp
 *
 * Created: 7/28/2015 12:52:19 PM
 *  Author: user
 */ 


#include <avr/io.h>
unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b){
	return (b ? (0x01 << k) : x & ~(0x01 << k));
}

unsigned char GetBit(unsigned char x, unsigned char k){
	return ( (x&(0x01 << k)) != 0 );
}
int main(void)
{
	DDRA = 0x00; PORTA = 0x00;
	DDRB = 0xFF; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	while(1){
		unsigned char doorOpen = GetBit(PINA, 0);
		unsigned char lightSensor = GetBit(PINA, 1);//(PINA|0x02) & 0x02 != 0;
		if( doorOpen && lightSensor ){
			PORTB = PORTB | 0x01;
			} else {
			PORTB = PORTB & ~0x01;
		}
	}
}