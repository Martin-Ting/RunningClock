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
#include "keypad.h"
// getKeypadKey() Function

// Shared Variables ========
extern eetime_t time;
// unsigned char timeString[32] declared in module_timecrunchsm

enum SystemDriverSMStates { SystemDriver_init,
	SystemDriver_timedisplaytitle, SystemDriver_timedisplay, SystemDriver_timedisplaytitle_fall, SystemDriver_timedisplay_fall,
	SystemDriver_alarmaddtitle, SystemDriver_alarmaddscroll, SystemDriver_alarmaddtitle_fall, SystemDriver_alarmaddscroll_fall,
																// Add Alarm Title Screen,					Alarm scroller
																//	Press C for Select and go to scroller	Left (A) or Right (B) to scroll through
	SystemDriver_error } SYSTEMSTATE;

enum KeypadButtons { Keypad_left, Keypad_right, Keypad_select, Keypad_delete }; // 
unsigned char menuNavigationInput;
// End Shared Variables ====
// State Machine drivers
// MenuNavigationInput State Machine ===================================================================================================================================

// SystemDriver State Machine ===================================================================================================================================							
/*  
	State Machine: SystemDriverSM
	Inputs: ABCD on Keypad
	Outputs: time.Hours, time.Minutes, time.Seconds */
#define MAXMENUTITLETIME 40 //100period ms * 40 = 4000 s seconds
signed char SystemDriverSMTick (signed char state){
	static unsigned char stateTimer = 0;
	// Transitions
	switch(state){
		//=================================================================
		case SystemDriver_timedisplaytitle:
			// count the Timer for 4 seconds until forced transition or else "Select button"
			stateTimer++;
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput == 'C'){ // c is select
				state = SystemDriver_timedisplaytitle_fall;
			}
			break;
		case SystemDriver_timedisplaytitle_fall:
			// consume input or else transition due to state timer
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput != 'C'){
				state = SystemDriver_timedisplay;
				stateTimer = 0;
			}
			break;
		case SystemDriver_timedisplay:
			if(menuNavigationInput == '#'){
				state = SystemDriver_timedisplay_fall;
			}
			break;
		case SystemDriver_timedisplay_fall:
			if(menuNavigationInput != '#'){
				state = SystemDriver_timedisplaytitle;
			}
			break;
		//=================================================================
		case SystemDriver_alarmaddtitle:
			break;
		case SystemDriver_alarmaddscroll:
			break;
		case SystemDriver_alarmaddtitle_fall:
			break;
		case SystemDriver_alarmaddscroll_fall:
			break;
		//=================================================================
		default:
			state = SystemDriver_timedisplaytitle;
			break;
	}
	// Actions
	switch(state){
		case SystemDriver_init:
		break;
		case SystemDriver_timedisplaytitle:
		break;
		case SystemDriver_timedisplay:
		break;
		case SystemDriver_alarmaddtitle:
		break;
		case SystemDriver_alarmaddscroll:
		break;
		default:
		break;
	}
	
	SYSTEMSTATE = state;
	return state;
}

enum UpdateInputSMStates { UpdateInput_update };
/*  
	State Machine: UpdateInputSM
	NOTE: KEEP THIS SM IN MAIN FILE FOR TIME BEING.
	Inputs: PORTA -> Keypad
	Outputs: keyPadInput */
signed char UpdateInputSMTick (signed char state){
	state = UpdateInput_update;
	// Keypad Keys ============================================================================================================
	unsigned char tmpKeypadInput = GetKeypadKey();
	// Process Menu Input keys : A B C D * #
	if(tmpKeypadInput == 'A' || tmpKeypadInput == 'B' || tmpKeypadInput == 'C' || tmpKeypadInput == 'D'
		|| tmpKeypadInput == '#' || tmpKeypadInput == '*'  ){
		menuNavigationInput = tmpKeypadInput;
	} else {
		menuNavigationInput = 0;	
	}
	return state;
}

// LCDDisplaySM	
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
			state = LCDDisplay_updatelcd; // initialize anything that is going to be displayed and such
			for(int i = 0; i < 32; ++i){
				timeString[i] = ' ';
			}
			break;
	}
	switch(state){
		case LCDDisplay_updatelcd:
			switch(SYSTEMSTATE){
				case SystemDriver_init:
					break;
				case SystemDriver_timedisplaytitle:
				case SystemDriver_timedisplaytitle_fall:
					LCD_DisplayString(1, "Clock");
					break;
				case SystemDriver_timedisplay:
				case SystemDriver_timedisplay_fall:
					updateTimeString();
					LCD_DisplayString(1, timeString);
					break;
				case SystemDriver_alarmaddtitle:
					break;
				case SystemDriver_alarmaddscroll:
					break;
				default:
					break;
			}

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
	//Keypad on A
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	// Period Calculations
	unsigned long int TimeCrunchSM_calc = 1000; // 1 s period
	unsigned long int LCDDisplaySM_calc = 1000;	// 100ms period
	unsigned long int UpdateInputSM_calc = 100; // 100ms period
	unsigned long int SystemDriverSM_calc = 100; // 100ms period
	
	// Calculate GCD	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(TimeCrunchSM_calc, LCDDisplaySM_calc );
	tmpGCD = findGCD(tmpGCD, UpdateInputSM_calc);
	tmpGCD = findGCD(tmpGCD, SystemDriverSM_calc);
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
		
	unsigned long int GCD = tmpGCD;
	
	// Recalculate GCD periods for scheduler
	unsigned long int TimeCrunchSM_period = TimeCrunchSM_calc/GCD;
	unsigned long int LCDDisplaySM_period = LCDDisplaySM_calc/GCD;
	unsigned long int UpdateInputSM_period = UpdateInputSM_calc/GCD;
	unsigned long int SystemDriverSM_period = SystemDriverSM_calc/GCD;
	
	// Set up task scheduler
	static task TimeCrunchSMTask,
				LCDDisplaySMTask,
				UpdateInputSMTask,
				SystemDriverSMTask;
	task *tasks[] = {&UpdateInputSMTask, &SystemDriverSMTask, &TimeCrunchSMTask, &LCDDisplaySMTask}; // keep display at the end. keep UpdateInput in the beginning and SystemDriverSM second
	const unsigned short numTasks = sizeof(tasks)/sizeof(task*);
	
	//UpdateInputSM declaration
	UpdateInputSMTask.state = -1;
	UpdateInputSMTask.period = UpdateInputSMTask.elapsedTime = UpdateInputSM_period;
	UpdateInputSMTask.TickFct = &UpdateInputSMTick;
	
	//SystemDriverSM declaration
	SystemDriverSMTask.state = -1;
	SystemDriverSMTask.period = SystemDriverSMTask.elapsedTime = SystemDriverSM_period;
	SystemDriverSMTask.TickFct = &SystemDriverSMTick;
	
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