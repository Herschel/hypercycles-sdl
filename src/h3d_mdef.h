// D E F I N E S /////////////////////////////////////////////////////////////

// keyboard stuff

#define KEYBOARD_INT    0x09         // the keyboard interrupt vector
#define KEY_BUFFER      0x60         // keyboard buffer area
#define KEY_CONTROL     0x61         // keyboard control register
#define INT_CONTROL     0x20         // interrupt control register

// make and break codes for the arrow keys

#define MAKE_RIGHT      77
#define MAKE_LEFT       75
#define MAKE_UP         72
#define MAKE_DOWN       80

#define BREAK_RIGHT     205
#define BREAK_LEFT      203
#define BREAK_UP        200
#define BREAK_DOWN      208

// indices into arrow key state table

#define INDEX_UP        0
#define INDEX_DOWN      1
#define INDEX_RIGHT     2
#define INDEX_LEFT      3


// these dataums are used as the records for the demo data

#define DEMO_RIGHT      1
#define DEMO_LEFT       2
#define DEMO_UP         4
#define DEMO_DOWN       8

// these are for the door system

#define DOOR_DEAD            0     // the door is gone
#define DOOR_DYING           1     // the door is phasing
#define PROCESS_DOOR_DESTROY 0     // tells the door engine to continue processing
#define START_DOOR_DESTROY   1     // telsl the door engine to begin


#define MAX_LENGTH_DEMO      2048  // maximum length a demo sequence can be
#define END_OF_DEMO          255   // used in the demo file to flag EOF

#define VGA_INPUT_STATUS_1   0x3DA // vga status reg 1, bit 3 is the vsync
                                   // when 1 - retrace in progress
                                   // when 0 - no retrace

#define VGA_VSYNC_MASK       0x08  // masks off unwanted bits of status reg

// #define DEBUG                1  // used to toggle debugging

#define OVERBOARD              52  // the closest a player can get to a wall

#define INTERSECTION_FOUND      1  // used by ray caster to flag an intersection

#define MAX_SCALE        201  // maximum size and wall "sliver" can be

#define WINDOW_HEIGHT    200   // height of the game view window152

#define WINDOW_MIDDLE    100 //      76  // the center or horizon of the view window

#define VERTICAL_SCALE      13312  // used to scale the "slivers" to get proper
                                   // perspective and aspect ratio

// constants used to represent angles for the ray caster

#define ANGLE_0     0
#define ANGLE_1     5
#define ANGLE_2     10
#define ANGLE_4     20
#define ANGLE_5     25
#define ANGLE_6     30
#define ANGLE_15    80
#define ANGLE_30    160
#define ANGLE_45    240
#define ANGLE_60    320
#define ANGLE_90    480
#define ANGLE_135   720
#define ANGLE_180   960
#define ANGLE_225   1200
#define ANGLE_270   1440
#define ANGLE_315   1680
#define ANGLE_360   1920     // note: the circle has been broken up into 1920
                             // sub-arcs

#define STEP_LENGTH 16       // number of units player moves foward or backward

#define WORLD_ROWS     64     // number of rows in the game world
#define WORLD_COLUMNS  64     // number of columns in the game world

#define CELL_X_SIZE   64     // size of a cell in the gamw world
#define CELL_Y_SIZE   64

#define CELL_X_SIZE_FP   6   // log base 2 of 64 (used for quick division)
#define CELL_Y_SIZE_FP   6

// size of overall game world

#define WORLD_X_SIZE  (WORLD_COLUMNS * CELL_X_SIZE)
#define WORLD_Y_SIZE  (WORLD_ROWS    * CELL_Y_SIZE)

