#ifndef MODULE_ALARMCLOCKSOUND_H
#define MODULE_ALARMCLOCKSOUND_H

#include "pwm.h"
#include "module_alarm.h"


#define MAXSONGSIZE 128


unsigned char songString[32];
#define TMPSONGINDEX 5
#define CURRSONGINDEX 13






enum NoteFrequencies { Note_END, Note_XX,
	Note_A2,
	Note_C3, Note_C3s, Note_D3, Note_D3s, Note_E3, Note_F3, Note_F3s, Note_G3, Note_G3s, Note_A3, Note_A3s, Note_B3,
	Note_C4, Note_C4s, Note_D4, Note_D4s, Note_E4, Note_F4, Note_F4s, Note_G4, Note_G4s, Note_A4, Note_A4s, Note_B4,
	Note_C5, Note_C5s, Note_D5, Note_D5s, Note_E5, Note_F5, Note_F5s, Note_G5, Note_G5s, Note_A5, Note_A5s, Note_B5,
Note_C6, Note_C6s, Note_D6, Note_D6s, Note_E6, Note_F6, Note_F6s, Note_G6, Note_G6s, Note_A6, Note_A6s, Note_B6, Note_C7};
double NoteFrequencyLookup(unsigned char NoteNum){
	static double returnFrequency;
	switch(NoteNum){
		case Note_END: returnFrequency = 0; break;
		case  Note_XX: returnFrequency = 0; break;
		case Note_A2:  returnFrequency = 110.00; break;
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
unsigned char MarioThemeSong[MAXSONGSIZE];
unsigned char MarioThemeTiming[MAXSONGSIZE];
unsigned char PokemonThemeSong[MAXSONGSIZE];
unsigned char PokemonThemeTiming[MAXSONGSIZE];
unsigned char PiratesThemeSong[MAXSONGSIZE];
unsigned char PiratesThemeTiming[MAXSONGSIZE];

#define MTWHOLE 2
#define MTTRIP 3
#define PKWHOLE 6
#define PKHALF (PKWHOLE/2)
#define PHALF 2
#define PWHOLE (PHALF*2)
void PiratesThemeInit(){
	unsigned char i = 0;
	PiratesThemeSong[i]= Note_A2; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_XX; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_A2; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_XX; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_A2; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_C3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]= PHALF;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_A3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_A3s; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_A3s; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_A3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_A3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_XX; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_G3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_A3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_XX; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_F3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_D3; PiratesThemeTiming[i++]=PHALF;
	PiratesThemeSong[i]= Note_E3; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_XX; PiratesThemeTiming[i++]=PWHOLE;
	PiratesThemeSong[i]= Note_END; PiratesThemeTiming[i++]=0;
}
void PokemonThemeInit(){
	PokemonThemeSong[0] = Note_A4; PokemonThemeTiming[0] = PKHALF;
	PokemonThemeSong[1] = Note_A4; PokemonThemeTiming[1] = PKHALF;
	PokemonThemeSong[2] = Note_A4; PokemonThemeTiming[2] = PKHALF;
	PokemonThemeSong[3] = Note_A4; PokemonThemeTiming[3] = PKHALF*3;
	PokemonThemeSong[4] = Note_A4; PokemonThemeTiming[4] = PKHALF;
	PokemonThemeSong[5] = Note_G4; PokemonThemeTiming[5] = PKWHOLE;
	PokemonThemeSong[6] = Note_E4; PokemonThemeTiming[6] = PKHALF;
	PokemonThemeSong[7] = Note_C4; PokemonThemeTiming[7] = PKHALF*3;
	PokemonThemeSong[8] = Note_C4; PokemonThemeTiming[8] = PKHALF;
	PokemonThemeSong[9] = Note_A4; PokemonThemeTiming[9] = PKWHOLE;
	PokemonThemeSong[10] = Note_A4; PokemonThemeTiming[10] = PKWHOLE;
	PokemonThemeSong[11] = Note_G4; PokemonThemeTiming[11] = PKHALF;
	PokemonThemeSong[12] = Note_F4; PokemonThemeTiming[12] = PKHALF;
	PokemonThemeSong[13] = Note_G4; PokemonThemeTiming[13] = PKWHOLE;
	PokemonThemeSong[14] = Note_XX; PokemonThemeTiming[14] = PKWHOLE*3;
	PokemonThemeSong[15] = Note_F4; PokemonThemeTiming[15] = PKHALF;
	PokemonThemeSong[16] = Note_A4s; PokemonThemeTiming[16] = PKWHOLE;
	PokemonThemeSong[17] = Note_A4s; PokemonThemeTiming[17] = PKWHOLE;
	PokemonThemeSong[18] = Note_A4s; PokemonThemeTiming[18] = PKHALF;
	PokemonThemeSong[19] = Note_A4; PokemonThemeTiming[19] = PKWHOLE;
	PokemonThemeSong[20] = Note_G4; PokemonThemeTiming[20] = PKWHOLE;
	PokemonThemeSong[21] = Note_F4; PokemonThemeTiming[21] = PKHALF*3;
	PokemonThemeSong[22] = Note_F4; PokemonThemeTiming[22] = PKHALF;
	PokemonThemeSong[23] = Note_A4; PokemonThemeTiming[23] = PKWHOLE;
	PokemonThemeSong[24] = Note_A4; PokemonThemeTiming[24] = PKHALF;
	PokemonThemeSong[25] = Note_G4; PokemonThemeTiming[25] = PKWHOLE;
	PokemonThemeSong[26] = Note_F4; PokemonThemeTiming[26] = PKHALF;
	PokemonThemeSong[27] = Note_A4; PokemonThemeTiming[27] = PKWHOLE*4;
 	PokemonThemeSong[28] = Note_END; PokemonThemeTiming[28] = 0;
// 	PokemonThemeSong[29] = Note_; PokemonThemeTiming[2] = ;
// 	PokemonThemeSong[30] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[31] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[32] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[33] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[34] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[35] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[36] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[37] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[38] = Note_; PokemonThemeTiming[3] = ;
// 	PokemonThemeSong[39] = Note_; PokemonThemeTiming[3] = ;
}
void MarioThemeInit(){
	MarioThemeSong[0] = Note_XX; MarioThemeTiming[0] = 0;
	MarioThemeSong[1] = Note_XX; MarioThemeTiming[1] = 0;
	MarioThemeSong[2] = Note_E5; MarioThemeTiming[2] = MTWHOLE;
	MarioThemeSong[3] = Note_E5; MarioThemeTiming[3] = MTWHOLE;
	MarioThemeSong[4] = Note_XX; MarioThemeTiming[4] = MTWHOLE;
	MarioThemeSong[5] = Note_E5; MarioThemeTiming[5] = MTWHOLE;
	MarioThemeSong[6] = Note_XX; MarioThemeTiming[6] = MTWHOLE;
	MarioThemeSong[7] = Note_C5; MarioThemeTiming[7] = MTWHOLE;
	MarioThemeSong[8] = Note_E5; MarioThemeTiming[8] = MTWHOLE;
	MarioThemeSong[9] = Note_XX; MarioThemeTiming[9] = MTWHOLE*2;
	MarioThemeSong[10] = Note_G5; MarioThemeTiming[10] = MTWHOLE;
	MarioThemeSong[11] = Note_XX; MarioThemeTiming[11] = MTWHOLE*3;
	MarioThemeSong[12] = Note_G4; MarioThemeTiming[12] = MTWHOLE;
	MarioThemeSong[13] = Note_XX; MarioThemeTiming[13] = MTWHOLE*2;
	MarioThemeSong[14] = Note_C5; MarioThemeTiming[14] = MTWHOLE;
	MarioThemeSong[15] = Note_XX; MarioThemeTiming[15] = MTWHOLE*2;
	MarioThemeSong[16] = Note_G4; MarioThemeTiming[16] = MTWHOLE;
	MarioThemeSong[17] = Note_XX; MarioThemeTiming[17] = MTWHOLE*2;
	MarioThemeSong[18] = Note_E4; MarioThemeTiming[18] = MTWHOLE;
	MarioThemeSong[19] = Note_XX; MarioThemeTiming[19] = MTWHOLE;
	MarioThemeSong[20] = Note_A4; MarioThemeTiming[20] = MTWHOLE;
	MarioThemeSong[21] = Note_XX; MarioThemeTiming[21] = MTWHOLE;
	MarioThemeSong[22] = Note_B4; MarioThemeTiming[22] = MTWHOLE;
	MarioThemeSong[23] = Note_XX; MarioThemeTiming[23] = MTWHOLE;
	MarioThemeSong[24] = Note_A4s; MarioThemeTiming[24] = MTWHOLE;
	MarioThemeSong[25] = Note_A4; MarioThemeTiming[25] = MTWHOLE;
	MarioThemeSong[26] = Note_XX; MarioThemeTiming[26] = MTWHOLE;
	MarioThemeSong[27] = Note_G4; MarioThemeTiming[27] = MTTRIP;
	MarioThemeSong[28] = Note_E5; MarioThemeTiming[28] = MTTRIP;
	MarioThemeSong[29] = Note_G5; MarioThemeTiming[29] = MTTRIP;
	MarioThemeSong[30] = Note_A5; MarioThemeTiming[30] = MTWHOLE;
	MarioThemeSong[31] = Note_XX; MarioThemeTiming[31] = MTWHOLE;
	MarioThemeSong[32] = Note_F5; MarioThemeTiming[32] = MTWHOLE;
	MarioThemeSong[33] = Note_G5; MarioThemeTiming[33] = MTWHOLE;
	MarioThemeSong[34] = Note_XX; MarioThemeTiming[34] = MTWHOLE;
	MarioThemeSong[35] = Note_E5; MarioThemeTiming[35] = MTWHOLE;
	MarioThemeSong[36] = Note_XX; MarioThemeTiming[36] = MTWHOLE;
	MarioThemeSong[37] = Note_C5; MarioThemeTiming[37] = MTWHOLE;
	MarioThemeSong[38] = Note_D5; MarioThemeTiming[38] = MTWHOLE;
	MarioThemeSong[39] = Note_B4; MarioThemeTiming[39] = MTWHOLE;
	MarioThemeSong[40] = Note_XX; MarioThemeTiming[40] = MTWHOLE*2;
	MarioThemeSong[41] = Note_C5; MarioThemeTiming[41] = MTWHOLE;
	MarioThemeSong[42] = Note_XX; MarioThemeTiming[42] = MTWHOLE*2;
	MarioThemeSong[43] = Note_G4; MarioThemeTiming[43] = MTWHOLE;
	MarioThemeSong[44] = Note_XX; MarioThemeTiming[44] = MTWHOLE*2;
	MarioThemeSong[45] = Note_E4; MarioThemeTiming[45] = MTWHOLE;
	MarioThemeSong[46] = Note_XX; MarioThemeTiming[46] = MTWHOLE;
	MarioThemeSong[47] = Note_A4; MarioThemeTiming[47] = MTWHOLE;
	MarioThemeSong[48] = Note_XX; MarioThemeTiming[48] = MTWHOLE;
	MarioThemeSong[49] = Note_B4; MarioThemeTiming[49] = MTWHOLE;
	MarioThemeSong[50] = Note_XX; MarioThemeTiming[50] = MTWHOLE;
	MarioThemeSong[51] = Note_A4s; MarioThemeTiming[51] = MTWHOLE;
	MarioThemeSong[52] = Note_A4; MarioThemeTiming[52] = MTWHOLE;
	MarioThemeSong[53] = Note_XX; MarioThemeTiming[53] = MTWHOLE;
	MarioThemeSong[54] = Note_G4; MarioThemeTiming[54] = MTTRIP;
	MarioThemeSong[55] = Note_E5; MarioThemeTiming[55] = MTTRIP;
	MarioThemeSong[56] = Note_G5; MarioThemeTiming[56] = MTTRIP;
	MarioThemeSong[57] = Note_A5; MarioThemeTiming[57] = MTWHOLE;
	MarioThemeSong[58] = Note_XX; MarioThemeTiming[58] = MTWHOLE;
	MarioThemeSong[59] = Note_F5; MarioThemeTiming[59] = MTWHOLE;
	MarioThemeSong[60] = Note_G5; MarioThemeTiming[50] = MTWHOLE;
	MarioThemeSong[61] = Note_XX; MarioThemeTiming[61] = MTWHOLE;
	MarioThemeSong[62] = Note_E5; MarioThemeTiming[62] = MTWHOLE;
	MarioThemeSong[63] = Note_XX; MarioThemeTiming[63] = MTWHOLE;
	MarioThemeSong[64] = Note_C5; MarioThemeTiming[64] = MTWHOLE;
	MarioThemeSong[65] = Note_D5; MarioThemeTiming[65] = MTWHOLE;
	MarioThemeSong[66] = Note_B4; MarioThemeTiming[66] = MTWHOLE;
	MarioThemeSong[67] = Note_XX; MarioThemeTiming[67] = MTWHOLE*2;
	MarioThemeSong[68] = Note_END; MarioThemeTiming[68] = 0;
}
void MarioUndergroundThemeInit(){
		MarioUndergroundThemeSong[0] = Note_C4; MarioUndergroundThemeTiming[0] = 2;
		MarioUndergroundThemeSong[1] = Note_C5; MarioUndergroundThemeTiming[1] = 2;
		MarioUndergroundThemeSong[2] = Note_A4; MarioUndergroundThemeTiming[2] = 2;
		MarioUndergroundThemeSong[3] = Note_A5; MarioUndergroundThemeTiming[3] = 2;
		MarioUndergroundThemeSong[4] = Note_A4s; MarioUndergroundThemeTiming[4] = 2;
		MarioUndergroundThemeSong[5] = Note_A5s; MarioUndergroundThemeTiming[5] = 2;
		MarioUndergroundThemeSong[6] = Note_XX; MarioUndergroundThemeTiming[6] = 6;
		MarioUndergroundThemeSong[7] = Note_C4; MarioUndergroundThemeTiming[7] = 2;
		MarioUndergroundThemeSong[8] = Note_C5; MarioUndergroundThemeTiming[8] = 2;
		MarioUndergroundThemeSong[9] = Note_A4; MarioUndergroundThemeTiming[9] = 2;
		MarioUndergroundThemeSong[10] = Note_A5; MarioUndergroundThemeTiming[10] = 2;
		MarioUndergroundThemeSong[11] = Note_A4s; MarioUndergroundThemeTiming[11] = 2;
		MarioUndergroundThemeSong[12] = Note_A5s; MarioUndergroundThemeTiming[12] = 2;
		MarioUndergroundThemeSong[13] = Note_XX; MarioUndergroundThemeTiming[13] = 6;
		MarioUndergroundThemeSong[14] = Note_F3; MarioUndergroundThemeTiming[14] = 2;
		MarioUndergroundThemeSong[15] = Note_F4; MarioUndergroundThemeTiming[15] = 2;
		MarioUndergroundThemeSong[16] = Note_D3; MarioUndergroundThemeTiming[16] = 2;
		MarioUndergroundThemeSong[17] = Note_D4; MarioUndergroundThemeTiming[17] = 2;
		MarioUndergroundThemeSong[18] = Note_D3s; MarioUndergroundThemeTiming[18] = 2;
		MarioUndergroundThemeSong[19] = Note_D4s; MarioUndergroundThemeTiming[19] = 2;
		MarioUndergroundThemeSong[20] = Note_XX; MarioUndergroundThemeTiming[20] = 6;
		MarioUndergroundThemeSong[21] = Note_F3; MarioUndergroundThemeTiming[21] = 2;
		MarioUndergroundThemeSong[22] = Note_F4; MarioUndergroundThemeTiming[22] = 2;
		MarioUndergroundThemeSong[23] = Note_D3; MarioUndergroundThemeTiming[23] = 2;
		MarioUndergroundThemeSong[24] = Note_D4; MarioUndergroundThemeTiming[24] = 2;
		MarioUndergroundThemeSong[25] = Note_D3s; MarioUndergroundThemeTiming[25] = 2;
		MarioUndergroundThemeSong[26] = Note_D4s; MarioUndergroundThemeTiming[26] = 2;
		MarioUndergroundThemeSong[27] = Note_XX; MarioUndergroundThemeTiming[27] = 6;
		MarioUndergroundThemeSong[28] = Note_E5; MarioUndergroundThemeTiming[28] = 1;
		MarioUndergroundThemeSong[29] = Note_G5; MarioUndergroundThemeTiming[29] = 1;
		MarioUndergroundThemeSong[30] = Note_E6; MarioUndergroundThemeTiming[30] = 1;
		MarioUndergroundThemeSong[31] = Note_C6; MarioUndergroundThemeTiming[31] = 1;
		MarioUndergroundThemeSong[32] = Note_D6; MarioUndergroundThemeTiming[32] = 1;
		MarioUndergroundThemeSong[33] = Note_G6; MarioUndergroundThemeTiming[33] = 1;
		//MarioUndergroundThemeSong[34] = Note_END; MarioUndergroundThemeTiming[34] = 1;
		MarioUndergroundThemeSong[34] = Note_D4s; MarioUndergroundThemeTiming[34] = 1;
		MarioUndergroundThemeSong[35] = Note_D4;  MarioUndergroundThemeTiming[35] = 1;
		MarioUndergroundThemeSong[36] = Note_C4s; MarioUndergroundThemeTiming[36] = 2;
		MarioUndergroundThemeSong[37] = Note_C4;  MarioUndergroundThemeTiming[37] = 4;
		MarioUndergroundThemeSong[38] = Note_D4s; MarioUndergroundThemeTiming[38] = 4;
		MarioUndergroundThemeSong[39] = Note_D4;  MarioUndergroundThemeTiming[39] = 4;
		MarioUndergroundThemeSong[40] = Note_G3s; MarioUndergroundThemeTiming[40] = 4;
		MarioUndergroundThemeSong[41] = Note_G3;  MarioUndergroundThemeTiming[41] = 4;
		MarioUndergroundThemeSong[42] = Note_C4s; MarioUndergroundThemeTiming[42] = 4;
		MarioUndergroundThemeSong[43] = Note_C4;  MarioUndergroundThemeTiming[43] = 1;
		MarioUndergroundThemeSong[44] = Note_F4s; MarioUndergroundThemeTiming[44] = 2;
		MarioUndergroundThemeSong[45] = Note_F4;  MarioUndergroundThemeTiming[45] = 1;
		MarioUndergroundThemeSong[46] = Note_E4;  MarioUndergroundThemeTiming[46] = 1;
		MarioUndergroundThemeSong[47] = Note_A4s; MarioUndergroundThemeTiming[47] = 2;
		MarioUndergroundThemeSong[48] = Note_A4;  MarioUndergroundThemeTiming[48] = 2;
		MarioUndergroundThemeSong[49] = Note_G4s; MarioUndergroundThemeTiming[49] = 4;
		MarioUndergroundThemeSong[50] = Note_D4s; MarioUndergroundThemeTiming[50] = 4;
		MarioUndergroundThemeSong[51] = Note_B3;  MarioUndergroundThemeTiming[51] = 4;
		MarioUndergroundThemeSong[52] = Note_A3s; MarioUndergroundThemeTiming[52] = 5;
		MarioUndergroundThemeSong[53] = Note_A3;  MarioUndergroundThemeTiming[53] = 5;
		MarioUndergroundThemeSong[54] = Note_G3s; MarioUndergroundThemeTiming[54] = 5;
		MarioUndergroundThemeSong[55] = Note_END; MarioUndergroundThemeTiming[55] = 0;
}
void JurassicParkThemeInit(){
		JurassicParkThemeSong[0] = Note_XX;	JurassicParkThemeTiming[0] = 2;
		JurassicParkThemeSong[1] = Note_C4;	JurassicParkThemeTiming[1] = 2;
		JurassicParkThemeSong[2] = Note_G4;	JurassicParkThemeTiming[2] = 10;
		JurassicParkThemeSong[3] = Note_C4;	JurassicParkThemeTiming[3] = 2;
		JurassicParkThemeSong[4] = Note_A4;	JurassicParkThemeTiming[4] = 10;
		JurassicParkThemeSong[5] = Note_XX;	JurassicParkThemeTiming[5] = 1;
		JurassicParkThemeSong[6] = Note_A4;	JurassicParkThemeTiming[6] = 2;
		JurassicParkThemeSong[7] = Note_B4;	JurassicParkThemeTiming[7] = 3;
		JurassicParkThemeSong[8] = Note_C5;	JurassicParkThemeTiming[8] = 6;
		JurassicParkThemeSong[9] = Note_XX;	JurassicParkThemeTiming[9] = 1;
		JurassicParkThemeSong[10] = Note_C5;	JurassicParkThemeTiming[10] = 3;
		JurassicParkThemeSong[11] = Note_B4;	JurassicParkThemeTiming[11] = 6;
		JurassicParkThemeSong[12] = Note_G4;	JurassicParkThemeTiming[12] = 6;
		JurassicParkThemeSong[13] = Note_A4;	JurassicParkThemeTiming[13] = 8;
		JurassicParkThemeSong[14] = Note_END;	JurassicParkThemeTiming[14] = 0;
}
void initSongs(){
	for(unsigned char i = 0; i < MAXSONGSIZE; ++i){
		JurassicParkThemeSong[i] = 0;
		JurassicParkThemeTiming[i] = 0;
		MarioUndergroundThemeSong[i] = 0;
		MarioUndergroundThemeTiming[i] = 0;
		MarioThemeSong[i] = 0;
		MarioThemeTiming[i] = 0;
		PiratesThemeSong[i] = 0;
		PiratesThemeTiming[i] = 0;
	}
	PiratesThemeInit();
	MarioThemeInit();
	MarioUndergroundThemeInit();
	JurassicParkThemeInit();
	PokemonThemeInit();

}


enum Songs { JurassicParkTheme =0, MarioUndergroundTheme=1, MarioTheme = 2, PokemonTheme = 3, PiratesTheme = 4, NUMSONGS };
// End Songs Portion ==================================================================================
// PlaySongSM uses input ALARMON flag
enum PlayAlarmSoundSMStates { PlaySong_wait, PlaySong_play, PlaySong_hold };
unsigned char currentSong;
unsigned char tmpSong;
unsigned char TEMPSONGON;
unsigned char songToPlay;
signed char PlayAlarmSoundSMTick(signed char state){
	static unsigned char currentNote;
	static unsigned char NoteLength;
	static double NoteTone;
	
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
			//PORTB = NoteLength;
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