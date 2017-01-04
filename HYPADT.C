// ADT2 File Builders

#include <stdlib.h>
#include <stdio.h>
#include <dos.h>
#include <conio.h>
#include <i86.h>
#include <sys/types.h>
#include <fcntl.h>
#include <io.h>
#include <string.h>

struct adt_struc
{
    unsigned char   fname[15];
    int             start;
    int             length;
} adt2[85];

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
  if(!(fp1=fopen("HYPER1R.ADT","rb"))) return(1);
  
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
  fclose(fp1);
  return(0);
}

int adt2_init()
{
  FILE *fp1;
  int a,b;
  
  //if(!(fp1=fopen("HYPER2.ADT","rb"))) return(1);
  if(!(fp1=fopen("HYPER2R.ADT","rb"))) return(1);
  
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


int open_adt1(unsigned char *fname)
{
  //FILE *fpf;
  int a,b=0,start;
  unsigned char fn2[20];

  GFL1_FP = NULL;
  strcpy(fn2,fname);
  strupr(fn2);
  
  for(a=0;a<135;a++)
  {
    
    if(adt1[a].fname[0]=='*')break;

    if(fn2[0]==adt1[a].fname[0])
    {
      if(!strcmp(fn2,adt1[a].fname)) { b++; break;}
    }
  }
  if(!b) return(-1);
  
  GFL1A = adt1[a].length;
  start = adt1[a].start;
  GFL1B = start+ GFL1A-768;
  
  if(!GFLTEXT)
  {
    //GFL1_FP = fopen("HYPER1.ADT", "rb");
    GFL1_FP = fopen("HYPER1R.ADT", "rb");
  }
  else
  {
    //GFL1_FP = fopen("HYPER1.ADT", "r");
    GFL1_FP = fopen("HYPER1R.ADT", "r");
  }
  if(GFL1_FP==NULL) return(-1);
  a=fseek( GFL1_FP, start, SEEK_SET);
  if(a<0) return(-1);
  return(1);
}



int open_adt2(unsigned char *fname)
{
  int fpf,a,b=0,start;
  unsigned char fn2[20];
  strcpy(fn2,fname);
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
  if(!b) return(-1);
  
  GFL2 = adt2[a].length;
  start = adt2[a].start;
  
  //fpf = open("HYPER2.ADT", O_RDONLY + O_BINARY);
  fpf = open("HYPER2R.ADT", O_RDONLY + O_BINARY);
  if(fpf==-1) return(-1);
  a=lseek( fpf, start, SEEK_SET);
  if(a<0) return(-2);
  return(fpf);
}
