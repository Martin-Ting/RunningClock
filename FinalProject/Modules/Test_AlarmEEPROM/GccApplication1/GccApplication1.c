/*
 * GccApplication1.c
 *
 * Created: 8/19/2015 7:27:58 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>
#include "io.h"
#include "keypad.h"
#include "timer.h"






Alarm tmpAlarms[EEPROM_ALARM_TOPADDRESS/3];


unsigned char keyInput;

unsigned char myString[5];`
void tick(){
	keyInput = GetKeypadKey();
	switch(keyInput){
		case 'A':
			PORTB = 0xFF;
			myString[5] = '\0';
			myString[0] = tmpAlarms[3].Hour;
			myString[1] = tmpAlarms[2].Minute;
			LCD_DisplayString(1, myString);
			break;
		case 'B':
			saveAlarmsToEEPROM();
		case 'C':
			loadAlarmsFromEEPROM();
		case 'D':
			// change some elements and save them here
			// then test with case 'A' to see if stuff really changed.
		default:
			PORTB = 0x55;
			break;
	}
}
int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTB = 0x00;
	savedAlarms[0].Hour = 'a';
	savedAlarms[0].Minute = 'b';
	savedAlarms[0].IsAM = 'c';
	savedAlarms[1].Hour = 'd';
	savedAlarms[1].Minute = 'e';
	savedAlarms[1].IsAM = 'f';
	savedAlarms[2].Hour = 'g';
	savedAlarms[2].Minute = 'h';
	savedAlarms[2].IsAM = 'i';
	savedAlarms[3].Hour = 'j';
	savedAlarms[3].Minute = 'l';
	savedAlarms[3].IsAM ='m';
	savedAlarms[4].Hour = 'n';
	savedAlarms[4].Minute = 'o';
	savedAlarms[4].IsAM = 'p';
	TimerSet(500);
	TimerOn();
	LCD_init();
	while(1)
	{
		tick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}