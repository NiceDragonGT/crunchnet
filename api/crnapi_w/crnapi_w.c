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

// Actual CrunchNet back-end application type
typedef struct _CTK { CNUM64 appId; CRNSTR dllPath; }*ATCTK;

// Function macro
#define GETFUNC(impName, capsName, rType, pTypes)\
ATCTK tk = (ATCTK)token;\
HINSTANCE dllHandle = LoadLibrary(tk->dllPath);\
typedef rType##(*##capsName)pTypes;\
capsName func = (capsName)GetProcAddress(dllHandle, impName)

// Standard functions
CHAND MemAlloc(CTK token, CNUM64 size) { GETFUNC("impMemAlloc", MEMALLOC, CHAND, (CTK, CNUM64)); return (func)(token, size); }
// Initialization functions
// Developer log API
// File API
// Wallet API
// Miner API
// Account API
// Application interactivity API
