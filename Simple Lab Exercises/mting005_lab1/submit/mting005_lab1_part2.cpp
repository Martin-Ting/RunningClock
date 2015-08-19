/*
 * mting005_lab1_part2.cpp
 *
 * Created: 7/28/2015 1:00:53 PM
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
    while(1){
	    unsigned char i = 0;
	    unsigned numAvailable = 0;
	    while( i < 4 ){
		    if(GetBit(PINA, i)){
			    ++numAvailable;
		    }
		    ++i;
	    }
	    PORTB = numAvailable;
    }
}