#ifndef MODULE_ALARM_H
#define MODULE_ALARM_H
#include "pwm.h"
/*
void set_PWM(double);
void PWM_on();
void PWM_off();
*/

#include "module_eepromaddresses.h"
// NUMALARMS definition
// NUMALARMELEMENTS definition
// EEPROMAddresses enum

#include "module_timecrunchsm.h"
// extern time and eetime_t struct

#define ALARMVIEWNUMBERINDEXTENS 7
#define ALARMVIEWNUMBERINDEXONES 8
#define ALARMVIEWONOFFINDEX 12
#define ALARMVIEWHOURSINDEX 17
#define ALARMVIEWCOLONINDEX 19
#define ALARMVIEWMINUTESINDEX 20
#define ALARMVIEWAMPMINDEX 23

#define ALARMACTIVE 0xFF
#define ALARMINACTIVE 0x00

// Alarm struct
typedef struct _alarm{
	unsigned char Hour;
	unsigned char Minute;
	unsigned char IsAM;
	unsigned char IsActive;
} Alarm;

Alarm savedAlarms[NUMALARMS];

unsigned char savedAlarmIterator;
unsigned char savedAlarmString[32];

unsigned char activatedAlarm;

extern eetime_t time;

volatile unsigned char ALARMON;


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
	if(eeprom_read_byte((uint8_t*)(EEPROM_ALARM_TOPADDRESS)) == EEPROM_ALARM_TOPADDRESS){
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
		eeprom_write_byte((uint8_t*)(EEPROM_ALARM_TOPADDRESS), EEPROM_ALARM_TOPADDRESS);
	}
	for(unsigned char i = 0; i < NUMALARMS; ++i){
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i), savedAlarms[i].Hour);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+1), savedAlarms[i].Minute);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+2), savedAlarms[i].IsAM);
		eeprom_write_byte((uint8_t*) (NUMALARMELEMENTS*i+3), savedAlarms[i].IsActive);
	}
}

void disableAlarm(){
	ALARMON = ALARMINACTIVE;
	savedAlarms[activatedAlarm].IsActive = ALARMINACTIVE;
	saveAlarmsToEEPROM();
}

// ensures that ALARMON will be INACTIVE until it is turned on
// While any alarm is inactive, it will return ALARMACTIVE until
// an alarm is triggered and return ALARMACTIVE. After that, it will
// return ALARMACTIVE until an outside force resets ALARMON to ALARMINACTIVE
void checkAlarms(){
	static unsigned char i;
	if(ALARMON == ALARMINACTIVE){
		for(i = 0; i < NUMALARMS; ++i){						// go through all alarms
			if(	(savedAlarms[i].IsActive == ALARMACTIVE) &&  // if active
				(savedAlarms[i].Hour == time.Hour) &&		// and matches current time,
				(savedAlarms[i].Minute == time.Minute) &&
				(savedAlarms[i].IsAM == time.IsAM)){
				// ALARM!!
				ALARMON = ALARMACTIVE;						// SET ALARMON = ALARMACTIVE
				activatedAlarm = i;		
			}
		}
	}
}


void updateAlarmString(){
	static Alarm* currAlarm;
	currAlarm = &savedAlarms[savedAlarmIterator];
	savedAlarmString[0] = '>';
	savedAlarmString[1] = 'A';
	savedAlarmString[2] = 'L';
	savedAlarmString[3] = 'A';
	savedAlarmString[4] = 'R';
	savedAlarmString[5] = 'M';

	savedAlarmString[ALARMVIEWNUMBERINDEXTENS] = '0' + (savedAlarmIterator+1)/10;
	savedAlarmString[ALARMVIEWNUMBERINDEXONES] = '0' + (savedAlarmIterator+1)%10;
	if(currAlarm->IsActive == ALARMINACTIVE){
		savedAlarmString[ALARMVIEWONOFFINDEX] = 'O';
		savedAlarmString[ALARMVIEWONOFFINDEX+1] = 'F';
		savedAlarmString[ALARMVIEWONOFFINDEX+2] = 'F';
		
		} else{
		savedAlarmString[ALARMVIEWONOFFINDEX] = ' ';
		savedAlarmString[ALARMVIEWONOFFINDEX+1] = 'O';
		savedAlarmString[ALARMVIEWONOFFINDEX+2] = 'N';
	}
	savedAlarmString[ALARMVIEWHOURSINDEX] = '0'+ currAlarm->Hour/10;
	savedAlarmString[ALARMVIEWHOURSINDEX+1] = '0'+ currAlarm->Hour%10;
	savedAlarmString[ALARMVIEWCOLONINDEX] = ':';
	savedAlarmString[ALARMVIEWMINUTESINDEX] = '0'+currAlarm->Minute/10;
	savedAlarmString[ALARMVIEWMINUTESINDEX+1] = '0'+currAlarm->Minute%10;
	if(currAlarm->IsAM == 0x00){
		//PM
		savedAlarmString[ALARMVIEWAMPMINDEX] = 'P';
	}else{
		//AM
		savedAlarmString[ALARMVIEWAMPMINDEX] = 'A';
	}
	savedAlarmString[ALARMVIEWAMPMINDEX+1] = 'M';
}

void AlarmOn(){
	PWM_on();
	set_PWM(261.63);
}
void AlarmOff(){
	PWM_off();
}

enum CheckAlarmSMStates { CheckAlarm_checkcurrenttime, CheckAlarm_AlarmOn };
signed char CheckAlarmSMTick(signed char state){
	switch(state){
		case CheckAlarm_checkcurrenttime:
			if(minuteTickFlag == 0xFF){
				checkAlarms();
				if( ALARMON == ALARMACTIVE){
					// activate alarm mechanism here!
					AlarmOn();
					state = CheckAlarm_AlarmOn;
				}
				minuteTickFlag = 0x00;					// must reset minuteTickFlag
			}
			break;
		case CheckAlarm_AlarmOn:
			if(minuteTickFlag == 0xFF){
				minuteTickFlag = 0x00;
			}
			if(ALARMON == ALARMINACTIVE){ // ALARMON disabled by button
				disableAlarm();			  // disable the active alarm and change state to check time
				AlarmOff();
				state = CheckAlarm_checkcurrenttime;
			}
			break;
		default:
			ALARMON = ALARMINACTIVE;
			state = CheckAlarm_checkcurrenttime;
		break;
	}
	return state;
}


#endif