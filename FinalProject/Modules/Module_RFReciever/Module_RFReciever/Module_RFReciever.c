/*
 * Module_RFReciever.c
 *
 * Created: 8/26/2015 1:55:27 PM
 *  Author: user
 */ 


#include <avr/io.h>
/*
 * mting005_lab1_part5.cpp
 *
 * Created: 7/28/2015 1:05:23 PM
 *  Author: user
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"

#define SIGNALINPUT (~PINA&0x01)
#define SIGNALHIGH 0x01
#define SIGNALLOW 0x00

// When signal is high, it will submit 5 pulses per second
#define SIGNALTHRESH 4



unsigned char SignalFrequency;
unsigned char RFSignalIn;

enum ResetSignalCountSMStates { ResetSignalCount_reset };
signed char RSCState = -1;
unsigned char MAXFREQ = 0;
void ResetSignalCountSMTick(){
		switch(RSCState){
			case ResetSignalCount_reset:
				if(SignalFrequency > MAXFREQ){
					MAXFREQ = SignalFrequency;
				}
				//PORTB = SignalFrequency;
				if(SignalFrequency < SIGNALTHRESH){
					PORTB |= 0x02;
					RFSignalIn = SIGNALHIGH;
				}else{
					PORTB &= 0x01;
					RFSignalIn = SIGNALLOW;
				}
				
				SignalFrequency = 0;
				break;
			default:
				RSCState = ResetSignalCount_reset;
				break;
		}
}

enum CountSignalSMStates {CountSignal_waitforsignal, CountSignal_signalrecieved, CountSignal_signalrecievedfall};
signed char CSState = -1;
void CountSignalSMTick(){
	switch(CSState){
		case CountSignal_waitforsignal:
			if(SIGNALINPUT == SIGNALHIGH){
				CSState = CountSignal_signalrecieved;
			}
			break;
		case CountSignal_signalrecieved:
			CSState = CountSignal_signalrecievedfall;
			break;
		case CountSignal_signalrecievedfall:
			if(SIGNALINPUT == SIGNALLOW){
				CSState = CountSignal_waitforsignal;
			}
			break;
		default:
			CSState = CountSignal_waitforsignal;
			SignalFrequency = 0;
			RFSignalIn = 0;
			break;
	}
	switch(CSState){
		case CountSignal_waitforsignal:
			break;
		case CountSignal_signalrecieved:
			SignalFrequency++;
			break;
		case CountSignal_signalrecievedfall:
			break;
		default:
			CSState = CountSignal_waitforsignal;
	}
}


void DriveWheels(){
	//			   \/ M2
	// sets 0000 0101
	//			 ^ M1
	PORTC = (PORTC & 0xF0) | 0x05;
	PORTB |= 0x01;
}
void StopDrivewheels(){
	PORTC = PORTC & 0xF0;
	PORTB &= 0xFE;
}

enum DriveCarSMStates { DriveCar_wait, DriveCar_drive };
signed char DriverCarSMstate = -1;
void DriveCarSMTick(){
	switch(DriverCarSMstate){
		case DriveCar_wait:
			if(RFSignalIn == SIGNALHIGH){
				DriverCarSMstate = DriveCar_drive;
			}
			break;
		case DriveCar_drive:
			if(RFSignalIn == SIGNALLOW){
				DriverCarSMstate = DriveCar_wait;
			}
			break;
		default:
			DriverCarSMstate = DriveCar_wait;
	}
	switch(DriverCarSMstate){
		case DriveCar_wait:
			StopDrivewheels();
			break;
		case DriveCar_drive:
			DriveWheels();
			break;
		default:
			break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;

	TimerOn();
	TimerSet(1);
	unsigned long counter = 0;
	unsigned long counter2 = 0;
	CountSignalSMTick();
	ResetSignalCountSMTick();
	while(1){
		if(counter == 30){
			CountSignalSMTick();
			DriveCarSMTick();
			counter = 0;
		}
		if(counter2 == 800){
			ResetSignalCountSMTick();
			counter2 = 0;
		}
		counter++;
		counter2++;
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
