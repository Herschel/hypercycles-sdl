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
        MIDIPLAY.C

        Driver to play a Standard MIDI File (1.0).

        The driver uses the PC's timer-0 for timing. (see TIMER.ASM )

        Interface instructions:
           - Call Midi_Init() to install the interrupt vector.
           - Call Midi_Play (*data) to play the file.
                 *data is a pointer to the already read file
           - Call Midi_End() to remove the interrupt vector.

           - You do not need to call Midi_Init and Midi_End every time you
                 play something.  You can call them once at the beginning and
                 end of your program.
           - The timer-0 is adjusted according to the tempo to adjust the clock
                 interrupt frequency.  The new interrupt routine takes care of
                 chaining to the old interrupt routine at the appropriate moment.
                 Timer-0 is set to its usual state when finished playing.
           - See the file midimain.c for an example.
*/

void Clk_uninstall(void);
void Clk_install(void);
void SetClkRate(int);
void NoteOff(int);
void NoteOn(int,int);
void SetVoiceVolume(int,int);
void SetVoicePitch(int,int);
void SetPitchRange(int);
void Volume_OnOff (int);


#include  "cflags.h"

#include  <stdio.h>
#include  <stdlib.h>
#include  <fcntl.h>
#include  <string.h>

/* Some MIDI codes. */
#define   END_OF_TRACK   0x2f
#define   TEMPO          0x51
#define   NR_CHANS       16
#define   SYSEX_F0       0xf0
#define   SYSEX_F7       0xf7
#define   META           0xff
#define   SEQ_SPECIFIC   0x7f

static int      tracks = 0;            /* number of tracks */
static int      tickQnote;             /* ticks per quarter note */
static UCHAR    **musPtrPtr;           /* ptr into trkPtrs */
static UCHAR    *status;               /* ptr to running status of current track */
//static unsigned clock_rate = 0;        /* original clock interrupt rate */

#define TRACKS 16
static UCHAR *trkPtrs [TRACKS];        /* ptrs to each data track */
static UCHAR trkStats [TRACKS];        /* running status for each track */
static long  abs_time [TRACKS];        /* time of next event for each track */

/* This is to make the program easier to read. */
#define  musPtr     (*musPtrPtr)

/* Keeps track of last change of volume to avoid unnecessary calls to
   change of volume routine. */
int current_vol [MAX_VOICES];
int volume_flag = 1;

/* Flags */
char    musRunning;                    /* != 0 if music is playing */
static char    end_of_data;            /* != 0 if end of data */
static char    clock_in = 0;           /* != 0 if installed */

/* Prototypes */
static SetUp_Data (UCHAR *);
static Start_Melo ();

/*-------------------------------------------------------------------------
        Install the clock interrupt routine.
*/
Midi_Init()
{
        if (clock_in) return;
        Clk_install();
        clock_in = 1;
}


/*-------------------------------------------------------------------------
   Main routine for playing a MIDI file.  It receives an array of function
   pointers which are used to call the routines which process the MIDI
   events.
   Returns 0 if interrupt routine not installed, else returns 1.
*/
Midi_Play (dataPtr)
   UCHAR *dataPtr;
{
	// MIKE TODO:
	return 0;

   //if (!clock_in) return (0);
   SetUp_Data (dataPtr);
   Start_Melo ();
   return (1);
}


/*-------------------------------------------------------------------------
        Uninstall the clock driver ...
*/
Midi_End ()
{
   //int n;
   //for (n=0; n < MAX_VOICES; n++) current_vol [n] = 0;
   Volume_OnOff (0);

   if (clock_in) Clk_uninstall();
   clock_in = 0;
}


/*-------------------------------------------------------------------------
   Get word value from data.  Value is stored MSB first. */
static unsigned Get_Word (ptr)
   UCHAR *ptr;
{
   unsigned n;
   n = *ptr++;
   n = (n << 8) + *ptr++;
   return (n);
}


/* Get long value from data.  Value is stored MSB to LSB. */
static long Get_Long (ptr)
   UCHAR *ptr;
{
   long l = 0L;
   int n;
   for (n=0; n < 4; n++)
          l = (l << 8) + *ptr++;
   return (l);
}


/*-------------------------------------------------------------------------
   Set up trkPtrs, which is an array of pointers, to point to the track
   chunks. Does not modify musPtr. */
static SetUp_Tracks (trcks, chunk)
   int trcks;
   UCHAR *chunk;
{
   int n;
   long length;
   UCHAR **tPtr = trkPtrs;

   for (n=0; n < trcks; n++) {
          length = Get_Long (chunk + 4);
          tPtr [n] = chunk + 8;
          chunk += (length + 8L);
   }
}


/*-------------------------------------------------------------------------
   Reads a variable length value from the MIDI file data and advances the
   data pointer.  */

static long Get_Length ()
{
   long value;
   UCHAR c, *data;

   data = musPtr;
   if ((value = *data++) & 0x80) {
          value &= 0x7f;
          do {
                 value = (value << 7) + ((c = *data++) & 0x7f);
          } while (c & 0x80);
   }
   musPtr = data;
   return (value);
}


/*-------------------------------------------------------------------------
  Set up all of the data structures used in playing a MIDI file. */
static SetUp_Data (dataPtr)
   UCHAR *dataPtr;
{
   long length;
   int i ;

   /* Read file header */
   length = Get_Long (dataPtr + 4);        /* header length */
   tracks = Get_Word (dataPtr + 10);       /* number of tracks */
   tickQnote = Get_Word (dataPtr + 12);    /* ticks per quarter note */

   /* Set musPtr to point to start of first chunk */
   dataPtr += (length + 8L);

   /* Set up the array trkPtrs */
   SetUp_Tracks (tracks, dataPtr);

   /* Initialize arrays */
   for (i=0; i < tracks; i++) {
          musPtrPtr = &trkPtrs [i];         /* set global data ptr */
          abs_time [i] = Get_Length ();     /* set start time for track */
          trkStats [i] = *musPtr;           /* current running status for track */
   }
}

/*-------------------------------------------------------------------------
        Start playing a melody. Set some global pointers and the tempo.  Start
        the clock driver with the first delay (>= 1) */
static Start_Melo ()
{
        extern void StartTimeOut (int);

        musPtrPtr = trkPtrs;
        status = trkStats;

        end_of_data = 0;
        musRunning = 1;

        Set_Tempo (480, 500000L);

#ifdef INT_METHOD
        StartTimeOut (1);
#else
        StartTimeOut (Do_Event ());
#endif
}


/*-------------------------------------------------------------------------
   Stop playing the melody. Reset the clock frequency to normal (18.2 Hz). */
Stop_Melo()
{
        Volume_OnOff (0);
        Set_Original_Clock ();
        musRunning = 0;
}


/*-------------------------------------------------------------------------
   Set clock rate to its original interrupt rate. Note that the clock rate
   has been saved at 10 times its real value in order to preserve some
   accuracy. */
Set_Original_Clock ()
{
   SetClkRate (0);
}


/*-------------------------------------------------------------------------
        Change the tempo.

        Reload the timer-0 with the proper divider for generating
        the appropriate frequency.

        If tempo is zero, reprogram the counter for 18.2 Hz.
*/
Set_Tempo (tickQnote, usec)
        unsigned tickQnote;         /* ticks per quarter note */
        long     usec;              /* micro-seconds per quarter note */
{
        long count;

        if (!tickQnote)
                count = 0L;
        else {
                /* Calculate required interrupt rate (ticks per sec) */
                usec /= 1000L;
                count = (1194L * usec) / tickQnote;
        }

        /* and set the counter: */
        SetClkRate ((unsigned) count);
}


/*-------------------------------------------------------------------------
   Finds the next event to be processed by checking 'abs_times', which
   contains the absolute time (in ticks) of when the next event occurs
   for each track. Sets the global data ptr musPtr to point to the next
   event and sets the running status for that track as well.
   Returns number of ticks until next event. */

static unsigned  Get_Next_Delay ()
{
   static long tickCount = 0;          /* current absolute time */
   static int  ctrk = 0;               /* current track */
   long delta;
   int n, min;

   if (*status != END_OF_TRACK) {
          delta = Get_Length ();              /* get delta time */
          abs_time [ctrk] += delta;           /* set new time for track */
   }
   else abs_time [ctrk] = 0x7fffffffL;    /* impossibly large value */

   /* Find earliest time in abs_time array.  This determines which track
          contains the next event.  */
   for (min=0, n=1; n < tracks; n++)
          if (abs_time [n] < abs_time [min] && trkStats [n] != END_OF_TRACK)
                 min = n;

   if (trkStats [min] == END_OF_TRACK) {
          /* end of data condition for all tracks */
          end_of_data = 1;
          Stop_Melo ();
          return (0);
   }

   delta = abs_time [min] - tickCount; /* calculate time until next event */
   tickCount = abs_time [min];         /* set current time */
   musPtrPtr = &trkPtrs [min];         /* reset data ptr */
   status = &trkStats [min];           /* set running status */
   ctrk = min;

   return ((unsigned) delta);
}

/*-------------------------------------------------------------------------*/
static void myNoteOn (voice, note, volume)
   int voice, note, volume;
{
   if (!volume_flag) {
          /* Exit if sound is disabled. */
          current_vol [voice] = volume;
          return;
   }
   if (!volume) {
          /* A note-on with a volume of 0 is equivalent to a note-off. */
          NoteOff (voice);
          current_vol [voice] = volume;
   }
   else {
          /* Regular note-on */
          if (current_vol [voice] != volume) {
                 SetVoiceVolume (voice, volume);
                 current_vol [voice] = volume;
          }
          NoteOn (voice, note);
   }
}

/*-------------------------------------------------------------------------
   Process a regular MIDI event.  Which routine to call is determined by
   using the 3 LSB's of the high nibble. */

static void  Midi_Event (event)
   unsigned event;
{
   /* Table of # of data bytes which follow a regular midi status byte */
   static int data_bytes [7] = { 2, 2, 2, 2, 1, 1, 2 };

   int stat, voice;

   stat = (event >> 4) & 7;
   voice = event & 0x0f;

   if (voice < MAX_VOICES) switch (stat) {
          case 0:
                 NoteOff (voice);
                 break;
          case 1:
                 myNoteOn (voice, *musPtr, *(musPtr+1));
                 break;
          case 2:
                 if (volume_flag) SetVoiceVolume (voice, *(musPtr+1));
                 current_vol [voice] = *(musPtr+1);
                 break;
          case 5:
                 if (volume_flag) SetVoiceVolume (voice, *musPtr);
                 current_vol [voice] = *musPtr;
                 break;
          case 6:
                 SetVoicePitch (voice, (*(musPtr+1) << 7) | *musPtr);
                 break;
   }

   musPtr += data_bytes [stat];
}


/*-------------------------------------------------------------------------
   Process an Ad Lib specifec meta-event. */

static void  AdLib_Specific (code, data)
   int code;
   unsigned char *data;
{
   if (code == 1) {
          /* Instrument change code.  First byte of data contains voice number.
                 Following bytes contain instrument parameters.  */
          extern void SetVoiceTimbre (int, unsigned *);
          int n;
          unsigned int params [28];
          for (n=0; n < 28; n++) params [n] = data [n+1];
          SetVoiceTimbre ((int) data [0], params);
   }
   else if (code == 2) {
          /* Melo/perc mode code.  0 is melodic, !0 is percussive. */
          extern SetMode (int);
          SetMode ((int) data [0]);
   }
   else if (code == 3) {
          /* Sets the interval over which pitch bend changes will be applied. */
          SetPitchRange ((int) data [0]);
   }
}
unsigned char hh[4];

unsigned char * hex(int a)
{
  int b;
  b = a>>8;
  if( b<10) hh[0]= b +'0';
  else hh[0] = a-10 + 'A';
  b = a & 0xf;
  if( b<10) hh[1]= b +'0';
  else hh[1] = a-10 + 'A';
  hh[2]=0;
  return (hh);
}

/*-------------------------------------------------------------------------
   Process meta-event.  All events other than end-of-track and tempo events
   are ignored.  */
static void  Meta_Event ()
{
   /* musPtr points to the event type byte which follows the 0xff. */
   if (*musPtr == END_OF_TRACK)
   {
          *status = END_OF_TRACK;
          musPtr--;                  /* leave ptr on EOT event */
   }
   else if (*musPtr == TEMPO) {
          long l;
          musPtr += 2;               /* event type and length bytes */
          l = *musPtr;
          l = (l << 8) + *(musPtr+1);
          l = (l << 8) + *(musPtr+2);
          musPtr += 3;
          Set_Tempo (tickQnote, l);
   }
   else if (*musPtr == SEQ_SPECIFIC) {
          UCHAR *data;
          long l;
          musPtr++;                  /* event type byte */
          l = Get_Length ();
          data = musPtr;

          /* Ad Lib midi ID is 00 00 3f. */
          if (data [0] == 0 && data [1] == 0 && data [2] == 0x3f)
          {
            /* The first two bytes after the ID contain the Ad Lib event code.
               The following bytes contain the data pertaining to the event. */

            AdLib_Specific ((data [3] << 8) | data [4], &data [5]);
          }
          musPtr += l;
   }
   else {
          UCHAR dat;
          long l;       
          dat = *musPtr;

          musPtr += 1;               /* event type byte */
          dat = *musPtr;

          l = Get_Length ();   /* event data */

          musPtr += l;   /* event data */

   }
}


/*-------------------------------------------------------------------------*/
static void Sysex_Event (event)
   UCHAR event;
{
   long len = Get_Length ();
   UCHAR event2;
   event2=event;
   /* skip over system exclusive event */
   musPtr += len;
   
}


#ifdef INT_METHOD
/*-------------------------------------------------------------------------
        Interrupt routine. Called by low-level clock driver when
        the delay count has expired.

        'musPtr' always points to the first byte AFTER the timing byte.

        When this routine is called, the active SS  (stack segment) is not
        the original of the application, so take care.
        This routine, and all others called by, must be compiled
        without stack-overflow checking, since the SS has changed!!!

        Return to caller the number of clock ticks to wait for before
        the next call.
*/
unsigned int TimeOut()
{
        unsigned delay;
        if (! musRunning)
           /* Music has not started or has been stopped, so wait the minimum delay ... */
           return 1;

        do
        {
           /* If high bit set, set status, else this is running status. */
           if (*musPtr & 0x80)
           {
             *status = *musPtr;
             musPtr++;
           }

           /* Process event. */
           if (*status == SYSEX_F7 || *status == SYSEX_F0)  Sysex_Event (*status);
           else
           {
             if (*status == META) Meta_Event ();
             else Midi_Event (*status);
           }
          
           /* Read next delta time. */
           delay = Get_Next_Delay ();

        } while (delay == 0 && !end_of_data);

        if (delay == 0) return (1);
        else return (delay);
}


#else
/*-------------------------------------------------------------------------
        Alternate method: instead of all the work being done by the interrupt
        routine, the interrupt routine sets a flag and exits.  The main
        program loops on this flag and processes an event whenever the flag
        is set.  The main program then resets the flag and tells the interrupt
        routine how long to wait until setting the flag again.
        The avantage of this is that it can be used for debugging.  Codeview
        cannot be used for debugging interrupt routines, for example.  As well,
        using "printf" inside an interrupt routine will cause the system to
        hang.
        The avantage of the first method is that you can go off and do other
        things while the music is playing.  With this method, you must watch
        the flag constantly.  Use this method to debug and then switch to the
        interrupt method for your final version.
        To use this method, go into the file "cflags.h", put the define for
        INT_METHOD in comments and recompile midimain.c and this file.
*/

static char timer_signal = 0;

/* Interrupt routine. Called by low-level clock driver when
   the delay count has expired.

   When this routine is called, the active SS  (stack segment) is not
   the original of the application, so take care.
   This routine, and all others called by, must be compiled
   without stack-overflow checking, since the SS has changed!!!
*/
unsigned TimeOut()
{
   timer_signal = 1;
   return (0x7fff);
}

/*
   Processes the next event and returns the delay until the next event
   occurs.
   'musPtr' always points to the first byte AFTER the timing byte.
*/
 int  Do_Event ()
{
        unsigned delay;
        timer_signal = 0;
        if (! musRunning)
                /* Music has not started or has been stopped, so wait the minimum delay ... */
                return 1;

        do {
                /* If high bit set, set status, else this is running status. */
                if (*musPtr & 0x80)
                {
                   *status = *musPtr;
                   musPtr++;
                }

                /* Process event. */
                if (*status == SYSEX_F7 || *status == SYSEX_F0)  Sysex_Event (*status);
                else
                {
                  if (*status == META) Meta_Event ();
                  else Midi_Event (*status);
                }
                /* Read next delta time. */
                delay = Get_Next_Delay ();

        } while (delay == 0 && !end_of_data);

        if (delay == 0) return (1);
          else return (delay);
}

void  Test_Event ()
{
   extern void StartTimeOut (int);

   if (!timer_signal) return;

   StartTimeOut (Do_Event ());
}
#endif
