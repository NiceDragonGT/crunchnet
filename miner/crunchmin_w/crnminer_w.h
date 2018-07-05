/*
crnminer_w.h: CrunchNet Miner console source
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

#ifndef CRNMINER_W_H
#define CRNMINER_W_H

// Standard library include
#include <stdio.h>

// Internal CrunchNet include
#include <miner/updcheck_w.h>

// Windows API include
#include <ShlObj.h>

BOOL isMinerRunning = FALSE; // Miner status

HANDLE hConsole; // Console output handle
DWORD charCount; // Characters written to console

LPCWSTR szTitle = L"CrunchNet Miner"; // Default window title

STARTUPINFO si = { 0 }; // Startup info structure
PROCESS_INFORMATION pi = { 0 }; // Process info structure

SYSTEM_INFO sysinfo; // System info structure
SYSTEMTIME systime; // System time structure

FILE* fiop; // File IO pointer
LPBYTE configBuf; // Configuration file buffer

#define MAX_NAME_LENGTH 64 // Maxmium length for name (username, miner name)

// Miner configuration
LPWSTR accountName, minerName, saveLocation; // Username, miner name and save location
DWORD64 accountId, walletId, storageAlloc; // Account ID, wallet ID and storage space to be allocated (in bytes)
DWORD maxProcesses; // CrunchNet applications that can run at once (limit is SYSTEM_INFO.dwNumberOfProcessors)

// Time macros
#define HOUR systime.wHour
#define MIN systime.wMinute
#define SEC systime.wSecond

// "Cannot edit configuration while miner is active!" error message macro
#define M_ACTIVE_ERR(hWnd) if(isMinerRunning){ MessageBox(hWnd, L"Cannot edit configuration while miner is active!",\
szTitle, MB_ICONERROR | MB_APPLMODAL); }else

// Prints time to console
void WriteTime(int destination) {
	WCHAR time[11]; // Time string
	GetSystemTime(&systime); // Gets system time
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

	if (destination == NULL) {
		WriteConsole(hConsole, time, 11, &charCount, NULL); // Prints time string to console
	}
	else {
		fwprintf(fiop, L"%s", time); // Saves time string to log file
	}
}

// Prints message to console
void WriteMessage(LPCWSTR msg, DWORD cch, int destination) {
	if (destination == NULL) {
		WriteTime(NULL); // Prints time
		WriteConsole(hConsole, msg, cch, &charCount, NULL); // Prints message
		WriteConsole(hConsole, L"\n", 1, &charCount, NULL); // Prints line break
	}
	else if (destination == 1) {
		WriteTime(1); // Saves time to log file
		fwprintf(fiop, L" %s\n", msg); // Saves message to log file
	}
	else {
		WriteTime(NULL); // Prints time
		WriteConsole(hConsole, msg, cch, &charCount, NULL); // Prints message
		WriteConsole(hConsole, L"\n", 1, &charCount, NULL); // Prints line break
		WriteTime(1); // Saves time to log file
		fwprintf(fiop, L" %s\n", msg); // Saves message to log file
	}
}

// Makes 2 errors (one on the console, one as a message box)
void WriteError(HWND hWnd, LPCWSTR msg, DWORD cch) {
	WriteMessage(msg, cch, 2); // Prints error
	MessageBox(hWnd, msg, szTitle, MB_ICONERROR | MB_APPLMODAL); // Shows error message box
}

// Resets miner if error occurs
void ResetMiner(HWND hWnd, HWND minerToggle) {
	isMinerRunning = FALSE; // Sets miner status to inactive
	// Saves end message to log file
	fwprintf(fiop, L"Log ended at %d/%d/%d ", systime.wDay, systime.wMonth, systime.wYear);
	WriteTime(1); // Saves current time to log file
	fwprintf(fiop, L"\n\n"); // Saves line break character to log file
	fclose(fiop); // Closes log file stream
	SetWindowText(hWnd, L"CrunchNet Miner (Inactive)"); // Changes text of dashboard window
	SetWindowText(minerToggle, L"Start miner"); // Changes text of miner toggle button
}

// Starts miner
void StartMiner(HWND hWnd, HWND minerToggle) {
	isMinerRunning = TRUE; // Sets miner status to active
	SetWindowText(hWnd, L"CrunchNet Miner (Active)"); // Changes text of dashboard window
	SetWindowText(minerToggle, L"Stop miner"); // Changes text of miner toggle button

	// Miner initialization
	GetSystemTime(&systime); // Gets current time

	// Opens log file
	if (!(_wfopen_s(&fiop, L"log.txt", L"a"))) {
		// Saves start message to log file
		fwprintf(fiop, L"Log started at %d/%d/%d ", systime.wDay, systime.wMonth, systime.wYear);
		WriteTime(1);
		fwprintf(fiop, L"\n"); // Prints line break character
	}
	else {
		WriteError(hWnd, L"Could not open log file.", 24); // Prints error
		ResetMiner(hWnd, minerToggle);
		return;
	}
	
	WriteMessage(L"Initializing miner...", 21, 2); // Prints initialization message

	// Checks for Internet connection
	if (!(InternetGetConnectedState(NULL, NULL))) {
		WriteError(hWnd, L"There is no Internet connection. Reconnect and try again.", 57); // Prints error
		ResetMiner(hWnd, minerToggle); // Resets miner
		return;
	}
}

// Stops miner
void StopMiner(HWND hWnd, HWND minerToggle) {
	/* Prompt code goes here */

	WriteMessage(L"Miner has stopped.", 22, 2); // Prints miner termination message to console and log file
	ResetMiner(hWnd, minerToggle); // Calls ResetMiner function (avoids code duplication)
}

#endif

