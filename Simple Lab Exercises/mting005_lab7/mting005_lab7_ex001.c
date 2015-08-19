/*
 * mting005_lab7_ex001.c
 *
 * Created: 8/12/2015 2:27:09 PM
 *  Author: user
 */ 


#include <avr/io.h>


unsigned char GetKeypadKey(){
	// Check keys in col 1
	PORTC = 0xEF;
	asm("nop");			// delay to allow PORTC to stabilize
	if( (PINC & 0x01) == 0) {return ('1');}
	if( (PINC & 0x02) == 0) {return ('4');}
	if( (PINC & 0x04) == 0) {return ('7');}
	if( (PINC & 0x08) == 0) {return ('*');}
		
	PORTC = 0xDF;
	asm("nop");			// delay to allow PORTC to stabilize
	if( (PINC & 0x01) == 0) {return ('2');}
	if( (PINC & 0x02) == 0) {return ('5');}
	if( (PINC & 0x04) == 0) {return ('8');}
	if( (PINC & 0x08) == 0) {return ('0');}

	PORTC = 0xBF;
	asm("nop");			// delay to allow PORTC to stabilize
	if( (PINC & 0x01) == 0) {return ('3');}
	if( (PINC & 0x02) == 0) {return ('6');}
	if( (PINC & 0x04) == 0) {return ('9');}
	if( (PINC & 0x08) == 0) {return ('#');}
	
	PORTC = 0x7F;
	asm("nop");			// delay to allow PORTC to stabilize
	if( (PINC & 0x01) == 0) {return ('A');}
	if( (PINC & 0x02) == 0) {return ('B');}
	if( (PINC & 0x04) == 0) {return ('C');}
	if( (PINC & 0x08) == 0) {return ('D');}
		
		
	return '\0';
}
int main(void)
{
	DDRB = 0xFF; PORTB = 0x00; // PORTB: out -- LEDs
	DDRC = 0xF0; PORTC = 0x0F; // PORTB[3:0]: IN -- KP rows
							   // PORTB[7:4]: OUT -- select KP Columns
	unsigned char x; 
    while(1)
    {
		x = GetKeypadKey();
		switch(x){
			case '\0': PORTB = 0x1F; break;
			case '1' : PORTB = 0x01; break;
			case '2' : PORTB = 0x02; break;	
			case '3' : PORTB = 0x03; break;
			case '4' : PORTB = 0x04; break;
			case '5' : PORTB = 0x05; break;
			case '6' : PORTB = 0x06; break;
			case '7' : PORTB = 0x07; break;
			case '8' : PORTB = 0x08; break;
			case '9' : PORTB = 0x09; break;
			case '0' : PORTB = 0x00; break;
			case 'A' : PORTB = 0x0A; break;
			case 'B' : PORTB = 0x0B; break;
			case 'C' : PORTB = 0x0C; break;
			case 'D' : PORTB = 0x0D; break;
			case '*' : PORTB = 0x0E; break;
			case '#' : PORTB = 0x0F; break;
			default: PORTB = 0xFF; break;		
		}
        //TODO:: Please write your application code 
    }
}