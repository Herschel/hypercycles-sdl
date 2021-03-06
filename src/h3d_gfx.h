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

// 3d_gfx.h - header file for basic graphics and sprites

// D E F I N E S  ////////////////////////////////////////////////////////////

#define ROM_CHAR_SET_SEG 0xF000  // segment of 8x8 ROM character set
#define ROM_CHAR_SET_OFF 0xFA6E  // begining offset of 8x8 ROM character set

#define VGA256            0x13
#define TEXT_MODE         0x03

#define PALETTE_MASK        0x3c6
#define PALETTE_REGISTER_RD 0x3c7
#define PALETTE_REGISTER_WR 0x3c8
#define PALETTE_DATA        0x3c9

#define SCREEN_WIDTH      (unsigned int)320
#define SCREEN_HEIGHT     (unsigned int)200

#define CHAR_WIDTH        8
#define CHAR_HEIGHT       8


// P R O T O T Y P E S ///////////////////////////////////////////////////////

void Set_Palette(void);

void Plot_Pixel_Fast(unsigned char *img, int x,int y,unsigned char color);

unsigned char Get_Pixel(unsigned char *img, int x, int y);

void PCX_Load(char *filename,int pic_num,int enable_palette);

void PCX_Unload(int pic_num);

void Grap_Bitmap(int orig_pic_num, int pic_num, int xs, int ys, int wide, int ht);

void Grap_Bitmap2(int orig_pic_num, int pic_num, int xs, int ys, int wide, int ht);


