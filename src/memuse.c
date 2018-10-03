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
     /*
         MEMORY.C - This example shows how to get information
         about free memory using DPMI call 0500h under DOS/4GW.
         Note that only the first field of the structure is
         guaranteed to contain a valid value; any field that
         is not returned by DOS/4GW is set to -1 (0FFFFFFFFh).

         Compile & Link: wcl386 /l=dos4g memory
      */
     // MIKE: #include <i86.h>
     #include <dos.h>
     #include <stdio.h>
     #include <string.h>

     #define DPMI_INT        0x31


     struct meminfo {

         unsigned LargestBlockAvail;
         unsigned MaxUnlockedPage;
         unsigned LargestLockablePage;
         unsigned LinAddrSpace;
         unsigned NumFreePagesAvail;
         unsigned NumPhysicalPagesFree;
         unsigned TotalPhysicalPages;
         unsigned FreeLinAddrSpace;
         unsigned SizeOfPageFile;
         unsigned Reserved[3];
     } MemInfo;


     void memuse()
     {
		 // MIKE:
         //union REGS regs;
         //struct SREGS sregs;

         //regs.x.eax = 0x00000500;
         //memset( &sregs, 0, sizeof(sregs) );

         //sregs.es = FP_SEG( &MemInfo );
         //regs.x.edi = FP_OFF( &MemInfo );


         //int386x( DPMI_INT, &regs, &regs, &sregs );
         //printf( "Largest available block (in bytes): %lu\n", MemInfo.LargestBlockAvail );
         //printf( "Maximum unlocked page allocation: %lu\n", MemInfo.MaxUnlockedPage );
         //printf( "Pages that can be allocated and locked: %lu\n", MemInfo.LargestLockablePage );
         //printf( "Total linear address space including allocated pages: %lu\n",MemInfo.LinAddrSpace );
         //printf( "Number of free pages available: %lu\n",MemInfo.NumFreePagesAvail );

         //printf( "Number of physical pages not in use: %lu\n",MemInfo.NumPhysicalPagesFree );
         //printf( "Total physical pages managed by host: %lu\n", MemInfo.TotalPhysicalPages );
         //printf( "Free linear address space (pages): %lu\n",  MemInfo.FreeLinAddrSpace );
         //printf( "Size of paging/file partition (pages): %lu\n", MemInfo.SizeOfPageFile );
     }

     int mem_ok()
     {
         /* MIKE:
		 union REGS regs;
         struct SREGS sregs;

         regs.x.eax = 0x00000500;
         memset( &sregs, 0, sizeof(sregs) );

         sregs.es = FP_SEG( &MemInfo );
         regs.x.edi = FP_OFF( &MemInfo );


         int386x( DPMI_INT, &regs, &regs, &sregs );
         if(MemInfo.NumFreePagesAvail>=275) return(1);
         return(0);*/
		 return 1;
     }
