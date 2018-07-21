/*
crnapi_w.c: CRNAPI implementation library source file (Windows)
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

// CrunchNet token type
typedef struct _CTK_T {
	ucnum16 processId;
	ucnum64 appId;
	handle winDll, winCon;
}*TCTK;

// Casts input token to struct pointer
#define TK_CONVERT TCTK tToken = (TCTK)token

// Implementation function call defenition
#define IMPCALL(name, impname, type, params)\
typedef type##(*##name)##params;\
name func = (name)GetProcAddress(tToken->winDll, impname)

/* Functions */

// CrunchInit
nullret CrunchInit(CTK token, FLOC fPath) {
	TK_CONVERT;
	tToken->winDll = LoadLibrary((LPCSTR)fPath[1]); // Loads crnapi_b.dll (or crnapidev_b.dll if run in emulator)
	IMPCALL(CRUNCHINIT, "impCrunchInit", nullret, (CTK, FLOC)); // Gets implementation function
	(func)(token, fPath); // Calls implementation function
}

// CrunchEnd
NCRNFUNC CrunchEnd(CTK token, FLOC fPath) {
	TK_CONVERT;
	IMPCALL(CRUNCHEND, "impCrunchEnd", NCRNFUNC, (CTK, FLOC)); // Gets implementation function
	NCRNFUNC rVal = (func)(token, fPath); // Calls implementation function
	FreeLibrary(tToken->winDll); // Unloads library
	return rVal; // Returns value returned by implementation function
}

// AppLog
nullret AppLog(CTK token, crnstr msg, ucnum16 len) {
	TK_CONVERT;
	IMPCALL(APPLOG, "impAppLog", nullret, (CTK, crnstr, ucnum16)); // Gets implementation function
	(func)(token, msg, len); // Calls implementation function
}

// AppErr
nullret AppErr(CTK token, crnstr msg, ucnum16 len) {
	TK_CONVERT;
	IMPCALL(APPERR, "impAppErr", nullret, (CTK, crnstr, ucnum16)); // Gets implementation function
	(func)(token, msg, len); // Calls implementation function
}

// GetAccountById
cbool GetAccountById(CTK token, ucnum64 id, pcrnacc crnacc) {
	TK_CONVERT;
	IMPCALL(GETACCOUNTBYID, "impGetAccountById", cbool, (CTK, ucnum64, pcrnacc)); // Gets implementation function
	return (func)(token, id, crnacc); // Calls implementation function
}

// GetAccountByUsername
cbool GetAccountByUsername(CTK token, crnstr username, pcrnacc crnacc) {
	TK_CONVERT;
	IMPCALL(GETACCOUNTBYUSERNAME, "impGetAccountByUsername", cbool, (CTK, crnstr, pcrnacc)); // Gets implementation function
	return (func)(token, username, crnacc); // Calls implementation function
}
