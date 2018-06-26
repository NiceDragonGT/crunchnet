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

HANDLE hConsole; // Console output handle
DWORD charCount; // Characters written to console

STARTUPINFO si = { 0 }; // Startup info structure
PROCESS_INFORMATION pi = { 0 }; // Process info structure

SYSTEM_INFO sysinfo; // System info structure
SYSTEMTIME systime; // System time structure

WSADATA wsa; // WinSock implementation structure

#endif

