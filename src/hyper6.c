
// I N C L U D E S ///////////////////////////////////////////////////////////
// MIKE
#include "port_types.h"

#include <io.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <dos.h>
/* MIKE
#include <i86.h>
#include <bios.h>
*/
#include <fcntl.h>
/*
#include <malloc.h>
*/
#include <math.h>

#include <string.h>
/*
#include <graph.h>
#include <process.h>
#include "h3d_gfx.h"  // load our graphics library
*/
#include "trig.h"
#include "fixpoint.h"

// MIKE
void Display_Text(int x, int y, char *txt, int color);

#define UCHAR unsigned char
#define SHORT short int
#ifdef USE_ADT_FILES
int ADT_FLAG=1;         //Off=0   On>0
#else
int ADT_FLAG=0;
#endif

// P R O T O T Y P E S //////////////////////////////////////////////////////

int adt1_init();
int adt2_init();

void _interrupt _far New_Key_Int(void);
void Timer(int clicks);
void Create_Scale_Data(int scale, int *row);
void Build_Tables(void);
void Allocate_World(void);
int Load_World(char *file,  char *wptr[64]);
void Wait_For_Vsync(void);
void Draw_Ground(void);

int open_adt1(unsigned char *);

void draw_maze( int ,int ,int );
void Bld_Ang(void);

void set_vmode( int vmode);
void clrscrn(void);
int getns(char *str,int maxchars);
void prints( int row, int column, unsigned char txtstr, int attr );
void Show_Notice(void);

int getdistance(int degrees,int column_angle,int x,int y);

int GFL1A=0, GFL1B=0,GFLTEXT=0;
extern FILE *GFL1_FP;
extern char musRunning;
extern unsigned char *midibuf;
void Midi_End(void);
int play_song( char * );
void play_again();
void Stop_Melo(void);
void Volume_OnOff (int);
int CTV_Init();
extern int volume_flag, digital_speed;

unsigned char *D32DosMemAlloc( unsigned int sz);
unsigned int D32DosMemFree();
extern unsigned char *digibuf;
int Get_EMS_Mem();
void Calc_Addr( unsigned char *addr1);
void Old_Calc_Addr( unsigned char *addr1);
extern int  DMA_CURRENT_PAGE, DMA_CURRENT_ADDX, DMA_CURRENT_COUNT;
extern int floor_resol;

void reset_buf();
void free_buf();
void memuse();
int mem_ok();

// Sound Calls

void play_vox(char *fname);
extern int CTV_voice_status;
extern int MAX_VOLUME;

// T Y P E S ////////////////////////////////////////////////////////////////

typedef int fixed;    // fixed point is 32 bits

// S T R U C T U R E S //////////////////////////////////////////////////////

struct demo_str
{
  int           xc,yc;
  int           speed;
  int           side_mode;
  int           curr_weapon;
  int           command;
  int           stat;
} demo[]={
                {  30, 4, 8,0,0,0,0 },
                {  30, 5,10,0,0,0,0 },
                {  30,10,12,1,0,512,0 },
                {  30,11,12,1,0,1024,0 },
                {  30,16,12,1,0,512,0 },
                {  30,17,12,0,0,0,0 },
                {  30,24,12,0,0,2,0 }, //tr
                {  21,24,12,0,0,2,0 }, //tr
                {  21, 8,14,0,0,3,0 }, //fire
                {  21, 2,16,0,0,1,0 }, //tl
                {   4, 2,16,0,0,1,0 }, //tl
                {   4,11,16,0,0,3,0 }, //fire
                {   4,13,16,0,0,3,0 }, //fire
                
                {   4,17,16,1,0,3584,0 },
                {   4,18,16,1,0,3072,0 },
                {   4,20,16,1,3,3,0 }, //fire
                {   4,21,16,1,3,3,0 }, //fire
                {   4,22,16,1,3,3,0 }, //fire
                
                {   4,25,16,1,0,3584,0 },
                {   4,26,16,1,0,0,0 },
                
                {   4,28,16,0,0,3,0 }, //fire

                {   4,29,16,0,0,1,0 }, //tl

                
                {  34,29,16,0,0, 99,0 },
        };



extern struct pic_def
{
  unsigned int * image;
  int            width;
  int            height;
  int            ratio;
} picture[192];

#pragma pack(1)
struct level_def_struc
{
    unsigned char       level_type;
    char                 description[26];
    char                 wall_map_fname[13];
    char                 floor_map_fname[13];
    char                 ceiling_map_fname[13];
    int                  x, y, view_angle;
    int                  exit_xmaze_sq, exit_ymaze_sq;
    char                 voice_intro_fname[13];
    char                 music_fname[13];
    char                 pcx_tile1_fname[13];
    char                 tile1_assign[16];
    char                 pcx_tile2_fname[13];
    char                 tile2_assign[16];
    char                 pcx_tile3_fname[13];
    char                 tile3_assign[16];
    char                 pcx_tile4_fname[13];
    char                 tile4_assign[16];
    int                  skycolor;
    int                  rings_req;
    int                  max_sim_riders;
    int                  max_tot_riders;
    int                  opt1,opt2,opt3,opt4,opt5,opt6,opt7,opt8;
    
} level_def;
#pragma pack()

#pragma pack(1)
struct obj_def
{
  int            level_num;
  int            image_num;             // Which pic_num
  int            map_letter;
  int            x, y, xcell, ycell, view_angle;    // Locations
  int            xinc, yinc, xdest, ydest;
  int            actx, acty, deactx, deacty; //squares that activate & deactivate object
  int            objtype;
  int            status;
  int            opt1, opt2, opt3, opt4;
} object[152];
#pragma pack()

struct obj_def single;

struct save_game_struc
{
  unsigned char games[15];
  int           level;
  int           diff;  
  int           score, power, shields;
  char          access[32];
  int           protons, neutrons;
  int           darts;
  int           chksum;
} game_data[10];

struct save_game_struc gm_one;

struct drs_st
{
  char sent1[41];
  char sent2[41];
  char fname1[14];
} drs[]={
                { "INDEED YOU HAVE GROWN STRONGER","*","DRS_5A.raw" },
                { "DON'T MAKE ME DESTROY YOU!","*","DRS_5B.raw" },
                { "QUIT NOW AND JOIN MY LEGION","*","DRS_5C.raw" },
                { "IF YOU CHOOSE TO CONTINUE","THIS FUTILE BATTLE","DRS_5D.raw" },
                { "I WILL SHOW NO MERCY", "IN YOUR DEMISE","DRS_5E.raw" },
        };

struct drf_st
{
  char sent1[41];
  char sent2[41];
  char fname1[14];
} drf[]={
                { "SO YOU THINK YOU HAVE WON","*","DRF_1.raw" },
                { "YES THIS BATTLE IS OVER FOR NOW","*","DRF_2.raw" },
                { "BUT THERE WILL BE ANOTHER TIME","*","DRF_3.raw" },
                { "BECAUSE I WILL BE BACK!","*","DRF_4.raw" },
        };

struct drface
{
  char sent1[41];
  char fname1[14];
  char sent2[41];
  char fname2[14];
} doc_face[]={
                { "Welcome to the Grid","DR_0.raw","Only the strong survive","DR_1.raw" },
                { "Impressive","DR_2.raw","But you have only begun","DR_3.raw"},
                { "You look tired","DR_4.raw","Why don't you quit and give up","DR_5.raw"},
                { "So you have brains","DR_6.raw","But do you have guts","DR_7.raw"},
                { "Let me introduce you","DR_8.raw","to my Laser Tanks","DR_25.raw"},
                
                { "Do you not realize","DR_10.raw","Your demise is near","DR_11.raw"},
                { "Let me introduce you","DR_8.raw","to my stalkers","DR_9.raw"},
                { "Beware cycle rider","DR_12.raw","You are trying my patience","DR_13.raw"},
                { "Impressive","DR_2.raw","But can you sustain this pace","DR_14.raw"},
                { "When you push too far","DR_15.raw","I push back","DR_16.raw"},
                { "We will see if you","DR_17.raw","are amuse my my fire pits","DR_18.raw"},
                { "You are good","DR_19.raw","But I will make a slave rider yet","DR_20.raw"},
                { "You look tired","DR_4.raw","I am wearing you down rider","DR_21.raw"},
                { "Yes you trouble me","DR_22.raw","But no one has beaten me yet","DR_23.raw"},
                { "Your demise is near","DR_11.raw","It is inevitable","DR_24.raw"},
                
                { "It is pointless","DR_26.raw","for you to try to continue","DR_27.raw"},
                { "Do you really think","DR_28.raw","that you have a chance","DR_29.raw"},
                { "I see destruction in your future","DR_30.raw","I will make certain of that","DR_31.raw"},
                { "Stop this battle join with me","DR_32.raw","Together we can destroy the saucers","DR_33.raw"},
                { "Defy me will you","DR_49.raw","Now I will show no mercy","DR_34.raw"},
                { "Beware cycle rider","DR_12.raw","All have failed against me","DR_35.raw"},
                { "We will see if you","DR_36.raw","really are the best","DR_37.raw"},
                { "Do you not realize","DR_10.raw","I am just toying with you","DR_38.raw"},
                { "When you push too far","DR_15.raw","I push back","DR_16.raw"},
                { "I have something new for you","DR_39.raw","Indestructable riders","DR_40.raw"},
                { "Legions of tanks await you","DR_41.raw","Why don't you quit and give up","DR_5.raw"},
                { "It is inevitable","DR_24.raw","that you will be beaten","DR_43.raw"},
                { "Impressive","DR_2.raw","You will be a fine addition to my legion","DR_44.raw"},
                { "I can play with you no longer","DR_45.raw","Prepare to Die","DR_46.raw"},
                { "Stop","DR_47.raw","I can not allow this","DR_48.raw"},

             };

struct equip_str
{
  char item[32];
} equipment[]={
                { "ACCELERATOR" },
                { "ATRAX MISSILE LAUNCHER" },
                { "LASER WALL PROJECTOR" },
                { "GRID RADAR - ALPHA UNIT" },
                { "LEVEL I LASER " },         
                
                { "TYPE 1 SHIELD GENERATOR" },
                { "DART MISSILE LAUNCHER" },
                { "LEVEL II LASER" },
                { "GRID RADAR - OMEGA UNIT" },
                { "REAR MOUNT MISSILE LAUNCHER" },
                
                { "ENERGY PHASE SHIFTER" },
                { "TYPE 2 SHIELD GENERATOR" },
                { "5 GIGAWATT LASER" },
          };

struct weap_str
{
  short int eq;
  short int qty;
  char item[36];

} weapon_list[]={
                { 1,0, "PHOTON MISSILES -> " }, 
                { 1,0, "NEUTRON MISSILES -> " }, 
                { 9,0, "LEVEL I LASER" }, 
                {14,0, "DART MISSILES -> " }, 
                {16,0, "LEVEL II LASER" }, 
                {20,0, "REAR MOUNT PHOTONS -> " }, 
                {20,0, "REAR MOUNT NEUTRONS -> " }, 
                {27,0, "5 GIGAWATT LASER" },
                };

short int eq_flag=-1, eq_spot=0, eq_noi=0, eq_gotit=0, mn1_flap=0;

char eq_image_cnt[]={ 6,5,7,5,3, 5,5,3,5,5, 5,5,3,0,0};
int factor = 192;// - 120;

char dt_mainbuf[42];
char dt_buf[5][40];                      //  0
short int dt_ctr=-1, dt_sctr=-1, curr_weapon=-1, diff_level_set=2;

unsigned int Level_Time=0;

int stick_x=0, stick_y=0, hgt=0, max_riders=5,curr_riders=0,debug_flag=0;
int system_delay=0;
unsigned char next_song[15], curr_song[15];
int num_of_objects,speed_zone=-1;
int dvar1, dvar2, dvar3;
int level_num=1, total_level_def = 0, old_level_num=-1;
int xmaze_sq=0, ymaze_sq=0, stick_move_rl=0, stick_move_ud=0;
int res_def=0, shield_level=256, power_level=1024;
int score=0,level_score=0, death_spin=0;
int master_control=0, speed_ck_flag=0;
int cheat_ctr=0, invun=0, level_jump=0;
struct setup_struc
{
    short int       screen_size;
    short int       port, intr_num, dma_num,music_addr;
    short int       mct,sct,contr;        
    short int       left,right,top,bottom,switch_buttons;
} hc_setup;

extern int Music_Address, DMA_Channel, io_addr, intr_num;
int mct=0, sct=0;

float circ = .35333333;
// D E F I N E S /////////////////////////////////////////////////////////////

#include  "h3d_mdef.h"
#define DIST_FROM_WALL_1 12
#define DIST_FROM_WALL_2 52

// G L O B A L S /////////////////////////////////////////////////////////////

void (_interrupt _far *Old_Key_Isr)(); // holds old keyboard interrupt handler

unsigned int *clockr, *vga_ram, *double_buffer_l, *equip;
unsigned char *double_buffer_c, *vga_ram_c;

int sliver_dist[321];                                                             
extern unsigned char bottoms[322], tops[322];
extern unsigned char first_bottoms[322], second_tops[322];

// 18.2 clicks/sec

unsigned int scd_table[902];

//#define VIEWPORT_CENTER  WINDOW_MIDDLE
#define VIEWER_DISTANCE  192

int viewer_height= 16; //8;*** 32rooms
int front_view_angle=0;
int view_angle=0, angle_adder=32;
int side_mode=0;
int game_mode=2;   //  1=grid  2=rooms
char left_right=0, l_r_past=0, up_down=0, gunfire=0, menu_mode=0, new_key=0, esc_chk=0;
char rings=0, rings_req=1, snd_ctr=0, music_ctr=0, rings_avail;
char hyper_boot=0, cycle_load_flag=0, rings_load_flag=0, tex_load_flag=0, carrier_load_flag=0;
char access_load_flag=0, wallpro_flag=1, wallpro_ctr=0, saucer_load_flag=0;
char missile_load_flag=0, keystat_load_flag=0;
char music_toggle=2, digi_flag=2, view_flag=0, controls=0,dead=0, ctrl_pressed=0;
unsigned char music_cnt=4, psi=0, pause=0;
unsigned char access_buf[44], rider_walls[38];
int grid_dir, grid_curspeed, grid_setspeed, radar_unit=1,low_power_flag=0;
int low_speed=4, hi_speed=32;
int adjust1=0,adjust2=0,adjust_turns=256;

int left_edge, right_edge;

int gunner_x=164, gunner_inc=4;
int prm_window_height=200, prm_window_middle=100, prm_window_bottom=199, prm_top=0;
int prm_top_copy = 0;
int prm_left  =   0;
int prm_right = 319 ;
int prm_copy1 = 200 * 320;
float p_w_s;
int prm_width_sh;
int xp, yp, xv, yv, tx= -1,ty= -1;

int demo_mode=0; demo_ctr=0, demo_command=0;

extern unsigned char red[257], green[257], blue[257];
unsigned char red2[257], green2[257], blue2[257];

unsigned char *world[WORLD_ROWS+5];       // pointer to matrix of cells that make up world of walls
unsigned char *flrmap[WORLD_ROWS+5];      // pointer to matrix of cells that make up world of floor
unsigned char *ceilmap[WORLD_ROWS+5];     // pointer to matrix of cells that make up world of ceiling

unsigned char wall_ht_map[4150];  //4098


int *atan_table;             // tangent tables used to compute initial
int *row_table;

float *ftan_table;           // tangent tables used to compute initial
float *finv_tan_table;       // intersections with ray
int *inv_tan_table;          // intersections with ray
int *y_step;                 // x and y steps, used to find intersections
int *x_step;                 // after initial one is found
int *cos_table;              // used to cancel out fishbowl effect
int *inv_cos_table;          // used to compute distances by calculating
int *inv_sin_table;          // the hypontenuse

float *finv_cos_table;       // used to compute distances by calculate
float *finv_sin_table;       // the hypontenuse



int cos_tab2[4096];
int sin_tab2[4096];


int *scale_table[MAX_SCALE+1];     // table with pre-computed scale indices


// parameter block used by assembly language sliver engine
char far *sliver_texture; // pointer to texture being rendered
int sliver_column;        // index into texture i.e. which column of texture
int sliver_top;           // starting Y position to render at
int sliver_ray;           // current ray being cast

// keyboard stuff
int raw_key;                    // the global raw keyboard data aquired from the ISR
int key_table[4] = { 0,0,0,0};  // the key state table for the motion keys


// F U N C T I O N S /////////////////////////////////////////////////////////

void save_config()
{
  FILE *fp;
  
  fp = fopen("HYPER.CFG","wb+" );
  if( fp != NULL )
  {
      fwrite( &hc_setup, sizeof( hc_setup ), 1, fp);
      fclose(fp);
  }
}

int load_config()
{
  FILE *fp1;
  
  fp1 = fopen("HYPER.CFG","rb" );

  // MIKE:
  return 1;

  if(fp1 == NULL) return(0);
  fread( &hc_setup, sizeof( hc_setup ), 1, fp1);
  fclose(fp1);

  DMA_Channel = hc_setup.dma_num;
  intr_num = hc_setup.intr_num;
  io_addr= hc_setup.port;
  Music_Address = hc_setup.music_addr;
  mct= hc_setup.mct;
  sct= hc_setup.sct;
  
  controls= hc_setup.contr;
  
  if(mct) music_toggle=2;
  else music_toggle=0;
  
  if(sct) digi_flag=2;
  else digi_flag=0;
  return(1);
}

int save_game(int wh)
{
  int z3,zz5;
  

  FILE *fp1;
   
  game_data[wh].level=level_num;
  game_data[wh].diff=diff_level_set;
  game_data[wh].score=score;
  game_data[wh].power=power_level;
  game_data[wh].shields=shield_level;
  game_data[wh].protons=weapon_list[0].qty;
  game_data[wh].neutrons=weapon_list[1].qty;
  game_data[wh].darts=weapon_list[3].qty;
  strcpy(game_data[wh].access,access_buf);
  for(z3=0;z3<30;z3++) game_data[wh].access[z3]=~game_data[wh].access[z3];
  
  _disable();
  zz5=55;
  for(z3=0;z3<10000;z3++) zz5=(zz5/1234)*32*zz5;
  fp1 = fopen("hyper.sav","wb+" );
  if(fp1 == NULL) return(0);
  for(z3=0; z3<10; z3++) fwrite( &game_data[z3], sizeof( gm_one ), 1, fp1);
  fclose(fp1);
  _enable();

  return(1);
}

int load_gamefile()
{
  int z3;

  FILE *fp1;
   
  fp1 = fopen("hyper.sav","rb" );
  if(fp1 == NULL) return(0);
  for(z3=0; z3<10; z3++) fread( &game_data[z3], sizeof( gm_one ), 1, fp1);
  fclose(fp1);

  return(1);
}


/////////////////////////////////////////////////////////////////////////////
// Joystick Functions

int gameport()   // Returns 0 if no port or else 1
{
  unsigned int a;

  a = *equip;
  if( a & 4096 ) return(1);
  return(0);
}

int stick()
{
  int a=0,b=0,d=5000; 
  unsigned char c;
  _disable();
  outp( 0x201, 255);
  while(d)
  {
    c = inp(0x201);
    if(c & 1) a++;
    if(c & 2) b++;
    if(!(c & 3)) 
    {
      stick_x = a;
      stick_y = b;
      _enable();
      return(1);
    }
    d--;
  }
  _enable();
  return(0); // means no joystick attached
}

int buttons()  // returns joystick buttons
{
 return(~((inp(0x201) >> 4) & 3));
}

void stick_funcs()
{
 int a,b=1,c=2; 
 
 if(hc_setup.switch_buttons==55) {b=2; c=1;}

 a = buttons();
 
 if(a & c) // Ctrl
 {
   if(!side_mode && !left_right) 
   {
       side_mode = 1;
       front_view_angle = view_angle;
       ctrl_pressed=1;
       new_key=127;
   }
 }
 else 
 {
   if(ctrl_pressed==1)
   {
     side_mode=0;
     ctrl_pressed=0;
   }
 }
 
 if(a & b) //Fire
 {
   if(!gunfire && curr_weapon>=0) gunfire=3;
   new_key=127;
 } 

 stick();
 if(!stick_x) return;

 if(stick_x < hc_setup.left) 
 {
   stick_move_rl=1;
   if(!side_mode)
   {
     if(grid_dir==view_angle) new_key=89;
   }
   else left_right=1;
 }
 else if(stick_x > hc_setup.right) 
 {
   stick_move_rl=1;
   if(!side_mode)
   {
     if(grid_dir==view_angle) new_key=88;
   }
   else left_right=2;
 }
 else 
 {
    if(stick_move_rl) 
    {
      if(side_mode) left_right=0;
      l_r_past=0;
      angle_adder=32;
    }
    stick_move_rl=0;
 }
 
 //Acceleration
 if(stick_y < hc_setup.top) 
 {
   up_down=1;
   stick_move_ud=1;
 }
 else if(stick_y > hc_setup.bottom) 
 {
   up_down=2;
 }
 else 
 {
    if(stick_move_ud) up_down=0;
    stick_move_ud=0;
 }
}

void Ctalk(char * tx,int b)
{
  int a;
  a = 160 - ((strlen(tx) * 8)/2);
  Display_Text(  a-1, b-1,tx,10);
  Display_Text(    a, b,tx,255);
}

void calibrate_stick()
{
 int b=1,c=2; 
 int b1,b2,c1,c2,d1,d2;

 new_key=0;
 //Stop_Melo();
 
 if(hc_setup.switch_buttons==55) {b=2; c=1;}

 if(!gameport()) return;
 else
 {
    Ctalk("JOYSTICK CALIBRATION",100);
    Ctalk("Center Joystick and Press Button 1",115);
    while(1)
    {
      stick();
      if(new_key) goto calstick;
      if(buttons()&b) 
      {
        b1 = stick_x;
        b2 = stick_y;
        break;
      }
    }
    memcpy(vga_ram,double_buffer_l,prm_copy1);

    Ctalk("Move Joystick to Upper Left",100);
    Ctalk("and Press Button 1",115);
    while(buttons()&b) _enable();
    delay(100);
    while(1)
    {
      stick();
      if(new_key) goto calstick;
      if(buttons()&b) 
      {
        c1 = stick_x;
        c2 = stick_y;
        break;
      }
    }
    memcpy(vga_ram,double_buffer_l,prm_copy1);

    Ctalk("Move Joystick to Lower Right",100);
    Ctalk("and Press Button 1",115);
    while(buttons()&b) _enable();
    delay(100);
    while(1)
    {
      stick();
      if(new_key) goto calstick;
      if(buttons()&b) 
      {
        d1 = stick_x;
        d2 = stick_y;
        break;
      }
    }
    while(buttons()&b ==0) _enable();

    hc_setup.left   = ((b1-c1)/4) +c1;
    hc_setup.top    = ((b2-c2)/4) +c2;
    hc_setup.right  = d1 -((d1-b1)/4);
    hc_setup.bottom = d2 -((d2-b2)/4);
    controls=2;
    hc_setup.contr = 2;
    save_config();    
 }
 calstick:
 //play_again();
 new_key=0;
}

/////////////////////////////////////////////////////////////////////////////

void dt_add(char * tbuf)
{
  //short int a,b;
  
  strcpy(dt_mainbuf,tbuf);
  dt_sctr=25;
  
  /*
  
  if(dt_ctr==-1)
  {
    dt_ctr=0;
    strcpy(dt_buf[dt_ctr],tbuf);
  }
  else
  {
    a=dt_ctr+1;
    for(b=0;b<5;b++)
    {
      if(a>4) a=0;
      if(!dt_buf[a][0]) {strcpy(dt_buf[a],tbuf); break;}
      else a++;
    }
  }*/
}

void dt_display()
{
  
  if(dt_sctr==-1) return;

  psi=1;
  Display_Text(  20, 170,dt_mainbuf,10);
  Display_Text(  21, 171,dt_mainbuf,251);
  psi=0;

  if(dt_sctr<1) dt_sctr=-1;
  else dt_sctr--;   
}

void weapon_display()
{
  char f1_buf[40],f2_buf[40];
  if(curr_weapon==-1) return;
                
  itoa(weapon_list[curr_weapon].qty,f2_buf,10);
  strcpy(f1_buf,weapon_list[curr_weapon].item);
  if(curr_weapon!=2 && curr_weapon!=4 && curr_weapon!=6) strcat(f1_buf,f2_buf);

  psi=1;
  Display_Text(  30, 185,f1_buf,10);
  Display_Text(  31, 185,f1_buf,251);
  psi=0;

}

void powerit(int a)
{

 power_level+=a;
 if(power_level<0) power_level=0;
 if(power_level>1024) power_level=1024;
}

void shieldit(int a)
{
 shield_level+=a;
 if(shield_level<0) shield_level=0;
 if(access_buf[24]==' ') 
 {
   if(shield_level>2048) shield_level=2048;
   return;
 }
 if(access_buf[11]==' ') if(shield_level>1024) 
 {
   shield_level=1024;
   return;
 }
 if(shield_level>256) shield_level=256;
}

int hit_shields(int a)
{

 if(invun) return(0);
 shield_level-=a;
 if(shield_level<0) 
 {
   shield_level=0;
   return(1);
 }
 return(0);
}

void cycle_status_display()
{
  int a;
  
  memset(double_buffer_c+1620, 10,32);
  a = power_level >> 5;
  if(a>0) memset(double_buffer_c+1620,251, a);
  
  if(access_buf[0]!=' ') goto c_s_d;
  a = hi_speed - low_speed;
  memset(double_buffer_c+62344,10,a);
  a = grid_curspeed - low_speed;
  memset(double_buffer_c+62340,250,4);
  if(a>0 && a<50) memset(double_buffer_c+62344,250,a);

  if(access_buf[24]==' ') 
  {
    memset(double_buffer_c+3540, 10,64);
    a = shield_level >> 5;
    if(a>0) memset(double_buffer_c+3540,253, a);
    return;
  }

  if(access_buf[11]==' ') 
  {
    memset(double_buffer_c+3540, 10,32);
    a = shield_level >> 5;
    if(a>0) memset(double_buffer_c+3540,253, a);
    return;
  }
c_s_d:  
  memset(double_buffer_c+3540, 10,8);
  a =  shield_level >> 5;
  if(a>0) memset(double_buffer_c+3540,253, a);

}

void radar_display(int va, int xv, int yv)
{
  int a,b,c,d,xp1,yp1;
  int zx,zy,zz,zoff;
  int radar_array[210];
  int liners[]={ 1,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3,3};
  int v1,v2,v3,v4,v5,v6,v7,v8,v9;
  
  switch(radar_unit)
  {
    case 1:
      v1 = 3;
      v2 = 2;
      v3 = 7;
      v4 = 5;
      v5 = 54990;
      v6 = 15;
      v7 = 57557;
      v8 = 6;
      v9 = 171;
      liners[6]=1;
      break;
    case 2:
      v1 = 5;
      v2 = 4;
      v3 = 11;
      v4 = 9;
      v5 = 51150;
      v6 = 27;
      v7 = 55643;
      v8 = 10;
      v9 = 159;
      liners[10]=1;
      break;
    case 3:
      v1 = 7;
      v2 = 6;
      v3 = 15;
      v4 = 13;
      v5 = 47310;
      v6 = 39;
      v7 = 54049;
      v8 = 14;
      v9 = 147;
      liners[14]=1;
      break;

  }    

  
  for(a=0;a<208;a++) radar_array[a]=0;

  xp1 = xv>>6;
  yp1 = yv>>6;

  switch(va)
  {
    case 3072:
     zz = xv & 63;
     if(zz>42) zoff=2;
     else if(zz<21) zoff=0;
     else zoff=1;
     c=0;
     for(a=v1;a>=-v1;a--)   
     {
       for(b=-v2;b<=v2;b++)   
       {
         zx=a+xp1;
         zy=b+yp1;

         if(zx>=0 && zy>=0 && zx<=63 && zy<=63)
         {
           d=world[zy][zx]; 
           if(d)
           {
             if(d==23) radar_array[c]=255; 
             else if(d==24) radar_array[c]=15; 
             else if( d <= 'Z') radar_array[c]=250;
             else if (d<= 'z') radar_array[c]=251;
           }
           else
           {
             d=ceilmap[zy][zx]; 
             if(d) 
             {
               if(d==155) radar_array[c]=15;
               else radar_array[c]=255;
             }
           }

         }  
         c++;
       }
     }
     break;
    case 0:
     zz = yv & 63;
     if(zz>42) zoff=2;
     else if(zz<21) zoff=0;
     else zoff=1;
     c=0;
     for(a=v1;a>=-v1;a--)   
     {
       for(b=v2;b>=-v2;b--)   
       {
         zx=b+xp1;
         zy=a+yp1;

         if(zx>=0 && zy>=0 && zx<=63 && zy<=63)
         {
           d=world[zy][zx]; 
           if(d)
           {
             if(d==23) radar_array[c]=255; 
             else if(d==24) radar_array[c]=15; 
             else if( d <= 'Z') radar_array[c]=250;
             else if( d <= 'z') radar_array[c]=251;
           }
           else
           {
             d=ceilmap[zy][zx]; 
             if(d) 
             {
               if(d==155) radar_array[c]=15;
               else radar_array[c]=255;
             }
           }

         }  
         c++;
       }
     }
     break;
    case 1024:
     zz = xv & 63;
     if(zz<21) zoff=2;
     else if(zz<42) zoff=1;
     else zoff=0;
     c=0;
     for(a=-v1;a<=v1;a++)   
     {
       for(b=v2;b>=-v2;b--)   
       {
         zx=a+xp1;
         zy=b+yp1;

         if(zx>=0 && zy>=0 && zx<=63 && zy<=63)
         {
           d=world[zy][zx]; 
           if(d)
           {
             if(d==23) radar_array[c]=255; 
             else if(d==24) radar_array[c]=15; 
             else if( d <= 'Z') radar_array[c]=250;
             else if( d <= 'z') radar_array[c]=251;
           }
           else
           {
             d=ceilmap[zy][zx]; 
             if(d) 
             {
               if(d==155) radar_array[c]=15;
               else radar_array[c]=255;
             }
           }

         }  
         c++;
       }
     }
     break;
    case 2048:
     zz = yv & 63;
     if(zz<21) zoff=2;
     else if(zz<42) zoff=1;
     else zoff=0;
     c=0;
     for(a=-v1;a<=v1;a++)   
     {
       for(b=-v2;b<=v2;b++)   
       {
         zx=b+xp1;
         zy=a+yp1;

         if(zx>=0 && zy>=0 && zx<=63 && zy<=63)
         {
           d=world[zy][zx]; 
           if(d)
           {
             if(d==23) radar_array[c]=255; 
             else if(d==24) radar_array[c]=15; 
             else if( d <= 'Z') radar_array[c]=250;
             else if( d <= 'z') radar_array[c]=251;
           }
           else
           {
             d=ceilmap[zy][zx]; 
             if(d) 
             {
               if(d==155) radar_array[c]=15;
               else radar_array[c]=255;
             }
           }

         }  
         c++;
       }
     }
     break;

  }

  switch(zoff)
  {
    case 0:
      liners[0]=0;
      liners[v8]=2;
      break;
    case 2:
      liners[0]=2;
      liners[v8]=0;
      break;
  }

  //Clear Box
  b=v5;
  for(a=v9;a<188;a++)
  {
    memset(double_buffer_c+b, 10,v6);
    b+=320;
  }
  
  b=v5;
  for(zx=0;zx<v3;zx++)    
  {
    if(liners[zx])  
    {
      a=zx*v4;
      c=0;
      for(zy=0;zy<v4;zy++)
      {
        d=radar_array[zy+a];
        if(d)
        {
          switch(liners[zx])
          {
            case 1:
              memset(double_buffer_c+b+c,d,3);
              break;
            case 2:
              memset(double_buffer_c+b+c,d,3);
              memset(double_buffer_c+b+c+320,d,3);
              break;
            case 3:
              memset(double_buffer_c+b+c,d,3);
              memset(double_buffer_c+b+c+320,d,3);
              memset(double_buffer_c+b+c+640,d,3);
              break;
          }
        }
        c+=3;

      }
    }
    b+=liners[zx]*320;

  }

  double_buffer_c[v7]=255;
}

/////////////////////////////////////////////////////////////////////////////

int newdelay(int tm1)
{

  int tm2,a;

  tm2=tm1/10;
  new_key=0;
  for(a=0;a<10;a++)
  {
    delay(tm2);
    if(new_key)
    {
      new_key=0;
      return(1);
    }
  }
  return(0);
}

void reset_game_data()
{
  int a;
  for(a=0;a<10;a++) game_data[a].games[0]=0;
}

/////////////////////////////////////////////////////////////////////////////

void clear_object_def()
{
  char * cptr;
  int a,b;
  cptr = (char * ) &object;
  b = sizeof( single );
  b = b * 150 ;
  for(a=0; a < b; a++) cptr[a]=0;
}

/////////////////////////////////////////////////////////////////////////////

void linedraw(int x1,int y1,int x2,int y2,int color,unsigned char far *screen)
{
  int y_unit,x_unit; // Variables for amount of change in x and y
  int offset, xdiff, ydiff, length,i;
  int error_term=0;                     // Initialize error term


  //window clipping
  if(x1<prm_left) x1=prm_left; 
  if(x1>prm_right) x1=prm_right; 
  if(x2<prm_left) x2=prm_left; 
  if(x2>prm_right) x2=prm_right; 
  if(y1<prm_top) y1=prm_top; 
  if(y1>prm_window_bottom) y1=prm_window_bottom; 
  if(y2<prm_top) y2=prm_top; 
  if(y2>prm_window_bottom) y2=prm_window_bottom; 

  offset=y1*320+x1;  // Calculate offset into video RAM

  ydiff=y2-y1;   // Calculate difference between
                     //  y coordinates
  if (ydiff<0) 
  {     // If the line moves in the negative direction
    ydiff=-ydiff;    // ...get absolute value of difference
    y_unit=-320;     // ...and set negative unit in y dimension
  }
  else y_unit=320;   // Else set positive unit in y dimension

  xdiff=x2-x1;       // Calculate difference between x coordinates
  if (xdiff<0) 
  {                  // If the line moves in the negative direction
    xdiff=-xdiff;    // ...get absolute value of difference
    x_unit=-1;       // ...and set negative unit in x dimension
  }
  else x_unit=1;     // Else set positive unit in y dimension

  if (xdiff>ydiff) 
  {                  // If difference is bigger in x dimension
    length=xdiff+1;   // ...prepare to count off in x direction
    for (i=0; i<length; i++) 
    {  // Loop through points in x direction
      screen[offset]=color;     // Set the next pixel in the line to COLOR
      offset+=x_unit;           // Move offset to next pixel in x direction
      error_term+=ydiff;        // Check to see if move required in y direction
      if (error_term>xdiff) 
      {                         // If so...
        error_term-=xdiff;      // ...reset error term
        offset+=y_unit;         // ...and move offset to next pixel in y dir.
      }
    }
  }
  else 
  {                             // If difference is bigger in y dimension
    length=ydiff+1;             // ...prepare to count off in y direction
    for (i=0; i<length; i++) 
    {                           // Loop through points in y direction
      screen[offset]=color;     // Set the next pixel in the line to COLOR
      offset+=y_unit;           // Move offset to next pixel in y direction
      error_term+=xdiff;        // Check to see if move required in x direction
      if (error_term>0) 
      {                         // If so...
        error_term-=ydiff;      // ...reset error term
        offset+=x_unit;         // ...and move offset to next pixel in x dir.
      }
    }
  }
}

///////////////////////////////////////////////////////////////////////////////

void Timer(int clicks)
{
	// MIKE:
	printf("Timer %d\n", clicks);
	return 0;

  // this function uses the internal time keeper timer i.e. the one that goes
  // at 18.2 clicks/sec to to a time delay.  You can find a 32 bit value of
  // this timer at 0000:046Ch

  unsigned int now, th;

  // get current time

  now = *clockr;
  now += clicks;

  // wait till time has gone past current time plus the amount we eanted to
  // wait.  Note each click is approx. 55 milliseconds.

  while(1)
  {
    _enable();
    th = *clockr;
    if( th > now) return;
  }

} // end Timer

unsigned int timerval()
{
	// MIKE:
	return SDL_GetTicks() / 55;

  // this function uses the internal time keeper timer i.e. the one that goes
  // at 18.2 clicks/sec to to a time delay.  You can find a 32 bit value of
  // this timer at 0000:046Ch

  //unsigned int now;

  //// get current time

  //now = *clockr;
  //return now;

}

/////////////////////////////////////////////////////////////////////////////

void Display_Text(int x, int y, char *txt, int color)
{
  int a=0,b,c,d,e,f,h,i,j;
  unsigned char *screen_switch, *pic;

  if(!psi) screen_switch = vga_ram_c;
  else screen_switch = double_buffer_c;
  pic = (unsigned char*) picture[145].image;

  d=0;
  while(txt[a])
  {
    b=txt[a]; 
    c=-1;
    if(b>='0' && b<='9') c=26+b-'0';
    else if(b>='A' && b<='Z') c=b-'A';
    else if(b>='a' && b<='z') c=b-'a';
    else
    {
      switch(b)
      {
        case ' ':
          d+=8;   
          break;
        case '$':
          c=36;
          break;
        case '@':
          c=37;
          break;
        case '(':
          c=38;
          break;
        case ')':
          c=39;
          break;
        case 39:
          c=40;
          break;
        case '&':
          c=41;
          break;
        case '-':
          c=42;
          break;
        case '*':
          c=43;
          break;
        case '.':
          c=44;
          break;
        case '!':
          c=45;
          break;
        case '?':
          c=46;
          break;
      }
    }
    if(c>=0)
    {
      h=y*320+x+d;
      for(f=0;f<7;f++) //ht of char
      {
        i=h;
        j= (f*350) + (c*7);   //306
        for(e=0;e<7;e++) //width of char 
        {
          if(pic[j++]) screen_switch[i]=color;
          i++;
        }
        h+=320;
      }
      d+=8;
    }
    a++;
  }
}

void Shadow_Text(int x, int y, char *txt, int color,int color2)
{
  Display_Text( x-1, y-1,  txt, color2);
  Display_Text( x, y, txt,  color);
}  

/////////////////////////////////////////////////////////////////////////////

void Display_LText(int x, int y, char *txt, int color)
{
  int a=0,b,c,d,e,f,h,i,j;
  unsigned char *screen_switch, *pic;

  if(!psi) screen_switch = vga_ram_c;
  else screen_switch = double_buffer_c;
  pic = (unsigned char*) picture[145].image;

  d=0;
  while(txt[a])
  {
    b=txt[a]; 
    c=-1;
    if(b>='0' && b<='9') c=26+b-'0';
    else if(b>='A' && b<='Z') c=b-'A';
    else if(b>='a' && b<='z') c=b-'a';
    else
    {
      switch(b)
      {
        case ' ':
          d+=8*3;   
          break;
        case '$':
          c=36;
          break;
        case '@':
          c=37;
          break;
        case '(':
          c=38;
          break;
        case ')':
          c=39;
          break;
        case 39:
          c=40;
          break;
        case '&':
          c=41;
          break;
        case '-':
          c=42;
          break;
        case '*':
          c=43;
          break;
        case '.':
          c=44;
          break;
        case '!':
          c=45;
          break;
        case '?':
          c=46;
          break;
      }
    }
    if(c>=0)
    {
      h=y*320+x+d;
      for(f=0;f<7;f++) //ht of char
      {
        i=h;
        j= (f*350) + (c*7);
        for(e=0;e<7;e++) //width of char 
        {
          if(pic[j++]) 
          {
            screen_switch[i]=color;
            screen_switch[i+1]=color;
            screen_switch[i+2]=color;
            screen_switch[i+320]=color;
            screen_switch[i+321]=color;
            screen_switch[i+322]=color;
            screen_switch[i+640]=color;
            screen_switch[i+641]=color;
            screen_switch[i+642]=color;
          }
          i+=3;
        }
        h+=320*3;
      }
      d+=8*3;
    }
    a++;
  }
}

/////////////////////////////////////////////////////////////////////////////

void Fade_Pal()
{
 int a,b;

 //Backup Palette
 for(a=0; a<256; a++)
 {
   red2[a]=red[a];
   blue2[a]=blue[a];
   green2[a]=green[a];
 }

 for(b=0; b<64; b+=2)
 {
   for(a=0;a<256;a++)
   {
     if(red[a]) red[a]--;
     if(blue[a]) blue[a]--;
     if(green[a]) green[a]--;
   }
   Set_Palette();
   delay(25);
 }
 //Restore Palette but don't show yet
 for(a=0; a<256; a++)
 {
   red[a]=red2[a];
   blue[a]=blue2[a];
   green[a]=green2[a];
 }
}

/////////////////////////////////////////////////////////////////////////////
void doctor_ender1(void);
void doctor_ender2(void);
void how_to_order(void);


void Stats()
{
 int a,b;
 char tb1[80], tb2[20];        
 unsigned int tmr5;
  
 Stop_Melo();

 //Backup Palette
 for(a=0; a<256; a++)
 {
   red2[a]=red[a];
   blue2[a]=blue[a];
   green2[a]=green[a];
 }

 for(b=0; b<10; b++)
 {
   for(a=0;a<256;a++)
   {
     if(red[a]) red[a]--;
     if(blue[a]) blue[a]--;
     if(green[a]) green[a]--;
   }
   Set_Palette();
   delay(15);
 }
 level_score+=5000;

 Shadow_Text( 8, 10,"LEVEL RESULTS",255,12);
 
 // Display Info
 strcpy(tb1,"LEVEL SCORE > ");
 itoa(level_score,tb2,10);
 strcat(tb1,tb2);
 Shadow_Text(  9,24,tb1,9,12); //Shadow
 
 strcpy(tb1,"  KEY BONUS > ");
 if(rings==rings_avail)
 {
   score+=level_score+25000;
   itoa(25000,tb2,10);
 }
 else
 {
   score+=level_score;
   itoa(0,tb2,10);
 }
 strcat(tb1,tb2);
 Shadow_Text(  9,44,tb1,9,12); //Shadow
 
 strcpy(tb1,"TOTAL SCORE > ");
 itoa(score,tb2,10);
 strcat(tb1,tb2);
 Shadow_Text(  9,64,tb1,9,12); //Shadow
 
 strcpy(tb1,"TOTAL KEYS AVAILABLE > ");
 itoa(rings_avail,tb2,10);
 strcat(tb1,tb2);
 Shadow_Text(  9,84,tb1,9,12); //Shadow
 
 strcpy(tb1," TOTAL KEYS ACQUIRED > ");
 itoa(rings,tb2,10);
 strcat(tb1,tb2);
 Shadow_Text(  9,104,tb1,9,12); //Shadow
 
 delay(500);
 Shadow_Text( 68, 180,"PRESS A KEY TO CONTINUE",255,12);
 new_key=0;
 tmr5 = timerval();
 while(!new_key) 
 {
   _enable();
   if(!CTV_voice_status) 
   {
     if(tmr5 < timerval())
     {
       play_vox("intro9.raw");
       tmr5 = timerval()+95;
     }
   }
 }
 new_key=0;

 // For Shareware version only /////////////////////
 /*
 if(level_num==5) 
 {
   doctor_ender1();
   how_to_order();
   play_again();
 } 
 */
 
 // For Registered version only /////////////////////
 
 if(level_num==30) 
 {
   doctor_ender2();
   play_again();
 } 
 

 ////////////////////////

 //Restore Palette but don't show yet
 for(a=0; a<256; a++)
 {
   red[a]=red2[a];
   blue[a]=blue2[a];
   green[a]=green2[a];
 }
}

/////////////////////////////////////////////////////////////////////////////

void Brighten_Pal()
{
 int a,b;

 //Backup Palette
 for(a=0; a<256; a++)
 {
   red2[a]=red[a];
   blue2[a]=blue[a];
   green2[a]=green[a];
 }

 for(b=0; b<64; b++)
 {
   for(a=0;a<256;a++)
   {
     if(red[a]<64) red[a]++;
     if(blue[a]<64) blue[a]++;
     if(green[a]<64) green[a]++;
   }
   Set_Palette();
   delay(15);
 }
 //Restore Palette but don't show yet
 for(a=0; a<256; a++)
 {
   red[a]=red2[a];
   blue[a]=blue2[a];
   green[a]=green2[a];
 }
}


/////////////////////////////////////////////////////////////////////////////


void PCX_Show_FS(int pic_num)
{
 int a;
 for(a=0;a<16000;a++) vga_ram[a]=picture[pic_num].image[a];
}

/////////////////////////////////////////////////////////////////////////////

void PCX_Show_Image(int x, int y, int pic_num, int scaler)
{
 int a,b;   // Note: x & y represent center of image;
 unsigned char * temp,* screen_switch, c;
 int x2,y2,x3,y3,y4;
 int calc_ht;
 int scale_step1, scale_step2, scale_sum1, scale_sum2;

 if(!psi) screen_switch = vga_ram_c;
 else screen_switch = double_buffer_c;
 
 calc_ht = ((scaler<<16) / picture[pic_num].ratio) >> 8;
 
 if(!calc_ht) calc_ht=1;
 if(!scaler) scaler=1;

 x2 = x - ( scaler >> 1);       // x2 & y2 = left & top screen start ( could be off viewport)
 y2 = y - ( calc_ht >> 1);
  
 temp=(unsigned char*) picture[pic_num].image;
 
 scale_step1 = ((picture[pic_num].width)<<8) / scaler;  //*256
 scale_step2 = ((picture[pic_num].height-1)<<8) / calc_ht+1;  //*256

 scale_sum2 = 0;

 for(a=0;a<calc_ht-1;a++)
 {
    if(y2>=0 && y2 <200)
    {  
      y3 = y2 * 320;
      y4 = (scale_sum2>>8) * (picture[pic_num].width+1);
      x3=x2;  
      scale_sum1 = 0; 
      for(b=0;b<scaler;b++)
      {
        if(x3>=0 && x3 <320 )
        {
            c = temp[y4+(scale_sum1>>8)];
            if(c) screen_switch[y3+x3] = c;
        }
        scale_sum1+=scale_step1;
        x3++;
      }
    }
    y2++;
    scale_sum2+=scale_step2;
 }
}

/////////////////////////////////////////////////////////////////////////////

void PCX_Copy_Image(int x, int y, int pic_num)
{
 int a,b;   // Note: x & y represent upper left of image;
 unsigned char * temp,* screen_switch, c;
 int x2,y2,y3,y4;

 if(!psi) screen_switch = vga_ram_c;
 else screen_switch = double_buffer_c;
 temp=(unsigned char*) picture[pic_num].image;
 
 x2 = picture[pic_num].width+1;
 y2 = picture[pic_num].height-1;
 y3 = y*320;
 for(a=0;a<y2;a++)
 {
      y4= a*x2;    
      for(b=0;b<x2;b++)
      {
        c = temp[ y4 +  b];
        if(c) screen_switch[y3+x+b] = c;
      }
      y3+=320;
 }
}

/////////////////////////////////////////////////////////////////////////////

void PCX_Paste_Image(int x, int y, int img_y, int pic_num)
{
 int a,b;   // Note: x & y represent upper left of image;
 unsigned char * temp,* screen_switch, c;
 int x2,y2,y3,y4,y5;

 screen_switch = double_buffer_c;
 temp=(unsigned char*) picture[pic_num].image;
 
 x2 = picture[pic_num].width+1;
 y2 = picture[pic_num].height;
 y3 = y * 320;
 y4 = img_y * x2;
 y5 = img_y;

 for(a=y;a<199;a++)
 {
      for(b=0;b<x2;b++)
      {
        c = temp[ y4 +  b];
        if(c) screen_switch[y3+x+b] = c;
      }
      y3+=320;
      y4+=x2;
      y5++;
      if(y5 >= y2) return;
 }
}

/////////////////////////////////////////////////////////////////////////////

void num_of_records()
{
  //int handle;
  //int fl,sz;  
  total_level_def=30;

  /* open a file & find # of records              */
  /*handle = open( "LEVEL.DEF", O_RDONLY | O_BINARY );
  if( handle != -1 )
  {
      
    fl = filelength( handle ) + 5;
    sz= sizeof(level_def) ;
    total_level_def = fl / sz;
    close( handle );
  } */

}

/////////////////////////////////////////////////////////////////////////////

void load_object_def()
{
  FILE *fp1;
  int z2, z3,xx,yy ,c;
  
  num_of_objects = 0;
  curr_riders=0;
  rings_avail=0;
 
  strcpy(rider_walls,"abcdefghijklmnopqrstuv xyz");
  
  if(!ADT_FLAG) fp1 = fopen("OBJECT.DEF","rb" );
  else 
  {
    open_adt1("OBJECT.DEF");
    fp1=GFL1_FP;
  }

  if(fp1 == NULL) return;
  
  z2=0;
  while(!z2 && !feof( fp1 ) )
  {
    fread( &object[0], sizeof( single ), 1, fp1);
    if(!feof(fp1) && object[0].level_num == level_num) 
    {
      z2++;
      if(object[0].objtype<5)
      {
        curr_riders++;
        c= object[0].map_letter-'a';   
        rider_walls[c] = ' ';
        object[0].deacty=0;
        if(diff_level_set==3) object[0].opt1=8;
        if(diff_level_set==4) object[0].opt1=8;
        if(adjust2) object[0].opt1*=2;
      }
      if(object[0].objtype==155)
      {
          if(diff_level_set==3) object[0].opt1=8;
          if(diff_level_set==4) object[0].opt1=8;
          if(adjust2) object[0].opt1*=2;
      }
      if(object[0].objtype==60) 
      {
        xx=object[0].x >>6;
        yy=object[0].y >>6;
        ceilmap[yy][xx]=24;   
        rings_avail++;
      }
    }
  }      

  if(feof(fp1) || object[0].level_num != level_num)
  {
     char * cptr;
     int a;
     cptr = (char * ) &object[0];
     for(a=0; a<sizeof(single);a++) cptr[a]=0;
     fclose(fp1);
     goto Finish_Up;
  }

  for(z3=1; z3<100; z3++)
  {
    if( feof( fp1 ) )
    {
      num_of_objects = z3;
      fclose(fp1);
      goto Finish_Up;
    }
    fread( &object[z3], sizeof( single ), 1, fp1);
    if(!feof(fp1) && object[z3].objtype>0 )
    {
      if(object[z3].level_num != level_num)
      {
        char * cptr;
        int a;
        cptr = (char * ) &object[z3];
        for(a=0; a<sizeof(single);a++) cptr[a]=0;
        num_of_objects = z3;
        fclose(fp1);
        goto Finish_Up;
      }
      else
      { //item ok
        if(object[z3].objtype<5)
        {
          curr_riders++;
          c= object[z3].map_letter-'a';   
          rider_walls[c] = ' ';
          object[z3].deacty=0;
          if(diff_level_set==3) object[z3].opt1=8;
          if(diff_level_set==4) object[z3].opt1=8;
          if(adjust2) object[z3].opt1*=2;

        }
        if(object[z3].objtype==155)
        {
          if(diff_level_set==3) object[z3].opt1=8;
          if(diff_level_set==4) object[z3].opt1=8;
          if(adjust2) object[z3].opt1*=2;
        }
        if(object[z3].objtype==60) 
        {
          xx=object[z3].x >>6;
          yy=object[z3].y >>6;
          ceilmap[yy][xx]=24;   
          rings_avail++;
        }
      }
    }
    
  }
Finish_Up:  
  // Add Saucer
  z3=num_of_objects;
  object[z3].objtype=91;
  object[z3].image_num=171;
  object[z3].xcell=21;
  object[z3].ycell=17;
  object[z3].status=1;
  object[z3].x = object[z3].xcell*64+32;
  object[z3].y = object[z3].ycell*64+32;
  object[z3].opt1=0;
  object[z3].opt2=0;
  object[z3].opt3=0;
  object[z3].opt4=0;
  num_of_objects++;
  
  // Now Add for difficulty level
  if(diff_level_set>1)
  {
     z3=num_of_objects;
     object[z3].objtype=81;
     object[z3].image_num=71;
     object[z3].xcell=47;
     object[z3].ycell=49;
     object[z3].status=1;
     object[z3].x = object[z3].xcell*64+32;
     object[z3].y = object[z3].ycell*64+32;
     num_of_objects++;
     switch(diff_level_set)
     {
       case 2:
         level_def.max_sim_riders+=2;
         break;
       case 3:
         level_def.max_sim_riders+=4;
         break;
       case 4:
         level_def.max_sim_riders+=6;
         break;
     }
  }
  if(diff_level_set==4)
  {
     z3=num_of_objects;
     object[z3].objtype=81;
     object[z3].image_num=71;
     object[z3].xcell=34;
     object[z3].ycell=57;
     object[z3].status=1;
     object[z3].x = object[z3].xcell*64+32;
     object[z3].y = object[z3].ycell*64+32;
     num_of_objects++;
  }

}

/////////////////////////////////////////////////////////////////////////////

void load_level_def()
{
  FILE *fp;
  
 // fp = fopen("LEVEL.DEF","rb" );
  
  if(!ADT_FLAG) 
  {
    fp = fopen("LEVEL.DEF","rb" );

    if( fp != NULL )
    {
      fseek(fp, (level_num-1) * sizeof(level_def), SEEK_SET );
      fread( &level_def, sizeof( level_def ), 1, fp);
      fclose(fp);
    }

  }
  else 
  {
    open_adt1("LEVEL.DEF");
    fp=GFL1_FP;
    
    if( fp != NULL )
    {
      fseek(fp, (level_num-1) * sizeof(level_def), SEEK_CUR );
      fread( &level_def, sizeof( level_def ), 1, fp);
      fclose(fp);
    }

  }
  
}

/////////////////////////////////////////////////////////////////////////////

void list_levels()
{
  int a;
  char buf[10];

  _disable();
  _dos_setvect(KEYBOARD_INT, Old_Key_Isr);
  _enable();
  set_vmode( 2 );

  for(a=0;a<total_level_def;a++)
  {
    level_num =a+1;
    load_level_def();
    printf("%i - %s ",a+1,level_def.description);
    a++;
    level_num =a+1;
    load_level_def();
    printf("\t\t\t%i - %s\n",a+1,level_def.description);
  }
 cprintf("Enter Level > ");
 buf[0]='1'; buf[1]=0;
 getns( buf, 4 );
 a = atoi( buf);
 if( a > 0  && a <= total_level_def ) level_num = a;
 else level_num=1;
 set_vmode( 0x13 );
 Set_Palette();
  
 _disable();
 Old_Key_Isr = _dos_getvect(KEYBOARD_INT);
 _dos_setvect(KEYBOARD_INT, New_Key_Int);
 _enable();

}

/////////////////////////////////////////////////////////////////////////////
extern int sdl_key;
void _interrupt _far New_Key_Int(void)
{
  // this function links into the keyboard interrupt and takes over. Called
  // when a key is pressed.  Note: how it differs from the one were saw in the
  // chapter on I/O.  It has been modified to take into consideration the demo
  // mode of the system
  int r1; 

  // replacement for inline asm
  /*_enable();
  raw_key=inp(KEY_BUFFER);
  r1=inp(KEY_CONTROL) | 0x82;
  outp(KEY_CONTROL, r1);
  r1=r1 & 0x7f;
  outp(KEY_CONTROL, r1);
  outp(INT_CONTROL, 0x20);*/
  raw_key = sdl_key;
  
  // now for some C to update the arrow state table
  // process the key and update the table (only if not in demo mode)


  if (!menu_mode)
  {
    if(demo_mode)
    {
      if(raw_key==1 || raw_key==28 || raw_key==57) demo_mode=5;
    }
    else
    {
    
     switch(raw_key)
     {
      case 2: //1
        if(access_buf[1]==' ') curr_weapon=0;
        break;
      case 3: //2
        if(access_buf[1]==' ') curr_weapon=1;
        break;
      case 4: //3
        if(access_buf[9]==' ') curr_weapon=2;
        break;
      case 5: //4
        if(access_buf[14]==' ') curr_weapon=3;
        break;
      case 6: //5
        if(access_buf[16]==' ') curr_weapon=4;
        break;
      case 7: //6
        if(access_buf[20]==' ') curr_weapon=5;
        break;
      case 8: //7
        if(access_buf[20]==' ') curr_weapon=6;
        break;
      case 9: //8
        if(access_buf[27]==' ') curr_weapon=7;
        break;
      case 14: //BS
      case 19: //r
        new_key=14; // radar selection
        break;
      case 31: //s test spin
        //new_key = 15;
        break;
      case 36: //Stick Calib J
        new_key=91;
        break;
      case 25: //P Pause
        if(!pause) pause=1;
        break;
      case 24: //O Continue
        //demo_command=99;
        break;
      case 63: //F5 Music On,Off Mute
        if(music_toggle && music_ctr)
        {
          if(!volume_flag)  Volume_OnOff (1); 
          else Volume_OnOff (0); 
          music_ctr=10; 
        }
        break;
      case 64: //F6 Sound OFF/ON
        //new_key=56;
        break;
      case 65: //F7 smaller window
        rings++;
        //new_key=57;
        break;
      case 66: //F8 larger window
        rings--;
        //new_key=58;
        break;
      case 67: //F9
        new_key=59;
        break;
      case 57:   //space Key turns wall projector on & off
        if(wallpro_ctr<1 && access_buf[3]==' ')
        {
          if(wallpro_flag == 1)        
          {
            wallpro_flag=2;
            wallpro_ctr=5;
          }
          else if(wallpro_flag == 2)        
          {
            wallpro_flag=1;
            wallpro_ctr=5;
          }
        }
        new_key=127;
        break;
      case 29:   // Ctrl Key pressed
       if(game_mode==1)
       {
        if(!left_right) side_mode = 1;
       }
       else side_mode = 1;
       front_view_angle = view_angle;
       new_key=127;
       break;
      case 56: //Alt Fire Gun
       if(!gunfire && curr_weapon>=0) gunfire=3;
       new_key=127;
       break;  
      case 157:  // Ctrl Key unpressed
       side_mode=0;
       break;
      case 46:  // Cheat
        //new_key=74;
        break;  
      case 17: // W Weapons selection
      case 28: //Enter
        new_key=68;
        break;
      case MAKE_UP:    // pressing up
        up_down=1;
        break;
      case MAKE_DOWN:  // pressing down
        up_down=2;
        break;
      case MAKE_RIGHT: // pressing right
        if(!side_mode)
        {
          new_key=88;
        }
        else left_right =2;
        break;
      case MAKE_LEFT:  // pressing left
        if(!side_mode) 
        { 
          if(grid_dir == view_angle)
          {
            new_key=89;
          }
        }
        else left_right =1;
        break;
      case BREAK_UP:    // releasing up
        if(up_down==1) up_down=0;
        break;
      case BREAK_DOWN:  // releasing down
        if(up_down==2) up_down=0;
        break;
      case BREAK_RIGHT: // releasing right
        if(game_mode!=1 && left_right==2) left_right=0;
        if(side_mode) left_right=0;
        l_r_past=0;  angle_adder=32;
        break;
      case BREAK_LEFT:  // releasing left
        if(game_mode!=1 && left_right==1) left_right=0;
        if(side_mode) left_right=0;
        l_r_past=0;  angle_adder=32;
        break;
      default:
        break;
     } // end switch
    }

  }
  else
  {
    // menu mode
    switch(raw_key)
    {
      case 1:
        new_key=27;
        break;
      case 28:   //Enter Key
        new_key=13;
        break;
      case MAKE_UP:    // pressing up
        new_key=5;
        break;
      case MAKE_DOWN:  // pressing down
        new_key=8;
        break;
      case 14:
        new_key=14;
        break;
      case 2:
      case 3:
      case 4:
      case 5:
      case 6:
      case 7:
      case 8:
      case 9:
      case 10:
        new_key='1'+raw_key-2;
        break;
      case 11:
        new_key='0';
        break;
      case 16:
        new_key='Q';
        break;
      case 17:
        new_key='W';
        break;
      case 18:
        new_key='E';
        break;
      case 19:
        new_key='R';
        break;
      case 20:
        new_key='T';
        break;
      case 21:
        new_key='Y';
        break;
      case 22:
        new_key='U';
        break;
      case 23:
        new_key='I';
        break;
      case 24:
        new_key='O';
        break;
      case 25:
        new_key='P';
        break;
      case 30:
        new_key='A';
        break;
      case 31:
        new_key='S';
        break;
      case 32:
        new_key='D';
        break;
      case 33:
        new_key='F';
        break;
      case 34:
        new_key='G';
        break;
      case 35:
        new_key='H';
        break;
      case 36:
        new_key='J';
        break;
      case 37:
        new_key='K';
        break;
      case 38:
        new_key='L';
        break;
      case 44:
        new_key='Z';
        break;
      case 45:
        new_key='X';
        break;
      case 46:
        new_key='C';
        break;
      case 47:
        new_key='V';
        break;
      case 48:
        new_key='B';
        break;
      case 49:
        new_key='N';
        break;
      case 50:
        new_key='M';
        break;

    } // end switch
  } // end if in demo mode

} // end New_Key_Int

//////////////////////////////////////////////////////////////////////////////

void Render_Sliver( int pic_num, int scale, int column, int sl_col)
{
  // this function will scale a single sliver of texture data.  it uses fixed point
  // numbers. ggg

  unsigned char *work_sprite, *bufptr;
  unsigned int work_offset, scale_step,scale_sum;
  int yy, st2, stemp, sl_start;

  if(game_mode==1) scale>>=1;     //2****
  if(scale<7) scale=7;
  if(scale>899) scale=899;
  scale_step=scd_table[scale];
  
  if( scale <= prm_window_height)
  {
    stemp = scale;
    scale_sum=0;
  }
  else
  {
    stemp = prm_window_height; //WINDOW_HEIGHT;
    yy = (scale - prm_window_height) >> 1;
    scale_sum = yy * scale_step;
  }
  st2 = prm_window_middle - (stemp >> 1);


  bottoms[column]=st2+stemp;    //bottom row of sliver;

  sl_start=0;
  if(wall_ht_map[(ymaze_sq<<6) + xmaze_sq] != 63)
  {
    sl_start = (stemp  * ( 63 - (wall_ht_map[(ymaze_sq<<6) + xmaze_sq] & 0x3f)) ) >> 6;
    st2 += sl_start;
  }

  if(st2 < tops[column] ) tops[column] = st2;           //top row of sliver

        
  if(first_bottoms[column] == 0) first_bottoms[column] = bottoms[column];
  second_tops[column] = st2;
          
  // alias a pointer to sprite for ease of access
  if(pic_num <='Z') work_sprite = (char*) picture[pic_num - 'A'].image;
  else work_sprite = (char*) picture[pic_num - 'a' + 26].image;

  // compute offset of sprite in video buffer >>  offset=st*320+column;
  bufptr=double_buffer_c+(st2 << 8) + (st2 << 6) + column;
  
  // MIKE: Verify that pic_num is valid.

  for (yy=sl_start; yy<stemp; yy++)
  {
    work_offset=(scale_sum & 0xffffffc0) + sl_col;    //work_offset is the row in 64x64 grid
    *bufptr = work_sprite[work_offset];
    scale_sum += scale_step;
    bufptr+=320;
  } 

} // end Draw_Sliver


///////////////////////////////////////////////////////////////////////////////
// For speed test
void Render_Sliver_Test( int pic_num, int scale, int column, int sl_col)
{
  // this function will scale a single sliver of texture data.  it uses fixed point
  // numbers. ggg

  unsigned char *work_sprite, *bufptr;
  unsigned int work_offset, scale_step,scale_sum;
  int yy, st2, stemp, sl_start;

  if(game_mode==1) scale>>=1;     //2****
  if(scale<7) scale=7;
  if(scale>899) scale=899;
  scale_step=scd_table[scale];
  
  if( scale <= prm_window_height)
  {
    stemp = scale;
    scale_sum=0;
  }
  else
  {
    stemp = prm_window_height; //WINDOW_HEIGHT;
    yy = (scale - prm_window_height) >> 1;
    scale_sum = yy * scale_step;
  }
  st2 = prm_window_middle - (stemp >> 1);


  bottoms[column]=st2+stemp;    //bottom row of sliver;

  sl_start=0;
  if(wall_ht_map[(ymaze_sq<<6) + xmaze_sq] != 63)
  {
    sl_start = (stemp  * ( 63 - (wall_ht_map[(ymaze_sq<<6) + xmaze_sq] & 0x3f)) ) >> 6;
    st2 += sl_start;
  }

  if(st2 < tops[column] ) tops[column] = st2;           //top row of sliver

        
  if(first_bottoms[column] == 0) first_bottoms[column] = bottoms[column];
  second_tops[column] = st2;
          
  // alias a pointer to sprite for ease of access
  if(pic_num <='Z') work_sprite = (char*) picture[pic_num - 'A'].image;
  else work_sprite = (char*) picture[pic_num - 'a' + 26].image;

  // compute offset of sprite in video buffer >>  offset=st*320+column;
  bufptr=double_buffer_c+(st2 << 8) + (st2 << 6) + column;
  
  for (yy=sl_start; yy<stemp; yy++)
  {
    work_offset=(scale_sum & 0xffffffc0) + sl_col;    //work_offset is the row in 64x64 grid
    //*bufptr = work_sprite[work_offset];
    scale_sum += scale_step;
    bufptr+=320;
  } 

} // end Draw_Sliver


///////////////////////////////////////////////////////////////////////////////

void Build_Tables(void)
{
  // this function builds all the look up tables for the system
  int ang,cb;

  for(cb=1;cb<=900;cb++) scd_table[cb]=4096/cb;       //4096
  scd_table[0]=scd_table[1];  //if scale=0 then =1

  
  atan_table = (int *) malloc(sizeof(int) * 322);
  for(cb=0; cb<=319; cb++)
  {
    ang=atan((float)(cb-160) / VIEWER_DISTANCE) * (NUMBER_OF_DEGREES/6.28);
    if(ang < 0) ang += NUMBER_OF_DEGREES;
    if(ang > NUMBER_OF_DEGREES-1) ang -= NUMBER_OF_DEGREES;
    atan_table[cb] = ang;
  }

  row_table     = (int *) malloc(sizeof(int) * 201 );
  for(cb=101; cb<200; cb++)
  {
    row_table[cb] = fixdiv(viewer_height<<SHIFT,(cb-100)<<SHIFT) * 192;
  }

  for(cb=0; cb<4096; cb++)
  {
    sin_tab2[cb] = fixmul( -256, SIN(cb) );
    cos_tab2[cb] = fixmul(  256, COS(cb) );
  }

  
} // end Build_Tables

/////////////////////////////////////////////////////////////////////////////

void Allocate_World(void)
{
  // this function allocates the memory for the world
  int index;      // allocate each row
  for (index=0; index<WORLD_ROWS; index++)
  {
    world[index] = (char *) malloc(WORLD_COLUMNS+1);
    flrmap[index] = (char *) malloc(WORLD_COLUMNS+1);
    ceilmap[index] = (unsigned char *) malloc(WORLD_COLUMNS+1);
  } // end for index
} // end Allocate_World

////////////////////////////////////////////////////////////////////////////////

int Load_World(char *file, char *wptr[64])
{
  // this function opens the input file and loads the world data from it

  FILE  *fp, *fopen();
  int row,column;
  char ch;

  // open the file
  //fp = fopen(file,"r");
  
  if(!ADT_FLAG) fp = fopen(file,"r" );
  else 
  {
    GFLTEXT=1;
    open_adt1(file);
    GFLTEXT=0;
    fp=GFL1_FP;
  }

  if(fp==NULL)
  {
      // return(0);
        set_vmode( 2 );
        printf("File not found\n");
        exit(0);
  }       

  // load in the data
  for (row=0; row<WORLD_ROWS; row++)
  {
    // load in the next row
    for (column=0; column<WORLD_COLUMNS; column++)
    {
      while((ch = getc(fp))==10){} // filter out CR

      // translate character to integer
      if (ch == ' ') ch=0;

      // insert data into world
      wptr[row][column] = ch;

    } // end for column
  } // end for row
  // close the file
  fclose(fp);
  return(1);
} // end Load_World

/////////////////////////////////////////////////////////////////////////////

void Render_Buffer(void)
{
 //register int a;
 //for(a=0;a<16000;a++) vga_ram[a]=double_buffer_l[a];
 memcpy(vga_ram,double_buffer_l,prm_copy1);
 
}

/////////////////////////////////////////////////////////////////////////////

int Find_Open_Object()
{
  int a;    
  
  for(a=0; a<150; a++)
  {
    if( !object[a].status )
    {
      object[a].opt1=0;
      return a;
    }
  }
  if(num_of_objects<150)
  {
     num_of_objects++;
     object[num_of_objects-1].opt1=0;
     return(num_of_objects-1);
  }
      
  return(-1);
}

/////////////////////////////////////////////////////////////////////////////

void Draw_Ground(void)
{
  // clear the screen and draw the ground using 16 bit instructions.
  // the sky is black and the ground is grey using the standard Dpaint pallete
  memset(double_buffer_l,level_def.skycolor,32000);
  //if(res_def==1) for(a=8000;a<16000;a++) double_buffer_l[a]=0x1e1e1e1e;

} // end Draw_Ground


/////////////////////////////////////////////////////////////////////////////

int field_notempty( char * fd)
{
  int a=0;
  while( fd[a] >0 ) if(fd[a++]>32) return(1);
  return(0);
}

//////////////////////////////////////////////////////////////////////////////

void Texture_Load()
{
  int a,b,x,y;

  if(tex_load_flag) return;
  
  if( field_notempty(level_def.tile1_assign) )
  {
    PCX_Load(level_def.pcx_tile1_fname,128,2);
    a=0;
    x=0; y=0;
    while( level_def.tile1_assign[a] >0 ) 
    {
      if(level_def.tile1_assign[a]>='A' && level_def.tile1_assign[a]<='Z') 
      {
        b=level_def.tile1_assign[a]-'A';
        Grap_Bitmap(128,b,  x,  y, 64, 64);
      }
      if(level_def.tile1_assign[a]>='a' && level_def.tile1_assign[a]<='z') 
      {
        b = level_def.tile1_assign[a] - 'a' + 26;
        Grap_Bitmap(128,b   ,  x,  y, 64, 64);
        // MIKE: picture[b+13].image = picture[b].image;
		picture[b + 13].image = malloc(picture[b].width*picture[b].height + 1);
		memcpy(picture[b + 13].image, picture[b].image, picture[b].width*picture[b].height + 1);
        picture[b+13].width = picture[b].width;
        picture[b+13].height= picture[b].height;
        picture[b+13].ratio = picture[b].ratio;
      }
      a++;
      x+=64;
      if(x>256) {x=0; y+=64;}
    }
    PCX_Unload(128);
  }
  if( field_notempty(level_def.tile2_assign) )
  {
    PCX_Load(level_def.pcx_tile2_fname,128,2);
    a=0;
    x=0; y=0;
    while( level_def.tile2_assign[a] >0 ) 
    {
      if(level_def.tile2_assign[a]>='A' && level_def.tile2_assign[a]<='Z') 
      {
        b=level_def.tile2_assign[a]-'A';
        Grap_Bitmap(128,b,  x,  y, 64, 64);
      }
      if(level_def.tile2_assign[a]>='a' && level_def.tile2_assign[a]<='z') 
      {
        b = level_def.tile2_assign[a] - 'a' + 26;
        Grap_Bitmap(128,b   ,  x,  y, 64, 64);
        picture[b+13].image = picture[b].image;
        picture[b+13].width = picture[b].width;
        picture[b+13].height= picture[b].height;
        picture[b+13].ratio = picture[b].ratio;
      }
      a++;
      x+=64;
      if(x>256) {x=0; y+=64;}
    }
    PCX_Unload(128);
  }
  if( field_notempty(level_def.tile3_assign) )
  {
    PCX_Load(level_def.pcx_tile3_fname,128,2);
    a=0;
    x=0; y=0;
    while( level_def.tile3_assign[a] >0 ) 
    {
      if(level_def.tile3_assign[a]>='A' && level_def.tile3_assign[a]<='Z') 
      {
        b=level_def.tile3_assign[a]-'A';
        Grap_Bitmap(128,b,  x,  y, 64, 64);
      }
      if(level_def.tile3_assign[a]>='a' && level_def.tile3_assign[a]<='z') 
      {
        b = level_def.tile3_assign[a] - 'a' + 26;
        Grap_Bitmap(128,b   ,  x,  y, 64, 64);
        picture[b+13].image = picture[b].image;
        picture[b+13].width = picture[b].width;
        picture[b+13].height= picture[b].height;
        picture[b+13].ratio = picture[b].ratio;
      }
      a++;
      x+=64;
      if(x>256) {x=0; y+=64;}
    }
    PCX_Unload(128);
  }
  if( field_notempty(level_def.tile4_assign) )
  {
    PCX_Load(level_def.pcx_tile4_fname,128,2);
    a=0;
    x=0; y=0;
    while( level_def.tile4_assign[a] >0 ) 
    {
      if(level_def.tile4_assign[a]>='A' && level_def.tile4_assign[a]<='Z') 
      {
        b=level_def.tile4_assign[a]-'A';
        Grap_Bitmap(128,b,  x,  y, 64, 64);
      }
      if(level_def.tile4_assign[a]>='a' && level_def.tile4_assign[a]<='z') 
      {
        b = level_def.tile4_assign[a] - 'a' + 26;
        Grap_Bitmap(128,b   ,  x,  y, 64, 64);
        picture[b+13].image = picture[b].image;
        picture[b+13].width = picture[b].width;
        picture[b+13].height= picture[b].height;
        picture[b+13].ratio = picture[b].ratio;
      }
      a++;
      x+=64;
      if(x>256) {x=0; y+=64;}
    }
    PCX_Unload(128);
  }

  tex_load_flag=1;
}  

//////////////////////////////////////////////////////////////////////////////

void gtest()
{
 int a,b,c;
 unsigned char * tptr;

 tptr= (char *) picture[2].image;

 b=0;
 for(a=0;a<64;a++)
 {
     for(c=0;c<64;c++) vga_ram_c[c+(a*320)]= tptr[b++];
 }
}

//////////////////////////////////////////////////////////////////////////////

void Render_Objects(int xview, int yview)
{
  int a,b,c,d,e,f,g, objd;
  int x2, y2, ang, va2;
  int obj_dist, height, width;
  unsigned char * spr_ptr;
  float rad1, f_ang, fa;
  int width_inc, width_ctr, half_width, width_sh, ht_ctr, ht_inc,orgwidth;
  int column_ctr;
  int pic_num; //, factor;
  
  if(!num_of_objects) return;

  factor = VIEWER_DISTANCE - 110;

  if(!side_mode) va2 = view_angle;
  else va2 = front_view_angle;
  dvar1 = va2;

  for(a=0;a<150;a++)
  {
    // Added to check that objects don't wander off the grid and crash game
    if(object[a].status>0)
    {
      b = object[a].x;
      c = object[a].y;
      if(b<0 || c<0 || b>4095 || c>4095) object[a].status=0;       
    }
    
    if(object[a].status>0)
    {
      pic_num = object[a].image_num;      
      x2 = object[a].x - xview; 
      y2 = object[a].y - yview; 
      if(!x2) x2++;
      if(!y2) y2++;

      rad1 = atan((float) y2 / x2); 
      if(x2 < 0 ) f_ang = 1.570796327 + rad1;
      else f_ang = 4.712388981 + rad1;

      rad1 =  f_ang * 651.898647;
      ang = (int) rad1;  
      dvar2=ang;  

      if( ang < 460  && va2> 3636) ang += 4096;
      if( va2 < 460  && ang> 3636) va2 += 4096;
              
      b = ( ang - va2 + 450 ) ;   // equals screen column of center of image

      circ = .3555555;
      f_ang = (float) b * circ;
      fa = (160 - f_ang) * 0.10;  // Error Correction 0.15
      
      // b equals center column
      b = (int) f_ang + fa;

      obj_dist = sqrt( (x2*x2) + (y2*y2)) ;
      if(obj_dist<1) obj_dist=1;
      objd = obj_dist;
      if( objd > 800 ) objd = 800;
      
      //if(object[a].objtype==81 || object[a].objtype==91 ) width =  VIEWER_DISTANCE * picture[pic_num].width / obj_dist;  // Wall He=64
      //else width = factor * picture[pic_num].width / objd;  // Wall He=64
      
      if(object[a].objtype==81 || (object[a].objtype>=91 && object[a].objtype<=97))
      {   
        width =  VIEWER_DISTANCE * picture[pic_num].width / obj_dist;  // Wall He=64
      }
      else width = factor * picture[pic_num].width / objd;  // Wall He=64
      half_width = width >> 1;
      
      // Check if it possible that any part of image is in view
      
      if(object[a].objtype==81 || (object[a].objtype>=91 && object[a].objtype<=97))
      {

        if( (b+half_width >= prm_left) && ( b-half_width <= prm_right) && obj_dist> 12 )
        {
          height = VIEWER_DISTANCE * picture[pic_num].height / obj_dist;  // Wall He=64
          if(height>199) height=199;
          if(height<1) height=1;

          width_sh = width << 16;
          if(width_sh<1) width_sh=1;
          ht_inc = fixdiv( picture[pic_num].height << 16, height << 16);
          width_inc = fixdiv( picture[pic_num].width << 16, width_sh ) ;
          c = 104 - (height>>1);
          //c -= (height>>2) * object[a].actx ;
          c -= (( 14400 / obj_dist) * object[a].actx) / 8 ;

          if(!CTV_voice_status && obj_dist<250) 
          {
            switch(object[a].objtype)
            {
              case 81:
                play_vox("carr_fb.raw");
                break;
              case 91:
                play_vox("saucer1.raw");
                break;
            }
          }

          spr_ptr = (char*) picture[pic_num].image;

          width_ctr = 0;
          orgwidth = picture[pic_num].width;
          obj_dist -= 64;
          for(column_ctr=b-half_width; column_ctr<b+half_width; column_ctr++)
          {
            if( column_ctr>=prm_left && column_ctr<=prm_right )
            {
              if( sliver_dist[column_ctr] >= obj_dist ||  c < tops[column_ctr] )
              {
                d = c * 320;
                ht_ctr = 0;
                g = c;
                for(e = 0; e< height-1; e++)
                {
                  if(  g >= tops[column_ctr] ) break;      
                  f = spr_ptr[((ht_ctr>>16)*orgwidth)+(width_ctr>>16)];
                  if(f && d>=0) double_buffer_c[d+column_ctr] = f;
                  d+=320;
                  ht_ctr+=ht_inc;
                  g++;
                }
              }  
            }
            width_ctr+=width_inc;
          }
        }
      }
      else
      {
        if( (b+half_width >= prm_left) && ( b-half_width <= prm_right) && obj_dist> 48  )  
        {
          height = factor * picture[pic_num].height / objd;  // Wall He=64
          if(height>199) height=199;
          if(height<1) height=2;

          width_sh = width << 16;
          if(width_sh<1) width_sh=2;
          ht_inc = fixdiv( picture[pic_num].height << 16, height << 16);
          width_inc = fixdiv( picture[pic_num].width << 16, width_sh ) ;
          c = 104 - (height>>1);
          if(object[a].objtype<5)
          {
            if(obj_dist<100)  c += 6;
          }
          
          if(!CTV_voice_status && obj_dist<250) 
          {
            switch(object[a].objtype)
            {
              case 155:
                play_vox("tank1.raw");
                break;
            }
          }
 
          spr_ptr = (char*) picture[pic_num].image;
     
          width_ctr = 0;
          orgwidth = picture[pic_num].width+1;
          if(object[a].objtype>=60) orgwidth--;
          obj_dist -= 64;
          for(column_ctr=b-half_width; column_ctr<b+half_width; column_ctr++)
          {
            if( column_ctr>=prm_left && column_ctr<=prm_right )
            {
              if( sliver_dist[column_ctr] >= obj_dist)
              {
                d = c * 320;
                ht_ctr = 0;
                for(e = 0; e< height-1; e++)
                {
                  f = spr_ptr[((ht_ctr>>16)*orgwidth)+(width_ctr>>16)];
                  if(f && d>=0) double_buffer_c[d+column_ctr] = f;
                  d+=320;
                  ht_ctr+=ht_inc;
                }
              }
            }
            width_ctr+=width_inc;
          }
        }
      }
    }
  }
  
}

void find_turn( int cb, int xc, int yc )
{

  switch( object[cb].view_angle) 
  {
    case 0: //Ok turn left or right, 1st try towards player
    case 2048:  
      if( xc > xp ) 
      {
       if( !world[yc][xc-1]) object[cb].view_angle = 1024;      
       else if( !world[yc][xc+1]) object[cb].view_angle = 3072;
      }
      else
      {
       if( !world[yc][xc+1]) object[cb].view_angle = 3072;      
       else if( !world[yc][xc-1]) object[cb].view_angle = 1024;
      }
      break;
    case 1024:
    case 3072:
      if( yc < yp ) 
      {
       if( !world[yc+1][xc]) object[cb].view_angle = 0;      
       else if( !world[yc-1][xc]) object[cb].view_angle = 2048;
      }
      else
      {
       if( !world[yc-1][xc]) object[cb].view_angle = 2048;     
       else if( !world[yc+1][xc]) object[cb].view_angle = 0;
      }
      break;

  }    

}

void select_tank_view(int cb)
{
  int xc, yc, a,b, ob_va;

  xc = object[cb].xcell;
  yc = object[cb].ycell;
  ob_va = object[cb].view_angle;
  a = abs(xc - xp);
  b = abs(yc - yp);

  switch(ob_va)
  {
    case 0:    
      if(!a ) // Same column?
      {
       if(yc < yp ) object[cb].image_num=180; //front
       else object[cb].image_num=184; //rear face
       return;
      }
      // Not same column, 
      if( !b )
      {
        if( xc > xp ) object[cb].image_num=186;  //face right
        else object[cb].image_num=182;
        return;
      }
      if( yp >= yc )  
      {
        if( xc < xp ) object[cb].image_num=181; 
        else object[cb].image_num=187;
        return;
      }
      if( xc < xp ) object[cb].image_num=183; 
      else object[cb].image_num=185;
      return;
      break;
    case 2048:    
      if(!a ) // Same column?
      {
       if(yc < yp ) object[cb].image_num=184; //front
       else object[cb].image_num=180; //rear face
       return;
      }
      // Not same column, 
      if( !b )
      {
        if( xc > xp ) object[cb].image_num=182;  //face right
        else object[cb].image_num=186;
        return;
      }
      if( yp <= yc )  
      {
        if( xc < xp ) object[cb].image_num=187; 
        else object[cb].image_num=181;
        return;
      }
      if( xc > xp ) object[cb].image_num=183; 
      else object[cb].image_num=185;
      return;
      break;
    case 1024:    
      if(!b ) // Same row?
      {
       if(xc > xp ) object[cb].image_num=180; //front
       else object[cb].image_num=184; //rear face
       return;
      }
      // Not same column, 
      if( !a )
      {
        if( yc > yp ) object[cb].image_num=186;  //face right
        else object[cb].image_num=182;
        return;
      }
      if( xp <= xc )  
      {
        if( yc < yp ) object[cb].image_num=181; 
        else object[cb].image_num=187;
        return;
      }
      if( yc < yp ) object[cb].image_num=183; 
      else object[cb].image_num=185;
      return;
      break;
    case 3072:    
      if(!b ) // Same row?
      {
       if(xc < xp ) object[cb].image_num=180; //front
       else object[cb].image_num=184; //rear face
       return;
      }
      // Not same column, 
      if( !a )
      {
        if( yc < yp ) object[cb].image_num=186;  //face right
        else object[cb].image_num=182;
        return;
      }
      if( xp >= xc )  
      {
        if( yc > yp ) object[cb].image_num=181; 
        else object[cb].image_num=187;
        return;
      }
      if( yc > yp ) object[cb].image_num=183; 
      else object[cb].image_num=185;
      return;
      break;

  }
  return;
}

void select_stalker_view(int cb)
{
  int xc, yc, scope, a,b;

  xc = object[cb].xcell;
  yc = object[cb].ycell;
  scope = 1;
  a = abs(xc - xp);
  b = abs(yc - yp);
  
  if(!object[cb].view_angle)
  {

   if(yp > yc)
   {
     if( a<=2 ) object[cb].image_num=120; //front
     else if( xc < xp )
     {
       if(b < 2) object[cb].image_num=123;
       else if(b>a) object[cb].image_num=124;
       else object[cb].image_num=126;
     }
     else 
     {  
       if(b < 2) object[cb].image_num=122;
       else if(b>a) object[cb].image_num=125;
       else object[cb].image_num=127;
     }
   }
   else //rear views
   {
     if( a<=1 ) object[cb].image_num=121; //rear
     else 
     {
       if( xc < xp ) 
       {
         if(b < 5) object[cb].image_num=123;
         else object[cb].image_num=128;
       }
       else 
       {
         if(b < 3) object[cb].image_num=122;
         else object[cb].image_num=129;
       }
     }
   }

  }
}

void select_cycle_view(int cb)
{
  int xc, yc, scope, ob_va;

  xc = object[cb].xcell;
  yc = object[cb].ycell;
  scope = 1;
  ob_va = object[cb].view_angle;

  switch(ob_va)
  {
    case 0:    
      if(xc == xp ) // Same column?
      {
       if(yc < yp ) object[cb].image_num=87; //front
       else object[cb].image_num=88; //rear face
       return;
      }
      // Not same column, 
      if( yc > yp-scope && yc < yp+scope && (grid_dir==0 || grid_dir==2048))
      {
        if( xc > xp ) object[cb].image_num=81;  //face right
        else object[cb].image_num=86;
        return;
      }
      if( yp >= yc )  
      {
        if( xc < xp ) object[cb].image_num=85; 
        else object[cb].image_num=84;
        return;
      }
      if( xc < xp ) object[cb].image_num=83; 
      else object[cb].image_num=82;
      return;
      break;
    case 2048:
      if(xc == xp ) // Same column?
      {
       if(yc < yp ) object[cb].image_num=88; 
       else object[cb].image_num=87; 
       return;
      }
      // Not same column, 
      if( yc > yp-scope && yc < yp+scope && (grid_dir==0 || grid_dir==2048))
      {
        if( xc > xp ) object[cb].image_num=86; 
        else object[cb].image_num=81;
        return;
      }
      if( yp > yc )  
      {
        if( xc < xp ) object[cb].image_num=82; 
        else object[cb].image_num=83;
        return;
      }
      if( xc < xp ) object[cb].image_num=84; 
      else object[cb].image_num=85;
      return;
      break;
    case 3072:    
      if(yc == yp ) // Same row?
      {
       if(xc < xp ) object[cb].image_num=87; 
       else object[cb].image_num=88;
       return;
      }
      // Not same row
      if( xc > xp-scope && xc < xp+scope  && (grid_dir==1024 || grid_dir==3072))
      {
        if( yc > yp ) object[cb].image_num=86; 
        else object[cb].image_num=81;
        return;
      }
      if( xc > xp )  
      {
        if( yc < yp ) object[cb].image_num=82; 
        else object[cb].image_num=83;
        return;
      }
      if( yc < yp ) object[cb].image_num=84; 
      else object[cb].image_num=85;
      return;
      break;
    case 1024:    
      if(yc == yp ) // Same row?
      {
       if(xc < xp ) object[cb].image_num=88; 
       else object[cb].image_num=87; 
       return;
      }
      // Not same row 
      if( xc > xp-scope && xc < xp+scope && (grid_dir==1024 || grid_dir==3072))
      {
        if( yc > yp ) object[cb].image_num=81; 
        else object[cb].image_num=86;
        return;
      }
      if( xc >= xp )  
      {
        if( yc < yp ) object[cb].image_num=85; 
        else object[cb].image_num=84;
        return;
      }
      if( yc < yp ) object[cb].image_num=82; 
      else object[cb].image_num=83;
      return;
      break;
  }
  object[cb].image_num = 81;
  return;
}

int random( int a,int b)
{
   int c=-1;
   while(c==-1)
   {
      c = rand() & 63;
      if( c<a || c>b ) c=-1;
   }
   return(c);     
}
 
int select_openarea( int cb)
{
   int m1,m2;

   m1=random(2,62);
   m2=random(2,62);
   if( !world[m2][m1] )
   {
      object[cb].opt3 = m1*64+32;
      object[cb].opt4 = m2*64+32;
      return(1);
   }
   return(0);
}    


void move_objects()
{
  int xc,yc,a,b,c,d,cb,radar;
  char t1_buf[40];
    
  for(cb=0; cb<150; cb++)
  {
    if(object[cb].status > 0) //Currently active?
    {
      xc = object[cb].xcell;
      yc = object[cb].ycell;
      radar = object[cb].opt2;

      switch(object[cb].objtype)
      {
        case 1:  // Enemy cycle rider (surrounder)
          if( (object[cb].x & 63) == 32  && (object[cb].y & 63) == 32 )
          {
            switch(object[cb].view_angle)
            {
              case 0:
                if( world[(yc+radar) & 63 ][xc]>0 || yc+radar>63) find_turn(cb, xc, yc);
                //else if( ceilmap[(yc+radar) & 63 ][xc]>0 ) find_turn(cb, xc, yc);
                object[cb].y += object[cb].opt1;
                break;                
              case 1024:
                if( world[yc][(xc-radar) & 63 ]>0 || xc-radar<0) find_turn(cb, xc, yc);
                //else if( ceilmap[yc][(xc-radar) & 63 ]>0 ) find_turn(cb, xc, yc);
                object[cb].x -= object[cb].opt1;
                break;                
              case 2048:
                if( world[(yc-radar) & 63 ][xc]>0 || yc-radar<0) find_turn(cb, xc, yc);
                //else if( ceilmap[(yc-radar) & 63 ][xc]>0 ) find_turn(cb, xc, yc);
                object[cb].y -= object[cb].opt1;
                break;                
              case 3072:
                if( world[yc][(xc+radar) & 63 ]>0 || xc+radar>63) find_turn(cb, xc, yc);
                //else if( ceilmap[yc][(xc+radar) & 63 ]>0 ) find_turn(cb, xc, yc);
                object[cb].x += object[cb].opt1;
                break;                
            }
          }
          else
          {
            switch(object[cb].view_angle)
            {
              case 0:
                object[cb].y += object[cb].opt1;
                break;                
              case 1024:
                object[cb].x -= object[cb].opt1;
                break;                
              case 2048:
                object[cb].y -= object[cb].opt1;
                break;                
              case 3072:
                object[cb].x += object[cb].opt1;
                break;                
            }
          }

          object[cb].xcell = object[cb].x >> 6;
          object[cb].ycell = object[cb].y >> 6;
          
          a = object[cb].xcell; 
          b = object[cb].ycell;

          if( world[b][a] ) // Hit wall  
          {
            level_score+=555;
            object[cb].status = 0; //Dying
            for(c=0,a=0;a<64;a++,c+=64)                
            {
              for(b=0;b<64;b++)
              {
                // Alter so all walls of dead enemy rider start falling
                if(world[a][b] == object[cb].map_letter) 
                 wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
              }  
            }
            c = object[cb].map_letter - 'a'; //+ 26;
            rider_walls[c] = object[cb].map_letter;
          }
          else if( xc!= object[cb].xcell || yc!= object[cb].ycell )
          {
            world[yc][xc] = object[cb].map_letter;
            wall_ht_map[ (yc<<6) + xc] = 196; // Wall moving up
          }
                  
          break;
        case 2:  // Enemy cycle rider (cutter)
          if( (object[cb].x & 63) == 32  && (object[cb].y & 63) == 32 )
          {
            switch(object[cb].view_angle)
            {
              case 0:
                if( yc >= yp || world[(yc+radar) & 63 ][xc]>0 || yc+radar>63) find_turn(cb, xc, yc);
                break;                
              case 1024:
                if( xc <= xp || world[yc][(xc-radar) & 63 ]>0 || xc-radar<0) find_turn(cb, xc, yc);
                break;                
              case 2048:
                if( yc <= yp || world[(yc-radar) & 63 ][xc]>0 || yc-radar<0) find_turn(cb, xc, yc);
                break;                
              case 3072:
                if( xc >= xp || world[yc][(xc+radar) & 63 ]>0 || xc+radar>63) find_turn(cb, xc, yc);
                break;                
            }
          }
          switch(object[cb].view_angle)
          {
              case 0:
                object[cb].y += object[cb].opt1;
                break;                
              case 1024:
                object[cb].x -= object[cb].opt1;
                break;                
              case 2048:
                object[cb].y -= object[cb].opt1;
                break;                
              case 3072:
                object[cb].x += object[cb].opt1;
                break;                
          }
          
          a = object[cb].xcell; 
          b = object[cb].ycell;

          object[cb].xcell = object[cb].x >> 6;
          object[cb].ycell = object[cb].y >> 6;
          
          if(a!=object[cb].xcell || b!=object[cb].ycell)
          {
            // Has changed cells
            ceilmap[b][a]=0;
            a = object[cb].xcell; 
            b = object[cb].ycell;
            ceilmap[b][a]=object[cb].map_letter;
          }
          else
          {
            a = object[cb].xcell; 
            b = object[cb].ycell;
          }
          
          if( a==xp && b==yp )
          {
            // Player hit a rider
            //death_spin=1;
            dead=hit_shields(100);
            if(!dead && !invun)
            {

              grid_curspeed=0;
              side_mode=0;
              view_angle= (grid_dir+2048) & 4095;
              grid_dir= view_angle;
            }
          }
          else if( world[b][a] ) // Hit wall  
          {
            level_score+=555;
            curr_riders--;
            object[cb].status = 0; //Dying
            for(c=0,a=0;a<64;a++,c+=64)                
            {
              for(b=0;b<64;b++)
              {
                // Alter so all walls of dead enemy rider start falling
                if(world[a][b] == object[cb].map_letter)  
                 wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
              }  
            }
            c = object[cb].map_letter - 'a'; // + 26;
            rider_walls[c] = object[cb].map_letter;
          }
          else if( xc!= object[cb].xcell || yc!= object[cb].ycell )
          {
            world[yc][xc] = object[cb].map_letter;
            wall_ht_map[ (yc<<6) + xc] = 196; // Wall moving up
            //ceilmap[yc][xc] = cb; // New ceil map
          }
                  
          select_cycle_view(cb);
          break;
        case 32:  // Guardian laser output
          // move laser blast along view vector foward
          object[cb].x += object[cb].xinc;
          object[cb].y += object[cb].yinc; 
          //object[cb].opt2 += object[cb].xinc;
          //object[cb].opt3 += object[cb].yinc; 
          a = object[cb].opt2>>6;
          b = object[cb].opt3>>6;
          //if(object[cb].opt1<200) 
          object[cb].opt1-=2;
          //else object[cb].status=0;
          //if(world[b][a]) object[cb].status=0;
          if( a<0 || a>63 )object[cb].status=0;
          if( b<0 || b>63 )object[cb].status=0;
          break;
        
        case 105: // Fire Pits  
          for(c=0,a=0;a<64;a++,c+=64)                
          {
            for(b=0;b<64;b++)
            {
              // Rotate gfx
              switch(flrmap[a][b]) 
              {
                case 'J':
                  flrmap[a][b]='K';
                  break;
                case 'K':
                  flrmap[a][b]='L';
                  break;
                case 'L':
                  flrmap[a][b]='J';
                  break;
              }  
            }
          } // That' all there is to it
          break;
        case 110:  // Moving Walls x movement

          if(object[cb].opt1>0) object[cb].opt1--;
          else
          {
            object[cb].opt1=8;
            a = object[cb].xcell;           
            // First remove old wall
            for(b=object[cb].yinc;b<object[cb].ydest;b++) world[b][a]=0;
          
            if(!object[cb].xinc)
            {
              if(object[cb].xcell >= object[cb].xdest) object[cb].xinc++;           
              else object[cb].xcell++;
            }
            else
            {
              if(object[cb].xcell <=object[cb].xinc) object[cb].xinc=0;           
              else object[cb].xcell--;
            }
            
            a = object[cb].xcell;           
            // Now create new position for wall 
            for(b=object[cb].yinc;b<object[cb].ydest;b++) world[b][a]=object[cb].map_letter;
          }
          break;
        case 77:  // Player photon missile output REAR MT
          if(!object[cb].opt1)
          {
            // move laser blast along view vector foward
            object[cb].x +=  (fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
            object[cb].y += -(fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
            a = object[cb].x>>6;
            b = object[cb].y>>6;
            if(world[b][a]) 
            {
              c=world[b][a];
              if( (b>0 && b<62) && (a>1 && a<62) ) 
              {
                if(c!='D' && c!='E' && c!='Y'&& c!='Z') world[b][a]=0;
                c= abs(xp-xc)+abs(yp-yc);
                play_vox("expld1.raw");
              }
              object[cb].opt1++;              
            }
            
            if( ceilmap[b][a]==155)
            {
              // missile has hit a tank
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0 && object[c].objtype==155)
                {
                  if(b==object[c].y>>6 && a==object[c].x>>6)
                  {
                    play_vox("expld1.raw");
                    object[c].opt3++;              
                    if(object[c].opt3>3)
                    {
                      level_score+=6335;
                      object[c].status=0;
                      object[cb].opt1++;
                    }
                  }
                }
              }
            }
            
            
            if( ceilmap[b][a]>='a' && ceilmap[b][a]<='z') 
            {
              // missile has hit enemy rider
              d = ceilmap[b][a];
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0) 
                {
                  if(object[c].map_letter==d)
                  {
                    ceilmap[b][a]=0;
                    play_vox("expld1.raw");
                    object[cb].opt1++;              
                    level_score+=2255;
                  
                    curr_riders--;
                    object[c].status = 0; //Dying
                    for(c=0,a=0;a<64;a++,c+=64)                
                    {
                      for(b=0;b<64;b++)
                      {
                        // Alter so all walls of dead enemy rider start falling
                        if(world[a][b] == d) 
                          wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                      }
                    }
                    c = d - 'a'; // + 26;
                    rider_walls[c] = d;
                    break;
                  }
                }
              }  
            }
          }
          else
          {
            switch(object[cb].opt1)   
            {
              case 1:
              case 2:
               object[cb].image_num=116;
               break;
              case 3:
              case 4:
              case 6:
               object[cb].image_num=117;
               break;
              case 5:
              case 7:
               object[cb].image_num=118;
               break;
              case 8:
               object[cb].image_num=119;
               break;
              case 9:
               object[cb].status=0;                
               break;
            }
            object[cb].opt1++;              
          }
          break;
        case 78:  // Player neutron missile output read mount
          if(!object[cb].opt2)
          {
            // move laser blast along view vector foward
            object[cb].x +=  (fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
            object[cb].y += -(fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
            a = object[cb].x>>6;
            b = object[cb].y>>6;
            if(world[b][a]) 
            {
              c=world[b][a];
              if( (b>0 && b<62) && (a>0 && a<62) ) 
              {
                if(c!='D' && c!='E' && c!='Y'&& c!='Z') world[b][a]=0;
                c= abs(xp-xc)+abs(yp-yc);
                play_vox("expld1.raw");
                object[cb].opt4++;
                if(object[cb].opt4>4) object[cb].status=0;
              }
              else object[cb].status=0;
              object[cb].opt1=1;              
            }
            if( ceilmap[b][a]==155)
            {
              // missile has hit a tank
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0 && object[c].objtype==155)
                {
                  if(b==object[c].y>>6 && a==object[c].x>>6)
                  {
                    play_vox("expld1.raw");
                    object[c].opt3+=2;              
                    if(object[c].opt3>3)
                    {
                      level_score+=6335;
                      object[c].status=0;
                      object[cb].opt1++;
                    }
                  }
                }
              }
            }
            
            if( ceilmap[b][a]>='a' && ceilmap[b][a]<='z') 
            {
              // missile has hit enemy rider
              d = ceilmap[b][a];
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0) 
                {
                  if(object[c].map_letter==d)
                  {
                    ceilmap[b][a]=0;
                    play_vox("expld1.raw");
                    object[cb].opt1++;              
                    level_score+=2255;
                
                    curr_riders--;
                    object[c].status = 0; //Dying
                    for(c=0,a=0;a<64;a++,c+=64)                
                    {
                      for(b=0;b<64;b++)
                      {
                        // Alter so all walls of dead enemy rider start falling
                        if(world[a][b] == d) 
                          wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                      }
                    }
                    c = d - 'a'; // + 26;
                    rider_walls[c] = d;
                    break;
                  }
                }  
              }
            }
          }
          if(object[cb].opt1)
          {
            switch(object[cb].opt1)   
            {
              case 1:
              case 2:
               object[cb].image_num=116;
               break;
              case 3:
              case 4:
               object[cb].image_num=117;
               break;
              case 5:
               object[cb].image_num=118;
               break;
              case 6:
               object[cb].image_num=114;
               if(!object[cb].opt2) object[cb].opt1=0;                
               else object[cb].status=0;
               break;
            }
            object[cb].opt1++;              
          }
          break;
        

        case 83:  // Player photon missile output
          if(!object[cb].opt1)
          {
            // move laser blast along view vector foward
            object[cb].x += -(fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
            object[cb].y +=  (fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
            a = object[cb].x>>6;
            b = object[cb].y>>6;
            if(world[b][a]) 
            {
              c=world[b][a];
              if( (b>0 && b<62) && (a>0 && a<62) ) 
              {
                if(c!='D' && c!='E' && c!='Y'&& c!='Z') world[b][a]=0;
                c= abs(xp-xc)+abs(yp-yc);
                play_vox("expld1.raw");
              }
              object[cb].opt1++;              
            }

            if( ceilmap[b][a]==155)
            {
              // missile has hit a tank
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0 && object[c].objtype==155)
                {
                  if(b==object[c].y>>6 && a==object[c].x>>6)
                  {
                    play_vox("expld1.raw");
                    object[c].opt3++;              
                    if(object[c].opt3>3)
                    {
                      level_score+=6335;
                      object[c].status=0;
                      object[cb].opt1++;
                    }
                  }
                }
              }
            }

            if( ceilmap[b][a]>='a' && ceilmap[b][a]<='z') 
            {
              // missile has hit enemy rider
              d = ceilmap[b][a];
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0) 
                {
                  if(object[c].map_letter==d)
                  {
                    ceilmap[b][a]=0;
                    play_vox("expld1.raw");
                    object[cb].opt1++;              
                    level_score+=2255;
                  
                    curr_riders--;
                    object[c].status = 0; //Dying
                    for(c=0,a=0;a<64;a++,c+=64)                
                    {
                      for(b=0;b<64;b++)
                      {
                        // Alter so all walls of dead enemy rider start falling
                        if(world[a][b] == d) 
                          wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                      }
                    }
                    c = d - 'a'; // + 26;
                    rider_walls[c] = d;
                    break;
                  }
                }
              }  
            }
          }
          else
          {
            switch(object[cb].opt1)   
            {
              case 1:
              case 2:
               object[cb].image_num=116;
               break;
              case 3:
              case 4:
              case 6:
               object[cb].image_num=117;
               break;
              case 5:
              case 7:
               object[cb].image_num=118;
               break;
              case 8:
               object[cb].image_num=119;
               break;
              case 9:
               object[cb].status=0;                
               break;
            }
            object[cb].opt1++;              
          }
          break;
        case 84:  // Player neutron missile output
          if(!object[cb].opt2)
          {
            // move laser blast along view vector foward
            object[cb].x += -(fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
            object[cb].y +=  (fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
            a = object[cb].x>>6;
            b = object[cb].y>>6;
            if(world[b][a]) 
            {
              c=world[b][a];
              if( (b>0 && b<62) && (a>0 && a<62) ) 
              {
                if(c!='D' && c!='E' && c!='Y'&& c!='Z') world[b][a]=0;
                c= abs(xp-xc)+abs(yp-yc);
                play_vox("expld1.raw");
                object[cb].opt4++;
                if(object[cb].opt4>4) object[cb].status=0;
              }
              else object[cb].status=0;
              object[cb].opt1=1;              
            }
            if( ceilmap[b][a]==155)
            {
              // missile has hit a tank
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0 && object[c].objtype==155)
                {
                  if(b==object[c].y>>6 && a==object[c].x>>6)
                  {
                    play_vox("expld1.raw");
                    object[c].opt3+=2;              
                    if(object[c].opt3>3)
                    {
                      level_score+=6335;
                      object[c].status=0;
                      object[cb].opt1++;
                    }
                  }
                }
              }
            }

            if( ceilmap[b][a]>='a' && ceilmap[b][a]<='z') 
            {
              // missile has hit enemy rider
              d = ceilmap[b][a];
              for(c=0; c<150; c++)  
              {
                if(object[c].status>0) 
                {
                  if(object[c].map_letter==d)
                  {
                    ceilmap[b][a]=0;
                    play_vox("expld1.raw");
                    object[cb].opt1++;              
                    level_score+=2255;
                
                    curr_riders--;
                    object[c].status = 0; //Dying
                    for(c=0,a=0;a<64;a++,c+=64)                
                    {
                      for(b=0;b<64;b++)
                      {
                        // Alter so all walls of dead enemy rider start falling
                        if(world[a][b] == d) 
                          wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                      }
                    }
                    c = d - 'a'; // + 26;
                    rider_walls[c] = d;
                    break;
                  }
                }  
              }
            }
          }
          if(object[cb].opt1)
          {
            switch(object[cb].opt1)   
            {
              case 1:
              case 2:
               object[cb].image_num=116;
               break;
              case 3:
              case 4:
               object[cb].image_num=117;
               break;
              case 5:
               object[cb].image_num=118;
               break;
              case 6:
               object[cb].image_num=114;
               if(!object[cb].opt2) object[cb].opt1=0;                
               else object[cb].status=0;
               break;
            }
            object[cb].opt1++;              
          }
          break;
        
        case 85:  // Player laser output
        case 152:  // Player laser output level2
        case 153:  // Player laser output 5GW
          // move laser blast along view vector foward
          object[cb].x += -(fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
          object[cb].y +=  (fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
          a = object[cb].x>>6;
          b = object[cb].y>>6;
          if( !object[cb].opt1)
          {
           if(world[b][a]) 
           {
             object[cb].opt1=1; 
             if(object[cb].objtype==153)
             {
               if( a>1 && b>1 && b<62 && b<62) world[b][a]=0;
             }
           }
           else
           {

             if( ceilmap[b][a]>='a' && ceilmap[b][a]<='z') 
             {
               // laser has hit enemy rider
               d = ceilmap[b][a];
               for(c=0; c<150; c++)  
               {
                 if(object[c].map_letter==d)
                 {
                   switch(object[cb].objtype)
                   {
                     case 85:
                       object[c].deacty++;              
                       break;
                     case 152:
                       object[c].deacty+=2;              
                       break;
                     case 153:
                       object[c].deacty=10;              
                       break;
                   }
                   object[cb].opt1=5;              
                   if(object[c].deacty>3)              
                   {
                     ceilmap[b][a]=0;
                     play_vox("expld1.raw");
                     level_score+=2255;
                     
                     curr_riders--;
                     object[c].status = 0; //Dying
                     for(c=0,a=0;a<64;a++,c+=64)                
                     {
                       for(b=0;b<64;b++)
                       {
                         // Alter so all walls of dead enemy rider start falling
                         if(world[a][b] == d) 
                          wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                       }  
                     }
                     c = d - 'a'; // + 26;
                     rider_walls[c] = d;
                   }
                   break;
                 }
               }  
             }
           }  
          }
          else
          {
            switch(object[cb].opt1)   
            {
              case 1:
              case 2:
               object[cb].image_num=116;
               break;
              case 3:
               object[cb].status=0;
               break;
              case 5:
              case 6:
               object[cb].image_num=116;
               break;
              case 7:
              case 8:
               object[cb].image_num=117;
               break;
              case 9:
               object[cb].image_num=118;
               break;
              case 10:
               object[cb].status=0;
               break;
            }
          }
          break;
        case 60:  // Rings
          object[cb].opt1++;
          if(object[cb].opt1>3)
          {
            object[cb].image_num++; 
            if(object[cb].image_num >66) object[cb].image_num = 61;
            object[cb].opt1=0;
          }
          if(xp==object[cb].x>>6 && yp==object[cb].y>>6)
          {
    
            ceilmap[yc][xc] = 0; // clear it off
            object[cb].status=0;
            rings++;
            level_score+=2510;
            if( rings==rings_req ) // Got enough rings
            {
              //if(!CTV_voice_status)
               play_vox("allkeys.raw");
              for(c=0,a=0;a<64;a++,c+=64)                
              {
                for(b=0;b<64;b++)
                {
                  // Alter so all doors start falling
                  if(world[a][b] == 'Y' || world[a][b] == 'Z') 
                    wall_ht_map[c+b] = (wall_ht_map[c+b] & 63) | 128;
                }  
              }
            }
            else
            {
               // All rings?
               if( rings==rings_avail )play_vox("keybonus.raw");
               else play_vox("bell1.raw");
            }
          }
          break;
        case 62:  // Cycle Options
          object[cb].opt1++;
          if(object[cb].opt1>3)
          {
            object[cb].image_num++; 
            if(object[cb].image_num > 100 + eq_noi) object[cb].image_num = 101;
            object[cb].opt1=0;
          }
          if(xp==object[cb].x>>6 && yp==object[cb].y>>6)
          {
            if(eq_spot==1) 
            {
              curr_weapon=0; 
              weapon_list[0].qty=15;
              weapon_list[1].qty=5;
            }
            if(eq_spot==11) shield_level=512;
            if(eq_spot==24) shield_level=1024;
            play_vox("xylo1.raw");
            level_score+=3500;
            ceilmap[yc][xc] = 0; // clear it off
            object[cb].status=0;
            access_buf[eq_spot]=' ';
            strcpy( t1_buf, equipment[eq_flag].item);
            strcat( t1_buf, " INSTALLED");
            dt_add( t1_buf);
            eq_gotit=1; //1
          }
          break;
        case 74:  // Expanding Circle Block Walls
          object[cb].opt1++;
          if(object[cb].opt1>8)
          {
            object[cb].opt1=0;  
            a = object[cb].opt2;
                                
            c= object[cb].ycell-a;
            d= object[cb].ycell+a;
            for(b=object[cb].xcell-a; b<object[cb].xcell+a; b++)
            {
              world[c][b]=0;          
              world[d][b]=0;          
            }
            c= object[cb].xcell-a;
            d= object[cb].xcell+a;
            for(b=object[cb].ycell-a; b<object[cb].ycell+a; b++)
            {
              world[b][c]=0;          
              world[b][d]=0;          
            }
                                
            if( !object[cb].opt3 ) object[cb].opt2++;
            else object[cb].opt2--;
            if(object[cb].opt2 < 1) object[cb].opt3=0;
            if(object[cb].opt2 >= object[cb].xinc) object[cb].opt3=1;

                                
            a = object[cb].opt2;
                                
            c= object[cb].ycell-a;
            d= object[cb].ycell+a;
            for(b=object[cb].xcell-a; b<object[cb].xcell+a; b++)
            {
              world[c][b]='C';          
              world[d][b]='C';          
            }
            c= object[cb].xcell-a;
            d= object[cb].xcell+a;
            for(b=object[cb].ycell-a; b<object[cb].ycell+a; b++)
            {
              world[b][c]='C';          
              world[b][d]='C';          
            }
          }
          break;
        case 81:  // Carrier
          switch( object[cb].opt1 )
          {
            case 0:  //procede to 1,1
              a=0;
              object[cb].actx=14; 
              if( object[cb].x > 96 ) { object[cb].x-=8; a++; }
              if( object[cb].y > 96 ) { object[cb].y-=8; a++; }
              if(!a)   // Already there
              {
                object[cb].opt1 += select_openarea(cb);
                object[cb].image_num=75;
              }
              break;
            case 1:  //procede to selected location
              if( curr_riders < level_def.max_sim_riders)
              {
                a=0;
                if( object[cb].x < object[cb].opt3 ) { object[cb].x+=8; a++; }
                if( object[cb].y < object[cb].opt4 ) { object[cb].y+=8; a++; }
                if(!a) //Arrived?
                {
                  a = object[cb].x >> 6;
                  b = object[cb].y >> 6;
                  if( !world[b][a] ) object[cb].opt1++;
                  else object[cb].opt1=0;  // Return to home
                }
              }
              break;
            case 2:  //Drop load
              object[cb].image_num=78;
              a=Find_Open_Object();
              if(a>=0) //Skip if over 150 objects
              {
                object[cb].image_num=78;
                object[cb].opt1++;
                
                object[a].opt1=0; 
                object[a].actx=12; 
                object[a].status=1;
                object[a].objtype=93;
                object[a].image_num=79;
                object[a].x = object[cb].x;
                object[a].y = object[cb].y;
                object[a].view_angle = random(0,3) * 1024;
              }
              break;
            case 3:  
              object[cb].image_num=74;
              object[cb].opt1++;
              break;
            case 4:  
              object[cb].image_num=71;
              object[cb].opt1=0;
              break;
          }
          break;
        case 93:  // Dropping sphere
          if(object[cb].actx) object[cb].actx--;
          else
          {
            switch( object[cb].opt1 )
            {
              case 0: //Look for opening 
                a=0; 
                b=object[cb].x>>6;
                c=object[cb].y>>6;
                object[cb].opt1++;
                while(a<4)
                {
                  switch(object[cb].view_angle)
                  {
                    case 0:
                      if(!world[c+1][b]) a=5;
                      else
                      {
                        object[cb].view_angle = (object[cb].view_angle +1024) & 4095;
                        a++;
                      }
                      break;
                    case 1024:
                      if(!world[c][b-1]) a=5;
                      else
                      {
                        object[cb].view_angle = (object[cb].view_angle +1024) & 4095;
                        a++;
                      }
                      break;
                    case 2048:
                      if(!world[c-1][b]) a=5;
                      else
                      {
                        object[cb].view_angle = (object[cb].view_angle +1024) & 4095;
                        a++;
                      }
                      break;
                    case 3072:
                      if(!world[c][b+1]) a=5;
                      else
                      {
                        object[cb].view_angle = (object[cb].view_angle +1024) & 4095;
                        a++;
                      }
                      break;
                  }
                }
                break;
              case 1:  
                //Convert to rider
                object[cb].objtype=2;
                object[cb].image_num=0;
                c = 'a';
                for(b=0; b<26;b++)
                {
                    if( rider_walls[b] >' ')
                    {
                       c = rider_walls[b];
                       rider_walls[b]=' ';
                       break;
                    }    
                }
                object[cb].map_letter=c;
                object[cb].xcell = object[cb].x>>6;
                object[cb].ycell = object[cb].y>>6;
                object[cb].opt1=4; 
                object[cb].opt2=1; 
                object[cb].opt3=1; 
                object[cb].opt4=0; 
                object[cb].status=1;
                curr_riders++;
                break;                              

            }
          }
          break;
        case 91:  // Saucer
          object[cb].image_num++;
          if(object[cb].image_num>173) object[cb].image_num=171;
          switch( object[cb].opt1 )
          {
            case 0:  //procede to 62,62
              a=0;
              object[cb].actx=14; 
              if( object[cb].x <4000 ) { object[cb].x+=16; a++; }
              if( object[cb].y <4000 ) { object[cb].y+=16; a++; }
              if(!a)   // Already there
              {
                object[cb].opt1 += select_openarea(cb);
              }
              break;
            case 1:  //procede to selected location
              a=0;
              if( object[cb].x > object[cb].opt3 ) { object[cb].x-=16; a++; }
              if( object[cb].y > object[cb].opt4 ) { object[cb].y-=16; a++; }
              if(!a) //Arrived?
              {
                a = object[cb].x >> 6;
                b = object[cb].y >> 6;
                if( !world[b][a] ) object[cb].opt1++;
                else object[cb].opt1=0;  // Return to home
              }
              break;
            case 2:  //Drop load
              a=Find_Open_Object();
              if(a>=0) //Skip if over 150 objects
              {
                object[cb].opt1++;
                object[a].opt1=0; 
                object[a].actx=12; 
                object[a].status=1;
                object[a].objtype=92;
                object[a].image_num=174;
                object[a].x = object[cb].x;
                object[a].y = object[cb].y;
                object[a].view_angle = random(0,3);
              }
              break;
            case 3:  
              object[cb].opt1=0;
              break;
          }
          break;
        case 92:  // Dropping supplies
          object[cb].image_num++;
          if(object[cb].image_num>178) object[cb].image_num=174;
          if(object[cb].actx) object[cb].actx--;
          else
          {     
                //Convert to supplies
                object[cb].objtype=99;
                object[cb].image_num=175;
                object[cb].xcell = object[cb].x>>6;
                object[cb].ycell = object[cb].y>>6;
                object[cb].status=1;
                object[cb].actx=0;
                object[cb].opt3=random(1,4);
                a = object[cb].xcell;
                b = object[cb].ycell;
                ceilmap[b][a]=23;
          }
          break;
        case 94:  // Stalker
          switch( object[cb].opt1 )
          {
            case 0:
              object[cb].actx=14; 
              object[cb].xcell=random(5,57);
              object[cb].x = object[cb].xcell * 64 + 32;
              object[cb].y = 32;
              object[cb].ycell = 0;
              object[cb].opt1++;
              object[cb].opt2=random(2,7);
              select_stalker_view(cb);
              break;
            case 1:  
              object[cb].y+=12; 
              object[cb].ycell = object[cb].y >> 6;
              select_stalker_view(cb);
              if(object[cb].ycell>=63)
              { 
                object[cb].opt1 = 0;
              }
              object[cb].opt2++;
              if(object[cb].opt2>38)
              {
                //Drop Bombs
                object[cb].opt2=0;
                a=Find_Open_Object();
                if(a>=0) //Skip if over 150 objects
                {
                  object[a].opt1=0; 
                  object[a].actx=12; 
                  object[a].status=1;
                  object[a].objtype=96;
                  object[a].image_num=111;
                  object[a].x = object[cb].x;
                  object[a].y = object[cb].y;
                }
              }
              break;
          }
          break;
        case 95: // Player dart missile output
          switch(object[cb].opt1)
          {
            case 0:
             if(object[cb].actx<8) object[cb].actx+=1;

             // move laser blast along view vector foward
             object[cb].x += -(fixmul(  64 << 16 , SIN(object[cb].view_angle))) >> 16; 
             object[cb].y +=  (fixmul(  64 << 16 , COS(object[cb].view_angle))) >> 16;
             a = object[cb].x>>6;
             b = object[cb].y>>6;
             if(a<1 || b<1 || a>62 || b>62) object[cb].status=0;               
             else
             {
               object[cb].xcell=a;
               object[cb].ycell=b;

               if( object[cb].actx>=4)
               {
                 for(c=0;c<100;c++)
                 {
                   if(object[c].status)
                   {
                     d=object[c].objtype;
                     if(d==94 || d==91 || d==81)
                     {
                       if( a==object[c].x>>6 && b==object[c].y>>6)
                       {
                         // hit target
                         play_vox("expld1.raw");
                         object[c].status=0;
                         object[cb].actx=14;
                         object[cb].opt1++;
                         break;
                       }
                     }
                   }
                 }
               }
             }
             break;
            case 1:
            case 2:
              object[cb].image_num=116;
              object[cb].opt1++;              
              break;
            case 3:
            case 4:
            case 6:
              object[cb].image_num=117;
              object[cb].opt1++;              
              break;
            case 5:
            case 7:
              object[cb].image_num=118;
              object[cb].opt1++;              
              break;
            case 8:
              object[cb].image_num=119;
              object[cb].opt1++;              
              break;
            case 9:
              object[cb].status=0;                
              break;
          }
          break;
        case 96:  // Dropping Concussion Bomb
          if(object[cb].actx>2) object[cb].actx--;
          else
          {
            object[cb].status=0;
            a=abs( xp - (object[cb].x>>6));
            b=abs( yp - (object[cb].y>>6));
            if( a<=4 && b<=4 ) 
            {
              dead=hit_shields(100);
              play_vox("expld1.raw");
            }                 
          }
          break;
        case 155:  // tanks
          radar=1;
          if( (object[cb].x & 63) == 32  && (object[cb].y & 63) == 32 )
          {
            switch(object[cb].view_angle)
            {
              case 0:
                if( object[cb].xcell == xp ) {
                  if( object[cb].ycell < yp ){
                    a=Find_Open_Object();
                    if(a>=0) //Skip if over 150 objects
                    {
                      play_vox("expld1.raw");
                      object[a].objtype=166;
                      object[a].image_num=111;
                      object[a].x = object[cb].x;
                      object[a].y = object[cb].y;
                      object[a].view_angle=0;
                      object[a].status=1;
                    }}}
                if( yc >= yp || world[(yc+radar) & 63 ][xc]>0 || yc+radar>63) find_turn(cb, xc, yc);
                break;                
              case 1024:
                if( object[cb].ycell == yp ) {
                  if( object[cb].xcell > xp ){
                    a=Find_Open_Object();
                    if(a>=0) //Skip if over 150 objects
                    {
                      play_vox("expld1.raw");
                      object[a].objtype=166;
                      object[a].image_num=111;
                      object[a].x = object[cb].x;
                      object[a].y = object[cb].y;
                      object[a].view_angle=1024;
                      object[a].status=1;
                    }}}
                if( xc <= xp || world[yc][(xc-radar) & 63 ]>0 || xc-radar<0) find_turn(cb, xc, yc);
                break;                
              case 2048:
                if( object[cb].xcell == xp ) {
                  if( object[cb].ycell > yp ){
                    a=Find_Open_Object();
                    if(a>=0) //Skip if over 150 objects
                    {
                      play_vox("expld1.raw");
                      object[a].objtype=166;
                      object[a].image_num=111;
                      object[a].x = object[cb].x;
                      object[a].y = object[cb].y;
                      object[a].view_angle=2048;
                      object[a].status=1;
                    }}}
                if( yc <= yp || world[(yc-radar) & 63 ][xc]>0 || yc-radar<0) find_turn(cb, xc, yc);
                break;                
              case 3072:
                if( object[cb].ycell == yp ) {
                  if( object[cb].xcell < xp ){
                    a=Find_Open_Object();
                    if(a>=0) //Skip if over 150 objects
                    {
                      play_vox("expld1.raw");
                      object[a].objtype=166;
                      object[a].image_num=111;
                      object[a].x = object[cb].x;
                      object[a].y = object[cb].y;
                      object[a].view_angle=3072;
                      object[a].status=1;
                    }}}
                if( xc >= xp || world[yc][(xc+radar) & 63 ]>0 || xc+radar>63) find_turn(cb, xc, yc);
                break;                
            }
          }
          a = object[cb].xcell; 
          b = object[cb].ycell;
          ceilmap[b][a]=0;
          switch(object[cb].view_angle)
          {
              case 0:
                object[cb].y += object[cb].opt1;
                break;                
              case 1024:
                object[cb].x -= object[cb].opt1;
                break;                
              case 2048:
                object[cb].y -= object[cb].opt1;
                break;                
              case 3072:
                object[cb].x += object[cb].opt1;
                break;                
          }
          
          a = object[cb].xcell; 
          b = object[cb].ycell;

          if(a<3 || b<3 || a>61 || b>61)
          {
            object[cb].view_angle = (object[cb].view_angle+2048) & 4095;
            switch(object[cb].view_angle)
            {
              case 0:
                object[cb].y += (object[cb].opt1*2);
                break;                
              case 1024:
                object[cb].x -= (object[cb].opt1*2);
                break;                
              case 2048:
                object[cb].y -= (object[cb].opt1*2);
                break;                
              case 3072:
                object[cb].x += (object[cb].opt1*2);
                break;                
            }

          }
          else
          { 
          object[cb].xcell = object[cb].x >> 6;
          object[cb].ycell = object[cb].y >> 6;
          }
          a = object[cb].xcell; 
          b = object[cb].ycell;
          ceilmap[b][a]=155;
          
          
          if( a==xp && b==yp )
          {
            // Tanks hits 
            dead=hit_shields(1000);
            if(!dead)
            {
              grid_curspeed=low_speed;
              side_mode=0;
              view_angle= (grid_dir+2048) & 4095;
              grid_dir= view_angle;
            }
          }
          else if( world[b][a] ) // Hit wall  
          {
            if(a>=3 || b>=3 || a<=61 || b<=61)
            {
              world[b][a]=0;
            }
            else 
            {
              object[cb].status = 0; 
              ceilmap[b][a]=0;
            }

          }
                  
          select_tank_view(cb);
          break;

        case 166: //tanks shots
          switch(object[cb].view_angle)
          {
            case 0:
              object[cb].y+=64;
              break;
            case 1024:
              object[cb].x-=64;
              break;
            case 2048:
              object[cb].y-=64;
              break;
            case 3072:
              object[cb].x+=64;
              break;
          }
          object[cb].xcell = object[cb].x >>6;
          object[cb].ycell = object[cb].y >>6;
          a = object[cb].xcell; 
          b = object[cb].ycell;
          if(a==xp && b==yp) 
          {
            dead=hit_shields(100);
            object[a].status=0;
          }
          else if(a<1 || b<1 || a>62 || b>62) object[a].status=0;
          else if(world[b][a]) object[a].status=0;
          break;
        case 99:  // Supplies
          object[cb].image_num++;
          if(object[cb].image_num>178) object[cb].image_num=174;
          if(xp==object[cb].x>>6 && yp==object[cb].y>>6)
          {
            powerit(500);
            shieldit(500);
            level_score+=440;
            play_vox("rise1.raw");
            object[cb].status=0;
            switch(object[cb].opt3)
            {
              case 1:
                dt_add( "10 Photon Missiles Loaded");
                weapon_list[0].qty+=10;
                break;
              case 2:
                dt_add( "5 Photon Missiles Loaded");
                weapon_list[0].qty+=5;
                break;
              case 3:
                dt_add( "5 Neutron Missile Loaded");
                weapon_list[1].qty+=5;
                break;
              case 4:          
                dt_add( "5 DART Missiles Loaded");
                weapon_list[3].qty+=5;
                break;
            }
            a = object[cb].xcell;
            b = object[cb].ycell;
            ceilmap[b][a]=0; //clear it
          }
          break;

      } //end switch
    }
  }
}

void menu1_load()
{
  PCX_Load("mi_new.pcx", 131,1);
  PCX_Load("mi_load.pcx",132,1);
  PCX_Load("mi_save.pcx",133,1);
  PCX_Load("mi_opt.pcx", 134,1);
  PCX_Load("mi_read.pcx",135,1);
  PCX_Load("mi_how.pcx", 136,1);
  PCX_Load("mi_cre.pcx", 137,1);
  PCX_Load("mi_demo.pcx",138,1);
  PCX_Load("mi_exit.pcx",139,1);
  PCX_Load("alogo1.pcx", 140,1);
}

void menu1_unload()
{
  int a;
  for(a=140; a>=131; a--) PCX_Unload(a);
}

void menu2_load()
{
  PCX_Load("smi_1.pcx", 131,1);
  PCX_Load("smi_2.pcx",132,1);
  //PCX_Load("smi_3.pcx", 133,1);
  PCX_Load("smi_4.pcx",133,1);
  PCX_Load("smi_5.pcx", 135,1);
  PCX_Load("smi_6.pcx", 136,1);
  PCX_Load("smi_7.pcx",137,1);
  //PCX_Load("smi_8.pcx",138,1);
  //PCX_Load("smi_9.pcx",139,1);
  //PCX_Load("smi_10.pcx",140,1);
  PCX_Load("smi_11.pcx",141,1);
  PCX_Load("smi_12.pcx",142,1);
  PCX_Load("smi_13.pcx",143,1);
}

void menu2_unload()
{
  int a;
  for(a=143; a>=131; a--) PCX_Unload(a);
}

void difflvl_load()
{
  PCX_Load("dfl1.pcx", 131,1);
  PCX_Load("dfl2.pcx", 132,1);
  PCX_Load("dfl3.pcx", 133,1);
  PCX_Load("dfl4.pcx", 134,1);
}

void difflvl_unload()
{
  int a;
  for(a=134; a>=131; a--) PCX_Unload(a);
}

void doctor_load()
{
  PCX_Load("docmain.pcx", 131,1);
  PCX_Load("docm1.pcx", 132,1);
  PCX_Load("docm2.pcx", 133,1);
  PCX_Load("docm6.pcx", 134,1);
  PCX_Load("doceye1.pcx", 135,1);
  PCX_Load("doceye2.pcx", 136,1);
}

void doctor_unload()
{
  int a;
  for(a=136; a>=131; a--) PCX_Unload(a);
}

void texture_unload()
{
  int a;
  for(a=0; a<52; a++) PCX_Unload(a);
  tex_load_flag=0;
}

void alnum_load()
{
  PCX_Load("alpha.pcx", 145, 0);
}

void cycle_load()
{
  if(cycle_load_flag) return;
  // Load Cycles ///////////////////////
  PCX_Load("hyper1.pcx", 81,1);
  PCX_Load("hyper2.pcx", 82,1);
  PCX_Load("hyper3.pcx", 83,1);
  PCX_Load("hyper4.pcx", 84,1);
  PCX_Load("hyper5.pcx", 85,1);
  PCX_Load("hyper6.pcx", 86,1);
  PCX_Load("hyper7.pcx", 87,1);
  PCX_Load("hyper8.pcx", 88,1);
  cycle_load_flag=1;
}

void cycle_unload()
{
  int a;
  for(a=88; a>=81; a--) PCX_Unload(a);
  cycle_load_flag=0;
}

void rings_load()
{
  if(rings_load_flag) return;
  PCX_Load("rings.pcx", 60, 0);
  Grap_Bitmap(60, 61,  0,  0, 42, 51);
  Grap_Bitmap(60, 62, 45,  0, 46, 51);
  Grap_Bitmap(60, 63, 88,  0, 52, 51);
  Grap_Bitmap(60, 64,140,  0, 55, 51);
  Grap_Bitmap(60, 65,195,  0, 54, 51);
  Grap_Bitmap(60, 66,249,  0, 45, 51);
  PCX_Unload( 60 );
  PCX_Load("lights1.pcx", 67, 0);
  PCX_Load("lights2.pcx", 68, 0);
  rings_load_flag=1;
}

void rings_unload()
{
  int a;
  for(a=68; a>=61; a--) PCX_Unload(a);
  rings_load_flag=0;
}

void grlch_load()
{
  if(access_load_flag) return;
  PCX_Load("grlch.pcx", 60, 0);
  Grap_Bitmap(60, 101,  0,  0, 73, 71);
  Grap_Bitmap(60, 102, 77,  0, 71, 71);
  Grap_Bitmap(60, 103,147,  0, 76, 71);
  Grap_Bitmap(60, 104,  0, 70, 72, 70);
  Grap_Bitmap(60, 105, 71, 70, 74, 70);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void wallpro_load()
{
  if(access_load_flag) return;
  PCX_Load("item2.pcx", 60, 0);
  Grap_Bitmap(60, 101,  2,  2, 92, 47);
  Grap_Bitmap(60, 102,  1, 72, 71, 80);
  Grap_Bitmap(60, 103, 78, 66, 47, 90);
  Grap_Bitmap(60, 104,139, 69, 58, 83);
  Grap_Bitmap(60, 105, 98,  5, 80, 62);
  Grap_Bitmap(60, 106,205, 68, 46, 91);
  Grap_Bitmap(60, 107,194,  0, 66, 66);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void accel_load()
{
  if(access_load_flag) return;
  PCX_Load("vase1.pcx", 60, 0);
  Grap_Bitmap(60, 101,  1,  1, 50, 55);
  Grap_Bitmap(60, 102, 54,  1, 50, 55);
  Grap_Bitmap(60, 103,106,  1, 52, 56);
  Grap_Bitmap(60, 104,  1, 56, 51, 54);
  Grap_Bitmap(60, 105, 52, 57, 52, 54);
  Grap_Bitmap(60, 106,108, 58, 51, 57);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void shield_load()
{
  if(access_load_flag) return;
  PCX_Load("dish1.pcx", 60, 0);
  Grap_Bitmap(60, 101,  2,  3, 50,114);
  Grap_Bitmap(60, 102, 64,  3, 71, 80);
  Grap_Bitmap(60, 103,145,  1, 35,100);
  Grap_Bitmap(60, 104,185,  0, 63, 87);
  Grap_Bitmap(60, 105,247,  1, 72, 82);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void shifter_load()
{
  if(access_load_flag) return;
  PCX_Load("shift.pcx", 60, 0);
  Grap_Bitmap(60, 101,  1,  1, 60, 56);
  Grap_Bitmap(60, 102, 70,  1, 72, 58);
  Grap_Bitmap(60, 103,146,  2, 86, 58);
  Grap_Bitmap(60, 104,  1, 59, 87, 59);
  Grap_Bitmap(60, 105, 95, 61, 69, 57);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void mslch_load()
{
  if(access_load_flag) return;
  PCX_Load("mslch.pcx", 60, 0);
  Grap_Bitmap(60, 101,  1,  1, 74, 75);
  Grap_Bitmap(60, 102, 78,  1, 84, 75);
  Grap_Bitmap(60, 103,165,  1, 82, 75);
  Grap_Bitmap(60, 104,  1, 77, 81, 76);
  Grap_Bitmap(60, 105, 84, 77, 84, 75);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void radar_load()
{
  if(access_load_flag) return;
  PCX_Load("radar.pcx", 60, 0);
  Grap_Bitmap(60, 101,116,  5, 85, 67);
  Grap_Bitmap(60, 102, 83, 77, 83, 72);
  Grap_Bitmap(60, 103,  8,  4, 49, 72);
  Grap_Bitmap(60, 104, 61,  4, 50, 72);
  Grap_Bitmap(60, 105,  0, 77, 82, 72);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void laser_load()
{
  if(access_load_flag) return;
  PCX_Load("eq5.pcx", 60, 0);
  Grap_Bitmap(60, 101,  4,  1, 58, 76);
  Grap_Bitmap(60, 102, 68,  1, 58, 76);
  Grap_Bitmap(60, 103,128,  1, 58, 76);
  PCX_Unload( 60 );
  access_load_flag=1;
}

void access_unload()
{
  int a;
  for(a=109; a>=101; a--) PCX_Unload(a);
  access_load_flag=0;
}

void missile_load()
{
  if(missile_load_flag) return;
  PCX_Load("missile.pcx", 60, 0);
  Grap_Bitmap(60, 111, 36, 69, 24, 24);  //cbomb
  Grap_Bitmap(60, 112,  1, 65, 30, 30);  //dart
  Grap_Bitmap(60, 113, 34, 34, 24, 34);  //laser                
  Grap_Bitmap(60, 114,  0, 31, 30, 31);  //neutron more powerful
  Grap_Bitmap(60, 115,  0,  0, 30, 31);  //photon
  Grap_Bitmap(60, 116, 36,  1, 31, 32);
  Grap_Bitmap(60, 117, 75,  0, 56, 49);
  Grap_Bitmap(60, 118,145,  0, 78, 49);
  Grap_Bitmap(60, 119,234,  0, 82, 49);
  Grap_Bitmap(60, 179, 65, 60, 34, 39);  //5GW Laser
  PCX_Unload( 60 );
  
  missile_load_flag=1;
}


void keystat_load()
{
  if(keystat_load_flag) return;
  PCX_Load("keystat.pcx", 60, 0);
  Grap_Bitmap(60, 160,  0,  0, 14, 14);
  Grap_Bitmap(60, 161, 15,  0, 14, 14);
  Grap_Bitmap(60, 162, 29,  0, 14, 14);
  Grap_Bitmap(60, 163, 43,  0, 14, 14);
  Grap_Bitmap(60, 164, 57,  0, 14, 14);
  Grap_Bitmap(60, 165, 71,  0, 14, 14);
  Grap_Bitmap(60, 166, 85,  0, 14, 14);
  Grap_Bitmap(60, 167, 99,  0, 14, 14);
  Grap_Bitmap(60, 168,113,  0, 14, 14);
  Grap_Bitmap(60, 169,127,  0, 14, 14);
  Grap_Bitmap(60, 170,141,  0, 14, 14);
  PCX_Unload( 60 );
  keystat_load_flag=1;
}

void saucer_load()
{
  if(saucer_load_flag) return;
  PCX_Load("saucer1.pcx", 60, 0);
  Grap_Bitmap(60, 171,  1,  6,140, 41);
  Grap_Bitmap(60, 172,141,  4,138, 47);
  Grap_Bitmap(60, 173,  2, 56,142, 45);
  Grap_Bitmap(60, 174,  4,101, 50, 53);
  Grap_Bitmap(60, 175, 56,101, 51, 53);
  Grap_Bitmap(60, 176,109,101, 49, 53);
  Grap_Bitmap(60, 177,162,101, 49, 53);
  Grap_Bitmap(60, 178,213,101, 50, 53);
  PCX_Unload( 60 );
  saucer_load_flag=1;
}
    
void cycle_options()
{
  int a,b;
    
  b=-1;
  for(a=0; a<level_num; a++)  //level_num 30
  {
    if(access_buf[a]!=' ') 
    {
      b = access_buf[a]-'A';
      break;
    }
  }
  if(b==-1)
  {
    for(a=0; a<100; a++)
    {
      if( object[a].status )
      {
        if( object[a].objtype==62 ) 
        {
          object[a].status=0;
          return;
        }
      }
    }
  }
  if(access_load_flag) access_unload();

  switch(b)
  {
    case 0:
      accel_load();
      break;
    case 2:
      wallpro_load();
      break;
    case 1:
    case 6:
      mslch_load();
      break;
    case 3:
    case 8:
      radar_load();
      break;
    case 4:
    case 7:
    case 12:
      laser_load();
      break;
    case 5:
    case 11:
      shield_load();
      break;
    case 9:
      grlch_load();
      break;
    case 10:
      shifter_load();
      break;
  }
}
    
    

void carriers_load()
{
  if(carrier_load_flag) return;
  PCX_Load("carriers.pcx", 60, 0);
  Grap_Bitmap(60, 71,  2,  0, 75, 75);
  Grap_Bitmap(60, 72, 83,  0, 75, 75);
  Grap_Bitmap(60, 73,163,  0, 75, 75);
  Grap_Bitmap(60, 74,245,  0, 75, 75);
  Grap_Bitmap(60, 75,  2, 75, 75,112);
  Grap_Bitmap(60, 76, 83, 75, 75,112);
  Grap_Bitmap(60, 77,163, 75, 75,112);
  Grap_Bitmap(60, 78,245, 75, 75, 75);
  Grap_Bitmap(60, 79,254,151, 52, 48);
  PCX_Unload( 60 );
  carrier_load_flag=1;
}

void carriers_unload()
{
  int a;
  for(a=79; a>=71; a--) PCX_Unload(a);
  carrier_load_flag=0;
}

void stalkers_load()
{
  PCX_Load("stalkers.pcx", 60, 0);
  Grap_Bitmap(60, 120, 19, 62,135, 34); //Front
  Grap_Bitmap(60, 121, 14,  1,143, 62); //Rear
  Grap_Bitmap(60, 122,  1,139,165, 61); //Right
  Grap_Bitmap(60, 123,152,190,168, 59); //Left
  Grap_Bitmap(60, 124,  1, 96,129, 47); //Fwd-Left
  Grap_Bitmap(60, 125,  1,203,127, 40); //Fwd-Right
  Grap_Bitmap(60, 126,160, 86,147, 50); //Left-Fwd
  Grap_Bitmap(60, 127,166,138,154, 50); //Right-Fwd
  Grap_Bitmap(60, 128,177,  1,143, 43); //Left-Rear
  Grap_Bitmap(60, 129,177, 42,143, 45); //Right-Rear
  PCX_Unload( 60 );
  
}

void stalker_unload()
{
  int a;
  for(a=129; a>=120; a--) PCX_Unload(a);
}

void tanks_load()
{
  PCX_Load("tanks1.pcx", 60, 0);
  Grap_Bitmap(60, 180,  3,  0,102, 59); //Front
  Grap_Bitmap(60, 184,110,  0,104, 59); //Rear
  Grap_Bitmap(60, 186,152,129,157, 60); //Right
  Grap_Bitmap(60, 182,151, 68,161, 59); //Left
  Grap_Bitmap(60, 181,  1,137,146, 62); //Fwd- Left
  Grap_Bitmap(60, 183,  1, 68,153, 63); //Rear- right
  PCX_Unload( 60 );
  
  PCX_Load("tanks2.pcx", 60, 0);
  Grap_Bitmap(60, 187, 17,  1,145, 62); //Fwd-Right
  Grap_Bitmap(60, 185,166,  2,152, 62); //rear - left
  PCX_Unload( 60 );
}

void tank_unload()
{
  int a;
  for(a=187; a>=180; a--) PCX_Unload(a);
}


void Display( int x, int y, int pic_num)
{
  int x1,x2,x3,x4, y1,y2,y3, y4, ht, width;
  unsigned char * spr_ptr, a;
  if(y > prm_window_bottom) return;
  width = picture[pic_num].width+1;
  ht = picture[pic_num].height-2; //-3;
  if(pic_num>=160) 
  {
   width--;
   ht = picture[pic_num].height-1;
  }
  x1 = x - (width >>1);
  y2 = y + ht;  if( y2 > prm_window_bottom) y2 = prm_window_bottom;
  spr_ptr = (unsigned char *) picture[pic_num].image;

  y3 = y * 320;
  y4 = 0;
  x4 = x1+width;
  for(y1=y ; y1<=y2;y1++)
  {
    x3=0;   
    for(x2=x1; x2<x4;x2++)
    {
       a = spr_ptr[y4+x3];
       if(a) double_buffer_c[y3+x2] = a;
       x3++;
    }
    y3+=320;
    y4+=width;
  }     

}

void Display2( int x, int y, int pic_num)
{
  int x1,x2,x3,x4, y1,y2,y3, y4, ht, width;
  unsigned char * spr_ptr, a;
  if(y > prm_window_bottom) return;
  width = picture[pic_num].width+1;
  ht = picture[pic_num].height -3;
  if(pic_num>=160) 
  {
   width--;
   ht = picture[pic_num].height-1;
  }
  x1 = x - (width >>1);
  y2 = y + ht;  if( y2 > prm_window_bottom) y2 = prm_window_bottom;
  spr_ptr = (unsigned char *) picture[pic_num].image;

  y3 = y * 320;
  y4 = 0;
  x4 = x1+width;
  for(y1=y ; y1<=y2;y1++)
  {
    x3=0;   
    for(x2=x1; x2<x4;x2++)
    {
       a = spr_ptr[y4+x3];
       if(a) vga_ram_c[y3+x2] = a;
       x3++;
    }
    y3+=320;
    y4+=width;
  }     

}

void Draw_Weapon()
{
  
  int gunner_y=150,a;
  
  if(gunfire<2)
  {
    a = abs(gunner_x-164);
    if(a<8) gunner_y++;
    if(a<4) gunner_y++;
    Display( gunner_x,gunner_y,91);
  }
  else
  {
    gunner_x=164; 
    Display( gunner_x-6,135,93);
    Display( gunner_x,145,92);
    gunfire--;
  }
}


void Move_Weapon()
{
  if(gunner_x<147 || gunner_x>173) gunner_inc = -gunner_inc;
  gunner_x += gunner_inc;

}

void cont_music()
{
       _enable();
       if(!musRunning && music_toggle==2) 
       { 
        if( music_cnt==4) { play_again(); music_cnt--; }
       }
       if(music_toggle==2 && music_cnt<4)
       {
         music_cnt--;
         if(!music_cnt)
         {  
           if(next_song[0])
           {
             play_song( next_song );
             strcpy(curr_song,next_song);
             next_song[0]=0;
           }
           else play_again();
           music_cnt=4;
         }  
       }
}

void how_to_order()
{
  
 int a;

  //Backup Palette
  for(a=0; a<256; a++)
  {
    red2[a]=red[a];
    blue2[a]=blue[a];
    green2[a]=green[a];
  }
  
  PCX_Load("inet.pcx", 148,1);
  PCX_Load("hcl1.pcx", 146,1);
  PCX_Load("cards.pcx", 147,1);
  Set_Palette();
  memset(vga_ram,0,64000);
  
  memcpy(vga_ram, picture[146].image,63360);
  
  /*Shadow_Text(  5, 5,"  AFTER PLAYING 5 LEVELS OF THIS",252,9);
  Shadow_Text(  5,15,"EXCITING  SHAREWARE  VERSION  OF",252,9);
  Shadow_Text(  5,25,"HYPERCYCLES.  YOU MUST  CONTINUE",252,9);
  Shadow_Text(  5,35,"YOU KNOW YOU  MUST!  DO NOT MISS",252,9);
  Shadow_Text(  5,45,"THE  30 ACTION PACKED LEVELS  AS",252,9);
  Shadow_Text(  5,55,"THE EVIL DOCTOR THROWS IT AT YOU",252,9);
  Shadow_Text(  5,65,"AT DIZZYING SPEEDS. OF COURSE HE",252,19);
  Shadow_Text(  5,75,"WILL TAUNT YOU ALL WAY. YOU MUST",252,9);
  Shadow_Text(  5,85,"DESTROY  THIS DIABOLICAL WIZARD.",252,9);
  Shadow_Text(  5,95,"BUT YOU CAN'T WIN IF YOU DON'T PLAY!",252,9);
  */

  Shadow_Text(  5, 5,"  THIS IS THE REGISTERED VERSION",252,9);
  Shadow_Text(  5,15,"OF HYPERCYCLES.  IF YOU ARE  NOT",252,9);
  Shadow_Text(  5,25,"REGISTERED USER  AND ARE PLAYING",252,9);
  Shadow_Text(  5,35,"THIS ON  YOUR  COMPUTER  THEN WE",252,9);
  Shadow_Text(  5,45,"HAVE A BIG PROBLEM. JUST PICK UP",252,9);
  Shadow_Text(  5,55,"THE PHONE  & ORDER YOUR OWN COPY",252,9);
  Shadow_Text(  5,65,"TODAY.   HONESTY  IS  ALWAYS THE",252,19);
  Shadow_Text(  5,75,"POLICY AND IS ALSO  THE ONLY WAY",252,9);
  Shadow_Text(  5,85,"WE  CAN  BRING  YOU  GREAT GAMES",252,9);
  Shadow_Text(  5,95,"TO PLAY. THANKS....",252,9);

  Shadow_Text( 5,121,"ORDER NOW AND GET ALL 30 LEVELS!",10,9);
  Shadow_Text( 5,131,"ONLY $29 (PLUS $5 S&H)  (SEE ORDER.TXT)",10,9);
  Shadow_Text( 5,141,"CALL (812) 875-2852 (VOICE)",10,9);
  Shadow_Text( 5,151,"OR FAX ORDER (812) 875-2836 (FAX LINE)",10,9);
  
  Shadow_Text( 231,171,"PAGE 1 OF 4", 10,9);
  Shadow_Text( 231,181,"  - MORE -", 10,9);
  
  Display2( 100,165, 147);
  delay(550);
  new_key=0;
  while(!new_key) cont_music();
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"ORDER NOW AND GET ALL 30 LEVELS!",10,9);
  Shadow_Text(  5,15,"ONLY $29 (PLUS $5 S&H)  (SEE ORDER.TXT)",10,9);
  Shadow_Text(  5,25,"CALL (812) 875-2852 (VOICE)",10,9);
  Shadow_Text(  5,35,"OR FAX ORDER (812) 875-2836 (FAX LINE)",10,9);

  Shadow_Text(  5,51,"* FIRE SURFACE TO AIR DART MISSILES", 14,9);
  Shadow_Text(  5,61,"* FIGHT THE OMINOUS LASER TANKS", 14,9);
  Shadow_Text(  5,71,"* STALKERS & CARRIERS FROM ABOVE", 14,9);
  Shadow_Text(  5,81,"* LASER GUNS TO BLOW AWAY RIDERS", 14,9);
  Shadow_Text(  5,91,"* ESCAPE FROM DEADLY FIRE PITS", 14,9);
  Shadow_Text( 5,101,"* NEW MISSILES FOR MAXIMUM FIREPOWER",14,9);
  Shadow_Text( 5,111,"* NEW LONG RANGE GRID RADAR SCOPES", 14,9);
  Shadow_Text( 5,121,"* NEW REGEN SHIELD GENERATORS", 14,9);
  Shadow_Text( 5,131,"* PHASE SHIFT RIGHT THRU WALLS", 14,9);
  Shadow_Text( 5,141,"* ULTIMATE FACE-OFF WITH DR DEVARO",14,9);
  Shadow_Text( 5,151,"* GET CHEAT GOODIES IF YOU NEED THEM", 14,9);
 
  Shadow_Text( 231,171,"PAGE 2 OF 4", 10,9);
  Shadow_Text( 231,181,"  - MORE -", 10,9);
  
  Display2( 100,165, 147);
  delay(550);
  new_key=0;
  while(!new_key) cont_music();
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"ORDER NOW AND GET ALL 30 LEVELS!",10,9);
  Shadow_Text(  5,15,"ONLY $29 (PLUS $5 S&H)  (SEE ORDER.TXT)",10,9);
  Shadow_Text(  5,25,"CALL (812) 875-2852 (VOICE)",10,9);
  Shadow_Text(  5,35,"OR FAX ORDER (812) 875-2836 (FAX LINE)",10,9);
  
  Shadow_Text(  5,51,"YOU CAN WRITE TO:", 255,9);
  Shadow_Text(  5,64,"ACLYPSE CORPORATION",251,9);
  Shadow_Text(  5,74,"RT. 2 BOX 213H",251,9);
  Shadow_Text(  5,84,"WORTHINGTON, IN 47471-9764  USA",251,9);
  
  Shadow_Text( 5,104,"ACLYPSE E-MAIL ADDRESS:", 255,9);
  //Shadow_Text( 5,115,"  ACLYPSE.IQUEST.COM",251,9);
  Display2( 100,115, 148);

  
  Shadow_Text( 231,171,"PAGE 3 OF 4", 10,9);
  Shadow_Text( 231,181,"  - MORE -", 10,9);
  
  Display2( 100,165, 147);
  delay(550);
  new_key=0;
  while(!new_key) cont_music();
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"ORDER NOW AND GET ALL 30 LEVELS!",10,9);
  Shadow_Text(  5,15,"ONLY $29 (PLUS $5 S&H)  (SEE ORDER.TXT)",10,9);
  Shadow_Text(  5,25,"CALL (812) 875-2852 (VOICE)",10,9);
  Shadow_Text(  5,35,"OR FAX ORDER (812) 875-2836 (FAX LINE)",10,9);
  
  Shadow_Text(  5,51,"  HEY DON'T JUST BE COOL ...", 250,9);
  Shadow_Text(  5,61," LOOK COOL IN HYPERCYCLES GEAR!",250,9);
  Shadow_Text(  5,74,"SHOW THAT WORLD WHAT A HYPERACE", 250,9);
  Shadow_Text(  5,84,"    REALLY LOOKS LIKE!", 250,9);
  
  Shadow_Text( 5,100,"    HYPERCYCLES HATS ONLY $13.95 EACH",251,9);
  Shadow_Text( 5,115,"HYPERCYCLES T-SHIRTS ONLY $13.95 EACH",251,9);
  
  Shadow_Text( 231,171,"PAGE 4 OF 4", 10,9);
  Shadow_Text( 231,181,"  - END -", 10,9);
  
  Display2( 100,165, 147);
  delay(550);
  new_key=0;
  while(!new_key) cont_music();
  new_key=0;
  
  
  PCX_Unload(147);
  PCX_Unload(146);
  PCX_Unload(148);
  memset(vga_ram,0,64000);
  
  //Restore Palette but don't show yet
  for(a=0; a<256; a++)
  {
    red[a]=red2[a];
    blue[a]=blue2[a];
    green[a]=green2[a];
  }
  Set_Palette();

}


void read_me()
{
  
  PCX_Load("sky1.pcx", 146,1);
  PCX_Load("inet.pcx", 148,1);
  Set_Palette();
  memset(vga_ram,0,64000);
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"QUICK REFERENCE",255,12);
  Shadow_Text( 15,17,"* LEFT & RIGHT ARROW KEYS TURN CYCLE",250,12);
  Shadow_Text( 15,29,"* PRESS CTRL KEY AND LEFT OR RIGHT",250,12);
  Shadow_Text( 15,41,"   ARROW KEY FOR FULL 360 DEGREE VIEW",250,12);
  Shadow_Text( 15,53,"* UP & DOWN ARROWS CONTROL ACCELERATOR",250,12);

  Shadow_Text( 15,65,"*    ALT    - FIRES CURRENT WEAPON", 250,12);
  Shadow_Text( 15,77,"* SPACE BAR - WALL PROJECTOR ON & OFF", 250,12);
  Shadow_Text( 15,89,"* BACKSPACE - SELECT RADAR SCOPE", 250,12);
  Shadow_Text(15,101,"*   ENTER   - CYCLES WEAPON SELECTION", 250,12);
  Shadow_Text(15,113,"*    1-8    - PICKS FROM 8 WEAPONS", 250,12);
  Shadow_Text(15,125,"*     J     - JOYSTICK CALIBRATION", 250,12);
  
  Shadow_Text(15,137,"POWER LEVEL  - UPPER LEFT YELLOW LINE", 250,12);
  Shadow_Text(15,149,"SHIELD LEVEL - UPPER LEFT RED LINE", 250,12);
  Shadow_Text(15,161,"    SPEED    - LOWER RIGHT GREEN LINE", 250,12);
 
  Shadow_Text( 211,171,"PAGE 1 OF 15", 249,12);
  Shadow_Text( 211,181,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;

  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 15,"  *** TABLE OF CONTENTS ***",249,12);
  Shadow_Text( 15, 30," 3 -  GATEWAYS AND KEYS",248,12);
  Shadow_Text( 15, 42," 4 -  DR. DEVARO AND HIS LEGION",248,12);
  Shadow_Text( 15, 54," 7 -  HYPERCYCLE WEAPONS & EQUIPMENT",248,12);
  Shadow_Text( 15, 66,"10 -  THE SAUCER & SUPPLIES",248,12);
  Shadow_Text( 15, 78,"13 -  JOYSTICK CONTROLLER",248,12);
  Shadow_Text( 15, 90,"14 -  HINTS & TIPS",248,12);
  Shadow_Text( 15,102,"15 -  TECHNICAL SUPPORT",248,12);
 
  Shadow_Text( 211,171,"PAGE 2 OF 15", 249,12);
  Shadow_Text( 211,181,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5,"           GATEWAYS AND KEYS", 255,12);
  Shadow_Text(  5,20,"  THE PRIMARY GOAL OF  EACH LEVEL IS TO",250,12);
  Shadow_Text(  5,32,"OBTAIN ENOUGH KEYS (BLUE RINGS) SO THAT",250,12);
  Shadow_Text(  5,44,"SO THAT THE GATEWAY  WILL OPEN  FOR YOU",250,12);
  Shadow_Text(  5,56,"TO EXIT. A STATUS DISPLAY WILL SHOW THE",250,12);
  Shadow_Text(  5,68,"MINIMUM NUMBER OF KEYS REQUIRED AND HOW",250,12);
  Shadow_Text(  5,80,"MANY ARE AVAILABLE.  YOU WILL RECEIVE A",250,12);
  Shadow_Text(  5,92,"LEVEL SUPER BONUS IF YOU CAN OBTAIN ALL",250,12);
  Shadow_Text( 5,104,"KEYS AVAILABLE.",250,12);
  
  Shadow_Text( 5,120,"  GATEWAYS CAN ONLY  BE OPENED BY KEYS.",252,12);
  Shadow_Text( 5,132,"MISSILES AND OTHER WEAPONS WILL HAVE NO", 252,12);
  Shadow_Text( 5,144,"EFFECT.",252,12);
  
  Shadow_Text( 211,171,"PAGE 3 OF 15", 249,12);
  Shadow_Text( 211,181,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5," ***  DR. DEVARO AND HIS LEGION  ***", 255,12);
  Shadow_Text(  5,20,"  DR. DEVARO HAS DOMAIN OVER THE GRID.",240,12);
  Shadow_Text(  5,30,"HE ENTRAPS USERS AND  BRINGS THEM DOWN",240,12);
  Shadow_Text(  5,40,"TO PLAY ON THE GRID.  HE WILL SEND HIS",240,12);
  Shadow_Text(  5,50,"LEGION OF TROOPS TO DESTROY YOU  OR TO",240,12);
  Shadow_Text(  5,60,"ENSLAVE YOU AS ONE OF HIS LEGION.",240,12);
  
  Shadow_Text(  5,75,"* ENEMY RIDERS *",249,12);
  Shadow_Text(  5,85,"THE DOCTOR HAS MANY THOUSANDS OF THESE",254,12);
  Shadow_Text(  5,95,"THAT HE WILL USE AGAINST YOU. MOST ARE",254,12);
  Shadow_Text( 5,105,"THOSE THAT HAVE BEEN ABSORBED INTO HIS",254,12);
  Shadow_Text( 5,115,"EVIL COLLECTIVE AFTER  THEY FOUGHT HIM",254,12);
  Shadow_Text( 5,125,"AND LOST.  EACH  MAY HAVE A  DIFFERENT",254,12);
  Shadow_Text( 5,135,"STRATEGY TO USE AGAINST YOU. THERE ARE",254,12);
  Shadow_Text( 5,145,"SOME REPORTS  OF  NEW RIDERS  KNOWN AS",254,12);
  Shadow_Text( 5,155,"THE 'KAZEE' AND  THAT  THEY ARE ALMOST",254,12);
  Shadow_Text( 5,165,"IMMUNE TO MOST MISSILES AND LASERS.",254,12);
  
  Shadow_Text( 211,178,"PAGE 4 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5,"* CARRIERS *",249,12);
  Shadow_Text(  5,15,"ON YOUR RUN  YOU WILL SEE  CARRIERS IN",240,12);
  Shadow_Text(  5,25,"THE SKY  BRINGING  MORE RIDERS  TO THE",240,12);
  Shadow_Text(  5,35,"GRID. THE ONLY WAY TO DESTROY AND BLOW",240,12);
  Shadow_Text(  5,45,"A CARRIER OUT OF THE SKY  IS TO OBTAIN",240,12);
  Shadow_Text(  5,55,"AND FIRE DART SURFACE TO AIR MISSILES.",240,12);
  
  Shadow_Text(  5,67,"* STALKERS *",249,12);
  Shadow_Text(  5,77,"DEVARO FIGURES IF ALL HIS RIDERS CAN'T",252,12);
  Shadow_Text(  5,87,"DESTROY YOU QUICKLY HIS STALKERS WILL.",252,12);
  Shadow_Text(  5,97,"THESE FAST FLYING  AIRBORNE DEATHTRAPS",252,12);
  Shadow_Text( 5,107,"DROP DEVASTATING CONCUSSION BOMBS.",252,12);
  Shadow_Text( 5,117,"THE ONLY  WAY TO SURVIVE  A BLAST FROM",71,12);
  Shadow_Text( 5,127,"A CONCUSSION BOMB THAT  EXPLODES CLOSE",71,12);
  Shadow_Text( 5,137,"TO YOU IS TO HAVE STRONG SHIELDS. WITH",71,12);
  Shadow_Text( 5,147,"SHIELDS YOU WILL BE KNOCKED AROUND BUT",71,12);
  Shadow_Text( 5,157,"YOY MAY JUST STAY ALIVE.",71,12);
  
  Shadow_Text( 211,178,"PAGE 5 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  new_key=0;
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5,"* LASER TANKS *",249 ,12);
  Shadow_Text(  5,15,"THE DOCTOR HAS A ANOTHER SURPRISE  FOR",250,12);
  Shadow_Text(  5,25,"YOU. HIS NEWLY CREATED LASER TANKS ARE",250,12);
  Shadow_Text(  5,35,"DESIGNED TO PENETRATE AND DESTORY  ANY",250,12);
  Shadow_Text(  5,45,"OF THE WALLS IN THEIR WAY IN AN EFFORT",250,12);
  Shadow_Text(  5,55,"HUNT AND  DESTROY YOU.  INFORMATION ON",250,12);
  Shadow_Text(  5,65,"THE WEAPONS OF LASER TANK ARE SOMEWHAT",250,12);
  Shadow_Text(  5,75,"SKETCHY AS NO SURVIVORS  HAVE RETURNED",250,12);
  Shadow_Text(  5,85,"TO GIVE A REPORT OF THEIR CAPABILITIES.",250,12);
  Shadow_Text( 5,100," SO THE BEST ADVICE SO FAR IS TO AVOID",240,12);
  Shadow_Text( 5,110,"THE TANKS AT ALL COSTS.",240,12);
  
  Shadow_Text( 5,129,"* MOVING WALLS *", 249,12);
  Shadow_Text( 5,139,"ON MANY LEVELS YOU WILL FIND RED BRICK",245,12);
  Shadow_Text( 5,149,"WALLS THAT MOVE  BACK AND FORTH ON THE", 245,12);
  Shadow_Text( 5,159,"GRID. THESE CAN BE USEFUL!", 245,12);
  
  Shadow_Text( 211,178,"PAGE 6 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5,"*** HYPERCYCLE WEAPONS & EQUIPMENT  ***",249 ,12);
  Shadow_Text(  5,15,"  ADDITIONAL  EQUIPMENT & SUPPLIES ARE",250,12);
  Shadow_Text(  5,25,"MADE AVAILABLE  AS YOU  MAKE YOUR  RUN",250,12);
  Shadow_Text(  5,35,"THROUGH THE LEVELS. THE STATUS DISPLAY",250,12);
  Shadow_Text(  5,45,"SHOWN  BEFORE  YOUR RUN  ON EACH LEVEL",250,12);
  Shadow_Text(  5,55,"WILL SHOW WHAT IS AVAILABLE.",250,12);
  
  Shadow_Text(  5,70,"* ACCELERATOR *",249,12);
  Shadow_Text(  5,80,"THIS  IS THE FIRST  HYPERCYCLE  OPTION",252,12);
  Shadow_Text(  5,90,"YOU CAN OBTAIN. ONCE PICKED UP YOU CAN",252,12);
  Shadow_Text( 5,100,"ADJUST CYCLE SPEED BY USING THE UP AND",252,12);
  Shadow_Text( 5,110,"DOWN ARROW KEYS.",252,12);
  
  Shadow_Text( 5,125,"* LASER WALL PROJECTOR *", 249,12);
  Shadow_Text( 5,135,"RIDERS LAY LASER WALLS TO  ENTRAP YOU.",244,12);
  Shadow_Text( 5,145,"PICK UP THIS  OPTION TO  PUT DOWN YOUR", 244,12);
  Shadow_Text( 5,155,"OWN. PRESS THE SPACE BAR TO TOGGLE THE", 244,12);
  Shadow_Text( 5,165,"DEVICE ON AND OFF", 244,12);
  
  Shadow_Text( 211,178,"PAGE 7 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text(  5, 5,"* PROTON & NEUTRON MISSILE LAUNCHER *",249 ,12);
  Shadow_Text(  5,15,"THE MISSILE LAUNCHER ALLOWS THE RIDER",246,12);
  Shadow_Text(  5,25,"TO FIRE BOTH  PROTON AND THE POWERFUL",246,12);
  Shadow_Text(  5,35,"NEUTRON MISSILES.  THESE MISSILES CAN",246,12);
  Shadow_Text(  5,45,"DESTROY BOTH WALLS  AND ENEMY RIDERS.",246,12);
  
  Shadow_Text(  5,60,"USE THE 'CTRL' KEY  WITH THE LEFT AND",252,12);
  Shadow_Text(  5,70,"RIGHT ARROW KEYS & USE THE 360 DEGREE",252,12);
  Shadow_Text(  5,80,"VIEW ALLOWING YOU  TO FIRE WEAPONS IN",252,12);
  Shadow_Text(  5,90,"ANY DIRECTION.",252,12);

  Shadow_Text( 5,112," PROTON MISSILES -  HAVE THE POWER TO",228,12);
  Shadow_Text( 5,122,"DESTROY  AN ENEMY RIDER  OR  ONE WALL",228,12);
  Shadow_Text( 5,132,"PANEL.",228,12);
  
  Shadow_Text( 5,147," NEUTRON MISSILES - ARE SIGNIFICANTLY",78,12);
  Shadow_Text( 5,157,"MORE  DESTRUCTIVE.  THEY CAN  DESTROY",78,12);
  Shadow_Text( 5,167,"UP TO FOUR WALLS AND A RIDER.",78,12);
  
  Shadow_Text( 211,178,"PAGE 8 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;

  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"* RADAR SCOPES *",249,12);
  Shadow_Text(  5,15,"THE RADAR SCOPES ALLOW YOU SEE A LOCAL",252,12);
  Shadow_Text(  5,25,"AREA MAP  OF LOCATION ON THE GRID.  IT",252,12);
  Shadow_Text(  5,35,"CHANGES DYNAMICALLY AS YOU FLY THROUGH",252,12);
  Shadow_Text(  5,45,"THE MAZE  OF  WALLS AND  ENEMIES.  THE",252,12);
  Shadow_Text(  5,55,"RADAR SCOPE MODELS AVAILABLE EACH HAVE",252,12);
  Shadow_Text(  5,65,"DIFFERENT RANGE CAPABILITIES. RIDERS &",252,12);
  Shadow_Text(  5,75,"THE WALLS THEY CREATE ARE YELLOW. THE",252,12);
  Shadow_Text(  5,85,"KEYS AND SAUCER SUPPLIES ARE WHITE.",252,12);
  
  Shadow_Text( 5,100,"* LASER GUNS *",249,12);
  Shadow_Text( 5,110,"THERE  ARE THREE  VARIOUS  LASER  GUNS",245,12);
  Shadow_Text( 5,120,"AVAILABLE  FOR YOU TO  OBTAIN AND USE.",245,12);
  Shadow_Text( 5,130,"EACH HAS DIFFERENT POWER OUTPUTS.  THE",245,12);
  Shadow_Text( 5,140,"LASER GUNS ARE  PRIMARILY USED AGAINST",245,12);
  Shadow_Text( 5,150,"ENEMY RIDERS.",245,12);
  
  
  Shadow_Text( 211,178,"PAGE 9 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  
  Shadow_Text( 5,  5,"* SHIELDS *",249,12);
  Shadow_Text( 5, 15,"SHIELD REGENERATORS HELP  PROTECT YOUR",250,12);
  Shadow_Text( 5, 25,"HYPERCYCLE AGAINST ENEMY ATTACKS.   AT",250,12);
  Shadow_Text( 5, 35,"THE BEGINNING  YOU  WILL  HAVE MINIMAL",250,12);
  Shadow_Text( 5, 45,"SHIELDING.  THERE ARE  STRONGER SHIELD",250,12);
  Shadow_Text( 5, 55,"REGENERATORS THAT YOU CAN PICK UP. ",250,12);
  Shadow_Text( 5, 65,"SHIELDS WILL SLOWLY REGEN ON THEIR OWN",250,12);
  Shadow_Text( 5, 75,"AND THE GLOBES DROPPED BY  THE SAUCERS",250,12);
  Shadow_Text( 5, 85,"PROVIDE LARGE BOOSTS IN SHIELD LEVELS.",250,12);
  
  Shadow_Text( 5,105,"* POWER LEVELS *",249,12);
  Shadow_Text( 5,115,"THERE ARE  POWER BATTERY PACKS ON YOUR",252,12);
  Shadow_Text( 5,125,"HYPERCYCLE. POWER IS USED  MOVING THRU",252,12);
  Shadow_Text( 5,135,"THE GRID  AND WHEN FIRING LASERS.  THE" ,252,12);
  Shadow_Text( 5,145,"ONLY WAY TO INCREASE  THE  POWER LEVEL",252,12);
  Shadow_Text( 5,155,"IS TO PICK UP THE DROPPED GLOBES.  LOW",252,12);
  Shadow_Text( 5,165,"POWER LEVELS WILL SLOW DOWN THE CYCLE.",252,12);
  
  
  Shadow_Text( 211,178,"PAGE 10 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"* REAR MOUNT MISSILE LAUNCHER *",249,12);
  Shadow_Text(  5,15,"THIS UNIT ALLOWS  THE HYPERCYCLE RIDER",252,12);
  Shadow_Text(  5,25,"TO QUICKLY  FIRE  PHOTON  AND  NEUTRON",252,12);
  Shadow_Text(  5,35,"MISSILES  AT  ENEMIES  DIRECTLY BEHIND",252,12);
  Shadow_Text(  5,45,"CYCLE WITHOUT HAVING TO TURN TO AIM.",252,12);
  
  Shadow_Text(  5,60,"* DART SURFACE TO AIR LAUNCHER *",249,12);
  Shadow_Text(  5,70,"THIS LAUNCHER  ALLOWS YOU  TO FIRE AND",250,12);
  Shadow_Text(  5,80,"BRING DOWN  AIRBORNE  TARGETS.  AIMING",250,12);
  Shadow_Text(  5,90,"& FIRING IS SIMILAR TO OTHER MISSILES.",250,12);
  
  Shadow_Text( 5,105,"* PHASE SHIFTER *",249,12);
  Shadow_Text( 5,115,"AFTER THIS  DEVICE IS OBTAINED  AND IS",245,12);
  Shadow_Text( 5,125,"INSTALLED  THE ENERGY OF  YOU AND YOUR",245,12);
  Shadow_Text( 5,135,"HYPERCYCLE IS  PUT INTO PHASE WITH THE",245,12);
  Shadow_Text( 5,145,"ENERGY EMITTED BY YOUR WALL PROJECTOR.",245,12);
  Shadow_Text( 5,155,"THIS ALLOWS YOU TO PASS RIGHT THOROUGH",245,12);
  Shadow_Text( 5,165,"THE WALLS YOU CREATE WITHOUT HARM.",245,12);
  
  
  Shadow_Text( 211,178,"PAGE 11 OF 15", 249,12);
  Shadow_Text( 211,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"*** THE SAUCER & SUPPLIES ***",249,12);
  Shadow_Text(  5,15,"YOU HAVE A FRIEND TO  HELP YOU AGAINST",252,12);
  Shadow_Text(  5,25,"THE EVIL DOCTOR.  THE SAUCER WILL DROP",252,12);
  Shadow_Text(  5,35,"GLOBES THAT  INCREASE POWER AND SHIELD",252,12);
  Shadow_Text(  5,45,"LEVELS AND HAVE SUPPLIES SUCH AS:",252,12);
  Shadow_Text(  5,60,"  * PROTON MISSILES",245,12);
  Shadow_Text(  5,73,"  * NEUTRON MISSILES",245,12);
  Shadow_Text(  5,86,"  * DART MISSILES",245,12);
  Shadow_Text( 5,115,"THE BRIGHT  RED & WHITE SAUCER IS ALSO",252,12);
  Shadow_Text( 5,125,"FIGHTING THE EVIL DOCTOR.  DON'T SHOOT",252,12);
  Shadow_Text( 5,135,"IT OUT THE SKY  OR  YOUR SUPPLIES WILL",252,12);
  Shadow_Text( 5,145,"DRY UP FAST!",252,12);
  
  Shadow_Text( 201,178,"PAGE 12 OF 15", 249,12);
  Shadow_Text( 201,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;
  
  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5, " *** JOYSTICK CONTROLLER *** " ,249,12);
  Shadow_Text(  5,15,"YOU CAN USE A JOYSTICK TO CONTROL YOUR",252,12);
  Shadow_Text(  5,25,"HYPERCYCLE.",252,12);
  Shadow_Text(  5,40,"* LEFT AND RIGHT MOVEMENTS OF JOYSTICK",252,12);
  Shadow_Text(  5,50,"   ALLOW FOR STEERING.",252,12);
  Shadow_Text(  5,65,"* UP AND DOWN  MOVEMENTS  OF  JOYSTICK",252,12);
  Shadow_Text(  5,75,"   CONTROL ACCELLERATOR.",252,12);
  Shadow_Text(  5,90,"* BUTTON 1 FIRES WEAPON (LIKE ALT KEY)",245,12);
  Shadow_Text( 5,115,"* BUTTON 2 ALLOWS  360  DEGREE VIEWING",245,12);
  Shadow_Text( 5,125,"   BY MOVING JOYSTICK LEFT OR RIGHT AS",245,12);
  Shadow_Text( 5,135,"   BUTTON 2 IS HELD DOWN.",245,12);
  
  Shadow_Text( 201,178,"PAGE 13 OF 15", 249,12);
  Shadow_Text( 201,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;

  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5,15,"*** HINTS & TIPS ***",249,12);
  Shadow_Text(  5,30,"FIRST SAFETY TIP: AVOID WALLS!",251,12);
  Shadow_Text(  5,45,"OK THAT'S NOT MUCH HELP  BUT  FOR MORE",251,12);
  Shadow_Text(  5,60,"HINTS AND TIPS ON  PLAYING HYPERCYCLES",251,12);
  Shadow_Text(  5,75,"READ THE 'HINTS.TXT' TEXT FILE THAT IS",251,12);
  Shadow_Text(  5,90,"INCLUDED.",251,12);
  
  Shadow_Text( 201,178,"PAGE 14 OF 15", 249,12);
  Shadow_Text( 201,188,"  - MORE -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;

  memcpy(vga_ram, picture[146].image,63360);
  Shadow_Text(  5, 5,"*** TECHNICAL SUPPORT ***",249,12);
  Shadow_Text(  5,18,"DO NOT CALL  ORDER LINE  FOR TECHNICAL",251,12);
  Shadow_Text(  5,28,"SUPPORT. FILL OUT TECHFORM.TXT AND FAX",251,12);
  Shadow_Text(  5,38,"QUESTIONS TO US AT (812) 875-2836.",251,12);
  Shadow_Text(  5,50,"YOU CAN SEND QUESTIONS VIA E-MAIL:",251,12);
  //Shadow_Text(  5,60,"ACLYPSE.AOM.COM",251,12);
  Display2( 100,60, 148);

  Shadow_Text(  5,90,"READ THE  TEXT FILES  LISTED BELOW FOR",240,12);
  Shadow_Text( 5,100,"INFORMATION ABOUT HYPERCYCLES.",240,12);
  Shadow_Text( 5,110,"TECHFORM.TXT TECH QUESTION FAX SHEET",249,12);
  Shadow_Text( 5,120," HELPME.TXT  HELP RUNNING HYPERCYCLES",252,12);
  Shadow_Text( 5,130," ORDER.TXT   ORDER FORM & CONTACT INFO",253,12);
  Shadow_Text( 5,140," HINTS.TXT   PLAYING HINTS & TIPS",245,12);
  Shadow_Text( 5,150,"VENDORS.TXT  INFORMATION FOR BOTH",254,12);
  Shadow_Text( 5,160,"        COMMERCIAL & SHAREWARE VENDORS",254,12);
  
  
  Shadow_Text( 201,178,"PAGE 15 OF 15", 249,12);
  Shadow_Text( 201,188,"  - END -", 249,12);
  
  delay(150);
  new_key=0;
  while(!new_key) cont_music();
  if(new_key==27) goto readme_done;
  new_key=0;

readme_done:  
  new_key=0;
  PCX_Unload(148);
  PCX_Unload(146);
  memset(vga_ram,0,64000);

}

void credits()
{
  int a;
  Stop_Melo();
  PCX_Load("sky1.pcx", 146,1);
  memset(vga_ram,0,64000);
  memset(double_buffer_l,0,64000);
  memcpy(double_buffer_l, picture[146].image,63360);
  PCX_Load("credits.pcx",147,1);
  if(music_toggle==2)
  {
      play_again();
      play_song("rmh12.mdi" );
      strcpy(curr_song,"rmh12.mdi");
  }
  raw_key=0;

  Set_Palette();

  for(a=199; a>0; a--)
  {
    _enable();
    memcpy(double_buffer_l, picture[146].image,63360);
    PCX_Paste_Image(60, a, 0, 147);
    memcpy(vga_ram, double_buffer_l,63360);
    delay(24);
    if( raw_key ) goto Cred_Jump;
  }
  
  for(a=1; a<780; a++)
  {
    _enable();
    memcpy(double_buffer_l, picture[146].image,63360);
    PCX_Paste_Image(60, 0, a, 147);
    memcpy(vga_ram, double_buffer_l,63360);
    delay(24);
    if( raw_key ) goto Cred_Jump;
  } 
Cred_Jump:
  PCX_Unload(147);
  Fade_Pal();
  memset(double_buffer_l,0,64000);
  memcpy(double_buffer_l, picture[146].image,63360);
  PCX_Unload(146);
  delay(10);
  Set_Palette();
  _enable();
}  

void DocTalk(char * tx)
{
  int a;
  a = 160 - ((strlen(tx) * 8)/2);
  Display_Text(  a-1, 180,tx,10);
  Display_Text(    a, 181,tx,255);
}

void doctor_ender1()
{
  int a,b,ctr=0;
  
  doctor_load();
  digital_speed=9500;
  b=1;
  for(a=25; a<picture[131].width-4; a+=4)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
  }
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open

  for(ctr=0;ctr<5;ctr++)
  {
   if(digi_flag==2) play_vox(drs[ctr].fname1);
   else
   {
    if(drs[ctr].sent2[0]=='*')
    {
      DocTalk(drs[ctr].sent1);
    }
    else
    {
      DocTalk(drs[ctr].sent1);
      a = 160 - ((strlen(drs[ctr].sent1) * 8)/2);
      Display_Text(  a-1, 190,drs[ctr].sent2,10);
      Display_Text(    a, 191,drs[ctr].sent2,255);

    }
   }
   if(digi_flag==2)
   {
    a=132;
    while(CTV_voice_status)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
    }
   }
   else
   {
    b=0;
    a=133;
    while(b<15)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
      b++;
    }
   }
   if(digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

   PCX_Show_Image(160,100,131,picture[131].width);
   PCX_Show_Image(163,95,136,picture[136].width); // eyes open
   delay(150);
   PCX_Show_Image(163,95,135,picture[135].width);
   delay(150);
   PCX_Show_Image(160,100,131,picture[131].width);
   delay(150);
   PCX_Show_Image(163,95,135,picture[135].width);
   delay(150);
   PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  }  
  
  if(!digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

  PCX_Show_Image(160,100,131,picture[131].width);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  delay(1000);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  b=1;
  for(a=picture[131].width-4;a>25; a-=10)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
    memcpy(vga_ram,double_buffer_l,prm_copy1);
  }
  digital_speed=11025;
  doctor_unload();
}

void doctor_ender2()
{
  int a,b,ctr=0;
  
  doctor_load();
  digital_speed=9500;
  b=1;
  for(a=25; a<picture[131].width-4; a+=4)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
  }
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open

  for(ctr=0;ctr<5;ctr++)
  {
   if(digi_flag==2) play_vox(drf[ctr].fname1);
   else
   {
    if(drf[ctr].sent2[0]=='*')
    {
      DocTalk(drf[ctr].sent1);
    }
    else
    {
      DocTalk(drf[ctr].sent1);
      a = 160 - ((strlen(drf[ctr].sent1) * 8)/2);
      Display_Text(  a-1, 190,drf[ctr].sent2,10);
      Display_Text(    a, 191,drf[ctr].sent2,255);

    }
   }
   if(digi_flag==2)
   {
    a=132;
    while(CTV_voice_status)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
    }
   }
   else
   {
    b=0;
    a=133;
    while(b<15)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
      b++;
    }
   }
   if(digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

   PCX_Show_Image(160,100,131,picture[131].width);
   PCX_Show_Image(163,95,136,picture[136].width); // eyes open
   delay(150);
   PCX_Show_Image(163,95,135,picture[135].width);
   delay(150);
   PCX_Show_Image(160,100,131,picture[131].width);
   delay(150);
   PCX_Show_Image(163,95,135,picture[135].width);
   delay(150);
   PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  }  
  
  if(!digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

  PCX_Show_Image(160,100,131,picture[131].width);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  delay(1000);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  b=1;
  for(a=picture[131].width-4;a>25; a-=10)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
    memcpy(vga_ram,double_buffer_l,prm_copy1);
  }
  digital_speed=11025;
  doctor_unload();
}

void doctor()
{
  int a,b;
  if(demo_mode) return;
  //if(level_num>15) return;
  if(level_num==old_level_num) return;
  //old_level_num=level_num;
  doctor_load();
  digital_speed=9500;
  b=1;
  for(a=25; a<picture[131].width-4; a+=4)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
  }
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open

  if(digi_flag==2) play_vox(doc_face[level_num-1].fname1);
  else
  {
    DocTalk(doc_face[level_num-1].sent1);
  }
  
  if(digi_flag==2)
  {
    a=132;
    while(CTV_voice_status)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
    }
  }
  else
  {
    b=0;
    a=133;
    while(b<12)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
      b++;
    }
  }
    
  if(digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

  PCX_Show_Image(160,100,131,picture[131].width);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  delay(250);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(250);
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(250);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(250);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  
  if(digi_flag==2) play_vox(doc_face[level_num-1].fname2);
  else
  {
    DocTalk(doc_face[level_num-1].sent2);
  }

  if(digi_flag==2)
  {
    a=133;
    while(CTV_voice_status)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
    }
  }
  else
  {
    b=0;
    a=133;
    while(b<12)
    {
      PCX_Show_Image(163,159,a,picture[a].width);
      delay(250);
      a++;
      if(a>134) a=132;
      b++;
    }
  }
  if(digi_flag<2)  memcpy(vga_ram,double_buffer_l,prm_copy1);

  PCX_Show_Image(160,100,131,picture[131].width);
  PCX_Show_Image(163,95,136,picture[136].width); // eyes open
  delay(1000);
  PCX_Show_Image(163,95,135,picture[135].width);
  delay(500);
  PCX_Show_Image(160,100,131,picture[131].width);
  delay(500);
  b=1;
  for(a=picture[131].width-4;a>25; a-=10)
  {
    _enable();
    PCX_Show_Image(160,100,131,a);
    delay(10);
    memcpy(vga_ram,double_buffer_l,prm_copy1);
  }
  digital_speed=11025;
  doctor_unload();
}

int menu2()
{
  int a,b,c,d,e,f,curr;
  b=0; c=0; d=0; curr=131; e=0; f=0;
  
  new_key=0;
  menu1_unload();
  menu2_load();

  while(!c)
  {
    memcpy(vga_ram,double_buffer_l,64000);
    for(a=131;a<=133;a++)
    {
      if(a==curr) PCX_Show_Image(160,50*(a-131)+15,a,picture[a].width+60);
      else PCX_Show_Image(160,50*(a-131)+15,a,picture[a].width);
    }
    switch(music_toggle)        
    {
      case 0:
       PCX_Show_Image(160,44,141,picture[141].width);
       break;
      case 1:
       PCX_Show_Image(160,44,142,picture[142].width);
       break;
      case 2:
       PCX_Show_Image(160,44,143,picture[143].width);
       break;
    }
    switch(digi_flag)        
    {
      case 0:
       PCX_Show_Image(160,89,141,picture[141].width);
       break;
      case 1:
       PCX_Show_Image(160,89,142,picture[142].width);
       break;
      case 2:
       PCX_Show_Image(160,89,143,picture[143].width);
       break;
    }
    switch(controls)        
    {
      case 0:
       PCX_Show_Image(160,141,135,picture[135].width);
       break;
      case 1:
      case 2:
       PCX_Show_Image(160,141,137,picture[137].width);
       break;
    }

    e=0;
    while(!e)
    {
       switch( new_key )
       {
         case 'J':
           calibrate_stick();
           new_key=0;
           e++;
           break;
         case 27:  
           menu2_unload();
           menu1_load();
           new_key=0;
           return(0);
           break;
         case 13:  
           switch(curr)
           {
             case 131:
               e++;
               if(music_toggle==1) music_toggle=2;
               else if(music_toggle==2) music_toggle=1;
               else e=0;
               break;
             case 132:
               e++;
               if(digi_flag==1) digi_flag=2;
               else if(digi_flag==2) digi_flag=1;
               else e=0;
               break;
             case 133:
               if(controls==2) controls=0;
               else
               {
                 controls=2; 
               }
               e++;
               break;
           }
           new_key=0;
           break;
         case 8:
           curr++; if(curr>133) curr=131;
           new_key=0;
           e++;
           break;  
         case 5:
           curr--; if(curr<131) curr=133;
           new_key=0;
           e++;
           break;  
       }
       _enable();
       if(!musRunning && music_toggle==2) 
       { 
        if( music_cnt==4) { play_again(); music_cnt--; }
       }
       if(music_toggle==2 && music_cnt<4)
       {
         music_cnt--;
         if(!music_cnt)
         {  
           if(next_song[0])
           {
             play_song( next_song );
             strcpy(curr_song,next_song);
             next_song[0]=0;
           }
           else play_again();
           music_cnt=4;
         }  
       }

    }
  }
  return(0);
}


int menu1(int ck)
{
  int a,b,c,d,e,f,curr;
  unsigned int tmr9;
  
  cheat_ctr=0;
  //ck=0; // ****Take out
  b=0; c=0; d=0; curr=131; e=0; f=0;
  menu1_load();

  while(!c)
  {
    memcpy(vga_ram,double_buffer_l,64000);
    PCX_Show_Image(270,178,140,picture[140].width);
    
    if(!ck)
    {
      for(a=131;a<=139;a++)
      {
        if(a!=curr) PCX_Show_Image(160,20*(a-131)+15,a,picture[a].width-10);
      }
      for(a=131;a<=139;a++)
      {
        if(a==curr) PCX_Show_Image(160,20*(a-131)+15,a,picture[a].width+80);
      }
    }
    else
    { //no demo mode 
      for(a=131;a<=138;a++)
      {
        if(a!=curr) 
        {
          if(a<138) PCX_Show_Image(160,20*(a-131)+15,a,picture[a].width-10);
          else PCX_Show_Image(160,20*(a-131)+15,139,picture[139].width-10);
        }
      }
      for(a=131;a<=139;a++)
      {
        if(a==curr) 
        {
          if(a<138) PCX_Show_Image(160,20*(a-131)+15,a,picture[a].width+80);
          else PCX_Show_Image(160,20*(a-131)+15,139,picture[139].width+80);
        }
      }
    }
    
    tmr9 = timerval()+270;  //15 seconds
    e=0;
    while(!e)
    {
		delay(16);
       if(!ck && timerval()>tmr9)
       {
         if(!mn1_flap) curr=138; 
         else curr=137; 
         new_key=13;
       }
       
       switch( new_key )
       {
         case 'A':
           if(cheat_ctr==0) cheat_ctr++;
           else cheat_ctr=0;
           new_key=0; 
           break;
         case 'H':
           if(cheat_ctr==1) cheat_ctr++;
           else cheat_ctr=0;
           new_key=0; 
           break;
         case 'C':
           if(cheat_ctr==2) cheat_ctr++;
           else cheat_ctr=0;
           new_key=0; 
           break;
         case 'R':
           if(cheat_ctr==3) cheat_ctr++;
           else cheat_ctr=0;
           new_key=0; 
           break;
         case 'P':
           if(ck && cheat_ctr==4) 
           {
             shieldit(5000);
             powerit(5000);
             digital_speed=9500;    
             play_vox("allkeys.raw");
             digital_speed=11025;
           }
           cheat_ctr=0;
           new_key=0; 
           break;
         /*case 'I':
           if(ck && cheat_ctr==4) 
           {
             if(!invun) invun=1;
             else invun=0;
             digital_speed=9500;    
             play_vox("allkeys.raw");
             digital_speed=11025;
           }
           cheat_ctr=0;
           new_key=0; 
           break;*/
         case 'O':
           if(ck && cheat_ctr==4) 
           {
              for(d=0,a=0;a<64;a++,d+=64)                
              {
                for(b=0;b<64;b++)
                {
                  // Alter so all doors start falling
                  if(world[a][b] == 'Y' || world[a][b] == 'Z') 
                    wall_ht_map[d+b] = (wall_ht_map[d+b] & 63) | 128;
                }  
              }
             digital_speed=9500;    
             play_vox("allkeys.raw");
             digital_speed=11025;
           }
           cheat_ctr=0;
           new_key=0; 
           break;
         case 'G':
           if(ck && cheat_ctr==4) cheat_ctr++;
           level_jump=0;
           new_key=0; 
           break;
         case  '0':
         case  '1':
         case  '2':
         case  '3':
         case  '4':
         case  '5':
         case  '6':
         case  '7':
         case  '8':
         case  '9':
           if(ck && cheat_ctr>4) 
           {
             if(cheat_ctr==5)
             {
              level_jump=(new_key-'0');
              cheat_ctr++;
             }
             else
             {
               level_jump*=10;
               level_jump+=(new_key-'0');
               if(level_jump>0 && level_jump<=30)
               {
                 level_num=level_jump;
                 dead=1;
                 digital_speed=9500;    
                 play_vox("allkeys.raw");
                 digital_speed=11025;
               }
               cheat_ctr=0;
             }
           }
           else cheat_ctr=0;
           new_key=0; 
           break;
         case 'E':
           if(ck && cheat_ctr==4) 
           {
            for(a=0;a<30;a++)
            {
              if(access_buf[a]>='A') 
              {
                if(access_buf[a]=='C') {curr_weapon=0; weapon_list[0].qty=50;}
                if(access_buf[a]=='F') shield_level=512;
                if(access_buf[a]=='L') shield_level=1024;
                b=access_buf[a]-'A';
                access_buf[a]=' ';
                digital_speed=9500;    
                play_vox("allkeys.raw");
                digital_speed=11025;
                eq_gotit=0;
                break;
              }
            }
           }
           cheat_ctr=0;
           new_key=0; 
           break;
         case 'W':
           if(ck && cheat_ctr==4) 
           {
             weapon_list[0].qty+=500;
             weapon_list[1].qty+=500;
             weapon_list[3].qty+=500;
             digital_speed=9500;    
             play_vox("allkeys.raw");
             digital_speed=11025;
           }
           cheat_ctr=0;
           new_key=0; 
           break;
         case 'J':
           calibrate_stick();
           new_key=0;
           e++;
           break;
         case 27:  
           if(ck)
           { 
             menu1_unload();
             menu_mode=0;
             memcpy(vga_ram,double_buffer_l,64000);
             new_key=0;
             return(0);
           }
           break;
         case 13:  
           switch(curr)
           {
             case 131:
               menu1_unload();
               b=difflvl();
               if(!b) 
               {
                 menu1_load();
                 e++;
               }
               else
               {
                 //menu1_unload();
                 menu_mode=0;
                 level_num=1;
                 power_level=1024;
                 shield_level=256;
                 old_level_num=-1;
                 memcpy(vga_ram,double_buffer_l,64000);
                 diff_level_set=b;
                 curr_weapon=-1;
                 for(b=0;b<8;b++) weapon_list[b].qty=0;
                 rings=0;
                               //   123456789012345678901234567890
                 strcpy(access_buf,"AB C  D  E F  G H I J K L  M  ");
                                  //AB C  D  E F  G H I J K L  M
                 return(1);
               }
               break;
             case 132:
               menu1_unload();
               b = save_load(1);
               if(b)
               {
                 menu_mode=0;
                 old_level_num=-1;
                 memcpy(vga_ram,double_buffer_l,64000);
                 rings=0;
                 return(1);

               }
               else menu1_load();
               e++;
               break;
             case 133:
               if(ck)
               {
                 menu1_unload();
                 save_load(0);
                 menu1_load();
                 e++;
               }
               break;
             case 134:
               menu2();
               e++;
               break;
             case 135:
               menu1_unload();
               read_me();
               menu1_load();
               e++;
               new_key=0; 
               break;
             case 136:
               menu1_unload();
               how_to_order();
               menu1_load();
               e++;
               new_key=0; 
               break;
             case 137:
               mn1_flap=0;
               credits();
               e++;
               new_key=0; 
               break;
             case 138:
               if(ck)
               {
                 menu1_unload();
                 menu_mode=0;
                 memcpy(vga_ram,double_buffer_l,64000);
                 return(2);
               }
               mn1_flap=1;
               menu1_unload();
               menu_mode=0;
               
               demo_mode=1; 
               demo_ctr=0;
               menu_mode=0;
               
               power_level=1024;
               shield_level=256;
               old_level_num=-1;
               diff_level_set=2;
               curr_weapon=0;
               weapon_list[0].qty=26;
               weapon_list[1].qty=11;
               weapon_list[3].qty=14;
               for(b=0;b<23;b++) demo[b].stat=1;

               rings=0;
                             //   123456789012345678901234567890
               //strcpy(access_buf,"AB C  D  E F  G H I J K L  M  ");
               strcpy(access_buf,"                              ");
                                //AB C  D  E F  G H I J K L  M
               //list_levels();            
               
               level_num=4;

               memcpy(vga_ram,double_buffer_l,64000);
               rings=0;
               return(1);
               break;
             case 139:
               menu1_unload();
               menu_mode=0;
               memcpy(vga_ram,double_buffer_l,64000);
               return(2);
               break;
           }
           break;
         case 8:
           curr++; 
           if(curr>139) curr=131;
           if(ck && curr>138) curr=131;
           new_key=0;
           e++;
           break;  
         case 5:
           curr--; 
           if(curr<131) 
           {
             if(!ck) curr=139;
             else curr=138;
           }
           new_key=0;
           e++;
           break;  
       }
       _enable();
       if(!musRunning && music_toggle==2) 
       { 
        if( music_cnt==4) { play_again(); music_cnt--; }
       }
       if(music_toggle==2 && music_cnt<4)
       {
         music_cnt--;
         if(!music_cnt)
         {  
           if(next_song[0])
           {
             play_song( next_song );
             strcpy(curr_song,next_song);
             next_song[0]=0;
           }
           else play_again();
           music_cnt=4;
         }  
      }

    }
  }
  return(0);
}

int difflvl()
{
  int a,b,c,d,e,f,curr;
  b=0; c=0; d=0; curr=132; e=0; f=0;
  digital_speed=9500;
  difflvl_load();
  for(b=0x7f;b>80;b-=10)
  {
    MAX_VOLUME=b;
    Volume_OnOff(1);
    delay(20);
  }
  new_key=0;
  play_vox("dflvl2.raw");

  while(!c)
  {
    memcpy(vga_ram,double_buffer_l,64000);
    for(a=131;a<=134;a++)
    {
      if(a!=curr) PCX_Show_Image(160,40*(a-131)+35,a,picture[a].width);
      else PCX_Show_Image(160,40*(a-131)+35,a,picture[a].width+80);
    }
    e=0;
    while(!e)
    {
       switch( new_key )
       {
         case 27:  
           difflvl_unload();
           new_key=0;
           digital_speed=11025;
           for(b=80;b<0x7f;b+=10)
           {
             MAX_VOLUME=b;
             Volume_OnOff(1);
             delay(20);
           }
           MAX_VOLUME=0x7f;
           Volume_OnOff(1);
           return(0);
           break;
         case 13:  
           for(b=80;b<0x7f;b+=10)
           {
             MAX_VOLUME=b;
             Volume_OnOff(1);
             delay(20);
           }
           MAX_VOLUME=0x7f;
           Volume_OnOff(1);
           digital_speed=11025;
           return(curr-130);
           break;
         case 8:
           curr++; if(curr>134) curr=131;
           switch(curr)
           {
             case 131:
               play_vox("dflvl1.raw");
               break;
             case 132:
               play_vox("dflvl2.raw");
               break;
             case 133:
               play_vox("dflvl3.raw");
               break;
             case 134:
               play_vox("dflvl4.raw");
               break;
           }
           delay(100);
           new_key=0;
           e++;
           break;  
         case 5:
           curr--; if(curr<131) curr=134;
           switch(curr)
           {
             case 131:
               play_vox("dflvl1.raw");
               break;
             case 132:
               play_vox("dflvl2.raw");
               break;
             case 133:
               play_vox("dflvl3.raw");
               break;
             case 134:
               play_vox("dflvl4.raw");
               break;
           }
           new_key=0;
           e++;
           break;  
       }
       _enable();
       if(!musRunning && music_toggle==2) 
       { 
        if( music_cnt==4) { play_again(); music_cnt--; }
       }
       if(music_toggle==2 && music_cnt<4)
       {
         music_cnt--;
         if(!music_cnt)
         {  
           if(next_song[0])
           {
             play_song( next_song );
             strcpy(curr_song,next_song);
             next_song[0]=0;
           }
           else play_again();
           music_cnt=4;
         }  
      }
	   delay(16); // MIKE
    }
  }
  return(0);
}


int cpu_speed()
{
  unsigned int tmb1;
  int a;

  a=0;
  puts("[");
  tmb1=timerval();
  while( tmb1 == timerval());

  tmb1=timerval()+20;
  while( tmb1 > timerval() )
  {
    a++;
  }
  puts("]");
  return(a);
}

int save_load(int which)  //0=save 1=load
{
  int a,b,c,pick=0,ready=0,clr,e;
  char entry[25];

  PCX_Load("pointer.pcx", 157,1);
  PCX_Load("poster1.pcx", 159,1);
  load_gamefile();
  
  if(!which)
  {
    PCX_Load("savegame.pcx", 158,1);
    clr=249;

  }
  else
  {
    PCX_Load("loadgame.pcx", 158,1);
    clr=6;

  }
  memset(vga_ram,clr,64000);
  
  Display2( 160,  6, 158);
  Display2(  60, 49, 157);
  b=50;
  for(a=0;a<10;a++)
  {
    Display2( 160, b, 159);
    if(!game_data[a].games[0]) Display_Text( 104, b+3, "EMPTY", 255);
    else Display_Text( 104, b+3, game_data[a].games, 255);
    b+=14;
  }
  c=0;
  new_key=0;  
  while(!c)
  {
    if(new_key==27) 
    {
      if(!ready) c++;
      else
      {
        ready=0;
        Display2( 160, pick*14+50, 159);
        if(!game_data[pick].games[0]) Display_Text( 104, pick*14+53, "EMPTY", 255);
        else Display_Text( 104, pick*14+53, game_data[pick].games, 255);
        delay(250);
        new_key=0;
      }
    }
    else if(new_key==13) 
    {
      if(!which)
      {
        if(!ready )
        {
          e=0; 
          ready++;
          Display2( 160, pick*14+50, 159);
          Display_Text( 104, pick*14+53, "@", 255);
          entry[0]=0;
        }
        else
        {
          
          ready=0;
          //Save game call
          if(entry[0]) 
          {
            strcpy(game_data[pick].games,entry);
            save_game(pick);
          }
          Display2( 160, pick*14+50, 159);
          Display_Text( 104, pick*14+53, entry, 255);
        }
      }
      else
      {
        level_num = game_data[pick].level;
        old_level_num= -1;
        diff_level_set= game_data[pick].diff;
        score= game_data[pick].score;
        power_level= game_data[pick].power;
        shield_level= game_data[pick].shields;
        weapon_list[0].qty= game_data[pick].protons;
        weapon_list[1].qty=game_data[pick].neutrons;
        weapon_list[3].qty=game_data[pick].darts;
        strcpy(access_buf,game_data[pick].access);
        for(a=0;a<30;a++) access_buf[a]=~access_buf[a];
        if(access_buf[1]==' ') curr_weapon=0;
        new_key=0;
        return(1);
      }
      delay(250);
      new_key=0;

    }
    else if(!ready && new_key==5)
    {
      pick--;
      if(pick<0) pick=9;
      memset(vga_ram,clr,64000);
      Display2( 160,  6, 158);
      b=50;
      for(a=0;a<10;a++)
      {
        Display2( 160, b, 159);
        if(!game_data[a].games[0]) Display_Text( 104, b+3, "EMPTY", 255);
        else Display_Text( 104, b+3, game_data[a].games, 255);
        b+=14;
      }
      Display2(  60, pick*14+49, 157);
      delay(150);
      new_key=0;
    }
    else if(!ready && new_key==8)
    {
      pick++;
      if(pick>9) pick=0;
      memset(vga_ram,clr,64000);
      Display2( 160,  6, 158);
      b=50;
      for(a=0;a<10;a++)
      {
        Display2( 160, b, 159);
        if(!game_data[a].games[0]) Display_Text( 104, b+3, "EMPTY", 255);
        else Display_Text( 104, b+3, game_data[a].games, 255);
        b+=14;
      }
      Display2(  60, pick*14+49, 157);
      delay(150);
      new_key=0;
    } 
    else if(ready && new_key==14 && entry[0]>='0') //BackSpace
    {
      for(a=0;a<14;a++)
      {
        if(!entry[a])
        {
          entry[a-1]=0;
          break;
        }
      }
      e=13;
      Display2( 160, pick*14+50, 159);
      Display_Text( 104, pick*14+53, entry, 255);
      Display_Text( a*8+96, pick*14+53, "@", 255);
      delay(150);
      new_key=0;
    } 
    else if(ready && new_key>='0')
    {
      for(a=0;a<14;a++)
      {
        if(!entry[a])
        {
          entry[a]=new_key;
          entry[a+1]=0;
          break;
        }
      }
      e=13;
      Display2( 160, pick*14+50, 159);
      Display_Text( 104, pick*14+53, entry, 255);
      Display_Text( a*8+112, pick*14+53, "@", 255);
      delay(100);
      new_key=0;
    }

       _enable();
       if(!musRunning && music_toggle==2) 
       { 
        if( music_cnt==4) { play_again(); music_cnt--; }
       }
       if(music_toggle==2 && music_cnt<4)
       {
         music_cnt--;
         if(!music_cnt)
         {  
           if(next_song[0])
           {
             play_song( next_song );
             strcpy(curr_song,next_song);
             next_song[0]=0;
           }
           else play_again();
           music_cnt=4;
         }  
      }
  }
  PCX_Unload(158);
  PCX_Unload(159);
  PCX_Unload(157);
  new_key=0;  
  return(0);
}  




void opening_screen()
{
  int a,b;
  //play_vox();
  set_vmode( 0x13 );
  for(a=0;a<256;a++) { red[a]=0; green[a]=0; blue[a]=0; }
  Set_Palette();
  PCX_Load("sky1.pcx", 4,1);
  PCX_Load("intro1.pcx", 5,1);
  if(music_toggle==2)
  {
    play_song("intro1.mdi" );
    strcpy(next_song, "rmh2001.mdi");
    strcpy(curr_song, next_song);
    raw_key=0;
  }
  memcpy(vga_ram, picture[4].image,63360);
  Set_Palette();
  b=1;
  for(a=25; a<1000; a+=b)
  {
    _enable();
    PCX_Show_Image(160,100,5,a);
    delay(10);
    if(a>320) b+=3;
    if( raw_key ) goto OS_Jump;
    //if( kbhit() ) { getch(); goto OS_Jump;}
  }
  memcpy(vga_ram, picture[4].image,63360);
  //memset(vga_ram,0,64000);
  PCX_Unload(5);
  PCX_Load("intro2.pcx", 5,1);
  b=1;
  for(a=25; a<1000; a+=b)
  {
    _enable();
    PCX_Show_Image(160,100,5,a);
    delay(10);    
    if(a>320) b+=3;
    //if( kbhit() ) { getch(); goto OS_Jump;}
    if( raw_key ) goto OS_Jump;
  
  }
  //memset(vga_ram,0,64000);
OS_Jump:
  _enable();
  memcpy(vga_ram, picture[4].image,63360);
  PCX_Unload(5);
  PCX_Load("intro3.pcx", 5,1);
  b=1;
  for(a=25; a<320; a+=b) 
  {
    _enable();
    PCX_Show_Image(160,100,5,a);
  }
  memcpy(double_buffer_l, vga_ram, 63360);  //Store in buffer for menu
  _enable();
  if(!raw_key) delay(3000);
  raw_key=0;
  PCX_Unload(5);
  PCX_Unload(4);
  delay(12);
  //Timer(12);      
  _enable();
  Timer(12);      
  _enable();
  Timer(12);      
  _enable();
}  


void starter_lights()
{
    char tb1[80], tb2[20];        
    int a,b=0,d;

    // Display Info
    if(!demo_mode)
    {
      strcpy(tb1,"LEVEL ");
      itoa(level_num,tb2,10);
      strcat(tb1,tb2);
      Display_Text(  9, 9,tb1,10); //Shadow
      Display_Text( 10,10,tb1,253);
    }
    else Shadow_Text( 124,10,"DEMO MODE",253,10);
    strcpy(tb1," GATE KEYS REQUIRED >");
    itoa(rings_req,tb2,10);
    strcat(tb1,tb2);
    Display_Text(  9,29,tb1,10);
    Display_Text( 10,30,tb1,7);
    strcpy(tb1,"GATE KEYS AVAILABLE >");
    itoa(rings_avail,tb2,10);
    strcat(tb1,tb2);
    Display_Text(  9,43,tb1,10);
    Display_Text( 10,44,tb1,249);
    
    Display_Text(  9,59,"CYCLE OPTIONS AVAILABLE:",10);
    Display_Text( 10,60,"CYCLE OPTIONS AVAILABLE:",251);
    eq_flag=-1;
    for(a=0; a<level_num; a++) //30
    {
      if(access_buf[a]>='A' && access_buf[a]<='Z') 
      {
        d = access_buf[a]-'A';
        b++; 
        Display_Text( 29,71, equipment[d].item,10);
        Display_Text( 30,72, equipment[d].item,250);
        eq_flag = d;
        eq_spot = a;
        eq_noi=eq_image_cnt[d];
        eq_gotit=1;
        break;
      }
    }
    if(!b)
    {
      Display_Text( 29,71,"NONE ON THIS LEVEL",10);
      Display_Text( 30,72,"NONE ON THIS LEVEL",250);
    }
    
    /*splay_Text(  9,89,"SUPPLIES AVAILABLE:",10);
    Display_Text( 10,90,"SUPPLIES AVAILABLE:",255);

    if(!c)
    {
      Display_Text( 29,101,"NONE ON THIS LEVEL",10);
      Display_Text( 30,102,"NONE ON THIS LEVEL",250);
    } */

    if(level_num==old_level_num || demo_mode) newdelay(500);
    else newdelay(3500);
    old_level_num=level_num;
    
    PCX_Copy_Image(296,10, 67);
    PCX_Copy_Image(296,24, 67);
    PCX_Copy_Image(296,38, 67);
    PCX_Copy_Image(296,52, 67);
    _enable();
    Timer(8);
    digital_speed=11025;
    play_vox("start1.raw");
    PCX_Copy_Image(296,52, 68);
    _enable();
    Timer(8);
    digital_speed=12025;
    play_vox("start1.raw");
    PCX_Copy_Image(296,38, 68);
    _enable();
    Timer(8);
    digital_speed=13025;
    play_vox("start1.raw");
    PCX_Copy_Image(296,24, 68);
    _enable();
    Timer(8);
    digital_speed=14025;
    play_vox("start1.raw");
    PCX_Copy_Image(296,10, 68);
    digital_speed=11025;
    _enable();
}

void game_setup()
{
  
  if(hyper_boot) return;

  p_w_s = ( 1 / 320 ) * 65536;
  prm_width_sh = 320 * 65536;

  Allocate_World();
  // build all the lookup tables
  Build_Tables();
  Bld_Ang();
  hyper_boot++;
}


void find_ammo()
{
  curr_weapon=0;
  if(!weapon_list[0].qty)
  {
    if(weapon_list[1].qty>0) curr_weapon=1;
    else if(access_buf[9]==' ' && power_level>100) curr_weapon = 2;
  }
}  



int speed_test()
{
  unsigned int tm;
  int nos=0,a;
  level_num=1;
  
  load_level_def();
  
  game_mode = level_def.level_type;
    
  for(a=0; a<WORLD_COLUMNS; a++) memset( ceilmap[a],0,64);
  clear_object_def();
  Load_World(level_def.wall_map_fname, world);
  Load_World(level_def.floor_map_fname, flrmap);
  Texture_Load();
  memset(wall_ht_map, 63, 4098); //Reset Heights at 64
  
  tm=timerval()+1;
  while( tm<timerval() ) _enable();
  tm=tm+18;
  a=1024;
  while( tm>timerval())
  {
    if(controls) stick();
    draw_maze(3200,a, a);
    if(a==1024) a=3072;
    else a=1024;
    nos++;
    delay(1);
  }
  texture_unload();
  memset(double_buffer_l,0,64000);
  return(nos);

}

int speed_adjust()
{
  int a;

  speed_ck_flag=1;
  if(speed_zone!=-1)
  {
    switch(speed_zone)
    {
      case 6:
      case 5:
        floor_resol=0;
        break;
      case 4:
        floor_resol=1;
        break;
      case 3:
        floor_resol=1;
        speed_zone=3;
        adjust1=2;
        low_speed=6;
        hi_speed=34;
        break;
      case 2:
        floor_resol=2;
        speed_zone=2;
        adjust1=2;
        low_speed=6;
        hi_speed=34;
        adjust_turns=512;
        break;
      case 1:  
        floor_resol=2;
        speed_zone=1;
        adjust1=6;
        adjust2=1;
        low_speed=10;
        hi_speed=38;
        adjust_turns=512;
        break;

    }
    return(1);
  }



  floor_resol=1;
  a = speed_test();
  if(debug_flag) printf("Speed index is > %i\n",a);
  
  
  if( a>=43 ) // Little to Fast
  {
    floor_resol=0;
    speed_zone=6;
    system_delay=a-42;
  }

  if( a>=36 && a<=42 ) // Little to Fast
  {
    floor_resol=0;
    speed_zone=5;
  }
  if( a>=27 && a<=35 ) // Normal Range
  {
    floor_resol=1;
    speed_zone=4;
  }
  if( a>=23 && a<=26 ) // Little to Slow
  {
    floor_resol=1;
    speed_zone=3;
    adjust1=2;
    low_speed=6;
    hi_speed=34;
  }
  if( a>=17 && a<=22 ) //Slow
  {
    printf("\nSpeed Index is Low - Lowering Resolution\n");
    floor_resol=2;
    speed_zone=2;
    adjust1=2;
    low_speed=6;
    hi_speed=34;
    adjust_turns=512;
  }
  if( a>=10 && a<=16 ) //Slower
  {
    printf("\nSpeed Index is Low - Lowering Resolution\n");
    floor_resol=2;
    speed_zone=1;
    adjust1=6;
    adjust2=1;
    low_speed=10;
    hi_speed=38;
    adjust_turns=512;
  }
  if( a<9 ) //Tooooo Slow
  {
    printf("\nSorry, Speed Index is too low. You need a faster\n");
    printf("\ncomputer to run HYPERCYCLES(tm).\n");
    return(0);
  }
  if(debug_flag) printf("Zone # %i\n",speed_zone);
  return(1);

}



// MCP1    ///////////////////////////////////////////////////////////////

void mcp1()
{
  unsigned int frm=0,tm1;
  //unsigned int final,tm1,tm2;
  //float a1,a2;
  int xview, yview, ox,oy;
  int a,b,c;
  char t2_buf[50]; //,t3_buf[40];
  // S E C T I O N   1 /////////////////////////////////////////////////////

  int done=0,mainloop=0;            // flags to exit loops & game
  master_control=1;


  game_setup(); // Will only run once at game boot

  // MIKE:???
  //if(!speed_ck_flag) 
  //{
  //  a=speed_adjust();
  //  menu_mode=1;
  //  new_key=0;
  //  if(debug_flag)
  //  {
  //    while(!new_key) _enable();
  //  }
  //  if(!a) 
  //  {
  //    mainloop++;
  //    goto All_Done; 
  //  }
  //}
  new_key=0;
  
  //printf("Opening Screen Suppressed \n \n");      
  opening_screen();
  
  alnum_load(); // Loads text 145
  new_key=0;
  menu_mode=1;
  done=menu1(0);
  if(done==2) { mainloop++; goto All_Done; }
  esc_chk=5;
  
  while( !mainloop )
  {
    done=0;
    rings=0;
    new_key=0;
    texture_unload();
    wallpro_flag=1;
    wallpro_ctr=0;
    left_right=0;  up_down=0; side_mode=0;
    load_level_def();
    xview=level_def.x;
    yview=level_def.y;
    view_angle=level_def.view_angle;
    rings_req=level_def.rings_req;
    
    for(a=0; a<WORLD_COLUMNS; a++) memset( ceilmap[a],0,64);
    clear_object_def();
    load_object_def();

    game_mode = level_def.level_type;
  
    Load_World(level_def.wall_map_fname, world);
    Load_World(level_def.floor_map_fname, flrmap);
    Texture_Load();
    memset(wall_ht_map, 63, 4098); //Reset Heights at 64
    //wall_ht_map[(25<<6)+9] = 128 + 32+ 64;

    eq_gotit=0;

    // S E C T I O N   3 ////////////////////////////////////////////////////

    // render the initial view
    //set_vmode( 0x13 );
    Set_Palette();
  
    grid_dir=view_angle;
    grid_curspeed=8+adjust1;
    grid_setspeed=8+adjust1;
  
    // Load Cycles ///////////////////////
    cycle_load();
    rings_load();
    missile_load();    
    stalkers_load();    
    tanks_load();    
    keystat_load();
    saucer_load();
    carriers_load();
    cycle_options();

    // Now change to song for level (if necessary)
    if( music_toggle==2 && strcmp(curr_song,level_def.music_fname) ) 
    {
      // different so stop current
      Stop_Melo();
      strcpy(next_song, level_def.music_fname);
    }
    
    // render the view
    Draw_Ground();
    draw_maze(xview, yview, view_angle);
    Render_Objects( xview, yview);
    memcpy(vga_ram,double_buffer_l,prm_copy1);
    
    // Reset Disp Text Buffers
    for(dt_ctr=0;dt_ctr<5; dt_ctr++) dt_buf[dt_ctr][0]=0;
    dt_sctr=-1;

    // Do Doctor
    doctor();
    
    starter_lights();
    tx=-1; ty=-1;

    if(power_level<512) power_level=512;
    shield_level=256;
    if(access_buf[11]==' ') shield_level=512;
    dead=0;
    // main event loop, wait for user to press ESC to quit
    Level_Time=timerval();
    tm1=timerval();
    new_key;
    while(!done)
    {
      _enable();
      if(music_toggle==2 && !musRunning)
      {     
        if(music_cnt==4) { play_again(); music_cnt--; }
      
      }
      if(music_toggle==2 && music_cnt<4)
      {
        music_cnt--;
        if(!music_cnt)
        {  
          if(next_song[0])
          {
            play_song( next_song );
            strcpy(curr_song,next_song);
            next_song[0]=0;
          }
          else play_again();
          music_cnt=4;
        }  
      }

      if(wallpro_ctr>0) wallpro_ctr--;
      if(music_ctr) music_ctr--;
        
      // Test if menus requested ////////////////////////////////////////////
      if(esc_chk) esc_chk--;
      if (raw_key==1 && !esc_chk && !demo_mode) 
      {                   //ESC means menus
        menu_mode=1;
        done=menu1(1);
        if(done>0) 
        { 
          if(done==2) mainloop++;
          goto All_Done; 
        }
        // Now change to song for level (if necessary)
        if( music_toggle==2 && strcmp(curr_song,level_def.music_fname) ) 
        {
          // different so stop current
          Stop_Melo();
          strcpy(next_song, level_def.music_fname);
        }
        esc_chk=5;
      }
      
      if(controls==2) stick_funcs();
      if(new_key)    
      {
        switch(new_key)
        {
          case 91:
            calibrate_stick();
            break;
          case 56:
            digital_speed=9500;    
            play_vox("snd1.raw");
            digital_speed=11025;
            break;        
          case 74: //Cheat
            for(a=0;a<30;a++)
            {
              if(access_buf[a]>='A') 
              {
                if(access_buf[a]=='C') {curr_weapon=0; weapon_list[0].qty=50;}
                if(access_buf[a]=='F') shield_level=512;
                if(access_buf[a]=='L') shield_level=1024;
                b=access_buf[a]-'A';
                strcpy( t2_buf, equipment[b].item);
                strcat( t2_buf, " INSTALLED");
                dt_add( t2_buf);
                access_buf[a]=' ';
                digital_speed=9500;    
                play_vox("allkeys.raw");
                digital_speed=11025;
                eq_gotit=0;
                break;
              }
            }
            break;        
          case 14:
            switch(radar_unit)
            {
              case 0:
                radar_unit++;
                break;
              case 1:
                if(access_buf[6]==' ') radar_unit=2;
                else radar_unit=0;
                break;
              case 2:
                if(access_buf[18]==' ') radar_unit=3;
                else radar_unit=0;
                break;
              case 3:
                radar_unit=0;
                break;
            }
            break;
          case 15:
            death_spin=1;
            break;
          case 57:
            if(prm_window_bottom>130) 
            {
              memset(double_buffer_l,0,64000);
              //prm_top+=20;
              prm_window_bottom-= 20;
              prm_window_height-= 20;
            }
            break;
          case 58:
            if(prm_window_bottom<199) 
            {
              memset(double_buffer_l,0,64000);
              //prm_top-=20;
              prm_window_bottom+= 20;
              prm_window_height+= 20;
            }
            break;
          case 59:
            //play_song("rmh5.mdi" );
            play_again();
            break;
          case 68: // Weapons Select
            if(curr_weapon==-1) dt_add("NO OFFENSIVE WEAPONS AVAILABLE" );
            else
            {
              a=curr_weapon+1;
              if(a>7) a=0;
              b = weapon_list[a].eq;
              if(access_buf[b]!=' ') a=0;
              b = weapon_list[a].eq;
              if(access_buf[b]==' ') curr_weapon = a;
            }
            break;
          case 88:
            if(grid_dir == view_angle)
            {
              switch( view_angle )
              {
                case 0:
                  grid_dir = 1024;
                  left_right=2;
                  break;
                case 1024:
                  grid_dir = 2048;
                  left_right=2;
                  break;
                case 2048:
                  grid_dir = 3072;
                  left_right=2;
                  break;
                case 3072:
                  grid_dir = 0;
                  left_right=2;
                  break;
              }
            } 
            break;
          case 89:
            if(grid_dir == view_angle)
            {
              switch( view_angle )
              {
                case 0:
                  grid_dir = 3072;
                  left_right=1;
                  break;
                case 1024:
                  grid_dir = 0;
                  left_right=1;
                  break;
                case 2048:
                  grid_dir = 1024;
                  left_right=1;
                  break;
                case 3072:
                  grid_dir = 2048;
                  left_right=1;
                  break;
              }
            } 
            break;

        }
        new_key=0;
      }  
      else



      // S E C T I O N   4 /////////////////////////////////////////////////
      // is player moving due to input 

      if(power_level<100)
      {
        low_power_flag=grid_curspeed;
        grid_curspeed = low_speed+2;
      }
      else
      {
        if(low_power_flag)
        {
          grid_curspeed+=2;
          if(grid_curspeed >= low_power_flag) low_power_flag=0;
        }
      }
      
      if( !death_spin)
      {
        if(up_down || left_right)
        {    
          if(up_down==1)
          {
             if(grid_curspeed < hi_speed && !low_power_flag) 
             { 
               if(access_buf[0]==' ') grid_curspeed +=2; 
               up_down=0; 
             }
               
          } // end if up
          if(up_down==2)
          {
             if(grid_curspeed > low_speed && !low_power_flag) 
             { 
               if(access_buf[0]==' ') grid_curspeed -=2; 
               up_down=0; 
             }
          } // end if down
          
          if(left_right==2)
          {
            if(!side_mode)
            {
                //if((xview & 63)==32 && (yview & 63)==32 )
                xview = (xview & 0xffffffc0) + 32;
                yview = (yview & 0xffffffc0) + 32;
                if(!CTV_voice_status) { play_vox("turns.raw"); demo_command=2;}
                
                // rotate player right
                view_angle+=adjust_turns;      
                if (view_angle >= NUMBER_OF_DEGREES) view_angle-=NUMBER_OF_DEGREES;
                if(view_angle==grid_dir) left_right=0;
              
            }  
            else
            {
              // Ctrl down rotate player head right
              demo_command=2;
              front_view_angle+=128;      
              if (front_view_angle > NUMBER_OF_DEGREES) front_view_angle-=NUMBER_OF_DEGREES;
            }
          } // end if right
          
          if(left_right==1)
          {
            if(!side_mode)
            {
                //if((xview & 63)==32 && (yview & 63)==32 )
                xview = (xview & 0xffffffc0) + 32;
                yview = (yview & 0xffffffc0) + 32;
                if(!CTV_voice_status) {play_vox("turns.raw");  demo_command=1;}
                // rotate player left
                view_angle-=adjust_turns;      
                if (view_angle <0) view_angle+=NUMBER_OF_DEGREES;
                if(view_angle==grid_dir) left_right=0;
              
            }
            else
            {
              // Ctrl Down rotate player head to left
              demo_command=1;
              front_view_angle-=128;      
              if (front_view_angle <0) front_view_angle+=NUMBER_OF_DEGREES;
            }
          } // end if left
        }  
      }
      else
      {
        wallpro_flag=1;
        if(!CTV_voice_status) play_vox("dspin.raw");
        side_mode=0;
        view_angle+=256;
        if(view_angle>4095) view_angle=0;
        death_spin++;
        if(death_spin>8) 
        {
          death_spin=0;
          view_angle=grid_dir;
          grid_curspeed = low_speed+4;
        }
      }

      // Object Movement ///////////////////////////////////////////////////
      ox = xview;
      oy = yview;
      // Let's constantly move player along view vector foward
      switch(view_angle)
      {
        case 0:
          yview+=grid_curspeed;  
          break;                
        case 1024:
          xview-=grid_curspeed;  
          break;                
        case 2048:
          yview-=grid_curspeed;  
          break;                
        case 3072:
          xview+=grid_curspeed;  
          break;                
      }     
      
      // Player hit a wall? 
      if(world[yview>>6][xview>>6])
      {
        if(world[yview>>6][xview>>6]!='w')
        {
          done=hit_shields(190);
          if(!done)
          {
            if(!death_spin)
            {  
              xview=ox;
              yview=oy;
              grid_curspeed=0;
              side_mode=0;
              view_angle= (grid_dir+2048) & 4095;
              grid_dir= view_angle;
              left_right=0;
              death_spin=1;
            }
          }
          else
          {
            a=random(1,5);
            switch(a)
            { 
              case 1:
              case 2:
                play_vox("nooo.raw");
                break;
              case 3:
              case 4:
                play_vox("scream.raw");
                break;
              case 5:
                digital_speed=9500;
                play_vox("laugh.raw");
                digital_speed=11025;
                break;
            }
            level_score-=1250; if(level_score<0) level_score=0;
            Fade_Pal();
            memset(vga_ram,0,64000);
            memset(double_buffer_l,0,64000);
            delay(500);
            grid_curspeed=0;
            if(eq_gotit) access_buf[eq_spot]='A'+eq_flag; //Put Equip Back
            if(eq_spot==1) curr_weapon=-1;
            goto All_Done;
          }
        }
        else
        {
          if(access_buf[22]!=' ') // Is Shifter Installed?
          {
            if(xp!=xview>>6 || yp!=yview>>6 )
            {
              done=hit_shields(25);
              if(!done)
              {
                  xview=ox;
                  yview=oy;
                  grid_curspeed=8;
                  side_mode=0;
                  view_angle= (grid_dir+2048) & 4095;
                  grid_dir= view_angle;
                  left_right=0;
              }
              else
              {
                a=random(1,5);
                switch(a)
                { 
                  case 1:
                  case 2:
                    play_vox("nooo.raw");
                    break;
                  case 3:
                  case 4:
                    play_vox("scream.raw");
                    break;
                  case 5:
                    play_vox("laugh.raw");
                    break;
                }

                level_score-=1250; if(level_score<0) level_score=0;
                Fade_Pal();
                memset(vga_ram,0,64000);
                memset(double_buffer_l,0,64000);
                delay(500);
                done=1;
                grid_curspeed=0;
                if(eq_gotit) access_buf[eq_spot]='A'+eq_flag; //Put Equip Back
                goto All_Done;
              }
            }
          }
        }
      }
      
      a=xview>>6; 
      b=yview>>6;
      c=flrmap[b][a];      
      if(c>='J' && c<='L')
      {
        done=hit_shields(6);
        if(!CTV_voice_status) play_vox("sizzle.raw");
        if(done)
        {
          play_vox("scream.raw");
          level_score-=1250; if(level_score<0) level_score=0;
          Fade_Pal();
          memset(vga_ram,0,64000);
          memset(double_buffer_l,0,64000);
          delay(200);
          if(!CTV_voice_status) play_vox("laugh.raw");
          done=1;
          grid_curspeed=0;
          if(eq_gotit) access_buf[eq_spot]='A'+eq_flag; //Put Equip Back
          goto All_Done;
        }
      }

      if(gunfire==3)
      {
        xp=Find_Open_Object();
        if(xp>=0) //Skip if over 150 objects
        {
          switch(curr_weapon)
          {
            case 0:
              if(weapon_list[0].qty)
              {
                play_vox("firemiss.raw");
                object[xp].status=1;
                object[xp].objtype=83;
                object[xp].image_num=115;
                weapon_list[0].qty--;
                if(!weapon_list[0].qty) find_ammo();
              }
              break;
            case 1:
              if(weapon_list[1].qty)
              {
                play_vox("firemiss.raw");
                object[xp].status=1;
                object[xp].objtype=84;
                object[xp].image_num=114;
                weapon_list[1].qty--;
                if(!weapon_list[1].qty) find_ammo();
              }
              break;
            case 2:
              if(power_level>100)
              {
                play_vox("firelas.raw");
                object[xp].status=1;
                object[xp].objtype=85;
                object[xp].image_num=113;
                power_level--;
                if(power_level<=100) find_ammo();
              }
              break;
            case 3: // darts
              if(weapon_list[3].qty)
              {
                play_vox("firemiss.raw");
                object[xp].status=1;
                object[xp].objtype=95;
                object[xp].image_num=112;
                object[xp].actx=1;
                weapon_list[3].qty--;
                if(!weapon_list[3].qty) find_ammo();
              }
              break;
            case 4:
              if(power_level>100)
              {
                play_vox("firelas.raw");
                object[xp].status=1;
                object[xp].objtype=152;
                object[xp].image_num=113;
                power_level-=2;
                if(power_level<=100) find_ammo();
              }
              break;
            case 5: //REAR PHOTON
              if(weapon_list[0].qty)
              {
                play_vox("firemiss.raw");
                object[xp].status=1;
                object[xp].objtype=77;
                object[xp].image_num=115;
                weapon_list[0].qty--;
                if(!weapon_list[0].qty) find_ammo();
              }
              break;
            case 6:  //REAR NEUTRON
              if(weapon_list[1].qty)
              {
                play_vox("firemiss.raw");
                object[xp].status=1;
                object[xp].objtype=78;
                object[xp].image_num=114;
                weapon_list[1].qty--;
                if(!weapon_list[1].qty) find_ammo();
              }
              break;
            case 7: //5gw
              if(power_level>100)
              {
                play_vox("firelas.raw");
                object[xp].status=1;
                object[xp].objtype=153;
                object[xp].image_num=179;
                power_level-=10;
                if(power_level<=100) find_ammo();
              }
              break;

          }
          if(!side_mode) object[xp].view_angle=view_angle;
          else object[xp].view_angle=front_view_angle;
          object[xp].x = xview;
          object[xp].y = yview;
          object[xp].opt1=0;
          object[xp].opt2=0;
          object[xp].opt3=0;
          object[xp].opt4=0;
        }
      }
      
      if(gunfire) gunfire--;

      // Now lets move enemy riders and other objects //////////////////////
      if(xp!=xview>>6 || yp!=yview>>6) 
      {
        shieldit(2);
        power_level--;
        if(power_level<1) power_level=1;

        if(demo_mode>0)
        {
          if(demo_mode==5)
          {          
            Stop_Melo();
            delay(200);
            play_again();
            level_num=1;
            done=1;
            goto All_Done;
          }
          if(demo[demo_ctr].stat==1)
          {
            if(demo[demo_ctr].xc==xview>>6 && demo[demo_ctr].yc==yview>>6)
            {
              //Ok give command
              grid_curspeed=demo[demo_ctr].speed+adjust1;
              side_mode=demo[demo_ctr].side_mode;
              curr_weapon=demo[demo_ctr].curr_weapon;
              if(side_mode)
              {
                if(demo[demo_ctr].command==3) gunfire=3;
                else front_view_angle=demo[demo_ctr].command;
              }
              else
              {
                switch(demo[demo_ctr].command)
                {
                  case 99:
                    Stop_Melo();
                    delay(200);
                    play_again();
                    level_num=1;
                    done=1;
                    goto All_Done;
                    break;
                  case 1:
                    new_key=89;
                    break;
                  case 2:
                    new_key=88;
                    break;
                  case 3:
                    gunfire=3;
                    break;
                }
              }

              demo[demo_ctr].stat=0;
              demo_ctr++;
            }
          }
        }

      }
      xp = xview >> 6;    yp = yview >> 6;
      xv = xview;         yv = yview;

      // if wall projector is activated
      if( wallpro_flag==2)
      {
        if(tx!=xp || ty!=yp)
        {
            world[yp][xp] = 'w';
            wall_ht_map[ (yp<<6) + xp] = 196; // Wall moving up
            //ceilmap[yp][xp] = 121; // New ceil map
            tx=xp;  ty=yp;    
        }
      }

      // Check if destination exit sqr's
      if( xp == level_def.exit_xmaze_sq && yp == level_def.exit_ymaze_sq )
        {  
          // Okay we have successfully finished the level
          //Fade_Pal();

          Stats();
          memset(vga_ram,0,64000);
          memset(double_buffer_l,0,64000);
          delay(500);
          done=1;
          //if(level_num < total_level_def) 
          level_num++;
          goto All_Done;
        }  
        
      move_objects();
      if(dead)
      {
          a=random(1,5);
          switch(a)
          { 
              case 1:
              case 2:
                play_vox("nooo.raw");
                break;
              case 3:
              case 4:
                play_vox("scream.raw");
                break;
              case 5:
                digital_speed=9500;
                play_vox("laugh.raw");
                digital_speed=11025;
                break;
          }
          Fade_Pal();
          memset(vga_ram,0,64000);
          memset(double_buffer_l,0,64000);
          delay(500);
          done=1;
          if(eq_gotit) access_buf[eq_spot]='A'+eq_flag; //Put Equip Back
          goto All_Done;
      }  
    

      // S E C T I O N   7 /////////////////////////////////////////////////

      // clear the double buffer and render the ground and ceiling
      Draw_Ground();

      // render the view
      if(!side_mode) draw_maze(xview, yview, view_angle);
      else draw_maze(xview, yview, front_view_angle);

    
      // render objects
      Render_Objects( xview, yview);
                
      for(a=0;a<4096;a++)                
      {
        if(wall_ht_map[a] & 128)
        {
          if(wall_ht_map[a] & 64) // Wall rising
          {
            wall_ht_map[a]+=2; 
            if(!wall_ht_map[a]) wall_ht_map[a] = 63; //Overflow > 255?
          }
          else
          {  
            wall_ht_map[a]--; 
            if(wall_ht_map[a] <= 128) 
            {
              wall_ht_map[a] = 63;
              world[a>>6][a&63] = 0;
            }
          }
        }
      }
      
      Display( 12, 182, 160+rings);
      dt_display();
      weapon_display();
      cycle_status_display();
      if(radar_unit) radar_display(grid_dir,xview, yview);

      if(demo_mode) { psi=1; Shadow_Text( 124,10,"DEMO MODE",253,10); psi=0;}
      
      // show the double buffer
      memcpy(vga_ram,double_buffer_l,prm_copy1);
      frm++;
      
      
      if(pause)
      {
       delay(100);
       new_key=0;
       Shadow_Text( 116, 95,"GAME PAUSED",255,12);
       while(!new_key)
       {
         _enable();
         cont_music();
       }
       pause=0;
      }

      if(system_delay) delay(system_delay);
	  delay(16); // MIKE

All_Done: ;   
      
      // For shareware version only 5 instead of 30
      if(done && (level_num>30 || demo_mode)) 
      {
        demo_mode=0;
        level_num=1;
        master_control=0;
        mainloop++;
        memset(double_buffer_l,0,64000);
        memset(vga_ram,0,64000);
      }


    } //end !done while

    if(demo_mode==2)
    {
      demo_mode=0;
      level_num=1;
    }
  } // end !mainloop while
  
  // restore original mode
  if(master_control) set_vmode( 3 );

} 


int cmd_line()
{
  unsigned char cl[260],ch;
  int a,b=0;
  const char* src = "";// getcmd(cl);
  strcpy(cl, src);
  a=0;
  while(cl[a])
  {
    if(cl[a]=='/')
    {
      switch(cl[a+1])
      {
        case 'D':
        case 'd':
          debug_flag=1;
          memuse();
          printf("Press any key to continue...\n");
          getch();
          break;
        case 'M':
        case 'm':
          b=1;
          break;
        case 'Z':
        case 'z':
          ch = cl[a+2];
          if(ch>='1' && ch<='5')
          {
            speed_zone=(int) ch-'0';
            printf("Speed Index Overridden - Speed Zone set to %i\n",speed_zone);
            delay(300);
            b=1;
          }
          break;

      }
    }
    a++;
  } 
  return(b);
}


// M A I N ///////////////////////////////////////////////////////////////

void main_hyper6(void)
{
  int a;


  // MIKE:
  equip = malloc(0x5C);// pointer to bios equip
  clockr = malloc(0x100); // pointer to internal
  vga_ram = malloc(0xFFFF); // points to vga ram
  vga_ram_c = (unsigned char *)vga_ram; // points to vga ram

  //equip = (unsigned int *) 0x00000410; // pointer to bios equip
  //clockr= (unsigned int *) 0x0000046C; // pointer to internal
  //vga_ram= (unsigned int *) 0x000a0000; // points to vga ram
  //vga_ram_c= (unsigned char *) 0x000a0000; // points to vga ram

  printf("\n   HYPERCYCLES(tm) V.99  Copyright 1995  Aclypse Corporation\n");
  printf("\n   HYPERCYCLES & ACLYPSE are Trademarks of Aclypse Corporation\n");
  printf("\n   ALL RIGHTS RESERVED\n");
  
  if(1)
  {
    /*
    printf("\n This version of HYPERCYCLES is shareware.\n");
    printf(" It may distributed by electronic means (ie. BBS, internet)\n");
    printf(" You are free to give a copies to your friends.\n\n");
    printf(" Please UPLOAD #1HYPER.ZIP to other BBS'S.\n\n");
    */
    
    printf("\n This is the Registered Version of HYPERCYCLES.\n");
    printf(" It may NOT used by anyone besides the registered user.\n");

    printf(" No one is allowed to charge money for playing or\n");
    printf(" distribution of HYPERCYCLES(tm) without expressed\n");
    printf(" written permission of Aclypse Corporation.  (See VENDORS.TXT)\n");
    printf("\n A 486 66MHz or faster PC is recommended for HYPERCYCLES.\n");
    printf(  " A 486 33MHz or faster PC is required for HYPERCYCLES.\n");
  }


  a=cmd_line();
  if(!a)
  {
    a=mem_ok();
    if(!a)
    {
      printf("\n Memory Status Report shows you do not have enough memory\n");
      printf(" to run Hypercycles properly. See TECH.TXT file for information\n");
      printf(" about freeing more memory.\n");
      printf(" See information about using the /D & /S command line parameters.\n");
      exit(1);
    }
  }
  printf("\n Initializing - Please Wait...\n");
  a = load_config();
  if(!a)
  {
    printf("\n\n*** Please run HCSETUP.EXE to configure system. ***\n");
    exit(0);
  }

#ifdef USE_ADT_FILES
  a = adt1_init();
  if(a)
  {
    printf("\n\n*** ERROR Could not find file HYPER1.ADT ***\n");
    exit(1);
  }
  a = adt2_init();
  if(a)
  {
    printf("\n\n*** ERROR Could not find file HYPER2.ADT ***\n");
    exit(1);
  }
#endif

  srand(timerval());
  Timer(18);      
  for(a=0;a<191;a++) picture[a].image = 0;
  reset_game_data();

  //memuse();
  
  if(digi_flag)
  {
       //printf("Init=%i\n",CTV_Init());
       if(debug_flag) printf("Allocating Digital Sound FX Memory\n");
       digibuf = D32DosMemAlloc( 44000 );
  }
  reset_buf();
  //digibuf = (unsigned char *) 0x00010000; // pointer to empty ram

  //for(a=0;a<20;a++) access_buf[a] = 0;
                //   012345678901234567890123456789
  strcpy(access_buf,"AB C  D  E F  G H I J K L  M  ");
  curr_song[0]=0;
  next_song[0]=0;

  //printf("%i\n", cpu_speed());
  //getch();
    
  num_of_records();
  if(!total_level_def) { printf("No levels defined\n"); exit(1);}
  level_num=1;
  
  // initialize the double buffer
  double_buffer_c=malloc(64100);
  if(double_buffer_c==NULL)
  {
    exit(1);
  }
  double_buffer_l = (unsigned int *) double_buffer_c;
  memset(double_buffer_l,0,64000);
  
  // install new isr's
  _disable();
  Old_Key_Isr = _dos_getvect(KEYBOARD_INT);
  _dos_setvect(KEYBOARD_INT, New_Key_Int);
  _enable();
  delay(250);
  master_control=0;
  while(!master_control) mcp1();
  
  if(music_toggle==2) Midi_End();
  // replace old ISR's
  delay(400);
  _disable();
  _dos_setvect(KEYBOARD_INT, Old_Key_Isr);
  _enable();
  
  for(a=0; a<191; a++) PCX_Unload(a); //Unload all gfx
  free_buf();
  
  D32DosMemFree();
 
  Show_Notice();
  _enable();
 
  exit(0);
}


// end main


