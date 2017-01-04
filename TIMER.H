/* TIMER.H - Header file for TIMER.C                               */
/* By Jim Dos‚.  This program is released to the public domain and */
/* may be used in your own programs.  Leave me a note if you find  */
/* it helpful!                                                     */
/* On CompuServe: Jim Dose' 76346,2061                             */
/* On AOL:        JimDose                                          */

void Set8253Rate( int speed );
void SetClockRate( int TicksPerSecond );
void InitTimer( int TicksPerSecond );
void RestoreTimer( void );
unsigned long GetCounter( void );
void SetCounter( unsigned long count );
