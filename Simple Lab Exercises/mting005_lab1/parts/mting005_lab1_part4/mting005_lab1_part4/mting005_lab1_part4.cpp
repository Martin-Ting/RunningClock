/*
 * mting005_lab1_part4.cpp
 *
 * Created: 7/28/2015 1:04:20 PM
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
	DDRB = 0x00; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	const unsigned char TOTALBITS = 8;
	while(1){
		unsigned char i = 0;
		unsigned char total = 0;
		while(i < TOTALBITS){
			if(GetBit(PINA, i)){
				total++;
			}
			if(GetBit(PINB, i)){
				total++;
			}
			i++;
		}
		PORTC = total;
	}
}