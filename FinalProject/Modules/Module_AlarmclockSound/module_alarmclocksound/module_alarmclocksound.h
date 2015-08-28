#ifndef MODULE_ALARMCLOCKSOUND_H
#define MODULE_ALARMCLOCKSOUND_H

#include "pwm.h"
#include "module_alarm.h"
// ALARMON flag
#include "module_songs.h"

#define TMPSONGINDEX 5
#define CURRSONGINDEX 13

extern unsigned char ALARMON;
unsigned char songString[32];
unsigned char currentSong;
unsigned char tmpSong;
unsigned char TEMPSONGON;

enum Songs { JurassicParkTheme =0, MarioUndergroundTheme=1, MarioTheme = 2, PokemonTheme = 3, PiratesTheme = 4, NUMSONGS };
// End Songs Portion ==================================================================================
// PlaySongSM uses input ALARMON flag
enum PlayAlarmSoundSMStates { PlaySong_wait, PlaySong_play, PlaySong_hold };
signed char PlayAlarmSoundSMTick(signed char state){
	static unsigned char currentNote;
	static unsigned char NoteLength;
	static double NoteTone;
	static unsigned char songToPlay;
	switch(state){
		case PlaySong_wait:
			if(ALARMON == ALARMACTIVE || TEMPSONGON == ALARMACTIVE){
				state = PlaySong_play;
			}
			break;
		case PlaySong_play:
			if(ALARMON == ALARMINACTIVE && TEMPSONGON == ALARMINACTIVE){
				state = PlaySong_wait;
			}
			state = PlaySong_hold;
			break;
		case PlaySong_hold:
			if(ALARMON == ALARMINACTIVE && TEMPSONGON == ALARMINACTIVE){
				state = PlaySong_wait;
			}else if(NoteLength == 0){
				state = PlaySong_play;
			}
			break;
		default:
			currentNote = NoteLength = tmpSong = 0;
			TEMPSONGON = ALARMINACTIVE;
			currentSong = MarioUndergroundTheme;
			NoteTone = 0.0;
			state = PlaySong_wait;
			songToPlay = 0;
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
			songToPlay =( (TEMPSONGON == ALARMACTIVE) ? tmpSong : currentSong);

			switch(songToPlay){
				case JurassicParkTheme:
					NoteTone = JurassicParkThemeSong[currentNote];
					NoteLength = JurassicParkThemeTiming[currentNote];
					break;
				case MarioUndergroundTheme:
					NoteTone = MarioUndergroundThemeSong[currentNote];
					NoteLength = MarioUndergroundThemeTiming[currentNote];
					break;
				case MarioTheme:
					NoteTone = MarioThemeSong[currentNote];
					NoteLength = MarioThemeTiming[currentNote];
					break;
				case PokemonTheme:
					NoteTone = PokemonThemeSong[currentNote];
					NoteLength = PokemonThemeTiming[currentNote];
					break;
				case PiratesTheme:
					NoteTone = PiratesThemeSong[currentNote];
					NoteLength = PiratesThemeTiming[currentNote];
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
			if(NoteLength != 0){
				NoteLength--;
			}
			break;
	}
	return state;
}

void updateSongString(){
	songString[0] = 'S';
	songString[1] = 'o';
	songString[2] = 'n';
	songString[3] = 'g';
	songString[4] = ' ';
	songString[TMPSONGINDEX] = '0'+tmpSong/10;
	songString[TMPSONGINDEX + 1] = '0' + tmpSong%10;
	songString[12] = '[';
	songString[CURRSONGINDEX] = '0'+currentSong/10;
	songString[CURRSONGINDEX+1] = '0'+currentSong%10;
	songString[15] = ']';
	songString[16] = 'S';
	songString[17] = 'E';
	songString[18] = 'L';
	songString[19] = ' ';
	songString[20] = 't';
	songString[21] = 'o';
	songString[22] = ' ';
	songString[23] = 's';
	songString[24] = 'e';
	songString[25] = 't';
	songString[31] = '\0';
}
#endif