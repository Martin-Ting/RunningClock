#ifndef MODULE_TIMECRUNCHSM_H
#define MODULE_TIMECRUNCHSM_H

#define HOURINDEX 0
//		:		at 2
#define MINUTEINDEX 3
//		:		at 5
#define SECONDINDEX 6
//	   <sp>     at 8
#define AMPMINDEX 9

//Time struct
typedef struct {
	// Time has Hours Minutes Seconds and an AM/PM flag
	// It is used for representing time
	// particularly displaying time on the screen
	unsigned char Hour;
	unsigned char Minute;
	unsigned char Second;
	unsigned char IsAM;
} eetime_t; // should only be used for system time. <time> variable

//Shared Variables
eetime_t time;
unsigned char timeString[32];
unsigned char minuteTickFlag;

void updateTimeString(){
	timeString[2] = timeString[5] = ':';
	if(time.Hour >= 10){// Update Hour
		timeString[HOURINDEX] = '0'+(time.Hour/10);
		}else{
		timeString[HOURINDEX] = '0';
	}
	timeString[HOURINDEX+1] = '0'+(time.Hour % 10);
	if(time.Minute >= 10){// Update Minute
		timeString[MINUTEINDEX] = '0'+(time.Minute/10);
		}else{
		timeString[MINUTEINDEX] = '0';
	}
	timeString[MINUTEINDEX+1]='0'+(time.Minute % 10);
	if(time.Second >= 10){// Update Second
		timeString[SECONDINDEX] = '0'+(time.Second/10);
	}else{
		timeString[SECONDINDEX] = '0';
	}
	timeString[SECONDINDEX+1]='0'+(time.Second % 10);
	
	if(time.IsAM != 0){
		timeString[AMPMINDEX] = 'A';
	}else{
		timeString[AMPMINDEX] = 'P';
	}
}
// State Machine
enum TimeCrunchSMStates { TimeCrunch_updatetime };
/*  
	State Machine: TimeCrunchSM
	Inputs: N/A
	Outputs: time.Hours, time.Minutes, time.Seconds */
signed char TimeCrunchSMTick (signed char state){
	//Transitions
	switch(state){
		case TimeCrunch_updatetime:
			break;
		default:
			time.Hour = 11;
			time.Minute = 58;
			time.Second = 50;
			time.IsAM = 0xFF;
			minuteTickFlag = 0x00;
			state =  TimeCrunch_updatetime;
			break;
	}
	switch(state){
		case TimeCrunch_updatetime:
			if(time.Second == 59){	// overflow -> minute
				if(time.Minute == 59){	// overflow -> hours
					if(time.Hour == 11){ // before incrementing Hour to 12, change flip the day!
						time.IsAM = ( (time.IsAM != 0) ? 0x00 : 0xFF);	//change AMPM
					}
					if(time.Hour == 12){ // overflow -> Day/Night
						time.Hour = 0;
					}else {	// update Hour
						time.Hour++;
					}
					time.Minute = 0;
				} else { // update Minutes
					time.Minute++;
				}
				// Seconds being reset to 0 means the top of a minute.
				minuteTickFlag = 0xFF;
				time.Second = 0;
			} else { // update Seconds
				time.Second++;
			}
			break;
		default:

			break;
	}
	return state;
}
#endif 