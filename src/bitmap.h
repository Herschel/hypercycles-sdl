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

void grab(int x1,int y1,int width,int height,int swidth,
           unsigned char far *screen,
           unsigned char far *buffer);
// void grab188(int x1,int y1,int width,int height,unsigned char far *screen,
//           unsigned char far *buffer);
void blit(int x1,int y1,int width,int height,unsigned char far *screen,
           unsigned char far *buffer);
extern "C" {
  void putscale(int,int,int,int,int,char far *,char far *);
}
