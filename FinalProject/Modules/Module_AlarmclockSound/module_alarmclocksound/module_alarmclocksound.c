/*
 * Module_TaskScheduler_TimeCrunchSM.c
 *
 * Created: 8/18/2015 6:45:44 PM
 *  Author: user
 */ 

#include <string.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/eeprom.h>

#include "io.h"
// LCD header file
#include "scheduler.h"
//	task struct
//	findGCD function
#include "timer.h"
//	TimerISR >>
//	TimerFlag 
#include "keypad.h"
// getKeypadKey() Function
#include "module_eepromaddresses.h"
// enum EEPROMAddresses
#include "module_alarm.h"
// _alarm struct
// Alarm savedAlarms[]
// unsigned char savedAlarmString[]
// unsigned char savedAlarmIterator
#include "module_timecrunchsm.h"
// eetime_t struct
// time : eetime_t
// TimeCrunchSMTick() Function




//=====================================================================================================================================================================
//Alarm functionality.




//=====================================================================================================================================================================
// Shared Variables ========
enum KeypadButtons { Keypad_next = 'A', Keypad_back = 'B', Keypad_select = 'C', Keypad_delete = 'D', Keypad_menu = '#', Keypad_HH = '*', Keypad_MM = '0' };
enum SystemDriverSMStates { SystemDriver_init,
	SystemDriver_timedisplaytitle, SystemDriver_timedisplay, SystemDriver_timedisplaytitle_fall, SystemDriver_timedisplay_fall, SystemDriver_timedisplaytitle_nextmenuitem,
	SystemDriver_alarmaddtitle, SystemDriver_alarmaddscroll, SystemDriver_alarmaddtitle_fall, SystemDriver_alarmaddscroll_fall, SystemDriver_alarmaddtitle_nextmenuitem,
	SystemDriver_alarmviewtitle,SystemDriver_alarmview, SystemDriver_alarmviewtitle_fall, SystemDriver_alarmview_fall, SystemDriver_alarmview_nextmenuitem,
		SystemDriver_alarmview_HH, SystemDriver_alarmview_MM, 
			SystemDriver_alarmview_HH_fall, SystemDriver_alarmview_MM_fall,
		SystemDriver_alarmview_next, SystemDriver_alarmview_select, SystemDriver_alarmview_back, 
			SystemDriver_alarmview_next_fall, SystemDriver_alarmview_select_fall, SystemDriver_alarmview_back_fall, 
	// Add Alarm Title Screen,					Alarm scroller
	//	Press C for Select and go to scroller	Left (A) or Right (B) to scroll through
SystemDriver_error } SYSTEMSTATE;
extern eetime_t time;
// unsigned char timeString[32] declared in module_timecrunchsm

// -----------------------------------------------------------------------------------------------------------]
unsigned char menuNavigationInput;

// End Shared Variables ====

// State Machine drivers
// SystemDriver State Machine ===================================================================================================================================							
/*  
	State Machine: SystemDriverSM
	Inputs: I/O variables //TODO fill this out
	Outputs: SYSTEMSTATE to drive certain actions. Particularly display.  */
#define MAXMENUTITLETIME 50 //100period ms * 50 = 5000 s seconds
#define FIRSTMENUITEM SystemDriver_timedisplaytitle
signed char SystemDriverSMTick (signed char state){
	static unsigned char stateTimer = 0;
	// Transitions
	switch(state){
		//=================================================================
		case SystemDriver_timedisplaytitle:
			// count the Timer for 4 seconds until forced transition or else "Select button"
			stateTimer++;
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput == Keypad_select){ // c is select
				state = SystemDriver_timedisplaytitle_fall;
			}
			if(menuNavigationInput == Keypad_next){
				state = SystemDriver_timedisplaytitle_nextmenuitem;
			}
			break;
		case SystemDriver_timedisplaytitle_fall:
			// consume input or else transition due to state timer
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput != Keypad_select){
				state = SystemDriver_timedisplay;
			} else if (menuNavigationInput ==  Keypad_next){
				state = SystemDriver_alarmaddtitle;
			}
			stateTimer = 0;
			break;
		case SystemDriver_timedisplay:
			if(menuNavigationInput == Keypad_menu){
				state = SystemDriver_timedisplay_fall;
			}
			break;
		case SystemDriver_timedisplay_fall:
			if(menuNavigationInput != Keypad_menu){
				state = SystemDriver_timedisplaytitle;
			}
			break;
		case SystemDriver_timedisplaytitle_nextmenuitem:
			if(menuNavigationInput !=  Keypad_next){
				state = SystemDriver_alarmaddtitle;
			}
			stateTimer = 0;
			break;
		//=================================================================
		case SystemDriver_alarmaddtitle:
			stateTimer++;
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput == Keypad_select){ // c is select
				state = SystemDriver_alarmaddtitle_fall;
			}else if (menuNavigationInput == Keypad_next){
				state = SystemDriver_alarmaddtitle_nextmenuitem;
			}
			break;
		case SystemDriver_alarmaddtitle_fall:
			// consume input or else transition due to state timer
			if(stateTimer >= MAXMENUTITLETIME || menuNavigationInput != Keypad_select){
				state = SystemDriver_alarmaddscroll;
			} 
			stateTimer = 0;
			break;
		case SystemDriver_alarmaddscroll:
			if(menuNavigationInput == Keypad_menu){
				state = SystemDriver_alarmaddscroll_fall;
			}
			break;
		case SystemDriver_alarmaddscroll_fall:
			if(menuNavigationInput != Keypad_menu){
				state = SystemDriver_alarmaddtitle;
			}
			break;
		case SystemDriver_alarmaddtitle_nextmenuitem:
			if(menuNavigationInput !=  Keypad_next){
				state = SystemDriver_alarmviewtitle;
			}
			stateTimer = 0;
			break;
		//=================================================================
		case SystemDriver_alarmviewtitle :
			if(menuNavigationInput == Keypad_select){ // c is select
				// going to alarm view so load alarms from EEPROM
				loadAlarmsFromEEPROM();
				state = SystemDriver_alarmviewtitle_fall;
			}else if (menuNavigationInput == Keypad_next){
				state = SystemDriver_alarmview_nextmenuitem;
			}
			break;
		case SystemDriver_alarmviewtitle_fall:
			if(menuNavigationInput != Keypad_select){
				state = SystemDriver_alarmview;
			}
			break;
		case SystemDriver_alarmview:
			if(menuNavigationInput == Keypad_menu){
				// leaving state so save Alarms
				saveAlarmsToEEPROM();
				state = SystemDriver_alarmview_fall;
			} else if(menuNavigationInput == Keypad_select){
				state = SystemDriver_alarmview_select;
			} else if(menuNavigationInput == Keypad_back){
				state = SystemDriver_alarmview_back;
			} else if(menuNavigationInput == Keypad_next){
				state = SystemDriver_alarmview_next;
			} else if(menuNavigationInput == Keypad_HH){
				state = SystemDriver_alarmview_HH;
			} else if(menuNavigationInput == Keypad_MM){
				state = SystemDriver_alarmview_MM;
			}
			break;
		// Input HH
		case SystemDriver_alarmview_HH:
			if(savedAlarms[savedAlarmIterator].Hour < 12){
				savedAlarms[savedAlarmIterator].Hour++;
			} else {
				savedAlarms[savedAlarmIterator].Hour = 0;
				savedAlarms[savedAlarmIterator].IsAM = ~savedAlarms[savedAlarmIterator].IsAM;
			}
			state = SystemDriver_alarmview_HH_fall;
			break;
		case SystemDriver_alarmview_HH_fall:
			if(menuNavigationInput != Keypad_HH){
				state = SystemDriver_alarmview;
			}
			break;
		// Input MM
		case SystemDriver_alarmview_MM:
			if(savedAlarms[savedAlarmIterator].Minute < 59){
				savedAlarms[savedAlarmIterator].Minute++;
			} else {
				savedAlarms[savedAlarmIterator].Minute = 0;
			}
			state = SystemDriver_alarmview_MM_fall;
			break;
		case SystemDriver_alarmview_MM_fall:
			if(menuNavigationInput != Keypad_MM){
				state = SystemDriver_alarmview;
			}
			break;
		case SystemDriver_alarmview_fall:
			if(menuNavigationInput != Keypad_menu){
				state = SystemDriver_alarmviewtitle;
			}
			break;
		// Input NEXT
		case SystemDriver_alarmview_next:
			// increment alarm index
			if(savedAlarmIterator < NUMALARMS - 1){
				savedAlarmIterator++;
			}			
			state = SystemDriver_alarmview_next_fall;
			break;
		case SystemDriver_alarmview_next_fall:
			if(menuNavigationInput != Keypad_next){
				state = SystemDriver_alarmview;
			}
			break;
		// Input SELECT
		case SystemDriver_alarmview_select:
			// reverse active for alarm at index
			savedAlarms[savedAlarmIterator].IsActive = ~savedAlarms[savedAlarmIterator].IsActive;
			state = SystemDriver_alarmview_select_fall;
			break;
		case SystemDriver_alarmview_select_fall:
			if(menuNavigationInput != Keypad_select){
				state = SystemDriver_alarmview;
			}
			break;
		// Input BACK
		case SystemDriver_alarmview_back:
			// decrement alarm index
			if(savedAlarmIterator > 0){
				savedAlarmIterator--;
			}
			state = SystemDriver_alarmview_back_fall;
			break;
		case SystemDriver_alarmview_back_fall:
			if(menuNavigationInput != Keypad_back){
				state = SystemDriver_alarmview;
			}
			break;
		case SystemDriver_alarmview_nextmenuitem:
			if(menuNavigationInput != Keypad_next){
				state = FIRSTMENUITEM;
			}
			break;
		//=================================================================
		default:
			state = FIRSTMENUITEM;
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
		|| tmpKeypadInput == '#' || tmpKeypadInput == '*' || tmpKeypadInput == '0' ){
		menuNavigationInput = tmpKeypadInput;
	} else {
		menuNavigationInput = 0;	
	}
	if(ALARMON == ALARMACTIVE && tmpKeypadInput == 'D'){
		ALARMON = ALARMINACTIVE;
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
		case -1:
			state = LCDDisplay_updatelcd; 
			// initialize anything that is going to be displayed and such
			// initializations for clockview			
			for(unsigned char i = 0; i < 32; ++i){
				timeString[i] = ' ';
			}
			//initializations for Alarms
			savedAlarmIterator = 0;
			for(unsigned char i = 0; i < 32; ++i){
				savedAlarmString[i] = ' ';
			}
			break;
	}

	switch(state){
		case LCDDisplay_updatelcd:
			switch(SYSTEMSTATE){
				case SystemDriver_init:
					break;
//=========================================================================================
				case SystemDriver_timedisplaytitle:
				case SystemDriver_timedisplaytitle_fall:
					LCD_DisplayString(1, "     Clock ");
					break;
				case SystemDriver_timedisplay:
				case SystemDriver_timedisplay_fall:
					updateTimeString();
					LCD_DisplayString(1, timeString);
					break;
				case SystemDriver_timedisplaytitle_nextmenuitem:
					break;
//=========================================================================================
				case SystemDriver_alarmaddtitle:
				case SystemDriver_alarmaddtitle_fall:
					LCD_DisplayString(1, "Add Alarm");
					break;
				case SystemDriver_alarmaddscroll:
				case SystemDriver_alarmaddscroll_fall:
					LCD_DisplayString(1, "alarmaddscroll");
					break;
				case SystemDriver_alarmaddtitle_nextmenuitem:
					break;
/*

*/
//=======================================================================================
				case SystemDriver_alarmviewtitle:
				case SystemDriver_alarmviewtitle_fall:
					LCD_DisplayString(1, "     Alarms");
					break;
				case SystemDriver_alarmview_HH:
				case SystemDriver_alarmview_HH_fall:
				case SystemDriver_alarmview_MM:
				case SystemDriver_alarmview_MM_fall:
				case SystemDriver_alarmview:
				case SystemDriver_alarmview_fall:
					updateAlarmString();
					LCD_DisplayString(1, savedAlarmString);
					break;
				// Input NEXT
				case SystemDriver_alarmview_next:
				case SystemDriver_alarmview_next_fall:
				// Input SELECT
				case SystemDriver_alarmview_select:
				case SystemDriver_alarmview_select_fall:
					break;
				// Input BACK
				case SystemDriver_alarmview_back:
				case SystemDriver_alarmview_back_fall:
					break;
				case SystemDriver_alarmview_nextmenuitem:
					break;
				default:
					LCD_DisplayString(1, "Initializing");
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
	unsigned long int CheckAlarmSM_calc = 100; // 100ms period
	// Calculate GCD	
	unsigned long int tmpGCD = 1;
	tmpGCD = findGCD(TimeCrunchSM_calc, LCDDisplaySM_calc );
	tmpGCD = findGCD(tmpGCD, UpdateInputSM_calc);
	tmpGCD = findGCD(tmpGCD, SystemDriverSM_calc);
	tmpGCD = findGCD(tmpGCD, CheckAlarmSM_calc);
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
	//tmpGCD = findGCD(tmpGCD, ... );
		
	unsigned long int GCD = tmpGCD;
	
	// Recalculate GCD periods for scheduler
	unsigned long int TimeCrunchSM_period = TimeCrunchSM_calc/GCD;
	unsigned long int LCDDisplaySM_period = LCDDisplaySM_calc/GCD;
	unsigned long int UpdateInputSM_period = UpdateInputSM_calc/GCD;
	unsigned long int SystemDriverSM_period = SystemDriverSM_calc/GCD;
	unsigned long int CheckAlarmSM_period = SystemDriverSM_calc/GCD;
	// Set up task scheduler
	static task TimeCrunchSMTask,
				LCDDisplaySMTask,
				UpdateInputSMTask,
				SystemDriverSMTask,
				CheckAlarmSMTask;
	// keep display at the end, checkalarm after TimeCrunchSM. keep UpdateInput in the beginning and SystemDriverSM second
	task *tasks[] = {&UpdateInputSMTask, &SystemDriverSMTask, &TimeCrunchSMTask, &CheckAlarmSMTask, &LCDDisplaySMTask}; 
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
	
	//CheckAlarmSM declaration
	CheckAlarmSMTask.state = -1;
	CheckAlarmSMTask.period = CheckAlarmSMTask.elapsedTime = CheckAlarmSM_period;
	CheckAlarmSMTask.TickFct = &CheckAlarmSMTick;
	// HW initializations ======================================================
	// Set timer and turn it on
	TimerSet(GCD);
	TimerOn();
	// Start LCD
	LCD_init();
	// MEM initializations ======================================================
	initializeSavedAlarms(); // zero saved Alarms 
	//loadAlarmsFromEEPROM();  // load Alarms if previously saved.
	
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