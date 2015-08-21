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
#include "module_timecrunchsm.h"
// eetime_t struct
// time : eetime_t
// TimeCrunchSMTick() Function
#include "keypad.h"
// getKeypadKey() Function

// EEPROMAdddresses 
#define NUMALARMS EEPROM_ALARM_TOPADDRESS/3
#define NUMALARMELEMENTS 4
enum EEPROMAddresses { _addrAlarm1Hour, _addrAlarm1Minute, _addr1IsAM, _addr1IsActive,
				  	   _addrAlarm2Hour, _addrAlarm2Minute, _addr2IsAM, _addr2IsActive,
					   _addrAlarm3Hour, _addrAlarm3Minute, _addr3IsAM, _addr3IsActive,
					   _addrAlarm4Hour, _addrAlarm4Minute, _addr4IsAM, _addr4IsActive,
					   _addrAlarm5Hour, _addrAlarm5Minute, _addr5IsAM, _addr5IsActive,
					   EEPROM_ALARM_TOPADDRESS,
					   EEPROM_TOPADDRESS };

//=====================================================================================================================================================================
//Alarm functionality.
// Alarm struct
typedef struct _alarm{
	unsigned char Hour;
	unsigned char Minute;
	unsigned char IsAM;
	unsigned char IsActive;
} Alarm;

Alarm savedAlarms[NUMALARMS];
// zero savedAlarms should only be called on startup
void initializeSavedAlarms(){
	for(unsigned char i = 0; i < NUMALARMS; ++i){
		savedAlarms[i].Hour = 0;
		savedAlarms[i].Minute = 0;
		savedAlarms[i].IsAM = 0;
		savedAlarms[i].IsActive = 0; // 0x00 for inactive 0xFF for active. ~
	}
}

// fill savedAlarms with EEPROM data. Should really only be called on startup. any updates to savedalarms will be immediately represented in EEPROM
void loadAlarmsFromEEPROM(){
	if(eeprom_read_byte((uint8_t)(EEPROM_ALARM_TOPADDRESS)) == EEPROM_ALARM_TOPADDRESS){
		// when this is false, it means that there is trash in the memory!
		for(unsigned char i = 0; i < NUMALARMS; ++i){	// These addresses must be read in this order in correspondance with EEPROMAddresses enum.
			savedAlarms[i].Hour = eeprom_read_byte((uint8_t*) (NUMALARMELEMENTS*i));
			savedAlarms[i].Minute = eeprom_read_byte((uint8_t*) (NUMALARMELEMENTS*i+1));
			savedAlarms[i].IsAM = eeprom_read_byte((uint8_t*) (NUMALARMELEMENTS*i+2));
			savedAlarms[i].IsActive = eeprom_read_byte((uint8_t*) (NUMALARMELEMENTS*i+3));
		}
	}
}
// save savedAlarms contents to EEPROM. This will be called everytime an Alarm is updated.
void saveAlarmsToEEPROM(){
	if (eeprom_read_byte((uint8_t*)(EEPROM_ALARM_TOPADDRESS)) != EEPROM_ALARM_TOPADDRESS){
		// store EEPROM_ALARM_TOPADDRESS at memory address EEPROM_ALARM_TOPADDRESS as a flag that the memory has been saved to already. 
		// This will persist through resets 
		eeprom_save_byte((uint8_t*)(EEPROM_ALARM_TOPADDRESS), EEPROM_ALARM_TOPADDRESS);
	}
	for(unsigned char i = 0; i < NUMALARMS; ++i){
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i), savedAlarms[i].Hour);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+1), savedAlarms[i].Minute);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+2), savedAlarms[i].IsAM);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+3), savedAlarms[i].IsActive);
	}
}

//=====================================================================================================================================================================
// Shared Variables ========
enum KeypadButtons { Keypad_next = 'A', Keypad_back = 'B', Keypad_select = 'C', Keypad_delete = 'D', Keypad_menu = '#' };
enum SystemDriverSMStates { SystemDriver_init,
	SystemDriver_timedisplaytitle, SystemDriver_timedisplay, SystemDriver_timedisplaytitle_fall, SystemDriver_timedisplay_fall, SystemDriver_timedisplaytitle_nextmenuitem,
	SystemDriver_alarmaddtitle, SystemDriver_alarmaddscroll, SystemDriver_alarmaddtitle_fall, SystemDriver_alarmaddscroll_fall, SystemDriver_alarmaddtitle_nextmenuitem,
	SystemDriver_alarmviewtitle,SystemDriver_alarmview, SystemDriver_alarmviewtitle_fall, SystemDriver_alarmview_alarmview_fall, 
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
				state = SystemDriver_timedisplaytitle;
			}
			stateTimer = 0;
			break;
		//=================================================================
		case SystemDriver_alarmviewtitle :
			break;
		case SystemDriver_alarmviewtitle_fall:
			break;
		case SystemDriver_alarmview:
			break;
		case SystemDriver_alarmview_alarmview_fall:
			break;
		// Input NEXT
		case SystemDriver_alarmview_next:
			break;
		case SystemDriver_alarmview_next_fall:
			break;
		// Input SELECT
		case SystemDriver_alarmview_select:
			break;
		case SystemDriver_alarmview_select_fall:
			break;
		// Input BACK
		case SystemDriver_alarmview_back:
			break;
		case SystemDriver_alarmview_back_fall:
			break;
		
		//=================================================================
		default:
			state = SystemDriver_timedisplaytitle;
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
					PORTB = 0x01;
					LCD_DisplayString(1, "Clock");
					break;
				case SystemDriver_timedisplay:
				case SystemDriver_timedisplay_fall:
				PORTB = 0x08;
					updateTimeString();
					LCD_DisplayString(1, timeString);
					break;
					case SystemDriver_alarmaddtitle:
					LCD_DisplayString(1, "Add Alarm");
					break;
				case SystemDriver_alarmaddscroll:
					LCD_DisplayString(1, "alarmaddscroll");
					break;
/*
	SystemDriver_alarmviewtitle,SystemDriver_alarmview, SystemDriver_alarmviewtitle_fall, SystemDriver_alarmview_alarmview_fall,
	SystemDriver_alarmview_next, SystemDriver_alarmview_select, SystemDriver_alarmview_back,
	SystemDriver_alarmview_next_fall, SystemDriver_alarmview_select_fall, SystemDriver_alarmview_back_fall,
*/
				case SystemDriver_alarmviewtitle:
				case SystemDriver_alarmviewtitle_fall:
					break;
				case SystemDriver_alarmview:
				case SystemDriver_alarmview_alarmview_fall:
					
					break;
				/*// Input NEXT
				case SystemDriver_alarmview_next:
				case SystemDriver_alarmview_next_fall:
					break;
				// Input SELECT
				case SystemDriver_alarmview_select:
				case SystemDriver_alarmview_select_fall:
					break;
				// Input BACK
				case SystemDriver_alarmview_back:
				case SystemDriver_alarmview_back_fall:
					break;*/
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
	
	// HW initializations ======================================================
	// Set timer and turn it on
	TimerSet(GCD);
	TimerOn();
	// Start LCD
	LCD_init();
	// MEM initializations ======================================================
	initializeSavedAlarms(); // zero saved Alarms 
	loadAlarmsFromEEPROM();  // load Alarms if previously saved.
	
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