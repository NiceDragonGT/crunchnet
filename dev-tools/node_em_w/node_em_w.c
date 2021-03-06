/*
node_em_w.c: CrunchNet Node Emulator main file
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

#pragma comment(lib, "user32")
#pragma comment(lib, "gdi32")
#pragma comment(lib, "shell32")
#pragma comment(lib, "comdlg32")
#pragma comment(lib, "kernel32")

// Standard library includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Windows API includes
#include <Windows.h>
#include <CommCtrl.h>

// Internal CrunchNet includes
#include "resource.h"
#include <api/include/crunchnet.h>

// Redefines null as "int" instead of "void*"
#undef NULL
#define NULL 0

HINSTANCE ghInst; // Global HINSTANCE
HMENU hMenu; // Window menu

typedef NCRNFUNC(*LPCRNFUNC)(CTK, CNUM32, CHAND, CNUM32); // CrunchMain function pointer type
typedef struct { CNUM8 addr[4]; }IPADDR4; // IPv4 address
typedef struct { CNUM16 addr[8]; }IPADDR6; // IPv6 address
// CrunchNet token type
typedef struct _CTK {
	CNUM64 appId, ownerId; // Application ID and owner account ID
	CHAND winConHandle; // Console handle (Windows)
	CRNSTR apiPath, savePath; // File paths for API DLL and data save location
	CBOOL ipMode; // IP mode (IPv4 or IPv6)
	union ip_addr { IPADDR4 ipv4_addr; IPADDR6 ipv6_addr; }; // IP address of sender
}*ATCTK;

// Forward declaration of WndProc function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Entry point of application
int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpCmdLine, int nCmdShow) {
	WNDCLASS wc = { 0 }; // Window structure

	/*
	Window structure attributes

	style: Window style
	lpfnWndProc: Pointer to WndProc function
	hInstance: Application instance
	hIcon: Icon to be used
	hCursor: Cursor to be used
	hbrBackground: Window background
	lpszClassName: Name of window structure
	*/

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WndProc;
	wc.hInstance = hInst;
	wc.hIcon = LoadIcon(hInst, MAKEINTRESOURCE(APPICON));
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszClassName = L"node_em_w";

	// Registers window structure
	if (!RegisterClass(&wc)) {
		// If failed, show error message box
		MessageBox(NULL, L"Window structure registration failed!", L"CrunchNet Node Emulator", MB_ICONERROR);
		return 1;
	}

	ghInst = hInst; // Stores hInst in global variable

	// Creates window
	HWND hWnd = CreateWindow(L"node_em_w", L"CrunchNet Node Emulator",
		WS_OVERLAPPED | WS_MINIMIZEBOX | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		400, 180, NULL, hMenu, hInst, NULL);

	// Checks if window was created successfully
	if (!hWnd) {
		// If not, show error message box
		MessageBox(NULL, L"Window creation failed!", L"CrunchNet Node Emulator", MB_ICONERROR);
		return 1;
	}

	ShowWindow(hWnd, nCmdShow); // Displays window
	UpdateWindow(hWnd); // Updates window

	MSG msg; // Message handle
	// Message loop
	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

// Event defenitions
#define OPEN_FILE 1
#define START_SCRIPT 2
#define ABOUT_DISPLAY 3
#define LICENSE_DISPLAY 4
#define GET_DATA_INFO 5

WCHAR fPath[100]; // File location string

OPENFILENAME ofn = { 0 }; // File dialog structure

// Number length function
int GetLength(CNUM64 n) {
	// Checks for length
	if (n >= 10000000000000000000) return 20;
	if (n >= 1000000000000000000) return 19;
	if (n >= 100000000000000000) return 18;
	if (n >= 10000000000000000) return 17;
	if (n >= 1000000000000000) return 16;
	if (n >= 100000000000000) return 15;
	if (n >= 10000000000000) return 14;
	if (n >= 1000000000000) return 13;
	if (n >= 100000000000) return 12;
	if (n >= 10000000000) return 11;
	if (n >= 1000000000) return 10;
	if (n >= 100000000) return 9;
	if (n >= 10000000) return 8;
	if (n >= 1000000) return 7;
	if (n >= 100000) return 6;
	if (n >= 10000) return 5;
	if (n >= 1000) return 4;
	if (n >= 100) return 3;
	if (n >= 10) return 2;
	return 1;
}

HINSTANCE tmpDll; // Temporary DLL handle
HWND appDisplay, inputCodeIn, dataTypeIn, dataIn; // Window handles
BOOL isApplicationRunning = FALSE; // Application status

// Application execution function
DWORD AppExecution(HWND hWnd) {
	// Checks if file can be loaded
	tmpDll = LoadLibrary((LPCWSTR)fPath);
	if (tmpDll != NULL) {
		// Attempt to get address of CrunchMain function
		LPCRNFUNC crnFunc = (LPCRNFUNC)GetProcAddress(tmpDll, "CrunchMain");
		if (crnFunc != NULL) {
			AllocConsole(); // Creates console window
			HANDLE conHandle = GetStdHandle(STD_OUTPUT_HANDLE); // Gets output handle to console

			// Gets input code
			LPWSTR codeStr = malloc(GetWindowTextLength(inputCodeIn) * 2); // Gets number of characters in input code text box
			GetWindowText(inputCodeIn, codeStr, GetWindowTextLength(inputCodeIn)); // Gets text from text box
			CNUM32 inputCode = wcstol(codeStr, NULL, 10); // Converts text to integer
			IPADDR4 defaultIp = { 127, 0, 0, 1 }; // Default IP address

			// Gets input data
			CNUM8* dataIn = 0; // Input data
			CNUM32 dataSize = 0; // Size of input data

			// Gets file paths to API DLL and file system root
			WCHAR execPath[MAX_PATH] = { 0 }; // Executable path
			char apiPath[MAX_PATH] = { 0 }; // API DLL path
			char savePath[MAX_PATH] = { 0 }; // CrunchNet file system root

			GetModuleFileName(NULL, execPath, MAX_PATH); // Gets executable path
			// Converts UTF-16 string to UTF-8 strings
			WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS, execPath, MAX_PATH, apiPath, MAX_PATH, NULL, NULL);
			WideCharToMultiByte(CP_UTF8, WC_NO_BEST_FIT_CHARS, execPath, MAX_PATH, savePath, MAX_PATH, NULL, NULL);

			int strLength = (int)wcslen(execPath); // Gets length of string
			// Edits strings
			apiPath[strLength - 10] = 'c';
			apiPath[strLength - 9] = 'r';
			apiPath[strLength - 8] = 'n';
			apiPath[strLength - 7] = 'a';
			apiPath[strLength - 6] = 'p';
			apiPath[strLength - 5] = 'i';
			apiPath[strLength - 4] = 'd';
			apiPath[strLength - 3] = 'e';
			apiPath[strLength - 2] = 'v';
			apiPath[strLength - 1] = '.';
			apiPath[strLength] = 'd';
			apiPath[strLength + 1] = 'l';
			apiPath[strLength + 2] = 'l';

			savePath[strLength - 10] = 's';
			savePath[strLength - 9] = 't';
			savePath[strLength - 8] = 'o';
			savePath[strLength - 7] = 'r';
			savePath[strLength - 6] = 'a';
			savePath[strLength - 5] = 'g';
			savePath[strLength - 4] = 'e';
			savePath[strLength - 3] = '/';
			savePath[strLength - 2] = NULL;
			savePath[strLength - 1] = NULL;
			savePath[strLength] = NULL;

			// Constructs token
			struct _CTK token;
			token.appId = 0;
			token.ownerId = 0;
			token.winConHandle = conHandle;
			token.apiPath = apiPath;
			token.savePath = savePath;
			token.ipMode = 0; // IPv4 mode
			token.ipv4_addr = defaultIp; // IP address 127.0.0.1

			CNUM64 funcRVal = crnFunc((CTK)&token, inputCode, (CHAND)dataIn, dataSize); // Executes CrunchMain function and stores return value
			int funcRValLength = GetLength(funcRVal); // Gets length of return value
			LPWSTR funcRValStr = malloc((funcRValLength + 19) * 2); // Allocates memory for string
			swprintf(funcRValStr, funcRValLength + 19, L"Execution time: %lldms", funcRVal); // Converts unsigned 64-bit integer to string
			MessageBox(hWnd, funcRValStr, L"CrunchNet Node Emulator", MB_APPLMODAL); // Shows message box with execution time
			free(funcRValStr); // Frees allocated memory
			FreeConsole(); // Closes console window
		}
		else {
			// Shows error message box
			MessageBox(hWnd, L"Invaild application!", L"CrunchNet Node Emulator", MB_ICONERROR | MB_APPLMODAL);
		}
	}
	else {
		// Shows error message box
		MessageBox(hWnd, L"Application could not be loaded", L"CrunchNet Node Emulator", MB_ICONERROR | MB_APPLMODAL);
	}
	FreeLibrary(tmpDll); // Frees DLL from memory
	isApplicationRunning = FALSE;
	return 0;
}

// Window procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc; // Device context handle

	// Temporary variables
	int dataTypePos = 0;

	switch (message) {
	case WM_CREATE:
		hMenu = CreateMenu(); // Creates window menu
		AppendMenu(hMenu, MF_STRING, OPEN_FILE, L"Open"); // Adds Open option to menu
		AppendMenu(hMenu, MF_STRING, START_SCRIPT, L"Start"); // Adds Start option to menu
		AppendMenu(hMenu, MF_STRING, ABOUT_DISPLAY, L"About"); // Adds About option to menu
		AppendMenu(hMenu, MF_STRING, LICENSE_DISPLAY, L"License"); // Adds License option to menu
		SetMenu(hWnd, hMenu); // Assigns menu to window

		// Application location display
		appDisplay = CreateWindow(L"Static", L"<No application selected>", WS_VISIBLE | WS_CHILD,
			145, 5, 235, 18, hWnd, NULL, ghInst, NULL);
		// Input code text box
		inputCodeIn = CreateWindow(L"Edit", NULL, WS_VISIBLE | WS_CHILD | ES_NUMBER,
			85, 25, 295, 18, hWnd, NULL, ghInst, NULL);
		// Data type drop-down list
		dataTypeIn = CreateWindow(L"ComboBox", NULL, WS_VISIBLE | WS_CHILD | CBS_DROPDOWNLIST | CBS_HASSTRINGS,
			110, 41, 270, 18, hWnd, NULL, ghInst, NULL);
		// Data input text box
		dataIn = CreateWindow(L"Edit", NULL, WS_VISIBLE | WS_CHILD | ES_AUTOHSCROLL,
			80, 65, 300, 15, hWnd, NULL, ghInst, NULL);
		// About file type button
		CreateWindow(L"Button", L"Data type info", WS_VISIBLE | WS_CHILD,
			5, 85, 375, 30, hWnd, (HMENU)GET_DATA_INFO, ghInst, NULL);
		
		// Adds placeholder tex
		SendMessage(inputCodeIn, EM_SETCUEBANNER, NULL, (LPARAM)&L"Value of 'input' parameter in CrunchMain");
		SendMessage(dataIn, EM_SETCUEBANNER, NULL, (LPARAM)&L"Value of 'data' parameter in CrunchMain");
		// Adds items to data type text box
		SendMessage(dataTypeIn, CB_ADDSTRING, NULL, (LPARAM)&L"None");
		SendMessage(dataTypeIn, CB_ADDSTRING, NULL, (LPARAM)&L"Text (UTF-8)");
		SendMessage(dataTypeIn, CB_ADDSTRING, NULL, (LPARAM)&L"Integer");
		SendMessage(dataTypeIn, CB_ADDSTRING, NULL, (LPARAM)&L"Floating-point number");
		SendMessage(dataTypeIn, CB_ADDSTRING, NULL, (LPARAM)&L"File data");
		SendMessage(dataTypeIn, CB_SETCURSEL, 0, NULL);
		break;
	case WM_COMMAND:
		switch (wParam) {
		case OPEN_FILE:
			// Updates file structure attributes
			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFile = fPath;
			ofn.lpstrFile[0] = L'\0';
			ofn.nMaxFile = 100;
			ofn.lpstrFilter = L"CrunchNet server-side application\0*.dll\0";
			ofn.nFilterIndex = 0;
			GetOpenFileName(&ofn); // Shows file dialog
			SetWindowText(appDisplay, fPath); // Updates application display
			break;
		case START_SCRIPT:
			if (!isApplicationRunning) {
				isApplicationRunning = TRUE;
				CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)AppExecution, hWnd, NULL, NULL); // Creates new thread for application
			}
			else MessageBox(hWnd, L"Application is already running!", L"CrunchNet Node Emulator", MB_ICONERROR | MB_APPLMODAL);
			break;
		case ABOUT_DISPLAY:
			// Show about message box
			MessageBox(hWnd, L"CrunchNet Node Emulator (Windows)\nVersion 0.1\nCopyright (c) 2018 Kade Burnside\n\nThis program is licensed under the GNU GPLv3.\nSource code is available at https://github.com/cadellapowell61/crunchnet \n\nTo get in touch with me, email me at cadellapowell61@gmail.com", L"About CrunchNet", MB_APPLMODAL);
			break;
		case LICENSE_DISPLAY:
			// Opens HTML file containing license
			ShellExecute(NULL, NULL, L"license.html", NULL, NULL, NULL);
			break;
		case GET_DATA_INFO:
			dataTypePos = (int)SendMessage(dataTypeIn, CB_GETCURSEL, NULL, NULL); // Gets current data type
			switch (dataTypePos) {
			case 0: MessageBox(hWnd, L"No data will be passed into the application.", L"CrunchNet Node Emulator", MB_APPLMODAL); break;
			case 1: MessageBox(hWnd, L"Text (UTF-8)\n\nA string of characters. Pretty self-explanatory.\nNote: The text box supports UTF-16 characters, which are not supported by CrunchNet.", L"CrunchNet Node Emulator", MB_APPLMODAL); break;
			case 2: MessageBox(hWnd, L"Integer\n\nA signed 64-bit integer.\nIf text box contains other characters, input will not be accepted.", L"CrunchNet Node Emulator", MB_APPLMODAL); break;
			case 3: MessageBox(hWnd, L"Floating-point number\n\nA double-precision floating-point number.\nIf text box contains other characters, input will not be accepted.", L"CrunchNet Node Emulator", MB_APPLMODAL); break;
			case 4: MessageBox(hWnd, L"File data\n\nLoads data stored in file. Text box should contain path to specified file.", L"CrunchNet Node Emulator", MB_APPLMODAL); break;
			}
		}
		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		TextOut(hdc, 5, 5, L"Selected application:", 21);
		TextOut(hdc, 5, 25, L"Input code:", 11);
		TextOut(hdc, 5, 45, L"Input data type:", 16);
		TextOut(hdc, 5, 65, L"Input data:", 11);
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
		break;
	}
	return 0;
}
