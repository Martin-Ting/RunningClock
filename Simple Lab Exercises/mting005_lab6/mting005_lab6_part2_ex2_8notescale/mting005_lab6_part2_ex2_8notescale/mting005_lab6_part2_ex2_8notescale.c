/*
 * mting005_lab6_ex001.c
 *
 * Created: 8/10/2015 11:55:38 AM
 *  Author: user
 */ 


#include <avr/io.h>
#include<avr/interrupt.h>
// ADC
void ADC_init();
// Timer
//TimerISR() sets this to 1. C Programmer should clear to 0
volatile unsigned char TimerFlag = 0;
// Timer =============================================================================
//TimerISR() sets this to 1. C Programmer should clear to 0
//volatile unsigned char TimerFlag = 0;
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

// PWM
void set_PWM(double);
void PWM_on();
void PWM_off();

#define INCBUTTON ~PINA & 0x01 // PA0
#define ONBUTTON  ~PINA & 0x02 // PA1
#define DECBUTTON ~PINA & 0x04 // PA2
#define PITCHSCALESIZE 8
									//0			1		2		3		4		5		6		7
double pitchScale[PITCHSCALESIZE] = {261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.25 };

enum SMChoosePitchStates { PITCH_INIT, PITCH_WAIT, PITCH_INC, PITCH_DEC, PITCH_INCFALL, PITCH_DECFALL} StatePitch;
unsigned char currPitch;
void SMChoosePitch_tick(){
	unsigned char bIncInput = INCBUTTON;
	unsigned char bDecInput = DECBUTTON;
	switch(StatePitch){
		case PITCH_INIT:
			//PORTD = 0x00;
			currPitch = PITCHSCALESIZE/2;
			break;
		case PITCH_WAIT:
			//PORTD = 0x0F;
			break;
		case PITCH_INC:
			if(currPitch < PITCHSCALESIZE - 1){
				currPitch++;
			}
			break;
		case PITCH_DEC:
			if(currPitch > 0){
				currPitch--;
			}
			break;
		case PITCH_INCFALL:
			//PORTD = 0x01;
			break;
		case PITCH_DECFALL:
			//PORTD = 0x02;
			break;
		default:
			break;
	}
	switch(StatePitch){
		case PITCH_INIT:
			StatePitch = PITCH_WAIT;
			break;
		case PITCH_WAIT:
			if(bIncInput != 0 && bDecInput == 0){
				StatePitch = PITCH_INC;
			}else if(bDecInput != 0 && bIncInput == 0){
				StatePitch = PITCH_DEC;
			}
			break;
		case PITCH_INC:
			StatePitch = PITCH_INCFALL;
			break;
		case PITCH_DEC:
			StatePitch = PITCH_DECFALL;
			break;
		case PITCH_INCFALL:
			if(bIncInput == 0 && bDecInput == 0){
				StatePitch = PITCH_WAIT;
			}
			break;
		case PITCH_DECFALL:
			if(bIncInput == 0 && bDecInput == 0){ // her is the weird place where flipping the conditions will make it stay stuck here.
				StatePitch = PITCH_WAIT;
			}
			break;
		default:
			break;
	}
	//PORTB = currPitch;
}
enum SMSpeakerStates { SPEAKER_INIT, SPEAKER_OFF, SPEAKER_ON, SPEAKER_OFF_WAIT, SPEAKER_ON_WAIT } StateSpeaker;
void SMSpeaker_tick(){
	unsigned char bOnInput = ONBUTTON;
	switch(StateSpeaker){
		case SPEAKER_INIT:
			break;
		case SPEAKER_OFF:
			PORTD = 0x01;
			PWM_off();
			break;
		case SPEAKER_ON:
			PORTD = 0x02;
			PWM_on();
			set_PWM(pitchScale[currPitch]);
			break;
		case SPEAKER_OFF_WAIT:
			PORTD = 0x04;
			break;
		case SPEAKER_ON_WAIT:
			PORTD = 0x08;
			set_PWM(pitchScale[currPitch]);
			break;
		default:
			break;
	}
	switch(StateSpeaker){
		case SPEAKER_INIT:
			StateSpeaker = SPEAKER_OFF_WAIT;
			break;
		case SPEAKER_OFF:
			if(bOnInput == 0){
				StateSpeaker = SPEAKER_OFF_WAIT;
			}
			break;
		case SPEAKER_ON:
			if(bOnInput == 0){
				StateSpeaker = SPEAKER_ON_WAIT;
			}
			break;
		case SPEAKER_OFF_WAIT:
			if(bOnInput != 0){
				StateSpeaker = SPEAKER_ON;
			}
			break;
		case SPEAKER_ON_WAIT:
			if(bOnInput != 0){
				StateSpeaker = SPEAKER_OFF;
			}
		default:
			break;
	}
	return 0;
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x0F;
	//PWM_on();
	TimerSet(500);
	TimerOn();
	StateSpeaker = SPEAKER_INIT;
	StatePitch = PITCH_INIT;
	TimerSet(100);
	TimerOn();

	//PWM_on();
    while(1)
    {
		//set_PWM(pitchScale[5]);
		SMSpeaker_tick();
		SMChoosePitch_tick();
		while(!TimerFlag);
		TimerFlag = 0;	
    }
}

void ADC_init(){
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	/* ADEN:  Setting this bit enables analog-to-digital conversion.
	   ADSC:  Setting this bit starts the first conversion.
	   ADATE: Setting this bit enables auto-triggering. Since we are in
	  		  Free Running Mode, a new conversion will trigger whenever
	  		  the previous conversion completes.
	*/
}



//NOTE*** THIS NEW CODE TARGETS PB6 NOT PB3

void set_PWM(double frequency) {
	// Keeps track of the currently set frequency
	// Will only update the registers when the frequency
	// changes, plays music uninterrupted.
	static double current_frequency;
	if (frequency != current_frequency) {

		if (!frequency) TCCR3B &= 0x08; //stops timer/counter
		else TCCR3B |= 0x03; // resumes/continues timer/counter
		
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) OCR3A = 0xFFFF;
		
		// prevents OCR3A from underflowing, using prescaler 64					// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) OCR3A = 0x0000;
		
		// set OCR3A based on desired frequency
		else OCR3A = (short)(8000000 / (128 * frequency)) - 1;

		TCNT3 = 0; // resets counter
		current_frequency = frequency;
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}

void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}