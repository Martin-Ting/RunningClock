/*
 * Test_CustomDisplay.c
 *
 * Created: 8/18/2015 2:12:16 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include "io.h"

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	LCD_init();
    while(1)
    {
        
    }
}