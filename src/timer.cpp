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
// TIMER.C
// MIKE:
#include "prelude.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define D32RealSeg(P)    ((((unsigned int) (P)) >> 4) & 0xFFFF)
#define D32RealOff(P)    (((unsigned int) (P))  & 0xF)

void ( _interrupt _far *Old_Timer_Isr)();  // Holds old timer 8 hndlr
void _interrupt _far clkint(void);
void _interrupt _far rmhandler(void);

unsigned TimeOut(void);
static unsigned int clkdivl, clkdivh, clkmod, user_routine_on, soundDelay;


void    *lowp;
void  far *fh;
unsigned int orig_pm_sel, orig_pm_off, orig_rm_seg,orig_rm_off;

unsigned char *D32DosMemAlloc5( unsigned int sz)
{
	// MIKE:
    //union REGS r;
    //r.x.eax = 0x100;
    //r.x.ebx = (sz+15)>>4;
    //int386(0x31, &r, &r);
    //if( r.x.cflag) return((unsigned int) 0);
    //return(void *) ((r.x.eax & 0xffff) <<4);
	printf("D32DosMemAlloc5 %d\n", sz);
	return NULL;
}


void clkrate(int dt1)
{
 unsigned int a;
 a=dt1;
 outp(0x43,0x36);       //sq. wave mode
 outp(0x40,a &0xff);
 outp(0x40,a>>8);

}

void New_Clk_install(void)
{
  // MIKE:
  //union REGS    r;
  //struct SREGS  sr;
  //
  //// Save prot. mode handler
  //r.x.eax = 0x3508;
  //sr.ds = sr.es = 0;
  //int386x(0x21,&r, &r, &sr);
  //orig_pm_sel = sr.es;
  //orig_pm_off = r.x.ebx;

  //// Save Real Mode hdlr 8
  //r.x.eax=0x200;
  //r.h.bl=8;
  //int386(0x31, &r, &r);
  //orig_rm_seg = r.x.ecx;
  //orig_rm_off = r.x.edx;

  //
  //// Copy rmhandler_ to low memory
  //if(! ( lowp = D32DosMemAlloc5(128) ) ) { printf("No DOS Mem\n"); exit(1);}
  //memcpy(lowp, (void *) rmhandler,70);

  //_disable();
  //clkrate(0);
  //clkdivh = 1;
  //clkdivl = 0;
  //clkmod = 0;
  //user_routine_on = 0;
  //_enable();
  //
  //// New prot mode hndlr
  //r.x.eax = 0x2508;
  //fh = (void far *) clkint;
  //r.x.edx = FP_OFF(fh);
  //sr.ds = FP_SEG(fh);
  //sr.es = 0;
  //int386x(0x21, &r, &r, &sr);
  //
  //// New RM hndlr
  //r.x.eax = 0x0201;
  //r.h.bl = 0x08;
  //r.x.ecx = D32RealSeg(lowp);
  //r.x.edx = D32RealOff(lowp);
  //int386(0x31, &r, &r);
	printf("New_Clk_install\n");
}

void New_Clk_uninstall(void)
{
	// MIKE:
  //union REGS    r;
  //struct SREGS  sr;

  //_disable();
  //clkrate(0);
  //r.x.eax = 0x2508;  
  //r.x.edx = orig_pm_off;
  //sr.ds = orig_pm_sel;
  //sr.es = 0;
  //int386x(0x21, &r, &r, &sr);

  //r.x.eax = 0x0201;
  //r.h.bl = 8;
  //r.x.ecx = (unsigned int) orig_rm_seg;
  //r.x.edx = (unsigned int) orig_rm_off;
  //int386(0x31, &r, &r);
  //_enable();
  printf("New_Clk_uninstall\n");
}

void Clk_install(void)
{
	// MIKE:
  //_disable();
  //clkrate(0);
  //clkdivh = 1;
  //clkdivl = 0;
  //clkmod = 0;
  //user_routine_on = 0;
  //Old_Timer_Isr = _dos_getvect( 8 );
  //_dos_setvect( 8 , clkint );
  //_enable();
	printf("Clk_install\n");
}

void Clk_uninstall(void)
{
	// MIKE:
  //_disable();
  //clkrate(0);
  //_dos_setvect( 8 , Old_Timer_Isr );
  //_enable();
  printf("Clk_uninstall\n");
}

void SetClkRate( int count )
{
	// MIKE:
  //_disable();
  //clkdivl = count;
  //clkdivh = 0;
  //if(!count) clkdivh++;
  //clkrate(count);
	//printf("SetClkRate %d\n", count);
}

void StartTimeOut( int delay )
{
	// MIKE:
  //_disable();
  //soundDelay = delay;
	printf("StartTimeout %u\n", delay);
}
 
static unsigned int ia,ib,ic;

void _interrupt far clkint(void)
{
	// MIKE:
//  ib=0;
//  ia= clkdivl + clkmod;
//  clkmod = ia & 0xffff;
//  ia = (ia >>16) + clkdivh;
//  if( ia!=0)
//  {
//    ib++;
//    _enable();
//    _chain_intr( Old_Timer_Isr);
//  }
//  //outp(0x20,0x20);
//  soundDelay--;
//  if(soundDelay || user_routine_on>0 ) goto clkend;
//go_user:
//  user_routine_on++;
//  _enable();
//  ic = TimeOut();
//  _disable();
//  user_routine_on--;
//  if(-soundDelay < ic) goto clk_delay_ok; 
//  soundDelay=0;
//  goto go_user;
//clk_delay_ok:
//  soundDelay+=ic;
//  _enable();
//  clkend:;
//  outp(0x20,0x20);
	printf("clkint\n");
}






