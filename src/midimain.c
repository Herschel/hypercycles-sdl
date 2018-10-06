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
   This program consisits of 2 C files and 3 assembler files.  The C files
   are midimain.c and midiplay.c.  The three assembler files are timer.asm,
   outchip.asm and setfreq.asm.
   Libraries are provided for the assembler files so that you do not
   need an assembler and you do not need to know assembly language.
   The libraries are name xADLIB.LIB where x is S, M, L for Small, Medium
   and Large model code.


   Microsoft C, v5.1 instructions
   ------------------------------
   We suggest compiling with the following options:
          cl -AL -Zi -J -Ot -Zp -Gs -c


   QuickC 2.0 Instructions
   -----------------------
   The stack checking must be turned off because the timer interrupt is
   used and the interrupt routines use a different stack.
   The files must be compiled with stack checking off (select "Compiler
   Flags" in the "Make" dialog of the "Options" menu) and they must be
   compiled for the release version ("Make" dialog).  QuickC ignores its
   stack check flag if it is not in release version mode.
   In the "Link Flags" dialog (selected in the "Make" dialog), set the
   global flag to ingnore case.  If this is not done, the linker will not
   find the routines in the library.


   The following files must be linked together to make the executable:
          midimain.obj +
          midiplay.obj +
          adlib.obj +
          timer.obj +
          outchip.obj +
          setfreq.obj

*/

#include  "cflags.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <string.h>
#include  <ctype.h>


/* Keeps track of last change of volume to avoid unnecessary calls to
   change of volume routine. */
extern int current_vol [MAX_VOICES];
extern int volume_flag;

int Music_Address=0x388;

int SoundColdInit(int);
void Midi_Init(void);
void Midi_End(void);
void Stop_Melo(void);
void NoteOff(int);
void SetVoiceVolume(int,int);
void Test_Event(void);
/*-------------------------------------------------------------------------
   Enable or disable the volume, but continue playing the song.
*/
void Volume_OnOff (flag)
   int flag;
{
   int n;
   //if (flag != volume_flag) {
          if (!flag)
                 for (n=0; n < MAX_VOICES; n++) {
                        SetVoiceVolume (n, 0);
                        NoteOff (n);
                 }
          else
                 for (n=0; n < MAX_VOICES; n++) SetVoiceVolume (n, current_vol [n]);
   //}
   volume_flag = flag;
}


/*-------------------------------------------------------------------------
   Read in the midi 1.0 file.
*/

extern int GFL2, ADT_FLAG;

int open_adt2(char *fname);

int midi_length = 0;

UCHAR *Read_Midi_File (file_name)
   char *file_name;
{
   int file ;
   UCHAR *events, *c;
   long length;

   if(!ADT_FLAG) file = open (file_name, O_RDONLY + O_BINARY);
   else file = open_adt2(file_name);
   
   if (file < 0) {
          printf ("Cannot open file %s \n", file_name);
          return (NULL);
   }

   if(!ADT_FLAG) length = filelength (file);
   else length=GFL2;

   if (length > 0xffff) {
          printf ("File too large.\n");
          return (NULL);
   }
   midi_length = length;
   /* Allocate buffer for entire file. */
   c = (UCHAR *) malloc (length);
   if (!c) {
          printf ("Insufficient memory available for reading input file.\n");
          return (c);
   }
   events = c;

   /* Read the file into the buffer. */
   read (file, c, length);

   close (file);
   return (events);
}

/*-------------------------------------------------------------------------
        Wait until the end of melody  (musRunning == 0).
*/
void  WaitEndMelo()
{
   extern char musRunning;
   char c;
   int n;

   printf ("\nWaiting for end.  Press <ESC> to exit.\n");

   while (musRunning) {
          #ifndef INT_METHOD
                 Test_Event ();
          #endif
          if (kbhit())
          {
                  c = getch();
                  if (c == 0x1b) {
                         /* User has pressed ESC.  Shut off all voices.  If they are not
                                shut off, instruments that have sustained sounds will
                                continue to play. */
                         for (n=0; n < MAX_VOICES; n++) {
                                SetVoiceVolume (n, 0);
                                NoteOff (n);
                         }
                         Stop_Melo();
                         return;
                  }
                  else {
                         /* User has pressed 'S' to silence the music, but the song
                                continues to play.  This is useful in applications where
                                you have music playing in the background and you wish to
                                allow the user the choice of whether or not he/she wants to
                                hear the soundtrack. */
                         c = toupper (c);
                         if (c == 'S') Volume_OnOff (!volume_flag);
                  }
          }
          
   }
   printf ("Done\n");
}

/*-------------------------------------------------------------------------
        Midi 1.0 file playback program.
*/
UCHAR *midibuf;
UCHAR *buf;
extern void Midi_Play (UCHAR *);

void reset_buf()
{
  buf=0;
}

void free_buf()
{
  if(buf>0) free( buf);
}

int play_song(char *song)
{

        int n;

        
        /* Initalize the low-level sound-driver: */
        /* MIKE TODO
        if (!SoundColdInit (Music_Address)) {
                printf ("\nAdlib board not found!");
                return(1);
        }
		*/

        /* Read the music file. */
        if(buf>0) free( buf);
        buf = Read_Midi_File (song);
        if (!buf) exit (0);

        midibuf = buf;
        /* Perform some initialisations ... */

        Midi_Init();
        
        Volume_OnOff(1);        
        for (n=0; n < MAX_VOICES; n++) current_vol [n] = 0;

        Midi_Play (buf);

        return(0);
}


void play_again()
{
                int n;
        
        Stop_Melo();
        SoundColdInit (Music_Address);
        buf = midibuf;
        Volume_OnOff(1);        
        for (n=0; n < MAX_VOICES; n++) current_vol [n] = 0;

        Midi_Play (buf);

        return;
}
