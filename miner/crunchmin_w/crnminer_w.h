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

// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// Internal CrunchNet include
#include <miner/updcheck_w.h>

// Windows API include
#include <ShlObj.h>

BOOL isMinerRunning = FALSE; // Miner status
BOOL promptStatus = FALSE; // Miner termination prompt status

HANDLE hConsole; // Console output handle
DWORD charCount; // Characters written to console

LPCWSTR szTitle = L"CrunchNet Miner"; // Default window title

STARTUPINFO si = { 0 }; // Startup info structure
PROCESS_INFORMATION pi = { 0 }; // Process info structure

SYSTEM_INFO sysinfo; // System info structure
SYSTEMTIME systime; // System time structure

FILE* fiop; // File IO pointer

// Miner configuration
LARGE_INTEGER accountId, storageAlloc; // Account ID	 and storage space to be allocated (in bytes)
WORD maxProcesses; // CrunchNet applications that can run at once (limit is SYSTEM_INFO.dwNumberOfProcessors)
LPWSTR accountName; // Username
LPWSTR minerName; // Miner name
LPWSTR saveLocation; // Save location

// Time macros
#define HOUR systime.wHour
#define MIN systime.wMinute
#define SEC systime.wSecond

// Gets data from configuration file
void GetConfig() {
	LPBYTE buffer; // File buffer

	// Opens file for reading
	if (_wfopen_s(&fiop, L"config.crn", L"r")) {
		// If unable to open file, show error message box
		MessageBox(NULL, L"Unable to open configuration file", L"CrunchNet Miner", MB_ICONERROR);
	}
	else {
		// Gets length of file
		fseek(fiop, 0, SEEK_END);
		int length = ftell(fiop);
		fseek(fiop, 0, SEEK_SET);
		// Checks if length is 0
		if (length) {
			buffer = malloc(length); // Allocates memory
			// Parses configuration file
			free(buffer); // Frees allocated memory
			fclose(fiop); // Closes file
		}
		else {
			fclose(fiop); // Closes file
			_wfopen_s(&fiop, L"config.crn", L"w"); // Opens file for writing
			// Creates blank configuration
			BYTE blankByte = 0; // Blank byte
			for (BYTE i = 0; i < 0; i++)
				fwrite(&blankByte, 1, 1, fiop); // Writes empty byte to file
		}
	}
}

// Saves data to configuration file
void SaveConfig(HWND username, HWND minerName, HWND allocTxt, HWND allocCB, HWND processTB) {

}

// Prints time to console
void WriteTime(int destination) {
	WCHAR time[11]; // Time string
	GetSystemTime(&systime); // Gets system time
	// Converts time to string
	if (HOUR < 10) if (MIN < 10) if (SEC < 10) swprintf(time, 11, L"[0%d:0%d:0%d]", HOUR, MIN, SEC);
	else swprintf(time, 11, L"[0%d:0%d:%d]", HOUR, MIN, SEC);
	else if (SEC < 10) swprintf(time, 11, L"[0%d:%d:0%d]", HOUR, MIN, SEC);
	else swprintf(time, 11, L"[0%d:%d:%d]", HOUR, MIN, SEC);
	else if (MIN < 10) if (SEC < 10) swprintf(time, 11, L"[%d:0%d:0%d]", HOUR, MIN, SEC);
	else swprintf(time, 11, L"[%d:0%d:%d]", HOUR, MIN, SEC);
	else if (SEC < 10) swprintf(time, 11, L"[%d:%d:0%d]", HOUR, MIN, SEC);
	else swprintf(time, 11, L"[%d:%d:%d]", HOUR, MIN, SEC);

	if (destination == NULL) WriteConsole(hConsole, time, 11, &charCount, NULL); // Prints time string to console
	else fwprintf(fiop, L"%s", time); // Saves time string to log file
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

// Makes 2 errors (one on the console/in log file, one as a message box)
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

// Updates timer every second
DWORD UpdateTimer(HWND window) {
	WCHAR time[9]; // Time string
	// Time units
	BYTE hour = 0;
	BYTE min = 0;
	BYTE sec = 0;
	// Loop until miner has stopped
	while (isMinerRunning) {
		Sleep(1000); // Waits 1 second
		// Appends current time
		sec++; if (sec == 60) {
			sec = 0; min++; if (min == 60) {
				min = 0; hour++;
			}
		}
		// Converts time to string
		if (hour < 10) if (min < 10) if (sec < 10) swprintf(time, 9, L"0%d:0%d:0%d", hour, min, sec);
		else swprintf(time, 9, L"0%d:0%d:%d", hour, min, sec);
		else if (sec < 10) swprintf(time, 9, L"0%d:%d:0%d", hour, min, sec);
		else swprintf(time, 9, L"0%d:%d:%d", hour, min, sec);
		else if (min < 10) if (sec < 10) swprintf(time, 9, L"%d:0%d:0%d", hour, min, sec);
		else swprintf(time, 9, L"%d:0%d:%d", hour, min, sec);
		else if (sec < 10) swprintf(time, 9, L"%d:%d:0%d", hour, min, sec);
		else swprintf(time, 9, L"%d:%d:%d", hour, min, sec);
		SetWindowText(window, time); // Changes text of timer window
	}
	SetWindowText(window, L"00:00:00"); // Resets timer
	return 0;
}

// Miner execution function
DWORD MinerExecution(void* unused) {
	// Miner execution variables
	WORD currentProcesses = 0; // Number of processes currently running

	// Miner execution loop
	while (isMinerRunning) {
		// Checks if miner termination prompt has been sent
		if (!promptStatus) {
			// If not, check if all processes are being ultilized
			if (currentProcesses < maxProcesses) {
				// If not, find application to run
			}
		}
		else {
			// Prints termination message to console
			WriteMessage(L"Miner termination prompt sent. Preparing to terminate miner.", 60, 2);
			while (currentProcesses) break; // Waits for all processes to finish

			/* Notifies CrunchNet that miner has ended */

			isMinerRunning = FALSE; // Sets miner status to false
		}
	}
	return 0;
}

// Starts miner
void StartMiner(HWND hWnd, HWND minerToggle, HWND timer) {
	isMinerRunning = TRUE; // Sets miner status to active
	SetWindowText(hWnd, L"CrunchNet Miner (Active)"); // Changes text of dashboard window
	SetWindowText(minerToggle, L"Stop miner"); // Changes text of miner toggle button
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)UpdateTimer, timer, NULL, NULL); // Starts timer

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

	/* Notifies CrunchNet that miner has started */

	WriteMessage(L"Miner initialization succeeded. Now mining", 42, 2); // Prints initialization success message
	CreateThread(NULL, 0, MinerExecution, NULL, NULL, NULL); // Starts miner thread
}

// Stops miner
void StopMiner(HWND hWnd, HWND minerToggle) {
	promptStatus = FALSE; // Sends miner loop termination prompt
	while (isMinerRunning) break; // Waits for miner to finish
	WriteMessage(L"Miner has stopped.", 22, 2); // Prints miner termination message to console and log file
	ResetMiner(hWnd, minerToggle); // Resets miner
}

#endif

