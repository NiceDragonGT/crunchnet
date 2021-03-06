/*
crnapidev_b_w.c: CRNAPI Developer DLL main file
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

#define EXP __declspec(dllexport) // Export defenition

// Includes everything from the miner version of crnapi_b.dll
#include <miner/crnapi_b_w/crnapi.h>

/*
Developer functions
These functions are only available through the CrunchNet Node Emulator
*/

// Standard library include
#include <stdio.h>

// Time macros
#define HOUR systime.wHour
#define MIN systime.wMinute
#define SEC systime.wSecond

// Prints to console
EXP nullret _ConsoleLog(ctk tk, crnstr msg, uint16 len) {
	SYSTEMTIME systime; // System time
	GetSystemTime(&systime); // Gets time from system
	WCHAR time[11]; // Time string

	// Converts time to string
	if (HOUR < 10) {
		if (MIN < 10) {
			if (SEC < 10) {
				swprintf(time, 11, L"[0%d:0%d:0%d]", HOUR, MIN, SEC);
			}
			else {
				swprintf(time, 11, L"[0%d:0%d:%d]", HOUR, MIN, SEC);
			}
		}
		else {
			if (SEC < 10) {
				swprintf(time, 11, L"[0%d:%d:0%d]", HOUR, MIN, SEC);
			}
			else {
				swprintf(time, 11, L"[0%d:%d:%d]", HOUR, MIN, SEC);
			}
		}
	}
	else {
		if (MIN < 10) {
			if (SEC < 10) {
				swprintf(time, 11, L"[%d:0%d:0%d]", HOUR, MIN, SEC);
			}
			else {
				swprintf(time, 11, L"[%d:0%d:%d]", HOUR, MIN, SEC);
			}
		}
		else {
			if (SEC < 10) {
				swprintf(time, 11, L"[%d:%d:0%d]", HOUR, MIN, SEC);
			}
			else {
				swprintf(time, 11, L"[%d:%d:%d]", HOUR, MIN, SEC);
			}
		}
	}

	ctkb usefulToken = (ctkb)tk; // Converts input token (void*) into usuable pointer (*_ctk)
	
	// Prints message to console
	WriteConsole(usefulToken->winConHandle, time, 11, &usefulToken->winConCharCount, NULL); // Time
	WriteConsole(usefulToken->winConHandle, msg, len, &usefulToken->winConCharCount, NULL); // Input message
}
