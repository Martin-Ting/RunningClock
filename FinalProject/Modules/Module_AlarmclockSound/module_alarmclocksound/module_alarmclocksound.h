#ifndef MODULE_ALARMCLOCKSOUND_H
#define MODULE_ALARMCLOCKSOUND_H

#include "pwm.h"
#include "module_alarm.h"


#define MAXSONGSIZE 128

enum NoteFrequencies {	 Note_END, Note_XX,
	Note_C3, Note_C3s, Note_D3, Note_D3s, Note_E3, Note_F3, Note_F3s, Note_G3, Note_G3s, Note_A3, Note_A3s, Note_B3,
	Note_C4, Note_C4s, Note_D4, Note_D4s, Note_E4, Note_F4, Note_F4s, Note_G4, Note_G4s, Note_A4, Note_A4s, Note_B4,
	Note_C5, Note_C5s, Note_D5, Note_D5s, Note_E5, Note_F5, Note_F5s, Note_G5, Note_G5s, Note_A5, Note_A5s, Note_B5,
Note_C6, Note_C6s, Note_D6, Note_D6s, Note_E6, Note_F6, Note_F6s, Note_G6, Note_G6s, Note_A6, Note_A6s, Note_B6, Note_C7};
double NoteFrequencyLookup(unsigned char NoteNum){
	static double returnFrequency;
	switch(NoteNum){
		case Note_END: returnFrequency = 0; break;
		case  Note_XX: returnFrequency = 0; break;
		case  Note_C3: returnFrequency = 130.81; break;
		case Note_C3s: returnFrequency = 138.59; break;
		case  Note_D3: returnFrequency = 146.83; break;
		case Note_D3s: returnFrequency = 155.56; break;
		case  Note_E3: returnFrequency = 164.81; break;
		case  Note_F3: returnFrequency = 174.61; break;
		case Note_F3s: returnFrequency = 185.00; break;
		case  Note_G3: returnFrequency = 196.00; break;
		case Note_G3s: returnFrequency = 207.65; break;
		case  Note_A3: returnFrequency = 220.00; break;
		case Note_A3s: returnFrequency = 233.08; break;
		case  Note_B3: returnFrequency = 246.94; break;
		case  Note_C4: returnFrequency = 261.63; break;
		case Note_C4s: returnFrequency = 277.18; break;
		case  Note_D4: returnFrequency = 293.66; break;
		case Note_D4s: returnFrequency = 311.13; break;
		case  Note_E4: returnFrequency = 329.63; break;
		case  Note_F4: returnFrequency = 349.23; break;
		case Note_F4s: returnFrequency = 369.99; break;
		case  Note_G4: returnFrequency = 392.00; break;
		case Note_G4s: returnFrequency = 415.30; break;
		case  Note_A4: returnFrequency = 440.00; break;
		case Note_A4s: returnFrequency = 466.16; break;
		case  Note_B4: returnFrequency = 493.88; break;
		case  Note_C5: returnFrequency = 523.25; break;
		case Note_C5s: returnFrequency = 544.37; break;
		case  Note_D5: returnFrequency = 587.33; break;
		case Note_D5s: returnFrequency = 622.25; break;
		case  Note_E5: returnFrequency = 659.25; break;
		case  Note_F5: returnFrequency = 698.46; break;
		case Note_F5s: returnFrequency = 739.99; break;
		case  Note_G5: returnFrequency = 783.99; break;
		case Note_G5s: returnFrequency = 830.61; break;
		case  Note_A5: returnFrequency = 880.00; break;
		case Note_A5s: returnFrequency = 932.33; break;
		case  Note_B5: returnFrequency = 987.77; break;
		case  Note_C6: returnFrequency = 1046.50; break;
		case Note_C6s: returnFrequency = 1108.73; break;
		case  Note_D6: returnFrequency = 1174.66; break;
		case Note_D6s: returnFrequency = 1244.51; break;
		case  Note_E6: returnFrequency = 1318.51; break;
		case  Note_F6: returnFrequency = 1396.91; break;
		case Note_F6s: returnFrequency = 1479.98; break;
		case  Note_G6: returnFrequency = 1567.98; break;
		case Note_G6s: returnFrequency = 1661.22; break;
		case  Note_A6: returnFrequency = 1760.00; break;
		case Note_A6s: returnFrequency = 1864.66; break;
		case  Note_B6: returnFrequency = 1975.53; break;
		case  Note_C7: returnFrequency = 2093.00; break;
		default: returnFrequency = 2000.00; break;
	}
	return returnFrequency;
}
extern unsigned char ALARMON;

// Songs! =====================================================================================================
unsigned char JurassicParkThemeSong[MAXSONGSIZE];
unsigned char JurassicParkThemeTiming[MAXSONGSIZE];
unsigned char MarioUndergroundThemeSong[MAXSONGSIZE];
unsigned char MarioUndergroundThemeTiming[MAXSONGSIZE];

void initSongs(){
	for(unsigned char i = 0; i < MAXSONGSIZE; ++i){
		JurassicParkThemeSong[i] = 0;
		JurassicParkThemeTiming[i] = 0;
		MarioUndergroundThemeSong[i] = 0;
		MarioUndergroundThemeTiming[i] = 0;
	}
	JurassicParkThemeSong[0] = Note_XX;
	JurassicParkThemeSong[1] = Note_C4;
	JurassicParkThemeSong[2] = Note_G4;
	JurassicParkThemeSong[3] = Note_C4;
	JurassicParkThemeSong[4] = Note_A4;
	JurassicParkThemeSong[5] = Note_XX;
	JurassicParkThemeSong[6] = Note_A4;
	JurassicParkThemeSong[7] = Note_B4;
	JurassicParkThemeSong[8] = Note_C5;
	JurassicParkThemeSong[9] = Note_XX;
	JurassicParkThemeSong[10] = Note_C5;
	JurassicParkThemeSong[11] = Note_B4;
	JurassicParkThemeSong[12] = Note_G4;
	JurassicParkThemeSong[13] = Note_A4;
	JurassicParkThemeSong[14] = Note_END;
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
	
	MarioUndergroundThemeSong[0] = Note_C4; MarioUndergroundThemeTiming[0] = 1;
	MarioUndergroundThemeSong[1] = Note_C5; MarioUndergroundThemeTiming[1] = 1;
	MarioUndergroundThemeSong[2] = Note_A4; MarioUndergroundThemeTiming[2] = 1;
	MarioUndergroundThemeSong[3] = Note_A5; MarioUndergroundThemeTiming[3] = 1;
	MarioUndergroundThemeSong[4] = Note_A4s; MarioUndergroundThemeTiming[4] = 1;
	MarioUndergroundThemeSong[5] = Note_A5s; MarioUndergroundThemeTiming[5] = 1;
	MarioUndergroundThemeSong[6] = Note_XX; MarioUndergroundThemeTiming[6] = 6;
	MarioUndergroundThemeSong[7] = Note_C4; MarioUndergroundThemeTiming[7] = 1;
	MarioUndergroundThemeSong[8] = Note_C5; MarioUndergroundThemeTiming[8] = 1;
	MarioUndergroundThemeSong[9] = Note_A4; MarioUndergroundThemeTiming[9] = 1;
	MarioUndergroundThemeSong[10] = Note_A5; MarioUndergroundThemeTiming[10] = 1;
	MarioUndergroundThemeSong[11] = Note_A4s; MarioUndergroundThemeTiming[11] = 1;
	MarioUndergroundThemeSong[12] = Note_A4s; MarioUndergroundThemeTiming[12] = 1;
	MarioUndergroundThemeSong[13] = Note_XX; MarioUndergroundThemeTiming[13] = 6;
	MarioUndergroundThemeSong[14] = Note_F3; MarioUndergroundThemeTiming[14] = 1;
	MarioUndergroundThemeSong[15] = Note_F4; MarioUndergroundThemeTiming[15] = 1;
	MarioUndergroundThemeSong[16] = Note_D3; MarioUndergroundThemeTiming[16] = 1;
	MarioUndergroundThemeSong[17] = Note_D4; MarioUndergroundThemeTiming[17] = 1;
	MarioUndergroundThemeSong[18] = Note_D3s; MarioUndergroundThemeTiming[18] = 1;
	MarioUndergroundThemeSong[19] = Note_D4s; MarioUndergroundThemeTiming[19] = 1;
	MarioUndergroundThemeSong[20] = Note_XX; MarioUndergroundThemeTiming[20] = 6;
	MarioUndergroundThemeSong[21] = Note_F3; MarioUndergroundThemeTiming[21] = 1;
	MarioUndergroundThemeSong[22] = Note_F4; MarioUndergroundThemeTiming[22] = 1;
	MarioUndergroundThemeSong[23] = Note_D3; MarioUndergroundThemeTiming[23] = 1;
	MarioUndergroundThemeSong[24] = Note_D4; MarioUndergroundThemeTiming[24] = 1;
	MarioUndergroundThemeSong[25] = Note_D3s; MarioUndergroundThemeTiming[25] = 1;
	MarioUndergroundThemeSong[26] = Note_D4s; MarioUndergroundThemeTiming[26] = 1;
	MarioUndergroundThemeSong[27] = Note_XX; MarioUndergroundThemeTiming[27] = 6;
	MarioUndergroundThemeSong[28] = Note_E5; MarioUndergroundThemeTiming[28] = 1;
	MarioUndergroundThemeSong[29] = Note_G5; MarioUndergroundThemeTiming[29] = 1;
	MarioUndergroundThemeSong[30] = Note_E6; MarioUndergroundThemeTiming[30] = 1;
	MarioUndergroundThemeSong[31] = Note_C6; MarioUndergroundThemeTiming[31] = 1;
	MarioUndergroundThemeSong[32] = Note_XX; MarioUndergroundThemeTiming[32] = 3;
	MarioUndergroundThemeSong[33] = Note_E5; MarioUndergroundThemeTiming[33] = 1;
	MarioUndergroundThemeSong[34] = Note_G5; MarioUndergroundThemeTiming[34] = 1;
	MarioUndergroundThemeSong[35] = Note_E6; MarioUndergroundThemeTiming[35] = 1;
	MarioUndergroundThemeSong[36] = Note_C6; MarioUndergroundThemeTiming[36] = 1;
	MarioUndergroundThemeSong[37] = Note_END; MarioUndergroundThemeTiming[37] = 0;
}


enum Songs { JurassicParkTheme, MarioUndergroundTheme, NUMSONGS };
// End Songs Portion ==================================================================================
// PlaySongSM uses input ALARMON flag
enum PlayAlarmSoundSMStates { PlaySong_wait, PlaySong_play, PlaySong_hold };
unsigned char currentSong;

signed char PlayAlarmSoundSMTick(signed char state){
	static unsigned char currentNote;
	static unsigned char NoteLength;
	static double NoteTone;
	
	switch(state){
		case PlaySong_wait:
			if(ALARMON == ALARMACTIVE){
				state = PlaySong_play;
			}
			break;
		case PlaySong_play:
			if(ALARMON == ALARMINACTIVE){
				state = PlaySong_wait;
			}
			state = PlaySong_hold;
			break;
		case PlaySong_hold:
			if(ALARMON == ALARMINACTIVE){
				state = PlaySong_wait;
			}else if(NoteLength == 0){
				state = PlaySong_play;
			}
			break;
		default:
			currentNote = NoteLength = 0;
			currentSong = JurassicParkTheme;
			NoteTone = 0.0;
			state = PlaySong_wait;
			initSongs();
			break;
	}
	switch(state){
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
			if(NoteTone == Note_END){
				currentNote = 0;
			}
			break;
		case PlaySong_hold:
			//PORTB = NoteLength;
			if(NoteLength != 0){
				NoteLength--;
			}
			break;
	}
	return state;
}

#endif