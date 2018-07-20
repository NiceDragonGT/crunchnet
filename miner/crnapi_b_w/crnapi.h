/*
crnapi.h: CRNAPI function defenitions
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

#define EXP __declspec(dllexport) // Export defenition

// Include API types
#include <api/c-api/crntypes.h>

// Windows API include
#include <WinSock2.h>

// CrunchNet token
typedef struct _ctk {
	uint16 processId; // Process ID
	uint64 appId; // Application ID
	// Windows types
	winhandle winConHandle; // Handle to console
	uint32 winConCharCount; // Character count
}*ctkb;

uint64 execTime; // How long the application took to execute (in milliseconds)

/* Initialization functions */

// CrunchInit subfunction
EXP ctk _subCrunchInit(ctk tk, floc fp) {
	ctkb usefulToken = (ctkb)tk; // Gets memory address of token

	// Stuff with token...

	execTime = GetTickCount64(); // Gets number of ticks elapsed since system started
	return usefulToken; // Returns token
}

// CrunchEnd subfunction
EXP uint64 _subCrunchEnd(ctk tk, floc fp) {
	ctkb usefulToken = (ctkb)tk; // Gets memory address of token

	// Stuff with token...

	return GetTickCount64() - execTime; // Calculates execution time and returns value
}
