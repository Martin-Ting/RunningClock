/*
 * mting005_lab1_part8.cpp
 *
 * Created: 7/28/2015 1:50:25 PM
 *  Author: user
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PINA = 0xDC; PORTA = 0xDC;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	unsigned number = 0;
	
    while(1)
    {
        number = PINA;
		PORTB = (number & 0xF0) >> 4;
		PORTD = (number & 0x0F) << 4;
    }	
}