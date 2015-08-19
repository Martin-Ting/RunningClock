/*
 * mting005_lab1_part5.cpp
 *
 * Created: 7/28/2015 1:05:23 PM
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
	DDRA = 0x00; PORTA=0xFF;
	DDRB = 0xFF; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char fuelLevel = 0x00;
	while(1){
		unsigned char fuelData = ~PINA & 0x0F;
		if(fuelData == 1 || fuelData == 2){
			fuelLevel = 0x20;
		}else if (fuelData == 3 || fuelData == 4){
			fuelLevel = 0x30;
		}else if (fuelData == 5 || fuelData == 6){
			fuelLevel = 0x38;
		}else if (fuelData >= 7 && fuelData <= 9){
			fuelLevel = 0x3C;
		}else if (fuelData >= 10 && fuelData <= 12){
			fuelLevel = 0x3E;
		}else if (fuelData >= 13 && fuelData <= 15){
			fuelLevel = 0x3F;
		} else {
			fuelLevel = 0x00;
		}
	
		if(fuelData <= 4){
			//light PC6
			fuelLevel = fuelLevel | 0x80;
		}
		else{
			fuelLevel = fuelLevel & (0xFF & ~0x80);
		}
		PORTB = fuelLevel;
	}
}