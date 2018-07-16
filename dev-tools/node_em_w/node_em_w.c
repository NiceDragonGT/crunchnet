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

// Windows API include
#include <Windows.h>

// Internal CrunchNet include
#include "resource.h"

// Redefines null as "int" instead of "void*"
#undef NULL
#define NULL 0

HINSTANCE ghInst; // Global HINSTANCE
HMENU hMenu; // Window menu

typedef unsigned long long uint64; // 64 bit unsigned integer type (from crunchnet.h)
typedef uint64(*CRNFUNC)(); // CrunchMain function pointer type

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
	wc.hbrBackground = CreateSolidBrush(RGB(255, 182, 193));
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
		330, 60, NULL, hMenu, hInst, NULL);

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
	while (GetMessage(&msg, NULL, 0, 0))
	{
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

LPWSTR fPath; // File location string

OPENFILENAME ofn = { 0 }; // File dialog structure

// Window procedure function
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	switch (message) {
	case WM_CREATE:
		ofn.lStructSize = sizeof(OPENFILENAME);
		ofn.hwndOwner = hWnd;
		ofn.lpstrFile = fPath;
		ofn.nMaxFile = 100;
		ofn.lpstrFilter = L"CrunchNet server-side application\0*.dll\0";
		ofn.nFilterIndex = 0;

		hMenu = CreateMenu(); // Creates window menu
		AppendMenu(hMenu, MF_STRING, OPEN_FILE, L"Open"); // Adds Open option to menu
		AppendMenu(hMenu, MF_STRING, START_SCRIPT, L"Start"); // Adds Start option to menu
		AppendMenu(hMenu, MF_STRING, ABOUT_DISPLAY, L"About"); // Adds About option to menu
		AppendMenu(hMenu, MF_STRING, LICENSE_DISPLAY, L"License"); // Adds License option to menu
		SetMenu(hWnd, hMenu); // Assigns menu to window
		break;
	case WM_COMMAND:
		switch (wParam) {
		case OPEN_FILE:
			fPath = GlobalAlloc(NULL, 200); // Allocates memory for string
			GetOpenFileName(&ofn); // Shows file dialog
			MessageBox(hWnd, fPath, L"CrunchNet Node Emulator", MB_APPLMODAL);
			// Checks if file can be loaded
			HINSTANCE tmpDll = LoadLibrary((LPCWSTR)fPath);
			if (!tmpDll) {
				MessageBox(hWnd, L"Application could not be loaded", L"CrunchNet Node Emulator", MB_ICONERROR | MB_APPLMODAL);
			}
			else {
				// Attempt to get address of CrunchMain function
				CRNFUNC crnFunc = (CRNFUNC)GetProcAddress(tmpDll, "CrunchMain");
				if (!crnFunc) {
					MessageBox(hWnd, L"Invaild application!", L"CrunchNet Node Emulator", MB_ICONERROR | MB_APPLMODAL);
				}
			}
			FreeLibrary(tmpDll);
			break;
		case START_SCRIPT:
			break;
		case ABOUT_DISPLAY:
			// Show about message box
			MessageBox(hWnd, L"CrunchNet Node Emulator (Windows)\nVersion 0.1\nCopyright (c) 2018 Kade Burnside\n\nThis program is licensed under the GNU GPLv3.\nSource code is available at https://github.com/cadellapowell61/crunchnet \n\nTo get in touch with me, email me at cadellapowell61@gmail.com", L"About CrunchNet", MB_APPLMODAL);
			break;
		case LICENSE_DISPLAY:
			// Opens HTML file containing license
			ShellExecute(NULL, NULL, L"license.html", NULL, NULL, NULL);
			break;
		}
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
