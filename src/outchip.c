// OUTCHIP.C
//       Set the register at address 'addr' to the value 'data'.
//       Take care of data register to data register write delay.

#include <stdlib.h>
#include <conio.h>

extern unsigned int genAddr;

void SndOutput( int addr1,int data1)
{
	//// MIKE
 //int a,b,c;
 //c = genAddr;
 //outp(c,addr1);
 //
 //// 12 cyles delay ( 3.6 Mhz )
 //for(a=0;a<6;a++) b=inp(c);
 //
 //outp(c+1,data1);       
 //
 //// 84 cyles delay ( 3.6 Mhz )
 //for(a=0;a<35;a++) b=inp(c);
	//printf("SndOutput %d %d\n", addr1, data1);
}

