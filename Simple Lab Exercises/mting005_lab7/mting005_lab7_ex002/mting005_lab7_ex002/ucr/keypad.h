#ifndef KEYPAD_H
#define KEYPAD_H

#define KEYPADPIN PINC
#define KEYPADPORT PORTC


unsigned char GetKeypadKey(){
	// Check keys in col 1
	KEYPADPORT = 0xEF;
	asm("nop");			// delay to allow KEYPADPORT to stabilize
	if( (KEYPADPIN & 0x01) == 0) {return ('1');}
	if( (KEYPADPIN & 0x02) == 0) {return ('4');}
	if( (KEYPADPIN & 0x04) == 0) {return ('7');}
	if( (KEYPADPIN & 0x08) == 0) {return ('*');}
	
	KEYPADPORT = 0xDF;
	asm("nop");			// delay to allow KEYPADPORT to stabilize
	if( (KEYPADPIN & 0x01) == 0) {return ('2');}
	if( (KEYPADPIN & 0x02) == 0) {return ('5');}
	if( (KEYPADPIN & 0x04) == 0) {return ('8');}
	if( (KEYPADPIN & 0x08) == 0) {return ('0');}

	KEYPADPORT = 0xBF;
	asm("nop");			// delay to allow KEYPADPORT to stabilize
	if( (KEYPADPIN & 0x01) == 0) {return ('3');}
	if( (KEYPADPIN & 0x02) == 0) {return ('6');}
	if( (KEYPADPIN & 0x04) == 0) {return ('9');}
	if( (KEYPADPIN & 0x08) == 0) {return ('#');}
	
	KEYPADPORT = 0x7F;
	asm("nop");			// delay to allow KEYPADPORT to stabilize
	if( (KEYPADPIN & 0x01) == 0) {return ('A');}
	if( (KEYPADPIN & 0x02) == 0) {return ('B');}
	if( (KEYPADPIN & 0x04) == 0) {return ('C');}
	if( (KEYPADPIN & 0x08) == 0) {return ('D');}
	
	
	return '\0';
}

#endif
