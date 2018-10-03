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
// EVENTMNGR.H
// Written July 29,1993 by Christopher Lampton
//  for GARDENS OF IMAGINATION (Waite Group Press)

// Definitions, data types and prototypes for
// the event management function in module
// EVNTMNGR.CPP

// Key definitions by scan code:

#define   FORWARDKEY 72
#define   BACKKEY    80
#define   LEFTKEY    75
#define   RIGHTKEY   77
#define   QUITKEY     1

// Definitions for requesting events:

const	MOUSE_EVENTS=1,JOYSTICK_EVENTS=2,KEYBOARD_EVENTS=4;

// Structure for passing events to calling program:

struct event_struct {
  int go_forward,go_back,go_left,go_right,quit_game;
};

void init_events();	// Initialize event manager
void end_events();  // Terminate event manager
void setmin();			// Set minimum joystick callibrations
void setmax();			// Set maximum joystick callibrations
void setcenter();		// Set center joystick callibrations
int joystick_present(); // Detect presence of joystick
void getevent(int,event_struct *);	// Get events from selected devices
