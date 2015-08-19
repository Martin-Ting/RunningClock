/*
 * mting005_lab6_ex001.c
 *
 * Created: 8/10/2015 11:55:38 AM
 *  Author: user
 */ 


#include <avr/io.h>

void ADC_init();

void set_PWM(double);
void PWM_on();
void PWM_off();

enum ADC_SMStates {INIT, READADC, OUTPUTLED } ADCState;
void ADC_tick(){
	static unsigned short currentADC;
	static unsigned short minADC;
	static unsigned short maxADC;
	static unsigned char outputPORTB;
	static unsigned char outputPORTD;
	
	switch(ADCState){
		case INIT:
			currentADC = 0;
			outputPORTB = 0;
			outputPORTD = 0;
			minADC = 0xFFFF;
			maxADC = 0x0000;
			break;
		case READADC:
			currentADC = ADC;
			if(currentADC < minADC){
				minADC = currentADC;
			}
			if(currentADC > maxADC){
				maxADC = currentADC;
			}
			break;
		case OUTPUTLED:
			currentADC = minADC;
			//shift bits
			outputPORTB = currentADC & 0x00FF;
			// mask top 8 bits of currentADC then shift over 2 bytes
			// mask out first two bits of the shifted value for output to two LEDs
			outputPORTD = ((currentADC & 0x0300) >> 8) & 0x03; 
			break;
		default:
			break;
	};
	PORTB = outputPORTB;
	PORTD = outputPORTD;
	switch(ADCState){
		case INIT:
			ADCState = READADC;
			break;
		case READADC:
			ADCState = OUTPUTLED;
			break;
		case OUTPUTLED:
			ADCState= READADC;
			break;
		default:
			break;
	}
}


int main(void)
{
	ADC_init();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x0F;
	while(1){
		ADC_tick();
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
/*
	0.954 hz is the lowest frequency possible with thisfunction, 
	based on settings in the PWM_On(). Passing 0 as the
	frequency stops the speaker from generating sound
*/
void set_PWM(double frequency){
	/* Keeps track of the currently set
	frequency. Will only update the registers when the freq
	changes, plays music uninterrupted.*/	
	static double current_frequency;
	if(frequency != current_frequency){
		//stop timer/count
		if(!frequency){ TCCR0B &= 0x08; }
		// resumes/continues timer/counter
		else{ TCCR0B |= 0x03;}
		
		//prevent OCCR03A from overflowing, using prescaler 64;
		// 0.954 is the smallest frequency that
		// will not result in overflow
		if(frequency < 0.954){ OCR0A = 0xFFF; }
		// set OCR3A based on desired frequency
		else { OCR0A = (short) (800000 / (128*frequency)) - 1; }
		
		// reset counter and update the current frequency
		TCNT0 = 0;
		current_frequency = frequency;
	}
}
void PWM_on(){
	TCCR0A = (1 << COM0A0);
	// COM3A0: Toggle PB3 on compare match between counter and OCRA0
	TCCR0B = (1 << WGM02) | (1 << CS01) | (1 << CS00);
	// WGM02: When counter (TCNT0) matches OCR0A, reset counter
	// CS01 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off(){
	TCCR0A = 0x00;
	TCCR0B = 0x00;
}