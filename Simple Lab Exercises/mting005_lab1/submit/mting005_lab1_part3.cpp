/*
 * mting005_lab1_part3.cpp
 *
 * Created: 7/28/2015 1:03:14 PM
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
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char totalSpots = 4;
	while(1){
		unsigned char i = 0;
		unsigned numAvailable = 0;
		while( i < totalSpots ){
			if(GetBit(PINA, i)){
				++numAvailable;
			}
			++i;
		}
		PORTB = numAvailable;
		if(numAvailable == totalSpots){
			PORTB = PORTB | SetBit(PORTB, 7, 1);
			}else{
			PORTB = PORTB & SetBit(PORTB, 7, 0);
		}
	}
}