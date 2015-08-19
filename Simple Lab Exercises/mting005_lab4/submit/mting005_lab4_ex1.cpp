/*
 * mting005_lab4_ex1.cpp
 *
 * Created: 8/4/2015 12:31:34 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include "io.c"
int main(void)
{
    while(1)
    { 
		DDRC = 0xFF; PORTC = 0x00; // LCD Data Lines
		DDRD = 0xFF; PORTD = 0x00; // LCD Control Lines [ PD6 | PD7 ]
		LCD_init();
		LCD_DisplayString(1, "Hello World");
		while(1){ continue;}		
    }
}