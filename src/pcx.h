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
#define	NO_ERROR 0
#define FILE_NOT_OPENED 1
#define INSUFFICIENT_MEMORY 2
#define TOO_MANY_ARGUMENTS 3

struct pcx_header {
  char manufacturer;
  char version;
  char encoding;
  char bits_per_pixel;
  int  xmin,ymin;
  int  xmax,ymax;
  int  hres;
  int  vres;
  char palette16[48];
  char reserved;
  char color_planes;
  int  bytes_per_line;
  int  palette_type;
  char filler[58];
};

struct pcx_struct {
  pcx_header header;
  unsigned char far *image;
  unsigned char palette[3*256];
  int clength;
  unsigned char far *cimage;
};

int  loadPCX(char far *filename,pcx_struct *pcx);
