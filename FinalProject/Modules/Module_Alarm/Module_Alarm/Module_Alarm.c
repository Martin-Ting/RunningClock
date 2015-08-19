/*
 * Module_TaskScheduler_TimeCrunchSM.c
 *
 * Created: 8/18/2015 6:45:44 PM
 *  Author: user
 */ 

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "io.h"
// LCD header file
#include "scheduler.h"
//	task struct
//	findGCD function
#include "timer.h"
//	TimerISR >>
//	TimerFlag 
#include "module_timecrunchsm.h"
// eetime_t struct
// time : eetime_t
// TimeCrunchSMTick() Function


// Shared Variables ========
extern eetime_t time;
// unsigned char timeString[32] declared in module_timecrunchsm


// End Shared Variables ====
// State Machine drivers
enum LCDDisplaySMStates { LCDDisplay_updatelcd };
/*
	State Machine LCDDisplaySM
	NOTE: KEEP THIS SM IN MAIN FILE
	Inputs: eetime time
	Outputs: to HW -> LCD Display */
signed char LCDDisplaySMTick( signed char state ){
	switch(state){
		case LCDDisplay_updatelcd:
			break;
		default:
			state = LCDDisplay_updatelcd;
			for(int i = 0; i < 32; ++i){
				timeString[i] = ' ';
			}
			break;
	}
	switch(state){
		case LCDDisplay_updatelcd:
			updateTimeString();
			LCD_DisplayString(1, timeString);
			break;
		default: 
			break;
	}
	return state;
}
/* External list:
	- TimeCrunchSMTick
	
*/
// End State Machine drivers
// 

// Scheduler
int main(void)
{
	// Set DDR and initialise PORTs
	/*//Keypad on A
	DDRA = 0xF0; PORTA = 0x0F;*/
	DDRB = 0xFF; PORTB = 0x01;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// Period Calculations
	unsigned long int TimeCrunchSM_calc = 1000; // 1 s period
	unsigned long int LCDDisplaySM_calc = 1000;	// 50ms period
	
	// Calculate GCD	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(TimeCrunchSM_calc, LCDDisplaySM_calc );
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
		
	unsigned long int GCD = tmpGCD;
	
	// Recalculate GCD periods for scheduler
	unsigned long int TimeCrunchSM_period = TimeCrunchSM_calc/GCD;
	unsigned long int LCDDisplaySM_period = LCDDisplaySM_calc/GCD;

	// Set up task scheduler
	static task TimeCrunchSMTask,
				LCDDisplaySMTask;
	task *tasks[] = {&TimeCrunchSMTask, &LCDDisplaySMTask}; // keep tasks that display at the end.
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	// TimeCrunchSM declaration
	TimeCrunchSMTask.state = -1;
	TimeCrunchSMTask.period = TimeCrunchSMTask.elapsedTime = TimeCrunchSM_period;
	TimeCrunchSMTask.TickFct = &TimeCrunchSMTick;
	//LCDDisplaySM declaration
	LCDDisplaySMTask.state = -1;
	LCDDisplaySMTask.period = LCDDisplaySMTask.elapsedTime = LCDDisplaySM_period;
	LCDDisplaySMTask.TickFct = &LCDDisplaySMTick;
	
	//======================================================
	// Set timer and turn it on
	TimerSet(GCD);
	TimerOn();
	// Start LCD
	LCD_init();
	unsigned short i; // for loop iterator
    while(1)
    {
		for(i=0; i<numTasks; ++i){
			if(tasks[i]->elapsedTime == tasks[i]->period){
				// tick and set next state
				tasks[i]->state = tasks[i]->TickFct(tasks[i]->state);
				//reset elapsed time
				tasks[i]->elapsedTime = 0;
			}
			tasks[i]->elapsedTime += 1;
		}
		while(!TimerFlag);
		TimerFlag = 0;
    }
	
	return 0;
}