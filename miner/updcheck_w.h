/*
updcheck_w.h: CrunchNet Updater check function (Windows)
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

#ifndef UPDCHECK_W_H
#define UPDCHECK_W_H

#define _WINSOCK_DEPRECATED_NO_WARNINGS

// Standard library include
#include <string.h>

// Windows API include
#include <WinSock2.h> // Includes Windows.h
#include <WinInet.h> // InternetGetConnectedState function

// Sets defenition of NULL to 0
#undef NULL
#define NULL 0

#define FAIL 2 // Defines fail code

// Update check function
BOOL CheckForUpdate(){
	BOOL rVal = FALSE; // Update status (value to be returned)
	
	WSADATA wsa; // WinSock implementation structure
	SOCKET sc; // TCP/IP socket

	// Checks if there is an internet connection
	if (!(InternetGetConnectedState(NULL, NULL))) {
		// Shows warning message box if disconnected
		MessageBox(NULL, L"CrunchNet cannot function without an Internet connection.", L"CrunchNet Updater", MB_ICONERROR);
		return FAIL;
	}

	// Initializes WinSock
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0) {
		// Show error message box if initialization failed
		MessageBox(NULL, L"WinSock initialization failed!", L"CrunchNet Updater", MB_ICONERROR);
		return FAIL;
	}

	HOSTENT* host; // Database entry description for a single host
	IN_ADDR** addrlist; // List of addresses from name server
	char ip[100]; // IP address

	// Gets host IP address
	if ((host = gethostbyname("www.google.com")) == NULL) {
		// Show error message box if gethostbyname failed
		MessageBox(NULL, L"Could not retrieve IP address from DNS server", L"CrunchNet Updater", MB_ICONERROR);
		return FAIL;
	}

	addrlist = (IN_ADDR**)host->h_addr_list; // Casts address list to IN_ADDR
	// Return the first address
	for (int i = 0; addrlist[i] != NULL; i++) {
		strcpy_s(ip, 100, inet_ntoa(*addrlist[i]));
	}

	SOCKADDR_IN addr; // Address to be binded to socket
	int addrlen = sizeof(addr); // Length of address (required for accept call)
	addr.sin_addr.s_addr = inet_addr(ip); // IP address
	addr.sin_port = htons(80); // Port
	addr.sin_family = AF_INET; // IPv4 socket

	// Creates socket
	if ((sc = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
		// Show error message if socket creation failed
		MessageBox(NULL, L"Socket creation failed!", L"CrunchNet Updater", MB_ICONERROR);
		return FAIL;
	}

	// Connects to update server (Updates are distributed via the download website, not the blockchain itself)
	if (connect(sc, (SOCKADDR*)&addr, addrlen) != 0) {
		// Show error message box if connection failed
		MessageBox(NULL, L"Could not connect to update server", L"CrunchNet Updater", MB_ICONERROR);
		return FAIL;
	}

	/* Request code goes here */

	closesocket(sc); // Closes socket
	WSACleanup(); // Unloads WinSock library
	
	return rVal; // Returns update status
}

#endif
