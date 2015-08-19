/*
 * mting005_lab7_ex002.c
 *
 * Created: 8/12/2015 6:14:01 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include "timer.h"
#include "keypad.h"
#include "io.h"
#include <stdio.h>

unsigned long int findGCD(unsigned long int a,		//		==== find GDC function.
						  unsigned long int b){
	unsigned long int c;
	while(1){
		c = a%b;
		if(c==0){return b;}
		a = b;
		b = c;
	}
	return 0;
}
// -->												//		==== Task Structure 
typedef struct _task {
	signed char state;
	unsigned long int period;
	unsigned long int elapsedTime;
	
	int (*TickFct)(int);
} task;
// -->												//		==== Task scheduler implementation
//	Task Scheduler
//	Shared Variables ===============
unsigned char KeypadInput = '\0';
// User Defined FSMs ===============
enum ReadKeypadStates { ReadKeypad_update };
int ReadKeypadSMtick(int state){
	switch(state){
		case ReadKeypad_update:		// one state keypad update 
			break;
		default:					// initial state
			state = ReadKeypad_update;
			break;
	}
	switch(state){
		case ReadKeypad_update:
			KeypadInput = GetKeypadKey();
			break;
		default:
			break;
	}
	return state;
}
enum displayKPInputStates{ displayKPInput_display };
int displayKPInputSMtick(int state){
	switch(state){
		case displayKPInput_display:
			break;
		default:
			state = displayKPInput_display;
			break;
	}	
	switch(state){
		case displayKPInput_display:
			switch(KeypadInput){
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
			break;
		default:
			break;
	}
	return state;
}

int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F; // PORTA[3:0]: IN -- KP rows
							   // PORTA[7:4]: OUT -- select KP Columns
	DDRB = 0xFF; PORTB = 0x00; // PORTB: out -- LEDs
	DDRC = 0xFF; PORTC = 0x00;	
	DDRD = 0xFF; PORTD = 0x00; // PORTD[0:3] LEDs
							   // PORTD[4:5] Unused
							   // PORTD[6:7] LCD
	
	
	// Period for the tasks
	unsigned long int ReadKeypadSM_calc = 50;
	unsigned long int displayKPInputSM_calc = 100;
	
	//Calculating GCD
	unsigned long int GCD = 1;
	GCD = findGCD(ReadKeypadSM_calc, displayKPInputSM_calc);
	
	//recalculate GCD periods for scheduler
	unsigned long ReadKeypadSM_period = ReadKeypadSM_calc/GCD;
	unsigned long displayKPInputSM_period = displayKPInputSM_calc/GCD;
	
	// Declare an array of tasks
	static task task1, task2;
	task *tasks[] = {&task1, &task2};
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//Task 1
	task1.state = -1;
	task1.period = ReadKeypadSM_period;
	task1.elapsedTime = ReadKeypadSM_period;
	task1.TickFct = &ReadKeypadSMtick;

	//Task 2
	task2.state = -1;
	task2.period = displayKPInputSM_period;
	task2.elapsedTime = displayKPInputSM_period;
	task2.TickFct = &displayKPInputSMtick;
	
	//Set Timer and turn on
	TimerSet(GCD);
	TimerOn();
	
	//Init LCD
	LCD_init();
	LCD_DisplayString(1, "Welcome to the  breadboard!");
	//scheduler for loop iterator
	unsigned short i;
    while(1)
    {
		for(i = 0; i < numTasks; i++){
			// task is ready to tick
			if(tasks[i]->elapsedTime == tasks[i]->period){
				// Setting next state for task
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);	
				// Reset elapsed time for next tick.
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
	return 0;
}