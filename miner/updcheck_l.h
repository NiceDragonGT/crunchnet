/*
updcheck_l.h: CrunchNet Updater check function (Linux)
Copyright (c) 2018 Kade Burnside
*/

/*
This file is part of CrunchNet.

CrunchNet is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

CrunchNet is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with CrunchNet. If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef UPDCHECK_L_H
#define UPDCHECK_L_H

// Standard library include
#include <string.h>

// POSIX API includes
#include <unistd.h>
#include <pthread.h>

// Redefines NULL to be of type "int" instead of "void*"
#undef NULL
#define NULL 0

#define FAIL 2 // Defines fail code
typedef int BOOL; // Typedefs BOOL (from Windows API)

// Defines TRUE and FALSE (from Windows API)
#define TRUE 1
#define FALSE 0

// Typedefs LARGE_INTEGER (from Windows API)
typedef union{
    struct{
        unsigned int LowPart;
        int HighPart;
    };
    long long QuadPart;
}LARGE_INTEGER;

// Update check function
BOOL CheckForUpdate(){
	BOOL rVal = FALSE; // Update status (value to be returned)

	// Update check code goes here

	return rVal; // Returns update status
}

#endif

