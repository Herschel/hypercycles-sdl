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
// ADT2 File Builders

#include "prelude.h"
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>

#pragma pack(1)
struct adt_struc
{
    char			fname[15];
    int             start;
    int             length;
} adt2[85];
#pragma pack()

struct adt_struc adt1[145];

struct adt_struc adt_single;

extern int GFL1A, GFL1B, GFLTEXT;
int GFL2=0;
FILE *GFL1_FP;

int adt1_init()
{
  FILE *fp1;
  int a,b;
  
  //if(!(fp1=fopen("HYPER1.ADT","rb"))) return(1);
  if(!(fp1=fopen("hyper1r.adt","rb"))) return(1);
  
  for(a=0;a<135;a++) 
  {
    fread( &adt1[a], sizeof( adt_single ), 1, fp1);
    if(adt1[a].fname[0]=='*') break;
    else
    {
      b=0;
      while(adt1[a].fname[b])
      {
        adt1[a].fname[b]= ~adt1[a].fname[b];
        b++;
      }
    }
  }
  // MIKE: Can we verify that we read things correctly?
  fclose(fp1);
  return(0);
}

int adt2_init()
{
  FILE *fp1;
  int a,b;
  
  //if(!(fp1=fopen("HYPER2.ADT","rb"))) return(1);
  if(!(fp1=fopen("hyper2r.adt","rb"))) return(1);
  
  for(a=0;a<120;a++) 
  {
    fread( &adt2[a], sizeof( adt_single ), 1, fp1);
    if(adt2[a].fname[0]=='*') break;
    else
    {
      b=0;
      while(adt2[a].fname[b])
      {
        adt2[a].fname[b]= ~adt2[a].fname[b];
        b++;
      }
    }
  }
  fclose(fp1);
  return(0);
}


int open_adt1(char *fname)
{
  FILE *fpf;
  int a,b=0,start;
  char fn2[20];

  GFL1_FP = NULL;
  strcpy(fn2,fname);

  extern int ADT_FLAG;
  if( !ADT_FLAG )
  {
	  strlower( fn2 );
	  GFL1_FP = fopen( fn2, GFLTEXT ? "r" : "rb" );
	  return 1;
  }

  strupr(fn2);
  
  for(a=0;a<135;a++)
  {
    
    if(adt1[a].fname[0]=='*')break;

    if(fn2[0]==adt1[a].fname[0])
    {
      if(!strcmp(fn2,adt1[a].fname)) { b++; break;}
    }
  }
  // MIKE: Verify that file is found.
  if(!b) return(-1);
  
  GFL1A = adt1[a].length;
  start = adt1[a].start;
  GFL1B = start+ GFL1A-768;
  
  if(!GFLTEXT)
  {
    //GFL1_FP = fopen("HYPER1.ADT", "rb");
    GFL1_FP = fopen("hyper1r.adt", "rb");
  }
  else
  {
    //GFL1_FP = fopen("HYPER1.ADT", "r");
    GFL1_FP = fopen("hyper1r.adt", "r");
  }
  if(GFL1_FP==NULL) return(-1);
  a=fseek( GFL1_FP, start, SEEK_SET);
  if(a<0) return(-1);
  return 1;
}



FILE* open_adt2(char *fname)
{
	FILE* fpf;
	int a, b = 0, start;
  char fn2[20];
  strcpy(fn2,fname);
  extern int ADT_FLAG;
  if( !ADT_FLAG )
  {
	  strlower( fn2 );
	  return fopen( fn2, "r" );
  }

  strupr(fn2);
  //printf("[[%s]]\n",fn2);
  for(a=0;a<120;a++)
  {
    
    if(adt2[a].fname[0]=='*')break;

    if(fn2[0]==adt2[a].fname[0])
    {
      if(!strcmp(fn2,adt2[a].fname)) { b++; break;}
    }
  }
  if(!b) return nullptr;
  
  GFL2 = adt2[a].length;
  start = adt2[a].start;
  
  //fpf = open("HYPER2.ADT", O_RDONLY + O_BINARY);
  fpf = fopen("hyper2r.adt", "rb");
  if(fpf==nullptr) return(nullptr);
  a=fseek( fpf, start, SEEK_SET);
  if(a<0) return(nullptr);
  return(fpf);
}
