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

const JOY_X=1, JOY_Y=2;
const JBUTTON1=0x10, JBUTTON2=0x20;
const LMBUTTON=1, RMBUTTON=2;

extern "C" {
	int readstick(int);				// Read joystick position
	int readjbutton(int);			// Read joystick button
	int	initmouse();					// Initialize mouse driver
	void rempointer();				// Remove mouse pointer from display
  void disppointer();       // Show mouse pointer on display
	int readmbutton();				// Read mouse buttons
	void relpos(int *,int *);	// Get relative mouse position
	int	initkey(char far *buffer); // Initialize keyboard handler
  int getscan();            // Get last scan code
  int remkey();             // Remove keyboard handler
}
