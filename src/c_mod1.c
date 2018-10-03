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

#include    <conio.h>
#include    <dos.h>
#include    <stdio.h>
#include    <stdlib.h>
// MIKE: #include    <i86.h>



void set_vmode( int vmode)
{
    /* MIKE
	union REGS  regs;

    regs.w.ax = vmode;
    int386( 0x10, &regs, &regs );*/

	printf("set_vmode %d\n", vmode);
}

void gotoxy(int row, int column)
{
   // MIKE
  //  union REGS regs;

  //  regs.h.dl=column;
  //  regs.h.dh=row;
  //  regs.h.bh=0;
  //  regs.h.ah=2;                    /* BIOS cursor address function */
  //  int386(0x10,&regs,&regs);

	printf("gotoxy (%d, %d)", row, column);
}

void clrscrn(void)
{
    //union REGS regs;

    //regs.h.bh=0;
    //regs.h.ah=8;               /*  get character/attribute under cursor  */
    //int386(0x10,&regs,&regs);
    //regs.h.bh=regs.h.ah;
    //regs.w.cx=0;
    //regs.w.dx=0x314f;
    //regs.w.ax=0x0600;          /*  clear screen by scrolling it  */
    //int386(0x10,&regs,&regs);
    //gotoxy(0,0);

	printf("clrscrn\n");
}

void dispch(int ch)
{
    /*union REGS regs;

    regs.h.ah=2;
    regs.h.dl=ch;
    int386(0x21,&regs,&regs);
	*/
	printf("dispch %d\n", ch);
}

void putchat(int ch, int attr)
{
    //union REGS regs;

    //regs.h.al=ch;
    //regs.h.bl=attr;
    //regs.h.bh=0;
    //regs.w.cx=1;
    //regs.h.ah=9;
    //int386(0x10,&regs,&regs);
	printf("putchat %d %d\n", ch, attr);
}

void prints( int row, int column, unsigned char *txtstr, int attr )
{
    /*int a=0,b;
    while(txtstr[a])
    {    
      gotoxy(row,column+a);
      b=txtstr[a];
      putchat(b,attr);
      a++;
    }*/
	printf("prints %d %d %s %d\n", row, column, txtstr, attr);
}  

/*  GETNS.C     - gets a string from the keyboard, limits length  */
int getns(char *str,int maxchars)
{
    //int a,count=0;
    //char *p,ch;

    //p=str;

    //a=0;
    //while(str[a]>0)
    //{
    //  a++;
    //  p++;
    //  putch(str[count]);
    //  count++;
    //}   

    //for(;;) {
    //    ch=getch() & 0xff;     /* get character, no echo */
    //    switch(ch) {
    //        case 0:                     /* is character extended ASCII code? */
    //            ch=getch();
    //            if(ch==80) {*p=0; return(0);}                
    //            if(ch==72) {*p=0; return(2);}                
    //            break;
    //        case 27:                   /* is character = Escape? */
    //            *p=0;                   /* empty receiving string */
    //            return(1);              /* return with error code */
    //        case 13:                    /* is character = Enter? */
    //            *p=0;                   /* add terminating null */
    //            return(0);              /* return normally */
    //        case 8:                    /* is character = BackSpace? */
    //            if(p!=str) {            /* are we at 1st positon of string? */
    //                dispch(ch);         /* display destructive backspace */
    //                dispch(' ');
    //                dispch(ch);
    //                p--;                /* decrement string pointer */
    //                count--;
    //            }
    //            break;
    //        default:                    /* default is normal character */
    //            if(count<maxchars) {    /* make sure not at max length */
    //                *p++=ch;            /* add character to string */
    //                dispch(ch);         /* echo character to screen */
    //                count++;
    //            }
    //    }
    //}
	printf("getns %d\n", maxchars);
	return 0;
}
/*
// Shareware Notice
void Show_Notice(void)  
{
  clrscrn();
  prints(0,0,"±±±±±±±±±±±±±±±±±±±±± Thanks for playing HYPERCYCLES(tm) ±±±±±±±±±±±±±±±±±±±±±", 64+15 ); //red
  prints(1,0,"±   After playing 5 levels of this  \x9 Play All 30 Levels! (25 NEW Levels)    ±", 31); //blue
  prints(2,0,"± exciting  shareware  version  of  \x9 Fire Surface to Air DART Missiles      ±", 31); //blue
  prints(3,0,"± HYPERCYCLES.  You must continue,  \x9 Fight the ominous LASER Tanks          ±", 31); //blue
  prints(4,0,"± you know  you MUST!   Don't miss  \x9 Stalkers, Carriers, coming from above! ±", 31); //blue
  prints(5,0,"± the  30 action packed levels  as  \x9 Get Laser Guns to blow away riders     ±", 31); //blue
  prints(6,0,"± the evil Doctor throws it at you  \x9 Escape from the Deadly Fire Pits       ±", 31); //blue
  prints(7,0,"± at dizzying speeds.  He'll taunt  \x9 Fire away with rear mounted weapons    ±", 31); //blue
  prints(8,0,"± you,  be strong and destroy this  \x9 New Shield Generators to protect you   ±", 31); //blue
  prints(9,0,"± diabolical wizard. But you can't  \x9 Use new long range Grid Radar Scopes   ±", 31); //blue
  prints(10,0,"± win if you don't play.            \x9 Pass thru walls with the Phase Shifter ±", 31); //blue
  prints(11,0,"±                                   \x9 Ultimate face-off with Dr. Devaro      ±", 31); //blue
  prints(12,0,"± So Order Now, All 30 Levels!      \x9 Get cheat goodies, if you need them    ±", 31); //blue
  prints(13,0,"±±±±±±±±±±±±±±±±±±±±±±±±±±±±     How to Order    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±", 47); //green
  prints(14,0,"±  HYPERCYCLES only $ 29 (+ $5 for S&H)         Hey, don't just be cool...   ±", 47); //green
  prints(15,0,"±   Call (812) 875-2852 (voice orders)      Look cool with HYPERCYCLES GEAR! ±", 47); //green
  prints(16,0,"±   FAX  (812) 875-2836 (order by FAX)   Get a cool HYPERCYCLES Hat $ 13.95  ±", 47); //green
  prints(17,0,"± OR WRITE: Aclypse Corporation            Cool HYPERCYCLES T-Shirt $ 13.95  ±", 47); //green
  prints(18,0,"±           RT 2 Box 213H                                                    ±", 47); //green
  prints(19,0,"±           Worthington, IN 47471  USA      HYPERCYCLES (30 Levels) $ 29.00  ±", 47); //green
  prints(20,0,"±   Master Card, VISA, Discover Card,    ±    Available Now!  3.5\" or 5.25\"  ±", 47); //green
  prints(21,0,"± Check, or Money Order  U.S. Funds Only ± -> Add $5 for Shipping & Handling ±", 47); //green
  prints(22,0,"±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±", 47); //green
}
*/

// Registered Notice
void Show_Notice(void)  
{
  clrscrn();
  prints(0,0,"±±±±±±±±±±±±±±±±±±±±± Thanks for playing HYPERCYCLES(tm) ±±±±±±±±±±±±±±±±±±±±±", 64+15 ); //red
  prints(1,0,"±   If you are playing this version \x9 Play All 30 Levels! (25 NEW Levels)    ±", 31); //blue
  prints(2,0,"± of HYPERCYCLES(tm)  and are not a \x9 Fire Surface to Air DART Missiles      ±", 31); //blue
  prints(3,0,"± registered user then we got a big \x9 Fight the ominous LASER Tanks          ±", 31); //blue
  prints(4,0,"± problem.  This is  the registered \x9 Stalkers, Carriers, coming from above! ±", 31); //blue
  prints(5,0,"± version and is only for those who \x9 Get Laser Guns to blow away riders     ±", 31); //blue
  prints(6,0,"± ordered and paid for it.  We know \x9 Escape from the Deadly Fire Pits       ±", 31); //blue
  prints(7,0,"± you are  one  of the  honest ones \x9 Fire away with rear mounted weapons    ±", 31); //blue
  prints(8,0,"± so  all you need  to do  is order \x9 New Shield Generators to protect you   ±", 31); //blue
  prints(9,0,"± HYPERCYCLES(tm)  (all 30  levels) \x9 Use new long range Grid Radar Scopes   ±", 31); //blue
  prints(10,0,"± and all  will be  right with the  \x9 Pass thru walls with the Phase Shifter ±", 31); //blue
  prints(11,0,"± world again!                      \x9 Ultimate face-off with Dr. Devaro      ±", 31); //blue
  prints(12,0,"± So Order Now, All 30 Levels!      \x9 Get cheat goodies, if you need them    ±", 31); //blue
  prints(13,0,"±±±±±±±±±±±±±±±±±±±±±±±±±±±±     How to Order    ±±±±±±±±±±±±±±±±±±±±±±±±±±±±±", 47); //green
  prints(14,0,"±  HYPERCYCLES only $ 29 (+ $5 for S&H)         Hey, don't just be cool...   ±", 47); //green
  prints(15,0,"±   Call (812) 875-2852 (voice orders)      Look cool with HYPERCYCLES GEAR! ±", 47); //green
  prints(16,0,"±   FAX  (812) 875-2836 (order by FAX)   Get a cool HYPERCYCLES Hat $ 13.95  ±", 47); //green
  prints(17,0,"± OR WRITE: Aclypse Corporation            Cool HYPERCYCLES T-Shirt $ 13.95  ±", 47); //green
  prints(18,0,"±           RT 2 Box 213H                                                    ±", 47); //green
  prints(19,0,"±           Worthington, IN 47471  USA      HYPERCYCLES (30 Levels) $ 29.00  ±", 47); //green
  prints(20,0,"±   Master Card, VISA, Discover Card,    ±    Available Now!  3.5\" or 5.25\"  ±", 47); //green
  prints(21,0,"± Check, or Money Order  U.S. Funds Only ± -> Add $5 for Shipping & Handling ±", 47); //green
  prints(22,0,"±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±±", 47); //green
}


