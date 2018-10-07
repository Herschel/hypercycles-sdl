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
// RAYCAST.C

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "raycast.h"
#include "optdemo.h"
#include "screen.h"
#include "trig.h"
#include "fixpoint.h"
#include <math.h>

unsigned char bottoms[322], tops[322];
unsigned char first_bottoms[322], second_tops[322];
unsigned int first_sliver_len[322];

int lside[201], rside[201];
int g_xview, g_yview;

// External variables:
void Render_Sliver( int pic_num, int scale, int column, int sl_col);

int z1, z2, z3, z4, z5, z6, z7, z8, z9, z0,z11,z12, z4a, z7a, r_angle, l_angle;
char *cptr, *flptr;
int z5_table[6000];
int last_va=-1, last_fs[322], last_fs2[322];
int last_fxi[110], last_fyi[110], last_fx[110], last_fy[110];
int floor_resol=1;

extern unsigned char wall_ht_map[4098];
extern int xmaze_sq, ymaze_sq, res_def;
extern unsigned char *double_buffer_c;
extern char *world[64];
extern char *flrmap[64];
extern char *ceilmap[64];
extern int prm_window_height;
extern int prm_window_middle;
extern int prm_window_bottom;
extern int prm_width_sh;
extern int prm_left;
extern int prm_right;
extern int game_mode;

extern int cos_tab2[4096];
extern int sin_tab2[4096];
extern int sliver_dist[321];                                                             

extern int left_edge, right_edge;

extern struct pic_def
{
  unsigned int * image;
  int            width;
  int            height;
  int            ratio;
} picture[150];

extern int *atan_table; 
extern int *row_table;
extern int viewer_height;
//extern unsigned char far *texturelist[15];

int getdistance(int degrees,int column_angle,int x,int y);
int getd2( int x, int y, int xview, int yview );

int column_angle;

void Bld_Ang(void)
{
  int a;  
  column_angle=atan((float)(prm_left-160) / VIEWER_DISTANCE)*(NUMBER_OF_DEGREES/6.28);
  if (column_angle<0) column_angle+=NUMBER_OF_DEGREES;
  if (column_angle>NUMBER_OF_DEGREES-1) column_angle-=NUMBER_OF_DEGREES;
  l_angle=column_angle;

  column_angle=atan((float)(prm_right-160) / VIEWER_DISTANCE)*(NUMBER_OF_DEGREES/6.28);
  if (column_angle<0) column_angle+=NUMBER_OF_DEGREES;
  if (column_angle>NUMBER_OF_DEGREES-1) column_angle-=NUMBER_OF_DEGREES;
  r_angle=column_angle;
  z3=viewer_height<<SHIFT;
  z5= VIEWER_DISTANCE * WALL_HEIGHT;
  z6 = VIEWER_DISTANCE * viewer_height;
  for(a=101;a<=200;a++)
  {
   lside[a]=fixdiv(row_table[a],COS(l_angle));   
   rside[a]=fixdiv(row_table[a],COS(r_angle));
  }

  for(a=1;a<6000;a++)
  {      
    z5_table[a] = z5 / a;
  }
  z5_table[0] = z5 ;
}

                
void draw_maze( int xview,int yview,int viewing_angle)
{
  // Draws a raycast image in the viewport of the maze represented
  // in array MAP[], as seen from position XVIEW, YVIEW by a
  // viewer looking at angle VIEWING_ANGLE where angle 0 is due
  // north. (Angles are measured in radians.)

  // Variable declarations:

  int fix_xcross_x,fix_xcross_y; // Ray intersection coordinates
  int fix_ycross_x,fix_ycross_y;
  int xdist,ydist; // Distance to x and y grid lines was unsig
  int xmaze,ymaze;     // Map location of ray collision
  int distance;  // Distance to wall along ray
  int tmcolumn;        // Column in texture map

  short int column, degrees, row;
  int fix_x, fix_y;
  int xdiff, ydiff, fix_slope, height;
  int fix_slope2;
  int right_x, right_y, fix_x_increment, fix_y_increment;
  int fix_distance, left_x, left_y;
  char looper, last_ctr;
  int maze_cube;
  int zbuf_dist, zbuf_xm, zbuf_ym, zbuf_tmcol, zbuf_mc;
  int fix_xinc, fix_yinc;

  short int *z11p, *z12p;

  z11p = (short int *) &z11;
  z12p = (short int *) &z12;

  z11p++; z12p++;

  // Loop through all columns of pixels in viewport:
  g_xview = xview << SHIFT;   // These two are for optimization; preshifted
  g_yview = yview << SHIFT;
    
  
  // Figure FOV for object check
  column_angle=atan_table[prm_left];
  degrees=(viewing_angle+column_angle) & 4095;
  left_edge = degrees;

  column_angle=atan_table[prm_right];
  degrees=(viewing_angle+column_angle) & 4095;
  right_edge = degrees;

  // Done FOV chks
  memset(tops,200,320);
  memset(first_bottoms,0,320);
  memset(first_sliver_len,0,320);      
  
  for(column=prm_left; column<=prm_right; column++) 
  {
    // Get horizontal angle of ray relative to center ray:
    column_angle=atan_table[column];
        
    // Calculate angle of ray relative to maze coordinates
    degrees=(viewing_angle+column_angle) & 4095;

    // Rotate endpoint of ray to viewing angle:
    xdiff = sin_tab2[degrees];
    ydiff = cos_tab2[degrees];

    // Initialize ray at viewer's position:
    fix_x=g_xview;
    fix_y=g_yview;

    // Cheat to avoid divide-by-zero error:
    if (!xdiff) xdiff=1;
    if (!ydiff) ydiff=1;

    if(xdiff>0) fix_xinc = 4194304;
    else fix_xinc = -65536;

    if(ydiff>0) fix_yinc = 4194304;
    else fix_yinc = -65536;

    if(last_va != viewing_angle)
    {
      // Get slope of ray:
      fix_slope  = fixdiv( ydiff<<SHIFT, xdiff<<SHIFT);
      fix_slope2 = fixdiv( 65536 , fix_slope);
      last_fs[column]  = fix_slope; 
      last_fs2[column] = fix_slope2;
    }
    else
    {
      fix_slope  = last_fs[column];
      fix_slope2 = last_fs2[column];
    }

    // Cast ray from grid line to grid line:
    looper = xmaze = ymaze = 0;
    zbuf_dist=-1;

    while(!looper)
    {

      // Get x,y coordinates where ray crosses x grid line:
      fix_xcross_x = ( fix_x & 0xffc00000) + fix_xinc;
      fix_xcross_y = fix_y + fixmul( fix_slope , fix_xcross_x - fix_x);

      // Get x,y coordinates where ray crosses y grid line:
      fix_ycross_y = ( fix_y & 0xffc00000) + fix_yinc;
      fix_ycross_x = fix_x + fixmul( fix_slope2, fix_ycross_y - fix_y);

      // Get distance to x grid line:
      xdist = getd2( fix_xcross_x, fix_xcross_y, xview, yview);
        
      // Get distance to y grid line:
      ydist = getd2( fix_ycross_x, fix_ycross_y, xview, yview);

      // If x grid line is closer...
      if ( xdist < ydist) 
      {
        // Calculate maze grid coordinates of square:
        xmaze = fix_xcross_x >> 22;
        ymaze = fix_xcross_y >> 22;

        // Set x and y to point of ray intersection:
        fix_x = fix_xcross_x;
        fix_y = fix_xcross_y;

        // Is there a maze cube here? If so, stop looping:
        if (world[ymaze][xmaze]) 
        { 
          if(wall_ht_map[(ymaze<<6) + xmaze]!=63)
          {
            if(zbuf_dist==-1)
            {
              zbuf_dist = getdistance( degrees, column_angle, fix_x, fix_y);
              // Non-standard ht so store in zbuf
              zbuf_tmcol = ( fix_y >> SHIFT) & 0x3f;
              zbuf_xm = xmaze;
              zbuf_ym = ymaze;
              zbuf_mc = world[ymaze][xmaze];
            }
          }
          else
          {
            looper++; 
            maze_cube = world[ymaze][xmaze]; 
            // Find relevant column of texture map:
            tmcolumn = ( fix_y >> SHIFT) & 0x3f;
          }
        }
      }
      else // If y grid line is closer:
      {
        
        // Calculate maze grid coordinates of square:
        xmaze= fix_ycross_x >> 22;
        ymaze= fix_ycross_y >> 22;

        // Set x and y to point of ray intersection:
        fix_x = fix_ycross_x;
        fix_y = fix_ycross_y;

        // Is there a maze cube here? If so, stop looping:
        if (world[ymaze][xmaze]) 
        { 
          if(wall_ht_map[(ymaze<<6) + xmaze]!=63)
          {
            if(zbuf_dist==-1)
            {
              zbuf_dist = getdistance( degrees, column_angle, fix_x, fix_y);
              zbuf_tmcol = ( fix_x >> SHIFT) & 0x3f;
              zbuf_xm = xmaze;
              zbuf_ym = ymaze;
              zbuf_mc = world[ymaze][xmaze];
            }
          }
          else
          {
            looper++; 
            maze_cube = world[ymaze][xmaze]; 
            // Find relevant column of texture map:
            tmcolumn = ( fix_x >> SHIFT) & 0x3f;
          }
        }
      }
    }
    distance = getdistance( degrees, column_angle, fix_x, fix_y);
    if (!distance) distance=1;

    // Store distance to sliver in array
    sliver_dist[column] = distance;

    // Calculate visible height of wall:
    //height = z5 / distance;
    height = z5_table[distance];    
    if (!height) height=1;

    // If top of current vertical line is outside of viewport, clip it:
    //dheight = height;
    xmaze_sq = xmaze;
    ymaze_sq = ymaze;
    Render_Sliver( maze_cube, height, column, tmcolumn);

    if(zbuf_dist>=0)
    {
      distance = zbuf_dist;
      if (!distance) distance=1;

      // Store distance to sliver in array
      sliver_dist[column] = distance;

      // Calculate visible height of wall:
      //height = z5 / distance;
      height = z5_table[distance];    
      if (!height) height=1;

      // If top of current vertical line is outside of viewport, clip it:
      //dheight = height;
      xmaze_sq = zbuf_xm;
      ymaze_sq = zbuf_ym;
      Render_Sliver( zbuf_mc, height, column, zbuf_tmcol);
    }
  }

  if( res_def==1)
  {
    last_va = viewing_angle;
    return;
  }  
  // Step through floor & ceiling pixels:
  
  switch(floor_resol)
  {
    case 0:
            
          z4a = 32320 + prm_left;
          z7a = 31360 + prm_left;
          z8 = 98;
          
          for (last_ctr=0,row = 101; row <= prm_window_bottom; row++, last_ctr++) 
          {
            if( last_va != viewing_angle )
            {
              // Calculate horizontal angle of leftmost column relative to center ray:
              column_angle=l_angle;
           
              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=lside[row];

              // Rotate distance to ray angle:
              left_x = - (fixmul(fix_distance,SIN(degrees)));
              left_y =    fixmul(fix_distance,COS(degrees));

              last_fx[last_ctr]  = left_x;
              last_fy[last_ctr]  = left_y; 

              // Translate relative to viewer coordinates:
              left_x+=g_xview;
              left_y+=g_yview;

              // Calculate horizontal angle of rightmost column relative to center ray:
              column_angle=r_angle;

              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=rside[row];
              
              // Rotate distance to ray angle:
              right_x = - (fixmul(fix_distance,SIN(degrees)));
              right_y = fixmul(fix_distance,COS(degrees));

              // Translate relative to viewer coordinates:
              right_x+=g_xview;
              right_y+=g_yview;

              // Calculate stepping increment:
              fix_x_increment = fixdiv((right_x-left_x), prm_width_sh ); //20840448); //318 << SHIFT);
              fix_y_increment = fixdiv((right_y-left_y), prm_width_sh ); //20840448); //318 << SHIFT);
              fix_x = left_x;
              fix_y = left_y;
              last_fxi[last_ctr] = fix_x_increment;
              last_fyi[last_ctr] = fix_y_increment;
            }
            else
            { 
              fix_x_increment = last_fxi[last_ctr];    
              fix_y_increment = last_fyi[last_ctr];    
              fix_x = last_fx[last_ctr] + g_xview;
              fix_y = last_fy[last_ctr] + g_yview;
            }

            z4=z4a;
            z7=z7a;
            //flptr=(char*) picture[22].image;  //W Floor Panel
            for(z1=prm_left; z1<=prm_right ;z1++)
            {
              if(bottoms[z1] <= row) 
              {
                // Find pixel in grid square
                z11 = fix_y >> 10;
                z12 = fix_x >> SHIFT; //16
                z2=(z11 & 0xfc0) + (z12 & 0x3f);
                // Find which panel in floor map
                z11 = z11 >> 12 & 0x3f;
                z12 = z12 >> 6  & 0x3f;
                z0 = flrmap[z11][z12];
                //z0 = 'A';
                flptr=(char*) picture[z0 - 'A'].image;  // Floor Panel
             
                double_buffer_c[z4] = flptr[z2];
              }
              fix_x+=fix_x_increment;  
              fix_y+=fix_y_increment;
              z4 ++;
              z7 ++;
            }    
            
            z4a+=320;
            z7a-=320;
            z8--;    
          }
          last_va = viewing_angle;
      break;
    case 1:
            
          z4a = 32320 + prm_left;
          z7a = 31360 + prm_left;
          z8 = 98;
          
          for (last_ctr=0,row = 101; row <= prm_window_bottom; row++, last_ctr++) 
          {
            if( last_va != viewing_angle )
            {
              // Calculate horizontal angle of leftmost column relative to center ray:
              column_angle=l_angle;
           
              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=lside[row];

              // Rotate distance to ray angle:
              left_x = - (fixmul(fix_distance,SIN(degrees)));
              left_y =    fixmul(fix_distance,COS(degrees));

              last_fx[last_ctr]  = left_x;
              last_fy[last_ctr]  = left_y; 

              // Translate relative to viewer coordinates:
              left_x+=g_xview;
              left_y+=g_yview;

              // Calculate horizontal angle of rightmost column relative to center ray:
              column_angle=r_angle;

              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=rside[row];
              
              // Rotate distance to ray angle:
              right_x = - (fixmul(fix_distance,SIN(degrees)));
              right_y = fixmul(fix_distance,COS(degrees));

              // Translate relative to viewer coordinates:
              right_x+=g_xview;
              right_y+=g_yview;

              // Calculate stepping increment:
              fix_x_increment = fixdiv((right_x-left_x), 10485760); //prm_width_sh ); 
              fix_y_increment = fixdiv((right_y-left_y), 10485760); //prm_width_sh ); 
              fix_x = left_x;
              fix_y = left_y;
              last_fxi[last_ctr] = fix_x_increment;
              last_fyi[last_ctr] = fix_y_increment;
            }
            else
            { 
              fix_x_increment = last_fxi[last_ctr];    
              fix_y_increment = last_fyi[last_ctr];    
              fix_x = last_fx[last_ctr] + g_xview;
              fix_y = last_fy[last_ctr] + g_yview;
            }

            z4=z4a;
            z7=z7a;
            //flptr=(char*) picture[22].image;  //W Floor Panel
            for(z1=prm_left; z1<=prm_right ;z1+=2)
            {
              if(bottoms[z1] <= row) 
              {
                // Find pixel in grid square
                z11 = fix_y >> 10;
                z12 = fix_x >> SHIFT; //16
                z2=(z11 & 0xfc0) + (z12 & 0x3f);
                // Find which panel in floor map
                z11 = z11 >> 12 & 0x3f;
                z12 = z12 >> 6  & 0x3f;
                z0 = flrmap[z11][z12];
                //z0 = 'A';
                flptr=(char*) picture[z0 - 'A'].image;  // Floor Panel
             
                
                z9=flptr[z2];
                double_buffer_c[z4] = z9;
                double_buffer_c[z4+1] = z9;
              }
              fix_x+=fix_x_increment;  
              fix_y+=fix_y_increment;
              z4 +=2;
              z7 ++;
            }    
            
            z4a+=320;
            z7a-=320;
            z8--;    
          }
          last_va = viewing_angle;
      break;
    case 2:
            
          z4a = 32320 + prm_left;
          z7a = 31360 + prm_left;
          z8 = 98;
          
          for (last_ctr=0,row = 101; row <= prm_window_bottom; row++, last_ctr++) 
          {
            if( last_va != viewing_angle )
            {
              // Calculate horizontal angle of leftmost column relative to center ray:
              column_angle=l_angle;
           
              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=lside[row];

              // Rotate distance to ray angle:
              left_x = - (fixmul(fix_distance,SIN(degrees)));
              left_y =    fixmul(fix_distance,COS(degrees));

              last_fx[last_ctr]  = left_x;
              last_fy[last_ctr]  = left_y; 

              // Translate relative to viewer coordinates:
              left_x+=g_xview;
              left_y+=g_yview;

              // Calculate horizontal angle of rightmost column relative to center ray:
              column_angle=r_angle;

              // Calculate angle of ray relative to maze coordinates
              degrees=(viewing_angle+column_angle) & 4095;

              // Get distance to visible pixel:
              fix_distance=rside[row];
              
              // Rotate distance to ray angle:
              right_x = - (fixmul(fix_distance,SIN(degrees)));
              right_y = fixmul(fix_distance,COS(degrees));

              // Translate relative to viewer coordinates:
              right_x+=g_xview;
              right_y+=g_yview;

              // Calculate stepping increment:
              fix_x_increment = fixdiv((right_x-left_x), 5242880); //prm_width_sh ); 
              fix_y_increment = fixdiv((right_y-left_y), 5242880); //prm_width_sh ); 
              fix_x = left_x;
              fix_y = left_y;
              last_fxi[last_ctr] = fix_x_increment;
              last_fyi[last_ctr] = fix_y_increment;
            }
            else
            { 
              fix_x_increment = last_fxi[last_ctr];    
              fix_y_increment = last_fyi[last_ctr];    
              fix_x = last_fx[last_ctr] + g_xview;
              fix_y = last_fy[last_ctr] + g_yview;
            }

            z4=z4a;
            z7=z7a;
            //flptr=(char*) picture[22].image;  //W Floor Panel
            for(z1=prm_left; z1<=prm_right ;z1+=4)
            {
              if(bottoms[z1] <= row) 
              {
                // Find pixel in grid square
                z11 = fix_y >> 10;
                z12 = fix_x >> SHIFT; //16
                z2=(z11 & 0xfc0) + (z12 & 0x3f);
                // Find which panel in floor map
                z11 = z11 >> 12 & 0x3f;
                z12 = z12 >> 6  & 0x3f;
                z0 = flrmap[z11][z12];
                //z0 = 'A';
                flptr=(char*) picture[z0 - 'A'].image;  // Floor Panel
             
                z9=flptr[z2];
                double_buffer_c[z4] = z9;
                double_buffer_c[z4+1] = z9;
                double_buffer_c[z4+2] = z9;
                double_buffer_c[z4+3] = z9;
              }
              fix_x+=fix_x_increment;  
              fix_y+=fix_y_increment;
              z4 +=4;
              z7 ++;
            }    
            
            z4a+=320;
            z7a-=320;
            z8--;    
          }
          last_va = viewing_angle;
      break;
  }
}


int getdistance(int degrees,int column_angle,int x,int y)
{
  int b,distance;

  b = degrees >> 9; // / 512;
  if (( (b&3)==0) || ((b&3)==3) ) 
   distance=(fixdiv((y-g_yview)>>SHIFT, COS(degrees-(b<<11))) * COS(column_angle) >>SHIFT);
  else
   distance=(fixdiv((x-g_xview)>>SHIFT, SIN(degrees-(b<<11))) * COS(column_angle) >>SHIFT);

  if(distance<0) distance=-distance;
  return(distance);
}

int getd2( int x, int y, int xview, int yview)
{
  int a,b,c;

  a = (x >>16) - xview;
  b = (y >>16) - yview;
  c = (a*a) + (b*b);
  return(c);
}
