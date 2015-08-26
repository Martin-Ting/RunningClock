/*
 * Test_ToneSelector.c
 *
 * Created: 8/24/2015 10:41:35 PM
 *  Author: user
 */ 


#include <avr/io.h>
#include <avr/interrupt.h>
#include "timer.h"
#include "scheduler.h"
#include "keypad.h"
#include "io.h"
#include "pwm.h"
#include "queue.h"

enum NoteFrequencies {	 END, XX, 
	C3, C3s, D3, D3s, E3, F3, F3s, G3, G3s, A3, A3s, B3,
	C4, C4s, D4, D4s, E4, F4, F4s, G4, G4s, A4, A4s, B4,
	C5, C5s, D5, D5s, E5, F5, F5s, G5, G5s, A5, A5s, B5, 
	C6, C6s, D6, D6s, E6, F6, F6s, G6, G6s, A6, A6s, B6, C7};
double NoteFrequencyLookup(unsigned char NoteNum){
	static double returnFrequency;
	switch(NoteNum){
		case END: returnFrequency = 0; break;
		case  XX: returnFrequency = 0; break;
		case  C3: returnFrequency = 130.81; break;
		case C3s: returnFrequency = 138.59; break;
		case  D3: returnFrequency = 146.83; break;
		case D3s: returnFrequency = 155.56; break;
		case  E3: returnFrequency = 164.81; break;
		case  F3: returnFrequency = 174.61; break;
		case F3s: returnFrequency = 185.00; break;
		case  G3: returnFrequency = 196.00; break;
		case G3s: returnFrequency = 207.65; break;
		case  A3: returnFrequency = 220.00; break;
		case A3s: returnFrequency = 233.08; break;
		case  B3: returnFrequency = 246.94; break;
		case  C4: returnFrequency = 261.63; break;
		case C4s: returnFrequency = 277.18; break;
		case  D4: returnFrequency = 293.66; break;
		case D4s: returnFrequency = 311.13; break;
		case  E4: returnFrequency = 329.63; break;
		case  F4: returnFrequency = 349.23; break;
		case F4s: returnFrequency = 369.99; break;
		case  G4: returnFrequency = 392.00; break;
		case G4s: returnFrequency = 415.30; break;
		case  A4: returnFrequency = 440.00; break;
		case A4s: returnFrequency = 466.16; break;
		case  B4: returnFrequency = 493.88; break;
		case  C5: returnFrequency = 523.25; break;
		case C5s: returnFrequency = 544.37; break;
		case  D5: returnFrequency = 587.33; break;
		case D5s: returnFrequency = 622.25; break;
		case  E5: returnFrequency = 659.25; break;
		case  F5: returnFrequency = 698.46; break;
		case F5s: returnFrequency = 739.99; break;
		case  G5: returnFrequency = 783.99; break;
		case G5s: returnFrequency = 830.61; break;
		case  A5: returnFrequency = 880.00; break;
		case A5s: returnFrequency = 932.33; break;
		case  B5: returnFrequency = 987.77; break;
		case  C6: returnFrequency = 1046.50; break;
		case C6s: returnFrequency = 1108.73; break;
		case  D6: returnFrequency = 1174.66; break;
		case D6s: returnFrequency = 1244.51; break;
		case  E6: returnFrequency = 1318.51; break;
		case  F6: returnFrequency = 1396.91; break;
		case F6s: returnFrequency = 1479.98; break;
		case  G6: returnFrequency = 1567.98; break;
		case G6s: returnFrequency = 1661.22; break;
		case  A6: returnFrequency = 1760.00; break;
		case A6s: returnFrequency = 1864.66; break;
		case  B6: returnFrequency = 1975.53; break;
		case  C7: returnFrequency = 2093.00; break;
		default: returnFrequency = 2000.00; break;
	}
	return returnFrequency;
}


#define MAXSONGSIZE 128
//=====================================================================================================
unsigned char JurassicParkThemeSong[MAXSONGSIZE];
unsigned char JurassicParkThemeTiming[MAXSONGSIZE];
unsigned char MarioUndergroundThemeSong[MAXSONGSIZE];
unsigned char MarioUndergroundThemeTiming[MAXSONGSIZE];

void initSongs(){
	JurassicParkThemeSong[0] = XX;
	JurassicParkThemeSong[1] = C4;
	JurassicParkThemeSong[2] = G4;
	JurassicParkThemeSong[3] = C4;
	JurassicParkThemeSong[4] = A4;
	JurassicParkThemeSong[5] = XX;
	JurassicParkThemeSong[6] = A4;
	JurassicParkThemeSong[7] = B4;
	JurassicParkThemeSong[8] = C5;
	JurassicParkThemeSong[9] = XX;
	JurassicParkThemeSong[10] = C5;
	JurassicParkThemeSong[11] = B4;
	JurassicParkThemeSong[12] = G4;
	JurassicParkThemeSong[13] = A4;
	JurassicParkThemeSong[14] = END;
	JurassicParkThemeTiming[0] = 2;
	JurassicParkThemeTiming[1] = 2;
	JurassicParkThemeTiming[2] = 8;
	JurassicParkThemeTiming[3] = 2;
	JurassicParkThemeTiming[4] = 8;
	JurassicParkThemeTiming[5] = 1;
	JurassicParkThemeTiming[6] = 2;
	JurassicParkThemeTiming[7] = 2;
	JurassicParkThemeTiming[8] = 4;
	JurassicParkThemeTiming[9] = 1;
	JurassicParkThemeTiming[10] = 2;
	JurassicParkThemeTiming[11] = 3;
	JurassicParkThemeTiming[12] = 5;
	JurassicParkThemeTiming[13] = 7;
	JurassicParkThemeTiming[14] = 0;
	
	MarioUndergroundThemeSong[0] = C4; MarioUndergroundThemeTiming[0] = 1;
	MarioUndergroundThemeSong[1] = C5; MarioUndergroundThemeTiming[1] = 1;
	MarioUndergroundThemeSong[2] = A4; MarioUndergroundThemeTiming[2] = 1;
	MarioUndergroundThemeSong[3] = A5; MarioUndergroundThemeTiming[3] = 1;
	MarioUndergroundThemeSong[4] = A4s; MarioUndergroundThemeTiming[4] = 1;
	MarioUndergroundThemeSong[5] = A5s; MarioUndergroundThemeTiming[5] = 1;
	MarioUndergroundThemeSong[6] = XX; MarioUndergroundThemeTiming[6] = 6;
	MarioUndergroundThemeSong[7] = C4; MarioUndergroundThemeTiming[7] = 1;
	MarioUndergroundThemeSong[8] = C5; MarioUndergroundThemeTiming[8] = 1;
	MarioUndergroundThemeSong[9] = A4; MarioUndergroundThemeTiming[9] = 1;
	MarioUndergroundThemeSong[10] = A5; MarioUndergroundThemeTiming[10] = 1;
	MarioUndergroundThemeSong[11] = A4s; MarioUndergroundThemeTiming[11] = 1;
	MarioUndergroundThemeSong[12] = A4s; MarioUndergroundThemeTiming[12] = 1;
	MarioUndergroundThemeSong[13] = XX; MarioUndergroundThemeTiming[13] = 6;
	MarioUndergroundThemeSong[14] = F3; MarioUndergroundThemeTiming[14] = 1;
	MarioUndergroundThemeSong[15] = F4; MarioUndergroundThemeTiming[15] = 1;
	MarioUndergroundThemeSong[16] = D3; MarioUndergroundThemeTiming[16] = 1;
	MarioUndergroundThemeSong[17] = D4; MarioUndergroundThemeTiming[17] = 1;
	MarioUndergroundThemeSong[18] = D3s; MarioUndergroundThemeTiming[18] = 1;
	MarioUndergroundThemeSong[19] = D4s; MarioUndergroundThemeTiming[19] = 1;
	MarioUndergroundThemeSong[20] = XX; MarioUndergroundThemeTiming[20] = 6;
	MarioUndergroundThemeSong[21] = F3; MarioUndergroundThemeTiming[21] = 1;
	MarioUndergroundThemeSong[22] = F4; MarioUndergroundThemeTiming[22] = 1;
	MarioUndergroundThemeSong[23] = D3; MarioUndergroundThemeTiming[23] = 1;
	MarioUndergroundThemeSong[24] = D4; MarioUndergroundThemeTiming[24] = 1;
	MarioUndergroundThemeSong[25] = D3s; MarioUndergroundThemeTiming[25] = 1;
	MarioUndergroundThemeSong[26] = D4s; MarioUndergroundThemeTiming[26] = 1;
	MarioUndergroundThemeSong[27] = XX; MarioUndergroundThemeTiming[27] = 6;
	MarioUndergroundThemeSong[28] = E5; MarioUndergroundThemeTiming[28] = 1;
	MarioUndergroundThemeSong[29] = G5; MarioUndergroundThemeTiming[29] = 1;
	MarioUndergroundThemeSong[30] = E6; MarioUndergroundThemeTiming[30] = 1;
	MarioUndergroundThemeSong[31] = C6; MarioUndergroundThemeTiming[31] = 1;
	MarioUndergroundThemeSong[32] = XX; MarioUndergroundThemeTiming[32] = 3;
	MarioUndergroundThemeSong[33] = E5; MarioUndergroundThemeTiming[33] = 1;
	MarioUndergroundThemeSong[34] = G5; MarioUndergroundThemeTiming[34] = 1;
	MarioUndergroundThemeSong[35] = E6; MarioUndergroundThemeTiming[35] = 1;
	MarioUndergroundThemeSong[36] = C6; MarioUndergroundThemeTiming[36] = 1;
	MarioUndergroundThemeSong[37] = END; MarioUndergroundThemeTiming[37] = 0;
}

unsigned char ALARMON = 0x00;


enum Songs { JurassicParkTheme, MarioUndergroundTheme, NUMSONGS } currentSong;
enum PlaySongSMStates { PlaySong_wait, PlaySong_play, PlaySong_hold };
unsigned char currentSong;
signed char PSState = -1;

void PlaySongSMTick(){
	static unsigned char currentNote;
	static unsigned char NoteLength;
	static double NoteTone;
	switch(PSState){
		case PlaySong_wait:
			if(ALARMON == 0xFF){
				PSState = PlaySong_play;
			}
			break;
		case PlaySong_play:
			if(ALARMON == 0x00){
				PWM_on();
				PSState = PlaySong_wait;
			}
			PSState = PlaySong_hold;
			break;
		case PlaySong_hold:
			if(ALARMON == 0x00){
				PSState = PlaySong_wait;
			}else if(NoteLength == 0){
				PSState = PlaySong_play;
			}
			break;
		default:
			currentNote = NoteLength = 0;
			currentSong = JurassicParkTheme;
			NoteTone = 0.0;
			PSState = PlaySong_wait;
			break;
	}
	switch(PSState){
		case PlaySong_wait:
			currentNote = 0;
			NoteLength = 0;
			PWM_off();
			break;
		case PlaySong_play:
			//update note and length
			switch(currentSong){
				case JurassicParkTheme:
					NoteTone = JurassicParkThemeSong[currentNote];
					NoteLength = JurassicParkThemeTiming[currentNote];
					break;
				case MarioUndergroundTheme:
					NoteTone = MarioUndergroundThemeSong[currentNote];
					NoteLength = MarioUndergroundThemeTiming[currentNote];
					break;
				default:
					break;
			}
			//play the note
			PWM_on();
			set_PWM(NoteFrequencyLookup(NoteTone));
			//increment
			++currentNote;
			if(NoteTone == END){
				currentNote = 0;
			}
			break;
		case PlaySong_hold:
			PORTB = NoteLength;
			if(NoteLength != 0){
				NoteLength--;
			}
			break;
	}
}
 enum ToggleSoundSMStates { ToggleSound_checkinput, ToggleSound_checkinput_fall} TSstate = -1;
unsigned char ToggleSoundSMTick(){
//	static unsigned char i;
	switch(TSstate){
		case ToggleSound_checkinput:
			if(GetKeypadKey() == 'A'){
				ALARMON = ALARMON == 0x00 ? 0xFF : 0x00;
				PORTD = ALARMON;
//				PORTB = i;
// 				PWM_on();
// 				set_PWM(NoteFrequencyLookup(MarioUndergroundThemeSong[(i)]));
// 				if(i < 32){
// 					++i;
// 				}else{
// 					i=0;
// 				}
				TSstate = ToggleSound_checkinput_fall;
			}
			break;
		case ToggleSound_checkinput_fall:
			if(GetKeypadKey() != 'A'){
				TSstate = ToggleSound_checkinput;
			}
			break;
		default:
//			i = 0;
			TSstate = ToggleSound_checkinput;
		break;
	}	
	return TSstate;
}
int main(void)
{
	DDRA = 0xF0; PORTA = 0x0F;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(50);
	TimerOn();
	ALARMON = 0x00;
initSongs();
	while(1)
	{
		//set_PWM(pitchScale[5]);
		ToggleSoundSMTick();
		PlaySongSMTick();
		while(!TimerFlag);
		TimerFlag = 0;
	}
}
