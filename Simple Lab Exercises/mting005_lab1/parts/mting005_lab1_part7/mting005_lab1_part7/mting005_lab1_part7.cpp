/*
 * mting005_lab1_part7.cpp
 *
 * Created: 7/28/2015 1:19:26 PM
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
	DDRC = 0x00; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// set PD0 = 1
	//	total weight > 140kg
	// set PD1 if A - C > 80kg
	// 
	unsigned char MAXWEIGHT = 140;
	unsigned char MAXBALANCEDISPLACEMENT = 80;
	
	unsigned short totalWeight = 0;
	//unsigned short avgWeight = 0;
	unsigned short balanceWeight = 0;
	unsigned char shiftWeight = 0;
    while(1)
    {
		PORTD = 0x0;
		totalWeight = (PINA + PINB + PINC);
		//avgWeight = totalWeight / 3;
		balanceWeight = (PINA > PINC) ? PINA - PINC : PINC - PINA;
		
		// set PD0 weight of three cars too heavy 
		if (totalWeight > MAXWEIGHT){
			PORTD = PORTD | 0x01;
		} else {
			PORTD = PORTD & ~0x01;
		}
		// set PD1 if |A - C| > 80
		if(balanceWeight > MAXBALANCEDISPLACEMENT){
			PORTD = PORTD | 0x02;
		} else {
			PORTD = PORTD & ~0x02;
		}
		if(totalWeight < MAXWEIGHT){
			shiftWeight = (MAXWEIGHT - totalWeight);
			PORTD = PORTD | (shiftWeight*4);
		}
		
    }
}