/*
 * Test_EEPROM.c
 *
 * Created: 8/18/2015 1:36:11 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/eeprom.h>
#include "keypad.h"

enum EEPROM_Addresses {First, Second, Third, Fourth};
	
int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	unsigned char bInput;
    while(1)
    {
        bInput = GetKeypadKey();
		switch(bInput){
			case '1':
				if(eeprom_read_byte((uint8_t*)First) == 'A'){
					PORTB = 0x01;
				}
				break;
			case '2':
				if(eeprom_read_byte((uint8_t*)Second) == 'B'){
					PORTB = 0x02;
				}
				break;
			case '3':
				if(eeprom_read_byte((uint8_t*)Third) == 'C'){
					PORTB = 0x04;
				}
				break;
			case '4':
				if(eeprom_read_byte((uint8_t*)Fourth) == 'D'){
					PORTB = 0x08;
				}
				break;
			case 'A':
				eeprom_write_byte((uint8_t*)First, 'A');
				break;
			case 'B':
				eeprom_write_byte((uint8_t*)Second, 'B');
				break;
			case 'C':
				eeprom_write_byte((uint8_t*)Third, 'C');
				break;
			case 'D':
				eeprom_write_byte((uint8_t*)Fourth, 'D');
				break;
			default:
				PORTB = 0x00;
				break;	
		}
    }
}