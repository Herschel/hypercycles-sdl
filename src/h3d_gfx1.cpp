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
// I N C L U D E S ///////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <string.h>
#include "h3d_gfx.h"

void set_vmode( int vmode);
extern int GFL1A, GFL1B, ADT_FLAG;
extern FILE *GFL1_FP;
int open_adt1(char *fname);

// G L O B A L S  ////////////////////////////////////////////////////////////

struct pic_def
{
  unsigned int * image;
  int            width;
  int            height;
  int            ratio;
} picture[192];

unsigned char red[257], green[257], blue[257];

// F U N C T I O N S /////////////////////////////////////////////////////////

void Set_Palette(void)
{
  // MIKE:
  printf("Set_Palette\n");

  //for(a=0;a<256;a++)
  //{
  //    outp(PALETTE_MASK,0xff);
  //    // tell vga card which register we will be updating
  //    outp(PALETTE_REGISTER_WR, a);
  //    // now update the RGB triple, note the same port is used each time
  //    outp(PALETTE_DATA,red[a]);
  //    outp(PALETTE_DATA,green[a]);
  //    outp(PALETTE_DATA,blue[a]);
  //}

}

//////////////////////////////////////////////////////////////////////////////

void Plot_Pixel_Fast(unsigned char *img, int x,int y,unsigned char color)
{
  // plots the pixel in the desired color a little quicker using binary shifting
  img[(y<<8) + (y<<6) + x] = color;
}

//////////////////////////////////////////////////////////////////////////////

unsigned char Get_Pixel(unsigned char *img, int x, int y)
{
  // gets the pixel from the screen
  return img[(y<<8) + (y<<6) + x];
} // end Get_Pixel

//////////////////////////////////////////////////////////////////////////////

void PCX_Load(char *filename, int pic_num,int enable_palette)
{
  // this function loads a pcx file into a pic structure, the actual image
  // data for the pcx file is decompressed and expanded into a secondary buffer
  // within the picture structure, the separate images can be grabbed from this
  // buffer later.  also the header and palette are loaded
	FILE *fp;
  int num_bytes,index;
  long count;
  unsigned char data, *ibuffer;
  unsigned char temp_buffer[130];
  int xlen,ylen;
  
  // open the file
  //fp = fopen(filename,"rb");
  
	open_adt1(filename);
	fp=GFL1_FP;
  
  if(fp == NULL )
  {
    set_vmode(2);  
    printf("File [%s] not found\n\n",filename);
     exit(1);
  }
  // load the header
  for (index=0; index<128; index++) temp_buffer[index] = getc(fp);
  
  xlen=(temp_buffer[8]+temp_buffer[9]*256)-(temp_buffer[4]+temp_buffer[5]*256);
  ylen=(temp_buffer[10]+temp_buffer[11]*256)-(temp_buffer[6]+temp_buffer[7]*256);

  picture[pic_num].width=xlen;
  picture[pic_num].height=ylen;
  picture[pic_num].ratio=((xlen+1)<<8) / (ylen+1);  // *256
  picture[pic_num].image=0;
  
  ibuffer = (unsigned char*)malloc(xlen*(ylen+1)+16);
  if(ibuffer==NULL) return;
  picture[pic_num].image= (unsigned int *) ibuffer;
  
  // load the data and decompress into buffer
  count=0;
  while( count <= xlen * ylen )
  {
    // get the first piece of data
    data = getc(fp);
    // is this a rle?
    if (data>=192 && data<=255)
    {
      num_bytes = data-192;           // how many bytes in run?
      data  = getc(fp);               // get the actual data for the run
      while(num_bytes-->0) ibuffer[count++] = data;
    }
    else
    { // actual data, just copy it into buffer at next location
      ibuffer[count++] = data;
    }
  }
  if (enable_palette)
  {
    // move to end of file then back up 768 bytes i.e. to begining of palette
    if(!ADT_FLAG) fseek(fp,-768L,SEEK_END);
    else fseek(fp,GFL1B,SEEK_SET);

    // load the pallete into the palette
    for (index=0; index<256; index++)
    {
     red[index]   = (getc(fp) >> 2);
     green[index] = (getc(fp) >> 2);
     blue[index]  = (getc(fp) >> 2);
    }
  }
  fclose(fp);
  // change the palette to newly loaded palette if commanded to do so
  if (enable_palette==2)  Set_Palette();
} // end PCX_Load

//////////////////////////////////////////////////////////////////////////////

void PCX_Unload(int pic_num)
{
 if(!picture[pic_num].image) return;
 free(picture[pic_num].image);
 picture[pic_num].image=0;
}

//////////////////////////////////////////////////////////////////////////////

void Grap_Bitmap(int orig_pic_num, int pic_num, int xs, int ys, int wide, int ht)
{
  // this function will grap a bitmap from the pcx frame buffer.
  int y_off, x,y,z;
  unsigned char *ibuffer, *jbuffer;

  picture[pic_num].width=wide;
  picture[pic_num].height=ht;
  picture[pic_num].image=0;
  picture[pic_num].ratio=((wide)<<8) / (ht);  // *256
      
  // first allocate the memory for the sprite in the sprite structure
  ibuffer = (unsigned char*)malloc(wide*ht + 1);
  if(ibuffer==NULL) return;
  picture[pic_num].image=(unsigned int *) ibuffer;
  // now load the data from the pcx picture
  y_off = picture[orig_pic_num].width+1;
  jbuffer=(unsigned char *) picture[orig_pic_num].image;

  ys=ys*320;
  z=0;      
  for (y=0; y<ht*y_off; y+=y_off)
  {
    for (x=0; x<wide; x++) ibuffer[z++] = jbuffer[ys+y+xs+x];         
  }
}


//////////////////////////////////////////////////////////////////////////////

void Grap_Bitmap2(int orig_pic_num, int pic_num, int xs, int ys, int wide, int ht)
{
  // this function will grap a bitmap from the pcx frame buffer.
  int y_off, x,y,z;
  unsigned char *ibuffer, *jbuffer;

  picture[pic_num].width=wide;
  picture[pic_num].height=ht;
  picture[pic_num].image=0;
      
  // first allocate the memory for the sprite in the sprite structure
  ibuffer = (unsigned char*)malloc(wide*ht + 1);
  if(ibuffer==NULL) return;
  picture[pic_num].image=(unsigned int *) ibuffer;
  // now load the data from the pcx picture
  y_off = picture[orig_pic_num].width+1;
  jbuffer=(unsigned char *) picture[orig_pic_num].image;

  ys=ys*64;
  z=0;      
  for (y=0; y<ht*y_off; y+=y_off)
  {
    for (x=0; x<wide; x++) ibuffer[z++] = jbuffer[ys+y+xs+x];         
  }
}
