/*
 * mting005_lab5_ex001.c
 *
 * Created: 8/5/2015 11:55:45 AM
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>

// Timer =============================================================================
//TimerISR() sets this to 1. C Programmer should clear to 0
volatile unsigned char TimerFlag = 0;
//Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1;		  // start count from here, down to 0. Default 1 ms.
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1 ms ticks

void TimerOn(){
	// AVR timer/counter controller register TCCR1
	// bit3 = 0: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: pre-scaler /64
	// 00001011: 0x0B
	// SO, 8MHz clock or 8,000,000/64 = 125,000 ticks/s
	// Thus, TCNT1register will count at 125,000 tick/s
	TCCR1B = 0x0B;
	
	// AVR output compare register OCR1A
	// Timer interrupt will be gnereated when TCNT1==OCR1A
	// We want a 1ms tick. 0.01 s * 125,000 tick/s = 125
	// So when TCNT1 register equals 125,
	// 1ms has passed. Thus, we compare to 125.
	OCR1A=125;
	
	// AVR timer interrupt mask register
	// bit1: OC1E1A -- enables compare match intterupt
	TIMSK1 = 0x02;
	// Initialize avr counter
	TCNT1=0;
	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;
	// Enable global interrupts: 0x80: 10000000
	SREG |= 0x80;
}
void TimerOff(){
	TCCR1B=0x00;
}
void TimerISR(){
	TimerFlag = 1;
}

ISR(TIMER1_COMPA_vect){
	// CPU automatically calls when TCNT1 == OCR1
	// (every 1ms per TimerOn settings)
	
	// Count down to 0 rather than up to TOP (results in a more efficient comparison)
	_avr_timer_cntcurr--;
	if(_avr_timer_cntcurr == 0){
		// call the isr that the user uses
		TimerISR();
		_avr_timer_cntcurr = _avr_timer_M;
	}
}
//
// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M){
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}
// =============================================================================
enum ThreeTickLEDStates {TLINIT, ONE, TWO, THREE } StateTL;
unsigned char threeLED;
void tickThreeLED(){
	switch (StateTL){
		case TLINIT:
			threeLED = 0x01;
			break;
		case ONE:
			threeLED = 0x01;
			break;
		case TWO:
			threeLED = 0x02;
			break;
		case THREE:
			threeLED = 0x04;
			break;
		default:
			break;
	}
	switch (StateTL){
		case TLINIT:
			StateTL = ONE;
			break;
		case ONE:
			StateTL = TWO;
			break;
		case TWO:
			StateTL = THREE;
			break;
		case THREE:
			StateTL = ONE;
			break;
		default:
			StateTL = TLINIT;
			break;
	}
}
enum BlinkLEDStates {BLINIT, ON, OFF} StateBL;
unsigned char blinkLED;
void tickBlinkLED(){
	switch (StateBL){
		case BLINIT:
			blinkLED = 0x08;
			break;
		case ON:
			blinkLED = 0x08;
			break;
		case OFF:
			blinkLED = 0x00;
			break;
		default:
			break;
	};
	switch (StateBL){
		case BLINIT:
			StateBL = ON;
			break;
		case ON:
			StateBL = OFF;
			break;
		case OFF:
			StateBL = ON;
			break;
		default:
			break;
	};
}

unsigned char outputLED;
enum SpeakerStates { SPINIT, SPON, SPOFF } StateSP;
unsigned char speakerStatus;
unsigned char speakerFreq;
void tickSpeaker(){
	unsigned char bInput = ~PINA & 0x01;
	switch(StateSP){
		case SPINIT:
			speakerStatus = 0x00;
			break;
		case SPON:
			speakerStatus = 0x10;
			break;
		case SPOFF:
			speakerStatus = 0x00;
			break;
	}
	switch(StateSP){
		case SPINIT:
			StateSP = SPOFF;
			break;
		case SPON:
			StateSP = SPOFF;
			break;
		case SPOFF:
			if(bInput != 0){
				StateSP = SPON;
			}
		break;
	}
	//PORTB = outputLED | speakerStatus;
}
enum FreqChangerStates { FCINIT, CHECKINPUTS } StateFC;
void tickFreqChanger(){
	unsigned char bLowFreq = ~PINA & 0x02;
	unsigned char bHighFreq = ~PINA & 0x04;
	unsigned char bExtra = ~PINA & 0x08;
	unsigned char bLol = ~PINA & 0x10;
	unsigned char bSuperLowFreq = ((bLowFreq != 0 && bHighFreq != 0) ? 1 : 0);
	
	switch(StateFC){
		case FCINIT:
			StateFC = CHECKINPUTS;
			break;
		case CHECKINPUTS:
			if(bHighFreq != 0){
				speakerFreq = 3;
			}else if(bLowFreq != 0){
				speakerFreq = 6;
			}else if(bExtra != 0){
				speakerFreq = 2;
			} else if (bLol != 0){
				speakerFreq = 1;
			} else {
				speakerFreq = 4;
			}
			break;
		default:
			break;
	};
}
enum OutputStates {OPINIT, OUTPUT} StateOP;
unsigned char outputLED;
void tickOutput(){
	switch(StateOP){
		case OPINIT:
			StateOP = OUTPUT;
			break;
		case OUTPUT:
			break;
		default:
			break;
	}
	switch(StateOP){
		case OPINIT:
			outputLED = 0x00;
			break;
		case OUTPUT:
			outputLED = threeLED | blinkLED | speakerStatus;
			break;
		default:
			break;
	}
	PORTB = outputLED;
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1);
	TimerOn();
	StateBL = BLINIT;
	StateTL = TLINIT;
	StateOP = OPINIT;
	StateSP = SPINIT;
	StateFC = FCINIT;
	static unsigned char counterSP = 0;
	static unsigned short counterTB = 0;
	static unsigned short counterTL = 0;
	speakerStatus = 0;
	speakerFreq = 3;
    while(1)
    {
		tickFreqChanger();
		if(counterSP++ >= speakerFreq){//% 2 == 0){
			tickSpeaker();
			counterSP = 0;
		}
		if(counterTB++ >= 1000){//% 300 == 0){
			tickBlinkLED();
			counterTB = 0;
		}
		if(counterTL++ >= 300){//% 1000 == 0){
			tickThreeLED();
			counterTL = 0;
		}
		tickOutput();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}