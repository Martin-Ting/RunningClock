/*
 * mting005_lab1_part5.cpp
 *
 * Created: 7/28/2015 1:05:23 PM
 *  Author: user
 */ 

#include <avr/interrupt.h>
#include <avr/io.h>
#include "timer.h"

#define SIGNALINPUT (~PIND&0x01)
#define SIGNALHIGH 0x01
#define SIGNALLOW 0x00

unsigned char SignalFrequency;
enum ResetSignalCountSMStates { ResetSignalCount_reset };
signed char RSCState = -1;
unsigned char MAXFREQ = 0;
void ResetSignalCountSMTick(){
		switch(RSCState){
			case ResetSignalCount_reset:
				if(SignalFrequency > MAXFREQ){
					MAXFREQ = SignalFrequency;
				}
				PORTB = MAXFREQ;
				
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

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFE; PORTD = 0x01;

	TimerOn();
	TimerSet(1);
	unsigned long counter = 0;
	unsigned long counter2 = 0;
	CountSignalSMTick();
	ResetSignalCountSMTick();
	while(1){
		if(counter == 1){
			CountSignalSMTick();
			counter = 0;
		}
		if(counter2 == 1000){
			ResetSignalCountSMTick();
			counter2 = 0;
		}
		counter++;
		counter2++;
		while(!TimerFlag);
		TimerFlag = 0;
	}
}