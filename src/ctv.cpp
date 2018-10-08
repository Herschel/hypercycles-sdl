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
// MIKE:
#include "prelude.h"

#include <stdlib.h>
#include <stdio.h>
#include <fcntl.h>
#include <string.h>

//#define  WAIT_TIME                    0x0200
#define  WAIT_TIME                    0x0800

//#define  DMA_VOICE_IN                 0x45
//#define  DMA_VOICE_OUT                0x49

#define  DMA_VOICE_IN                 0x44
#define  DMA_VOICE_OUT                0x48

#define  DSP_ID_CMD                   0x0E0
#define  DSP_VER_CMD                  0x0E1
#define  DSP_VI8_CMD                  0x24
#define  DSP_VO8_CMD                  0x14
#define  DSP_VO2_CMD                  0x17
#define  DSP_VO4_CMD                  0x75
#define  DSP_VO25_CMD                 0x77
#define  DSP_MDAC1_CMD                0x61
#define  DSP_MDAC2_CMD                0x62
#define  DSP_MDAC3_CMD                0x63
#define  DSP_MDAC4_CMD                0x64
#define  DSP_MDAC5_CMD                0x65
#define  DSP_MDAC6_CMD                0x66
#define  DSP_MDAC7_CMD                0x67
#define  DSP_TIME_CMD                 0x40
#define  DSP_SILENCE_CMD              0x80
#define  DSP_PAUSE_DMA_CMD            0x0D0
#define  DSP_ONSPK_CMD                0x0D1
#define  DSP_OFFSPK_CMD               0x0D3
#define  DSP_CONT_DMA_CMD             0x0D4
#define  DSP_INTRQ_CMD                0x0F2
#define  CMS_TEST_CODE                0x0C6
#define  RESET_TEST_CODE              0x0AA
#define  CMS_EXIST                    1
#define  FM_MUSIC_EXIST               2
#define  CTV_VOICE_EXIST              4

#define  FM_WAIT_TIME                 0x40

#define  DMA_ADDX_REG                 0x2
#define  DMA_COUNT_REG                0x3
#define  DMA_MASK_REG                 0x0A
#define  DMA_MODE_REG                 0x0B
#define  DMA_FF_REG                   0x0C
#define  DMA_PAGE_REG                 0x83

int  DMA_CURRENT_PAGE, DMA_CURRENT_ADDX, DMA_CURRENT_COUNT;
int  PAGE_TO_DMA, LEN_L_TO_DMA, LEN_H_TO_DMA, LAST_DMA_OFFSET;

int dmac_page_reg=0x83, dmac_curr_addx_reg=2, dmac_curr_len_reg=3;
unsigned char *digibuf;
extern int new_key;
// #define D32RealSeg(P)    (((( unsigned int) (P)) >>4) &0xFFFF)
// #define D32RealOff(P)    (((unsigned int) (P)) & 0xF)

int io_addr = 0x220, DMA_Channel=1;
int intr_num=5, gl_result, CTV_voice_status=0, dummy;
int dma_var1, dma_var2;
extern char digi_flag;
int digital_speed = 11025;
unsigned char *ems_addr;
extern int ADT_FLAG;

void (_interrupt _far *Orig_Int)();
void (_interrupt _far *Orig_Int2)();
void (_interrupt _far *Orig_Int3)();
void (_interrupt _far *Orig_Int5)();
void (_interrupt _far *Orig_Int7)();

void _interrupt _far Dummy_DMA_Int2(void);
void _interrupt _far Dummy_DMA_Int3(void);
void _interrupt _far Dummy_DMA_Int5(void);
void _interrupt _far Dummy_DMA_Int7(void);
void _interrupt _far DMA_Out_Intr(void);

void Write_DSP(int, int);

void FM_Delay()
{
	// MIKE:
	printf("FM_Delay\n");
  /*int a;
  a=inp(io_addr+8);
  a=inp(io_addr+8);
  a=inp(io_addr+8);
  a=inp(io_addr+8);
  a=inp(io_addr+8);*/
}
 
void Write_FM( int data1)
{
	// MIKE:
	printf("Write_FM %d\n", data1);
  //outp(io_addr+8, (data1 & 0xff));
  //FM_Delay();
  //outp(io_addr+9, (data1 >> 8) );
  //FM_Delay();
}

int Write_DSP_Time( int port, int data5)
{
	// MIKE:
	printf("Write_DSP_Time %d %d\n", port, data5);
	return 0;
  //int a,c;

  //for( c = 0; c< WAIT_TIME; c++)
  //{
  //  a = inp(port);
  //  if(a>127) { outp( port, data5); return(0); }
  //}
  //return(1);
}

int Read_DSP_Time()
{
	// MIKE:
	printf("Read_DSP_Time\n");
	return 0;
  //int a,c;

  //for( c = 0; c< WAIT_TIME; c++)
  //{
  //  a = inp( io_addr + 14);
  //  if( a >127 ) { gl_result = inp( io_addr + 10); return(0); }
  //}
  //return(1);
}

int Wait_FM_Status( int data5 )
{
  int a,b,c;

  for( c = 0; c< FM_WAIT_TIME; c++)
  {
    a = data5 & 0xe0;
    b = inp( io_addr + 8) & 0xe0;
    if( a==b) return(0);
  }
  return(1);
}  

void Prog_DMA( int dmamode, int page, int curaddr, int curcnt)
{
  
  outp( DMA_MASK_REG, 4 + DMA_Channel);
  outp( DMA_FF_REG, 0);
  outp( DMA_MODE_REG, dmamode + DMA_Channel);
  outp( dmac_curr_addx_reg, (curaddr & 0xff));
  outp( dmac_curr_addx_reg, (curaddr >> 8));
  outp( dmac_curr_len_reg, (curcnt & 0xff));
  outp( dmac_curr_len_reg, (curcnt >>8));
  outp( dmac_page_reg, page);
  outp( DMA_MASK_REG, DMA_Channel);
}  

/*void Prog_DMA( int dmamode, int page, int curaddr, int curcnt)
{
  
  //outp( DMA_MASK_REG, 5);
  outp( DMA_MASK_REG, 4 + DMA_Channel);
  outp( DMA_FF_REG, 0);
  outp( DMA_MODE_REG, dmamode + DMA_Channel);
  //outp( DMA_MODE_REG, dmamode);
  outp( DMA_ADDX_REG, (curaddr & 0xff));
  outp( DMA_ADDX_REG, (curaddr >> 8));
  outp( DMA_COUNT_REG, (curcnt & 0xff));
  outp( DMA_COUNT_REG, (curcnt >>8));
  outp( dmac_page_reg, page);
  //outp( DMA_PAGE_REG, page);
  //outp( DMA_MASK_REG, 1);
  outp( DMA_MASK_REG, DMA_Channel);
} */ 

void _interrupt _far Dummy_DMA_Int2(void)
{
  intr_num=2;
  dummy=inp(io_addr+14);
}
void _interrupt _far Dummy_DMA_Int3(void)
{
  intr_num=3;
  dummy=inp(io_addr+14);
}
void _interrupt _far Dummy_DMA_Int5(void)
{
  intr_num=5;
  dummy=inp(io_addr+14);
}
void _interrupt _far Dummy_DMA_Int7(void)
{
  intr_num=7;
  dummy=inp(io_addr+14);
}

void End_DMA_Transfer()
{
  //int a;
  //unsigned char c,d;
  //outp( DMA_MASK_REG, 4 + DMA_Channel);

  //c = 1 << intr_num;
  //
  //_disable();
  //_dos_setvect( intr_num + 8, Orig_Int);
  //d=inp(0x21);
  //d = d | c;
  //outp(0x21, d);

  //CTV_voice_status = 0;
  //a = inp( io_addr + 14);
  //_enable();
	printf("End_DMA_Transfer\n");
}

void DMA_Out_Transfer()
{
  //cprintf("[L%x][H%x][PTD%x][DCA%x]",LEN_L_TO_DMA,LEN_H_TO_DMA,PAGE_TO_DMA,DMA_CURRENT_ADDX);
  dma_var2 = 0xffff;
  if( !PAGE_TO_DMA) 
  {
    PAGE_TO_DMA++;
    dma_var2 = LAST_DMA_OFFSET;
  }
  dma_var2 -= DMA_CURRENT_ADDX;
  DMA_CURRENT_COUNT = dma_var2 & 0xffff;
  dma_var2 = (dma_var2+1) & 0xffff;
  if(dma_var2)
  {
    if( LEN_L_TO_DMA < dma_var2 ) LEN_H_TO_DMA--;
    LEN_L_TO_DMA = LEN_L_TO_DMA - dma_var2;
  }
  else LEN_H_TO_DMA--;


  Prog_DMA( DMA_VOICE_OUT, DMA_CURRENT_PAGE, DMA_CURRENT_ADDX, DMA_CURRENT_COUNT );
  PAGE_TO_DMA--;
  DMA_CURRENT_PAGE++;
  DMA_CURRENT_ADDX=0;

  Write_DSP( io_addr+12, DSP_VO8_CMD);
  Write_DSP( io_addr+12, (DMA_CURRENT_COUNT & 0xff) );
  Write_DSP( io_addr+12, (DMA_CURRENT_COUNT >> 8) & 0xff );
}

unsigned char aaa;
void _interrupt _far DMA_Out_Intr(void)
{
	// MIKE:
  //// inline asm cld
  //cld_asm();

  //dma_var1 = inp( io_addr+14);
  //if( !LEN_L_TO_DMA  ) End_DMA_Transfer();
  //else DMA_Out_Transfer();
  //_enable();
  //outp(0x20,0x20);
	printf("DMA_Out_Intr\n");
}


int Reset_DSP()
{
	// MIKE:
	printf("Reset_DSP\n");
	return 2;
  //int a,c;

  //outp( io_addr+6, 1);
  //a = inp(io_addr+6);
  //while(a <= 0xff) a++;
  //outp( io_addr+6, 0);
  //for(c=0;c < 0x20; c++)
  //{
  //  a = Read_DSP_Time();
  //  if( !a && gl_result == 0xaa) return(0);
  //}
  //return(2); 
}

int Verify_IO_Chk()
{
   int c;   
   
   c = Write_DSP_Time( io_addr + 12 , DSP_ID_CMD);
   if(c) return(2);

   c = Write_DSP_Time( io_addr + 12 , 0xaa);
   if(c) return(2);
         
   c = Read_DSP_Time();
   if(c || gl_result!=0x55) return(2);

   return(0);
}

int Verify_Intr()
{
//  int a, c;
//
//  Orig_Int2 = _dos_getvect( 2 + 8 );
//  _dos_setvect( 2 + 8, Dummy_DMA_Int2 );
//  
//  Orig_Int3 = _dos_getvect( 3 + 8 );
//  _dos_setvect( 3 + 8, Dummy_DMA_Int3 );
//  
//  Orig_Int5 = _dos_getvect( 5 + 8 );
//  _dos_setvect( 5 + 8, Dummy_DMA_Int5 );
//  
//  Orig_Int7 = _dos_getvect( 7 + 8 );
//  _dos_setvect( 7 + 8, Dummy_DMA_Int7 );
//
//  intr_num = 0;
//  Write_DSP( io_addr+12, DSP_INTRQ_CMD );
//  
//  a=0;
//  for(c=0; c<WAIT_TIME*4; c++)
//  {
//    if(intr_num) goto VI90;
//  }
//  a = 3;
//VI90:
//  _dos_setvect( 2 + 8, Orig_Int2);
//  _dos_setvect( 3 + 8, Orig_Int3);
//  _dos_setvect( 5 + 8, Orig_Int5);
//  _dos_setvect( 7 + 8, Orig_Int7);
// return(a);
	printf("Verify_Intr\n");
	return 0;
} 

void Write_DSP( int port, int data5)
{
	// MIKE:
	printf("Write_DSP %d %d\n", port, data5);
//  int a;
//wd10:
//  a = inp(port);
//  if(a>127) goto wd10;
//  outp(port, data5);
}

int Read_DSP()
{
	printf("Read_DSP\n");
    return 0;
//  int a;
//rd10:
//  a = inp(io_addr+14);
//  if(a>127) goto rd10;
//  a = inp(io_addr+10);
//  return(a);
}

void ON_OFF_Speaker( int stat2)
{
  int a;
  if(!stat2) a=DSP_OFFSPK_CMD;
  else a=DSP_ONSPK_CMD;
  Write_DSP(io_addr+12,a);
  
}

int Chk_DSP_Version()
{
  int a,b;
  Write_DSP( io_addr + 12, DSP_VER_CMD);
  a=Read_DSP();
  b=Read_DSP() + (a << 8);
  if(b < 0x101) return(1);
  return(0);
}

void Pause_DSP_DMA()
{
  //int a;
  //a=128;
  //while(a>127)
  //{
  //  _enable();  
  //  if(!CTV_voice_status) return;
  //  _disable();
  //  a=inp(io_addr+12);
  //}  
  //a=128;
  //while(a>127) a=inp(io_addr+12);
  //outp(io_addr + 12, DSP_PAUSE_DMA_CMD);  
	printf("Pause_DSP_DMA\n");
}

void Old_Calc_Addr( unsigned char *addr1)
{
//  unsigned int a,b,c;

//  a = D32RealSeg(addr1); 
//  b = D32RealOff(addr1);
//  printf(" RS=%x    RO=%x    \n",a,b);
//  a <<= 4;
//  c = a;
//  a = a & 0xf;
//  c = c & 0x0fff0;
//  b += c;
//  a += (b >>16);
//  b = b & 0xffff;


//  DMA_CURRENT_PAGE = a;
//  DMA_CURRENT_ADDX = b;

}

void Calc_Addr( unsigned char *addr1)
{
//  unsigned int a,b;
//  a = (unsigned int) addr1;
//  b = a;
//  a = a & 0xffff;
//  b = b >> 16;

//  DMA_CURRENT_ADDX = a;
//  DMA_CURRENT_PAGE = b;

}

///////////////// MAIN Calling Routines ////////////////////////

void CTV_Speaker(int stat1)
{
 ON_OFF_Speaker(stat1);
}

int CTV_Detect()
{
  int a;
    
  a=Reset_DSP();
  if(!a) return(a);
  a=Verify_IO_Chk();
  if(!a) return(a);
  a=Chk_DSP_Version();
  if(!a) return(a);
  a=Verify_Intr();
  if(!a) return(a);
  
  ON_OFF_Speaker(1);
  return(0);
}

int CTV_Card_Here()
{
  int a,b,c;
  
  b=0;

  // Detect Game Blaster
  outp( io_addr+6 , CMS_TEST_CODE);
  outp( io_addr+10, 0);
  a=inp( io_addr+10);
  outp( io_addr+6 , 0x39);
  outp( io_addr+10, 0);
  c=inp( io_addr+10);
  if( a == CMS_TEST_CODE && c == 0x39) b=CMS_EXIST;
  else
  {
    // Detect for Sound Blaster  
    a=Reset_DSP();
    if(!a) 
    {
      c = Write_DSP_Time( io_addr + 12 , DSP_ID_CMD);
      if(!c)
      {
        c = Write_DSP_Time( io_addr + 12, CMS_TEST_CODE);
        if(!c)
        {
          c = Read_DSP_Time();
          if(!c && gl_result == 0x39) b = CMS_EXIST + CTV_VOICE_EXIST; 
        }
      }
    }
  }

  Write_FM( 1 );
  Write_FM( 0x6004 );
  Write_FM( 0x8004 );

  c = Wait_FM_Status( 0 );
  if(c) return(b);

  Write_FM( 0xff02 );
  Write_FM( 0x2104 );
    
  c = Wait_FM_Status( 0xc0 );
  if(c) return(b);

  Write_FM( 0x6004 );
  Write_FM( 0x8004 );

  b += FM_MUSIC_EXIST;
  return(b);
  
}
  
void CTV_Uninstall()
{
 if(CTV_voice_status)
 {
   Pause_DSP_DMA();
   End_DMA_Transfer();
 }
 ON_OFF_Speaker(0);
}

void Select_DMA_Params()
{
  switch(DMA_Channel)
  {
    case 0:
      dmac_page_reg=0x87;
      dmac_curr_addx_reg=0;
      dmac_curr_len_reg=1;
      break;
    case 1:
      dmac_page_reg=0x83;
      dmac_curr_addx_reg=2;
      dmac_curr_len_reg=3;
      break;
    case 3:
      dmac_page_reg=0x82;
      dmac_curr_addx_reg=6;
      dmac_curr_len_reg=7;
      break;
  }
}

int CTV_Init()
{
  int a,c;

  outp( io_addr+6, 1);
  a = inp(io_addr+6);
  a = inp(io_addr+6);
  a = inp(io_addr+6);
  a = inp(io_addr+6);
  a = inp(io_addr+6);
  a = inp(io_addr+6);
  outp( io_addr+6, 0);
  c=0x64;
  while(c)
  {
    while(c)
    {
      a = inp(io_addr+0xe);
      if(a & 0x80) break;
      c--;
    } 
    if(!c) return(0);

    a = inp(io_addr+0xa);
    if(a==0xaa) return(1);
    c--;
  }
  return(0);
}  


int CTV_Output( unsigned char * bufaddr, int count,int sampling )
{
 //int a, b;
 //unsigned char c,d;
 //
 //if(CTV_voice_status ) return (1);
 //
 //Select_DMA_Params();

 //CTV_voice_status = 1;
 //a = 0xf4240 / sampling;
 //b = a & 0xff;
 //b = -b;

 //Write_DSP( io_addr+12, DSP_TIME_CMD );
 //Write_DSP( io_addr+12, b );

 //c = 1 << intr_num;
 //c = ~c;

 //_disable();
 //Orig_Int = _dos_getvect( intr_num + 8 );
 //_dos_setvect( intr_num + 8, DMA_Out_Intr );

 // d=inp(0x21);
 // d = d & c;
 // outp(0x21, d);

 //// Calculate physical address
 //Calc_Addr( bufaddr );

 //LEN_L_TO_DMA = (count & 0xffff);
 //LEN_H_TO_DMA = 0;

 //a = DMA_CURRENT_ADDX + count - 1;
 //b = ((a >> 16) + DMA_CURRENT_PAGE) & 0xffff;
 //LAST_DMA_OFFSET = a;
 //PAGE_TO_DMA = b - DMA_CURRENT_PAGE;
 //
 //DMA_Out_Transfer();
 //_enable();
 //return (0);
	printf("CTV_Output %d %d\n", count, sampling);
	return 0;
}

int CTV_Halt()
{
  if( !CTV_voice_status) return(1);
  Pause_DSP_DMA();
  End_DMA_Transfer();
  return(0);
}

int CTV_Pause()
{
  if( !CTV_voice_status) return(1);
  Pause_DSP_DMA();
  return(0);
}

int CTV_Continue()
{
  if( !CTV_voice_status) return(1);
  Write_DSP(io_addr+12, DSP_CONT_DMA_CMD);
  return(0);
}

// for testing


unsigned int DOS_Mem_Selector;


unsigned char *D32DosMemAlloc( unsigned int sz)
{
	// MIKE:
    //union REGS r;
    //r.x.eax = 0x100;
    //r.x.ebx = (sz+15)>>4;
    //int386(0x31, &r, &r);
    //if( r.x.cflag) 
    //{
    //  printf("System Error - Could allocate memory for digital sound!\n");
    //  printf("               See HELPME.TXT file for solutions.\n\n");
    //  exit(0);
    //  return((unsigned int) 0);
    //}
    //DOS_Mem_Selector = r.x.edx;
    //return(void *) ((r.x.eax & 0xffff) <<4);
	return NULL;
}

unsigned int D32DosMemFree()
{
    //union REGS r;
    //r.x.eax = 0x101;
    //r.x.edx = DOS_Mem_Selector;
    //int386(0x31, &r, &r);
    //if( r.x.cflag) return(0);
    //return(1); //OK
	// MIKE:
	return 0;
}


extern int GFL2;

FILE* open_adt2(char *fname);
void set_vmode( int vmode);

void play_vox(char * fname)
{
	// MIKE:
	return;

  int a, length;
  FILE* fp;

  if(digi_flag<2) return; 
  //a = CTV_Card_Here();
  
  a = CTV_Detect();
  ON_OFF_Speaker(1);
  CTV_Halt(); 
  
 
  fp = open_adt2(fname);

  if(fp == NULL) 
  {
    /*
     set_vmode(2);
     printf("CTV Error %i\n",fp);
     new_key=0;
     while(!new_key) _enable();
     delay(100);
     new_key=0;
     set_vmode(0x13);
    */
    return;
  }
  
  if(!ADT_FLAG) length = filelen (fp);
  else length=GFL2;
  
  fread(digibuf, 1, length, fp);
  fclose(fp);

  a = CTV_Output( digibuf, length, digital_speed);
}
