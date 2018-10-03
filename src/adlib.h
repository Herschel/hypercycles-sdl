/*
* hypercycles-sdl (https://github.com/Herschel/hypercycles-sdl)
* Copyright (c) 2015 The hypercycles-sdl authors
*
* This program is free software: you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation, version 3.
*
* This program is distributed in the hope that it will be useful, but
* WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
* General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
/*
	ADLIB.H

	Defintions of sound chip parameters.

	1987/01/20, Marc Savary

*/

/* Parameters of each voice: */
#define nbLocParam              14

#define prmKsl                  0
#define prmMulti                1
#define prmFeedBack     2                       /* use for opr. 0 only */
#define prmAttack               3
#define prmSustain              4
#define prmStaining     5                       /* Sustaining ... */
#define prmDecay                6
#define prmRelease              7
#define prmLevel                8
#define prmAm                   9
#define prmVib                  10
#define prmKsr                  11
#define prmFm                   12                      /* use for opr. 0 only */
#define prmWaveSel              13                      /* wave select */

/* globals parameters: */
#define prmAmDepth              14
#define prmVibDepth     15
#define prmNoteSel              16
#define prmPercussion   17

/* melodic voice numbers: */
#define vMelo0                  0
#define vMelo1                  1
#define vMelo2                  2
#define vMelo3                  3
#define vMelo4                  4
#define vMelo5                  5
#define vMelo6                  6
#define vMelo7                  7
#define vMelo8                  8

/* percussive voice numbers: */
#define BD                              6
#define SD                              7
#define TOM                     8
#define CYMB                    9
#define HIHAT                   10


//#define MAX_VOLUME              0x7f
#define LOG2_VOLUME     7                       /* log2( MAX_VOLUME) */
#define MAX_PITCH               0x3fff
#define MID_PITCH               0x2000

#define MID_C                   60                      /* MIDI standard mid C */
#define CHIP_MID_C              48                      /* sound chip mid C */
#define NR_NOTES                96                      /* # of notes we can play on chip */
	
