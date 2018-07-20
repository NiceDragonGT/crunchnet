/*
crnapi_w.c: CRNAPI implementation library main file (Windows)
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

#pragma comment(lib, "kernel32")

#undef UNICODE

#include <Windows.h>
#include <api/include/crunchnet.h>

// Function pointer type macro
#define FPTYPE(type, name, params) typedef type##(*##name)##params

// CrunchNet token type
typedef struct _CTK_T {
	ucnum16 processId;
	ucnum64 appId;
	handle winDll, winCon;
}*TCTK;

// Casts input token to struct pointer
#define TK_CONVERT TCTK tToken = (TCTK)token

/* Functions */

// CrunchInit
CTK CrunchInit(CTK token, FLOC fPath) {
	TK_CONVERT;
	tToken->winDll = LoadLibrary((LPCSTR)fPath[1]); // Loads crnapi_b.dll (or crnapidev_b.dll if run in emulator)
	FPTYPE(CTK, CRUNCHINIT, (CTK, FLOC)); // Creates function pointer type
	CRUNCHINIT func = (CRUNCHINIT)GetProcAddress(tToken->winDll, "impCrunchInit"); // Gets address of function
	return (CTK)(func)(token, fPath); // Calls implementation function
}

// CrunchEnd
NCRNFUNC CrunchEnd(CTK token, FLOC fPath) {
	TK_CONVERT;
	FPTYPE(NCRNFUNC, CRUNCHEND, (CTK, FLOC)); // Creates function pointer type
	CRUNCHEND func = (CRUNCHEND)GetProcAddress(tToken->winDll, "impCrunchEnd"); // Gets address of function
	NCRNFUNC rVal = (func)(token, fPath); // Calls implementation function
	FreeLibrary(tToken->winDll); // Unloads library
	return rVal; // Returns value returned by implementation function
}

// AppLog
nullret AppLog(CTK token, crnstr msg, ucnum16 len) {
	TK_CONVERT;
	FPTYPE(nullret, APPLOG, (CTK, crnstr, ucnum16)); // Creates function pointer type
	APPLOG func = (APPLOG)GetProcAddress(tToken->winDll, "impAppLog"); // Gets address of function
	(func)(token, msg, len); // Calls implementation function
}

// AppErr
nullret AppErr(CTK token, crnstr msg, ucnum16 len) {
	TK_CONVERT;
	FPTYPE(nullret, APPERR, (CTK, crnstr, ucnum16)); // Creates function pointer type
	APPERR func = (APPERR)GetProcAddress(tToken->winDll, "impAppErr"); // Gets address of function
	(func)(token, msg, len); // Calls implementation function
}
