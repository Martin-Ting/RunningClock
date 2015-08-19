#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "io.h"

#define SET_BIT(p,i) ((p) |= (1 << (i)))
#define CLR_BIT(p,i) ((p) &= ~(1 << (i)))
#define GET_BIT(p,i) ((p) & (1 << (i)))
          
/*-------------------------------------------------------------------------*/

#define DATA_BUS PORTC		// port connected to pins 7-14 of LCD display
#define CONTROL_BUS PORTD	// port connected to pins 4 and 6 of LCD disp.
#define RS 6			// pin number of uC connected to pin 4 of LCD disp.
#define E 7			// pin number of uC connected to pin 6 of LCD disp.

/*-------------------------------------------------------------------------*/

void LCD_ClearScreen(void) {
   LCD_WriteCommand(0x01);
}

void LCD_init(void) {

    //wait for 100 ms.
	delay_ms(100);
	LCD_WriteCommand(0x38);
	LCD_WriteCommand(0x06);
	LCD_WriteCommand(0x0f);
	LCD_WriteCommand(0x01);
	delay_ms(10);						 
}

void LCD_WriteCommand (unsigned char Command) {
   CLR_BIT(CONTROL_BUS,RS);
   DATA_BUS = Command;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(2); // ClearScreen requires 1.52ms to execute
}

void LCD_WriteData(unsigned char Data) {
   SET_BIT(CONTROL_BUS,RS);
   DATA_BUS = Data;
   SET_BIT(CONTROL_BUS,E);
   asm("nop");
   CLR_BIT(CONTROL_BUS,E);
   delay_ms(1);
}
unsigned short strleng(const unsigned char* displayString){
	unsigned short size = 0;
	while(displayString[size] != '\0'){
		size++;
	}

	return size;
}
void LCD_DisplayStringLong(/*unsigned char column,*/ const unsigned char* displayString){
	LCD_ClearScreen();
	//unsigned char c = column;
	unsigned short strLength = strleng(displayString);

	if(strLength <= 32){
		PORTD = 0x08;
		LCD_ClearScreen();
		LCD_DisplayString(1, displayString);
		return;
	} else {
		PORTD = 0x01;
		unsigned short startIndex = 0;
		unsigned short endIndex = 31;
		
		unsigned char displaySubStr[32];
		while(endIndex < strLength-2){
			// build substr i is used as an iterator throught displayString not displaySubStr
			for(unsigned short i = startIndex; i <= endIndex; ++i){
				displaySubStr[i - startIndex] = displayString[i];
			}
			// add periods
			if(startIndex == 0){ 
				// add in periods at the end.
				displaySubStr[30] = displaySubStr[31] = '.';
			}else{// add in periods
				displaySubStr[0] = displaySubStr[1] = displaySubStr[30] = displaySubStr[31] = '.';
			}
			// display substr
			delay_ms(750);
			LCD_ClearScreen();
			LCD_DisplayString(1, displaySubStr);
			
			// shift Indices
			if(startIndex == 0){ // add additional offset to bring startIndex column to 3 and not 1
				startIndex += 2;
			}
			startIndex += 2;
			endIndex += 2;
		}
		endIndex -=2;
		// fix final string
		if(endIndex <= strLength-2){
			displaySubStr[30] = displayString[strLength-1];
			displaySubStr[31] = displayString[strLength];
		}else{
			displaySubStr[31] = displayString[strLength];
		}
		// display substr
		delay_ms(750);
		LCD_ClearScreen();
		LCD_DisplayString(1, displaySubStr);
	}
}
void LCD_DisplayString( unsigned char column, const unsigned char* string) {
   LCD_ClearScreen();
   unsigned char c = column;
   while(*string) {
      LCD_Cursor(c++);
      LCD_WriteData(*string++);
   }
}

void LCD_Cursor(unsigned char column) {
   if ( column < 17 ) { // 16x1 LCD: column < 9
						// 16x2 LCD: column < 17
      LCD_WriteCommand(0x80 + column - 1);
   } else {
      LCD_WriteCommand(0xB8 + column - 9);	// 16x1 LCD: column - 1
											// 16x2 LCD: column - 9
   }
}

void delay_ms(int miliSec) //for 8 Mhz crystal

{
    int i,j;
    for(i=0;i<miliSec;i++)
    for(j=0;j<775;j++)
  {
   asm("nop");
  }
}
